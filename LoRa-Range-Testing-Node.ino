/*
 * =====================================================================================
 * LoRa Range Testing Node for Altoview 
 * =====================================================================================
 * Date:            12 May 17 
 * Program Author:  Dane Lennon
 * 
 * -------------------------------------------------------------------------------------
 * This is an example program writen for the Arduino UNO R3 and
 * uses the Multitech mDOT LoRa module running the Australian 
 * compatable AT enabled firmware.
 * 
 * This program,
 *  Joins the LoRa Network
 *  Sends a loop count to Altoview
 *  
 *  Hardware:
 *    1. mDot Lora Radio Module
 *    2. LiquidCrystal display (LCD) based on the Hitachi HD44780 (or a compatible) chipset
 *    3. Modified Arduino xBee Sheild. The mDot is controlled on pins 8 and 9 via the AltSoftSerial.h 
 *        library. The sheild required a modification to ensure the tx/rx pins were rerouted to pins 8/9
 *  
 *  Program Discription:
 *    This program is written for use in CRBasic programming training courses.
 *    It demonstrates how to use the CR1000 logger to measure a 4-20ma 
 *    output pressure sensor using a 100 OHM precision resistor
 *    
*/

/*--------------------------------------------------------------------------------------
  Includes
  --------------------------------------------------------------------------------------*/
#include <AltoviewMDot.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);

/*--------------------------------------------------------------------------------------
  Definitions
  --------------------------------------------------------------------------------------*/
/* library uses software serial to communicate with the mDot module */
AltSoftSerial mdotSerial;				                       // AltSoftSerial only uses ports 8, 9 for RX, TX 
HardwareSerial& debugSerial = Serial;                  // library uses hardware serial to print the debuggin information

AltoviewMDot mdot(&mdotSerial, &debugSerial);          // creat an object of a type LoRaAT called mDot

/*--- setup() --------------------------------------------------------------------------
  Called by the Arduino framework once, before the main loop begins.

  In the setup() routine:
   - Opens serial communication with MDOT
  --------------------------------------------------------------------------------------*/
void setup() {
  int responseCode;                                     // Variable to hold response from mDot LoRa radio
  debugSerial.begin(38400);                             // begins a serial communication of a hardware serial  
  mdotSerial.begin(38400);                              // begins a serial communication of a software serial 
  
  debugSerial.println("Joining Altoview...");
   
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows
  lcd.print("Joining Altoview");                        // Print a message to the LCD.
  mdot.begin();							

  do {
    lcd.setCursor(0, 1);                                // move curser to column 0, row 1
    lcd.print("join req");
    responseCode = mdot.join();                         // attempt to join to Altoview
    for (int i=0;i<3;++i){
      lcd.print(".");
      delay(3000);                                      // wait for the join process to finish
    }
    if (responseCode != 0){
      lcd.print("err  "); 
    }
  } while (responseCode != 0);
  
  clearLCD();                                            // return the cursor to the top row, first column 
  lcd.print("Joined.         ");                         // display msg and fill string to 16 characters long to clear any
  delay(1500);
}

/*--- loop() ---------------------------------------------------------------------------
  Main loop called by the Arduino framework

  In the loop() routine:
   - send the loop count in the JSON format to Altoview
  --------------------------------------------------------------------------------------*/
int loopNum = 0;
char msg[30];                                           // RSSI:XXX SNR:XX
char snr[4];                                            // XX.X
void loop() {
  int responseCode;
  lcd.setCursor(0,0);
  lcd.print("Sending Pairs");
  responseCode = mdot.sendPairs("L:" + String(loopNum));
  for (int i=0;i<3;++i){
      lcd.print(".");
      delay(3000);                                      // wait for the sendPairs process to finish
    }
    
  if (responseCode != 0){
    debugSerial.println("error in sending pairs");
    lcd.setCursor(0, 1);
    lcd.print("error           ");
  }else{
    mdot.ping(); 
    dtostrf(mdot.snr, 4, 1, snr);                       // needed to do this because sprintf does not support floats in arduino
    sprintf(msg, "R:%d,S:%s",mdot.rssi,snr); 
    debugSerial.println(msg);  
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print(msg);
    /*
    lcd.setCursor(0, 0);
    lcd.print("RSSI: ");
    lcd.print(mdot.rssi);
    lcd.setCursor(0, 1);
    lcd.print("                ");
    delay(300);
    lcd.print("SNR:");
    delay(300);
    lcd.print(snr);
    */
  }
  delay(10000);						
  loopNum++;
}

void clearLCD(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,0);                               //return the cursor to the top row, first column
}

