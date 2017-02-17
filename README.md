# LoRa-Range-Testing-Node

This is a program writen for the Seeeduino Stalker v2.3 and   uses the Multitech mDOT LoRa module running the Australian compatable AT   enabled firmware. This script makes use of the following libraries:\

1. [mdot_lora_arduino](https://github.com/lennon101/mdot_lora_arduino/blob/master/README.md)
2. [Nokia 5110 LCD](http://randomnerdtutorials.com/complete-guide-for-nokia-5110-lcd-with-arduino/)

## What: 
- The system provides a portable LoRa Range testing solution where **RSSI** and **SNR** of the last packet sent to the network is provided to the user via the nokia lcd.
- Script loops and sends loop count in the form of a char array: "L: *loop_count*". 
- Nokia lcd provides update of RSSI and SNR 
- Rotary encoder (knob) provides user ability to change **Data Rate** of the mDot. 

## How to use
1. Clone the project to a preferred folder.
2. Wire the nokia lcd as per the link provided above
3. Wire the rotary encoder as per these instructions: [link](9http://domoticx.com/wp-content/uploads/Rotary-encoder-module-KY-040-arduino-schema.png) 
	* clk --> pin 2 (pin 1 and 2 are the only interupt pins avaialbe) 
	* data --> pin 9 
	* vcc --> 5V 
4. Implementing capacitors (approx. 1uF) from the clock and data pins of the rotary encoder to **GND** was found to help de-bounce the circuit and remove unwanted noise which produced jumping affects from the encoder 

### Debugging: 
- Once you have uploaded the code to your dev board, you must remove the TX and RX wires (otherwise this will disrupt the mdot signals being sent to the LoRa netork and you node will not connect) 

#### Explanation of lcd returns:  
An explanation of some likely debug responses is listed below: 

```
j: 0            			--> the mdot successfully joined
j: -1            			--> mdot didn't joing LoRa network 
sp: ok						--> Sending Pairs --> success 
sp: err						--> Sending Pairs --> failed 
rssi: XX 					--> the rssi of the last packet sent 
snr: XX						--> the snr of the last packet 
dr: X						--> the current Data Rate the mDot is using
dr: err						--> setting the data rate failed 
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
