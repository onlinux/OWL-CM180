# OWL-CMR180
Decode and parse the Oregon Scientific V3 radio data transmitted by  OWL CM180 Energy sensor (433.92MHz)

Owl micro+ Wireless electricity monitor runs on Oregon Scientific V3 protocol.

After buying one of those wonderful little devices to study the energy consumed at home, i browsed the internet to find a way to decode frames sent by the transmitter (CMR180) but I have not found any application describing the coding system of the information in the radio frames. So I decided to study the radio packets to decode the system.

![DIY CM180 Monitor ](https://raw.github.com/onlinux/OWL-CMR180/master/images/diy-owl-cm180.jpg)
##Packet format
1. [Primary] <br>
OSV3 62803CE8006047D9120000D085[CMR180,...] Id:6280, size:13 ,Flags:8 ,power:224 ,total:316229472 ,total Wh:87.84<BR>
628 : id<BR>
0: primary packet<BR>
3C: type (?? it stays always that value,  and i suppose it's owl-cmr180 type. use it that way in the script)<BR>
E800: little-indian 0x00E8 (0x00E8 & 0xFFF0 = 224 Watts) last nibble (8) considered as Flags-1 (OSV3 protocol documentation)<BR>
6047D912: Little-indian 0x12D94760 (316229472 Watts/ 3600/100 = 87.84 kWh)<BR>
...: ??<BR>

1. [Secondary]<BR>
OSV3 6284 3C480B60 <BR>
Same as primary without total energy and nibble#4 from 1 to 4 ( one every 12 seconds)<BR>
CMR180 sends secondary packets in case of high power changes.<BR>

##Getting Started

Arduino

Wiring RFx433 receiver
 
 DATA  --> Arduino pin D3 (int-1) <BR>
 VCC   --> 5V <BR>
 GND   --> GND <BR>
 
 Load arduino sketch (https://raw.github.com/onlinux/OWL-CMR180/master/arduino/oregon_owl.ino)
 ![DIY CM180 Monitor ](https://raw.github.com/onlinux/OWL-CMR180/master/images/oregon_owl_serial_output.png)
 
Code available for Arduino and Raspberry Pi.
