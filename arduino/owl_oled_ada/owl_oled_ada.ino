// New code to decode OOK signals from Energy OWL CMR180 sensor
// Oregon V3 decoder added - Eric Vandecasteele (onlinux)
//
// Oregon V2 decoder modfied - Olivier Lebrun
// Oregon V2 decoder added - Dominique Pierre
// New code to decode OOK signals from weather sensors, etc.
// 2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
// $Id: ookDecoder.pde 5331 2010-04-17 10:45:17Z jcw $
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_RESET 4
Adafruit_SSD1306 display(OLED_RESET);


#define SDA_PIN 8
#define SCL_PIN 9
//Adafruit_ssd1306syp display(SDA_PIN,SCL_PIN); 


class DecodeOOK {
protected:
    byte total_bits, bits, flip, state, pos, data[31];
 
    virtual char decode (word width) =0;
 
public:
 
    enum { UNKNOWN, T0, T1, T2, T3, OK, DONE };
 
    DecodeOOK () { resetDecoder(); }
 
    bool nextPulse (word width) {
        if (state != DONE)
 
            switch (decode(width)) {
                case -1: resetDecoder(); break;
                case 1:  done(); break;
            }
        return isDone();
    }
 
    bool isDone () const { return state == DONE; }
 
    const byte* getData (byte& count) const {
        count = pos;
        return data; 
    }
 
    void resetDecoder () {
        total_bits = bits = pos = flip = 0;
        state = UNKNOWN;
    }
 
    // add one bit to the packet data buffer
 
    virtual void gotBit (char value) {
        total_bits++;
        byte *ptr = data + pos;
        *ptr = (*ptr >> 1) | (value << 7);
 
        if (++bits >= 8) {
            bits = 0;
            if (++pos >= sizeof data) {
                resetDecoder();
                return;
            }
        }
        state = OK;
    }
 
    // store a bit using Manchester encoding_rx
    void manchester (char value) {
        flip ^= value; // manchester code, long pulse flips the bit
        gotBit(flip);
    }
 
    // move bits to the front so that all the bits are aligned to the end
    void alignTail (byte max =0) {
        // align bits
        if (bits != 0) {
            data[pos] >>= 8 - bits;
            for (byte i = 0; i < pos; ++i)
                data[i] = (data[i] >> bits) | (data[i+1] << (8 - bits));
            bits = 0;
        }
        // optionally shift bytes down if there are too many of 'em
        if (max > 0 && pos > max) {
            byte n = pos - max;
            pos = max;
            for (byte i = 0; i < pos; ++i)
                data[i] = data[i+n];
        }
    }
 
    void reverseBits () {
        for (byte i = 0; i < pos; ++i) {
            byte b = data[i];
            for (byte j = 0; j < 8; ++j) {
                data[i] = (data[i] << 1) | (b & 1);
                b >>= 1;
            }
        }
    }
 
    void reverseNibbles () {
        for (byte i = 0; i < pos; ++i)
            data[i] = (data[i] << 4) | (data[i] >> 4);
    }
 
    void done () {
        while (bits)
            gotBit(0); // padding
        state = DONE;
    }
};
 
class OregonDecoderV2 : public DecodeOOK {
  public:   
 
    OregonDecoderV2() {}
 
    // add one bit to the packet data buffer
    virtual void gotBit (char value) {
        if(!(total_bits & 0x01))
        {
            data[pos] = (data[pos] >> 1) | (value ? 0x80 : 00);
        }
        total_bits++;
        pos = total_bits >> 4;
        if (pos >= sizeof data) {
            Serial.println("sizeof data");
            resetDecoder();
            return;
        }
        state = OK;
    }
 
