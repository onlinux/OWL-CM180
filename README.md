# OWL-CMR180
Decode and parse the Oregon Scientific V3 radio data transmitted by  OWL CM180 Energy sensor (433.92MHz)

Owl micro+ Wireless electricity monitor runs on Oregon Scientific V3 protocol.

After buying one of those wonderful little devices to study the energy consumed at home, i browsed the internet to find a way to decode frames sent by the transmitter (CMR180) but I have not found any application describing the coding system of the information in the radio frames. So I decided to study the radio packets to decode the system.

![DIY CM180 Monitor ](https://raw.github.com/onlinux/OWL-CMR180/master/images/diy-owl-cm180.jpg)

##Getting Started

Arduino

Wiring RFx433 receiver
 
 DATA  --> Arduino pin D3 (int-1) <BR>
 VCC   --> 5V <BR>
 GND   --> GND <BR>
 
 Load arduino sketch (https://raw.github.com/onlinux/OWL-CMR180/master/arduino/oregon_owl.ino)
 ![DIY CM180 Monitor ](https://raw.github.com/onlinux/OWL-CMR180/master/images/oregon_owl_serial_output.png)
 
Code available for Arduino and Raspberry Pi.
