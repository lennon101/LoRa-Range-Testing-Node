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
 *  Joins the LoRa Network.
 *  Sends a loop count to Altoview.
 *  Gets the RSSI and SNR from the last packet and displays this information to the LCD screen.
 *  
 *  Hardware:
 *    1. mDot Lora Radio Module
 *    2. LiquidCrystal display (LCD) based on the Hitachi HD44780 (or a compatible) chipset
 *    3. Modified Arduino xBee Sheild. The mDot is controlled on pins 8 and 9 via the AltSoftSerial.h 
 *        library. The sheild required a modification to ensure the tx/rx pins were rerouted to pins 8/9
*/

/*--------------------------------------------------------------------------------------
  Includes
  --------------------------------------------------------------------------------------*/
float getSlope(float history[]) __attribute__((__optimize__("O2")));

#define DEBUG                                 // uncomment to print ALL debug info and responses from mDot
//#define DEBUG2                                // uncomment to print only timed out responses and commands

#include <AltoviewMDot.h>
#include <AltSoftSerial.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal.h>

/*--------------------------------------------------------------------------------------
  Definitions
  --------------------------------------------------------------------------------------*/
/* library uses software serial to communicate with the mDot module */
AltSoftSerial mdotSerial;				                              // AltSoftSerial only uses ports 8, 9 for RX, TX 
//HardwareSerial& Serial = Serial;                         // library uses hardware serial to print the debuggin information
AltoviewMDot mdot(&mdotSerial, &Serial);                 // creat an object of a type LoRaAT called mDot

LiquidCrystal lcd(2, 3, 4, 5, 6, 7);                          // initialize the library with the numbers of the interface pins

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

/*--- setup() --------------------------------------------------------------------------
  Called by the Arduino framework once, before the main loop begins.

  In the setup() routine:
   - Opens serial communication with MDOT
  --------------------------------------------------------------------------------------*/
void setup() {
  int responseCode;                                     // Variable to hold response from mDot LoRa radio
  Serial.begin(38400);                             // begins a serial communication of a hardware serial  
  mdotSerial.begin(38400);                              // begins a serial communication of a software serial 
  Serial.println("========================================\n========= LoRa Range Testing =========\n========================================");

  Serial.println("Joining Altoview...");
   
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows
  mdot.begin();							

  int failCount = 0; 
  do {
    clearLCD(); 
    lcd.print("Joining Altoview");                        // Print a message to the LCD.
    lcd.setCursor(8,1);                                 // move to column 8, row 1 
    lcd.print("fail #:");
    lcd.print(failCount); 
    lcd.setCursor(0, 1);                                // move curser to column 0, row 1
    responseCode = mdot.join();                         // attempt to join to Altoview
    for (int i=0;i<3;++i){
      lcd.print(".");
      delay(1000);                                      // wait for the join process to finish
    }
    if (responseCode != 0){
      ++failCount;
    }
  } while (responseCode != 0);
  
  clearLCD();                                            // return the cursor to the top row, first column 
  lcd.print("Joined.         ");                         // display msg and fill string to 16 characters long to clear any
  delay(1500);

  lcd.setCursor(0,1);                                    // move to the begining of the second line
  lcd.print("Hit SEL to begin"); 
  while (lcd_key != btnSELECT){
    lcd_key = read_LCD_buttons();  // read the buttons
  } 
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
  clearLCD();
  lcd.print("Sending Pairs");
  responseCode = mdot.sendPairs("L:" + String(loopNum));
  for (int i=0;i<3;++i){
      lcd.print(".");
      delay(1500);                                      // wait for the sendPairs process to finish
    }
    
  if (responseCode != 0){
    Serial.println("error in sending pairs");
    lcd.setCursor(0, 1);
    lcd.print("error           ");
  }else{
    mdot.ping(); 
    clearLCD(); 
    lcd.print("R:");
    lcd.print(mdot.rssi);  
    lcd.setCursor(0, 1);
    delay(10);
    lcd.print("S:"); 
    lcd.print(mdot.snr);

    Serial.println("----------------------------------");
    Serial.print("mdot.rssi: "); 
    Serial.println(mdot.rssi);
    Serial.print("mdot.snr: "); 
    Serial.println(mdot.snr);  
  }

  //lcd.setCursor(1,14); 
  //lcd.print(getDataRate()); 
  
  delay(10000);						
  loopNum++;
}

// read the buttons
int read_LCD_buttons(){
  adc_key_in = analogRead(0);                           // read the value from the sensor
  if (adc_key_in < 620)  return btnSELECT;
  if (adc_key_in > 1000) return btnNONE; 
  if (adc_key_in < 820)  return btnRIGHT;
  if (adc_key_in < 860)  return btnLEFT;
  if (adc_key_in < 915)  return btnDOWN;
  if (adc_key_in < 940)  return btnUP;
  
  return btnNONE;  // when all others fail, return this...
}

uint8_t getDataRate(){
  int responseCode; 
  responseCode = mdot.getDataRate();
  if (responseCode != 0) {
    return mdot.dataRate;
  } else {
    return -1;
  }
}

void clearLCD(){
  lcd.setCursor(0,0);
  lcd.print("                ");
  lcd.setCursor(0,1);
  lcd.print("                ");
  lcd.setCursor(0,0);                               //return the cursor to the top row, first column
}