    virtual char decode (word width) {
       if (200 <= width && width < 1200) {
            //Serial.println(width);
            byte w = width >= 700;
 
            switch (state) {
                case UNKNOWN:
                    if (w != 0) {
                        // Long pulse
                        ++flip;
                    } else if (w == 0 && 24 <= flip) {
                        // Short pulse, start bit
                        flip = 0;
                        state = T0;
                    } else {
                        // Reset decoder
                        return -1;
                    }
                    break;
                case OK:
                    if (w == 0) {
                        // Short pulse
                        state = T0;
                    } else {
                        // Long pulse
                        manchester(1);
                    }
                    break;
                case T0:
                    if (w == 0) {
                      // Second short pulse
                        manchester(0);
                    } else {
                        // Reset decoder
                        return -1;
                    }
                    break;
              }
        } else if (width >= 2500  && pos >= 8) {
            return 1;
        } else {
            return -1;
        }
        return 0;
    }
};

//===================================================================
class OregonDecoderV3 : public DecodeOOK {
  public:   
 
    OregonDecoderV3() {}
 
    // add one bit to the packet data buffer
    virtual void gotBit (char value) {
        data[pos] = (data[pos] >> 1) | (value ? 0x80 : 00);
        total_bits++;
        pos = total_bits >> 3;
        if (pos >= sizeof data) {
            //Serial.println("sizeof data");
            resetDecoder();
            return;
        }
        state = OK;
    }
 
    virtual char decode (word width) {
       if (200 <= width && width < 1200) {
            //Serial.println(width);
            byte w = width >= 700;
 
            switch (state) {
                case UNKNOWN:
                    if (w == 0) {
                        // Long pulse
                        ++flip;
                    } else if (32 <= flip) {
                        flip = 1;
                        manchester(1);
                    } else {
                        // Reset decoder
                        return -1;
                    }
                    break;
                case OK:
                    if (w == 0) {
                        // Short pulse
                        state = T0;
                    } else {
                        // Long pulse
                        manchester(1);
                    }
                    break;
                case T0:
                    if (w == 0) {
                      // Second short pulse
                        manchester(0);
                    } else {
                        // Reset decoder
                        return -1;
                    }
                    break;
              }
        } else  {
            // Trame intermédiaire 48bits ex: [OSV3 6281 3C 6801 70]
            return  (total_bits <104 && total_bits>=40  ) ? 1: -1;
        }
        
        return (total_bits == 104) ? 1: 0;
    }
};

//===================================================================

OregonDecoderV2 orscV2;
OregonDecoderV3 orscV3;

 
volatile word pulse;
 
void ext_int_1(void)
{
    static word last;
    // determine the pulse length in microseconds, for either polarity
    pulse = micros() - last;
    last += pulse;
}
float temperature(const byte* data)
{
    int sign = (data[6]&0x8) ? -1 : 1;
    float temp = ((data[5]&0xF0) >> 4)*10 + (data[5]&0xF) + (float)(((data[4]&0xF0) >> 4) / 10.0);
    return sign * temp;
}
 
byte humidity(const byte* data)
{
    return (data[7]&0xF) * 10 + ((data[6]&0xF0) >> 4);
}
 
// Ne retourne qu'un apercu de l'etat de la batterie : 10 = faible
byte battery(const byte* data)
{
    return (data[4] & 0x4) ? 10 : 90;
}
 
byte channel(const byte* data)
{
    byte channel;
    switch (data[2])
    {
        case 0x10:
            channel = 1;
            break;
        case 0x20:
            channel = 2;
            break;
        case 0x40:
            channel = 3;
            break;
     }
 
     return channel;
}

unsigned int power(const byte* d){
  unsigned int val = 0;
  val += d[4] << 8;
  val += d[3];
  return val & 0xFFF0 ;
}

