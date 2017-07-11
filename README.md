# LoRa-Range-Testing-Node

This is an Auduino Script tested on the Uno using a modified xBee shield. The script uses the Multitech mDOT LoRa module running the Australian compatable AT   enabled firmware. This script makes use of the following libraries:

1. [#include <AltoviewMDot.h>](https://github.com/Altoview/Altoview_MDot)
2. [#include <AltSoftSerial.h>](https://www.pjrc.com/teensy/td_libs_AltSoftSerial.html)
3. #include <SoftwareSerial.h> --> included in the Arduino libraries
4. #include <LiquidCrystal.h> --> included in the Arduino libraries

## What: 
- The system provides a portable LoRa Range testing solution where **RSSI** and **SNR** of the last packet sent to the network is provided to the user via the nokia lcd.
- Script loops and sends loop count in the form of a char array: "L: *loop_count*". 

## Hardware:
- [mDot Lora Radio Module](http://www.multitech.com/brands/multiconnect-mdot)
- LiquidCrystal display (LCD) based on the Hitachi HD44780 (or a compatible) chipset. [Arduino Link](https://www.arduino.cc/en/Reference/LiquidCrystal)
- Modified Arduino xBee Sheild. The mDot is controlled on pins 8 and 9 via the `AltSoftSerial.h` library. The sheild required a modification to ensure the tx/rx pins were rerouted to pins 8/9. 

## How to use
1. Clone the project to a preferred folder.
2. Upload the project to the arduino board. 
3. Mount the hardware in the following order:
	1. Plug the arduino xbee shield onto the arduino board
	2. Plug the MDot into the xbee shield 
	3. Plug the LCD on top of the xbee shield, Ensure the tx/rx pins of the LCD are rerouted to software serial pins (in this case they are pins 2/3). 

#### Explanation of lcd returns:  
An explanation of some likely debug responses is listed below: 

```
j: 0		--> the mdot successfully joined
j: -1		--> mdot didn't joing LoRa network 
sp: ok		--> Sending Pairs --> success 
sp: err		--> Sending Pairs --> failed 
rssi: XX	--> the rssi of the last packet sent 
snr: XX		--> the snr of the last packet 
dr: X		--> the current Data Rate the mDot is using
dr: err		--> setting the data rate failed 
```
For more information on AT commands, please see the Multech [AT Command Getting Started Guide](http://www.multitech.net/developer/software/mdot-software/at-command-firmware-user-guide/) and the [DOT Series AT Command Reference Guide](http://www.multitech.com/documents/publications/manuals/s000643.pdf).

### Register Node in AltoView
- Once you have identified the LoRa node has successful connected to the LoRa network and is sending data, navigate to [AltoView](http://www.altoview.com/) (create a login if you don't already have one) 
- On the left hand side menu, select **My Nodes** and click **Register a Node** 
- Enter the DevEUI ([Development Extended Unique Identifier](https://en.wikipedia.org/wiki/MAC_address)) which can be found on the mDot node in small print (look for a 16 value long ID with numbers and letters in it, eg: 00 80 00 00 00 00 AA 66) 

#### NB when registering node in AltoView 
- although the DevEUI is written in *uppercase* on most mDot modules, altoview requirers the DevEUI in ***LOWERCASE*** 
- once the node details have been entered and you have hit the `register` button, the node will have either an `online` or `offline` status. Please note that this status **does not** mean that the node is powered on and connected to the LoRa network. Instead: 
  - `online` --> the node was connected and sending data in the past. In other words, there is a history of the node having sent data
  - `offline` --> the node has never sent data. In other words, AltoView has never seen this nodes devEUI before 
