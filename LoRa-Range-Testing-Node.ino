/*
  This is an example program writen for the Arduino UNO R3 and
  uses the Multitech mDOT LoRa module running the Australian 
  compatable AT enabled firmware.

  This program,
    Joins the LoRa Network
    Sends a loop count to Altoview
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
AltSoftSerial mdotSerial;				// AltSoftSerial only uses ports 8, 9 for RX, TX 
/* library uses hardware serial to print the debuggin information */
HardwareSerial& debugSerial = Serial;

/* creating an object of a type LoRaAT called mDot */
AltoviewMDot mdot(&mdotSerial, &debugSerial);

/*--- setup() --------------------------------------------------------------------------
  Called by the Arduino framework once, before the main loop begins.

  In the setup() routine:
   - Opens serial communication with MDOT
  --------------------------------------------------------------------------------------*/
void setup() {
  int responseCode;
  /* begins a serial communication of a hardware serial */
  debugSerial.begin(38400);             
  /* begins a serial communication of a software serial */
  mdotSerial.begin(38400);
  
  debugSerial.println("Joining Altoview...");
   
  lcd.begin(16, 2);                                     // set up the LCD's number of columns and rows:
  lcd.print("Joining Altoview");                        // Print a message to the LCD.
  lcd.setCursor(0, 1);                                  // move curser to column 0, row 1
  mdot.begin();							

  int loopCount = 0; 
  do {
	/* attempt to join to Altoview */
    responseCode = mdot.join();
    lcd.print(responseCode);
    lcd.print(",");
    ++loopCount;
    if (loopCount > 4) { 
      lcd.print("                ");                        // clear this row
      lcd.setCursor(0, 1);                                  // move curser to column 0, row 1
    }
	  /* waiting for the join process to finish. */
    delay(10000);
  } while (responseCode != 0);
  
  lcd.setCursor(0, 0);
  lcd.print("Joined.         ");
}

/*--- loop() ---------------------------------------------------------------------------
  Main loop called by the Arduino framework
  --------------------------------------------------------------------------------------*/
int loopNum = 0;
char msg[30];                                           // RSSI:XXX SNR:XX
char snr[4];                                            // XX.X
void loop() {
  int responseCode;
  /* send the loop count in the JSON format to Altoview */
  lcd.setCursor(0, 1);
  lcd.print("                ");
  lcd.setCursor(0, 0);
  lcd.print("Sending Pairs   ");
  responseCode = mdot.sendPairs("L:" + String(loopNum));
  delay(10000);                                         // delay for 10 seconds to give mDot time to send
  if (responseCode == 0){
    mdot.ping(); 
    dtostrf(mdot.snr, 4, 1, snr);                       // needed to do this because sprintf does not support floats in arduino
    sprintf(msg, "R:%d,S:%s",mdot.rssi,snr); 
    debugSerial.println(msg);  
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    //delay(1000);
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
  }else{
    debugSerial.println("error in sending pairs");
    lcd.setCursor(0, 1);
    lcd.print("error           ");
  }
  delay(10000);						
  loopNum++;
}