unsigned long total(const byte* d){
  long val = 0;
  val = (unsigned long)d[8]<<24;
//  Serial.println();
//  Serial.print(" val:"); Serial.print(val,HEX); Serial.print(" ");
//  Serial.println(d[8], HEX);
  val += (unsigned long)d[7] << 16;
//  Serial.print(" val:"); Serial.print(val,HEX); Serial.print(" ");
//  Serial.println(d[7], HEX);
  val += d[6] << 8;
//  Serial.print(" val:"); Serial.print(val,HEX); Serial.print(" ");
//  Serial.println(d[6], HEX);
  val += d[5];
//  Serial.print(" val:"); Serial.print(val,HEX); Serial.print(" ");
//  Serial.println(d[5], HEX);
  return val ;
}


void reportSerial (const char* s, class DecodeOOK& decoder) {
    byte pos;
    const byte* data = decoder.getData(pos);
    Serial.print(s);
    Serial.print(' ');
    for (byte i = 0; i < pos; ++i) {
        Serial.print(data[i] >> 4, HEX);
        Serial.print(data[i] & 0x0F, HEX);
    }
    
    // Energy OWL : CMR180
    if(data[2] == 0x3C )
    {
       Serial.print("[CMR180,...] Id:");
       Serial.print(data[0], HEX);Serial.print(data[1], HEX);
       Serial.print(", size:");
       Serial.print(pos);
       Serial.print(" ,Flags:");
       Serial.print(data[3] & 0x0F, HEX);
       Serial.print(" ,power:");
       display.setCursor(0,15);
       display.fillRect(0, 15, 128, 16, 0x0000);
       display.setTextSize(2);     
       Serial.print(power(data));      
       display.print(power(data));display.print("W");
       
       //display.updateRow(2);
       
       if (pos > 6) {
         // Display only for main frame
         // Secondary frame is only 6 Bytes long
         Serial.print(" ,total:");
         Serial.print(total(data));
         Serial.print(" ,total Wh:");
         Serial.print(total(data)/3600);
         display.setCursor(0,40);
         display.fillRect(0, 40, 128, 16, 0x0000);
         //display.print("Total: ");
         display.print(total(data)/3600); display.print("Wh");
       }
       Serial.println();
       display.println();
       display.display();
       delay(50);
    }
 
 
    // Outside/Water Temp : THN132N,...
    if(data[0] == 0xEA && data[1] == 0x4C)
    {
       Serial.print("[THN132N,...] Id:");
       Serial.print(data[3], HEX);
       Serial.print(" ,Channel:");
       Serial.print(channel(data));
       Serial.print(" ,temp:");
       Serial.print(temperature(data));
       Serial.print(" ,bat:");
       Serial.print(battery(data)); 
       Serial.println();
    }
    // Inside Temp-Hygro : THGR228N,...
    else if(data[0] == 0x1A && data[1] == 0x2D)
    {
       Serial.print("[THGR228N,...] Id:");
       Serial.print(data[3], HEX);
       Serial.print(" ,Channel:");
       Serial.print(channel(data));
       Serial.print(" ,temp:");
       Serial.print(temperature(data));
       Serial.print(" ,hum:");
       Serial.print(humidity(data));
       Serial.print(" ,bat:");
       Serial.print(battery(data)); 
       Serial.println();
    }
 
    decoder.resetDecoder();
}
 
void setup ()
{
   display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
   display.display();
    delay(2000);
    
    display.clearDisplay();
    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0,0);
    display.println("ENERGY OWL micro+");
    
    display.display();
    
    Serial.begin(115200);
    Serial.println("\n[ookDecoder]");
    attachInterrupt(1, ext_int_1, CHANGE);
 
    //DDRE  &= ~_BV(PE5); //input with pull-up 
    //PORTE &= ~_BV(PE5);
}
 
void loop () {
    static int i = 0;
    cli();
    word p = pulse;
 
    pulse = 0;
    sei();
 
    if (p != 0)
    {

        if (orscV3.nextPulse(p))
             reportSerial("OSV3", orscV3);
            
        if (orscV2.nextPulse(p))
             reportSerial("OSV2", orscV2);    
    }
}
