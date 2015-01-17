/* ==========================================================================
 * RcOoK.h
 * --------------------------------------------------------------------------
 * RF433 demonstrator for rfrpi Raspberry PI shield
 * see : http://www.disk91.com/?p=1323
 * --------------------------------------------------------------------------
 * This software is under GPLv3
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 * --------------------------------------------------------------------------
 *  Modified on Jan 15 2015
 *  Added OSV3 support for OWL CMR180 Energy sensor -- Onlinux (onlinux.fr)
 * 
 *  Created on: 23 Feb. 2014
 *   Author: disk91 - Paul Pinault (c) 2014
 * --------------------------------------------------------------------------
 * 433 Mhz decoding OoK frame from Oregon Scientific
 *
 *  Created on: 16 sept. 2013
 *  Author: disk91 modified from
 *   Oregon V2 decoder added - Dominique Pierre
 *   Oregon V3 decoder revisited - Dominique Pierre
 *   RwSwitch :  Copyright (c) 2011 Suat Özgür.  All right reserved.
 *     Contributors:
 *        - Andre Koehler / info(at)tomate-online(dot)de
 *        - Gordeev Andrey Vladimirovich / gordeev(at)openpyro(dot)com
 *        - Skineffect / http://forum.ardumote.com/viewtopic.php?f=2&t=48
 *        Project home: http://code.google.com/p/rc-switch/
 *
 *   New code to decode OOK signals from weather sensors, etc.
 *   2010-04-11 <jcw@equi4.com> http://opensource.org/licenses/mit-license.php
 *	 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * 	 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * 	 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * 	 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 *	 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * 	 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * 	 THE SOFTWARE.
 * --------------------------------------------------------------------------
 */

#ifndef RCOOK_H_
#define RCOOK_H_

typedef uint16_t word;
typedef uint8_t byte;

#define OOK_MAX_DATA_LEN 25
#define OOK_MAX_STR_LEN	 100

class DecodeOOK {
protected:
    byte total_bits, bits, flip, state, pos, data[OOK_MAX_DATA_LEN];
    int  nRepeatTransmit;
	RCSwitch * rcs;		// transmit function to call

    virtual int  decode (word width) =0;
	virtual void send0();
	virtual void send1();
	virtual void sendSync();

	char * dec2binWzerofill(unsigned int Dec, unsigned int bitLength);

public:

    enum { UNKNOWN, T0, T1, T2, T3, OK, DONE };

    DecodeOOK(RCSwitch * _rcs);
    virtual ~DecodeOOK();

    bool nextPulse (word width);
    bool isDone() const;

    const byte* getData (byte& count) const;
    void resetDecoder ();

    // store a bit using Manchester encoding
    void manchester (char value);

    // move bits to the front so that all the bits are aligned to the end
    void alignTail (byte max =0);
    void reverseBits ();
    void reverseNibbles ();
    void reverseData ();
    void done ();
    void print (const char* s);
    void sprint(const char * s, char * d);

    virtual void gotBit (char value);

	void send(unsigned int Code, unsigned int length);
	void send(char * sHexStr, unsigned int length);
	void send(char* sCodeWord);


};




class OregonDecoderV2 : public DecodeOOK {
	public:
      OregonDecoderV2(RCSwitch * _rcs);
      ~OregonDecoderV2();
      void gotBit (char value);
      int decode (word width);
};


class OregonDecoderV3 : public DecodeOOK {
public:
    OregonDecoderV3(RCSwitch * _rcs);
    ~OregonDecoderV3();
    void gotBit (char value);
    int decode (word width);
};

class CrestaDecoder : public DecodeOOK {
public:
	CrestaDecoder(RCSwitch * _rcs);
    ~CrestaDecoder();
    int decode (word width);
};

class KakuDecoder : public DecodeOOK {
public:
	KakuDecoder(RCSwitch * _rcs);
    ~KakuDecoder();
    int decode (word width);
};

class XrfDecoder : public DecodeOOK {
public:
	XrfDecoder(RCSwitch * _rcs);
    ~XrfDecoder();
    int decode (word width);
};

class HezDecoder : public DecodeOOK {
public:
	HezDecoder(RCSwitch * _rcs);
    ~HezDecoder();
    int decode (word width);
};

class VisonicDecoder : public DecodeOOK {
public:
	VisonicDecoder(RCSwitch * _rcs);
    ~VisonicDecoder();
    int decode (word width);
};

class EMxDecoder : public DecodeOOK {
public:
	EMxDecoder(RCSwitch * _rcs);
    ~EMxDecoder();
    int decode (word width);
};

class KSxDecoder : public DecodeOOK {
public:
	KSxDecoder(RCSwitch * _rcs);
    ~KSxDecoder();
    int decode (word width);
};

class FSxDecoder : public DecodeOOK {
public:
	FSxDecoder(RCSwitch * _rcs);
    ~FSxDecoder();
    int decode (word width);
};


class DIO : public DecodeOOK {
public:
	DIO(RCSwitch * _rcs);
    ~DIO();
    int decode (word width);
    void done ();

    static void getSwitchON(char * code);
	static void getSwitchOFF(char * code);

protected:
	void send0();
	void send1();
	void sendSync();

//    unsigned int timings[256];
//	unsigned int changeCount;

};


// Number of maximum High/Low changes per packet.
// We can handle up to (unsigned long) => 32 bit * 2 H/L changes per bit + 2 for sync
// Disk91 - change to use 40 bit max
// #define RCSWITCH_MAX_CHANGES 67
#define RCSWITCH_MAX_CHANGES 83

class RCSwitch_ : public DecodeOOK {
public:
	RCSwitch_(int protocol, RCSwitch * _rcs);
    ~RCSwitch_();
	int decode (word duration);

protected:
    int  nPulseLength;
	char nProtocol;
	int  nReceiveTolerance;
    unsigned int timings[RCSWITCH_MAX_CHANGES];
	unsigned int changeCount;


    void setPulseLength(int nPulseLength);
    void setRepeatTransmit(int nRepeatTransmit);
    void setReceiveTolerance(int nPercent);
	void setProtocol(int nProtocol);
	void setProtocol(int nProtocol, int nPulseLength);

	bool receiveProtocol1(unsigned int changeCount);
	bool receiveProtocol2(unsigned int changeCount);

	void sendT0();
	void sendT1();
	void sendTF();
	void send0();
	void send1();
	void sendSync();

};

class Phenix : public RCSwitch_ {

public:
	static void getSwitchON(char * code);
	static void getSwitchOFF(char * code);
	static bool checkCode(char * code);

};

#endif /* RCOOK_H_ */
