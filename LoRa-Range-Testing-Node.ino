
/*
  This is an example program writen for the Seeeduino Stalker v2.3 and
  uses the Multitech mDOT LoRa module running the Australian compatable AT
  enabled firmware.

  It also makes use of a nokia5110 lcd screen to display data to display data
  as well as a rotary encoder to manually adjust the data rate of the mdot.

  This program,
    Joins the LoRa Network
    Sends loop count
    Gets the RSSI and SNR of the last packet sent and displays the data on
    the lcd screen.

    Turning the rotary encoder knob clockwise will increment the data rate.
    Turning the knob counter-clockwise will decrement the data rate
*/

/*--------------------------------------------------------------------------------------
  Includes
  --------------------------------------------------------------------------------------*/
#include <LoRaAT.h>                              //Include LoRa AT libraray
#include <PCD8544.h>

/*--------------------------------------------------------------------------------------
  Definitions
  --------------------------------------------------------------------------------------*/
LoRaAT mdot;
static PCD8544 lcd;

//Rotary encoder variables:
int val = 1;
int lastVal = 0;
int pos = 2;
int pos_old = 2;
int clk_pin = 2;
int dt_pin = 9;
int ledPin = 8;

/*--- setup() --------------------------------------------------------------------------
  Called by the Arduino framework once, before the main loop begins.

  In the setup() routine:
   - Opens serial communication with MDOT
  --------------------------------------------------------------------------------------*/
void setup() {
  // PCD8544-compatible displays may have a different resolution...
  lcd.begin(84, 48);                             //Setup the lcd screen
  lcd.print(F("j:"));                 //Display setup msg on lcd screen

  int responseCode;                              //Response of mDot commands
  delay(1500);
  mdot.begin();                             //Opens serial comms with MDOT

  do {
    delay(1500);
    responseCode = mdot.join();                  //Send Join Request to LoRa server
    lcd.print(responseCode);
    lcd.print(", ");
    delay(10000);
  } while (responseCode != 0);

  // rotary encoder setup:
  pinMode(clk_pin, INPUT); //Rotary Encoder pin CLK
  pinMode(dt_pin, INPUT); //Rotary encoder pin DT
  pinMode(ledPin, OUTPUT);

  attachInterrupt(digitalPinToInterrupt(clk_pin), updatePot, FALLING);
  mdot.getDataRate();                           //update the data rate to match what is used in the LoRat library setDefault() function
}


/*--- loop() ---------------------------------------------------------------------------
  Main loop called by the Arduino framework
  --------------------------------------------------------------------------------------*/
int loopNum = 0;
int8_t responseCode;                              //Response code from the mdot
boolean state = false; 
void loop() {
  /*---------------NB-------------------------------------------------
    too many setCursor's in main loop causes stack overflow. be careful.
    --------------------------------------------------------------------
  */
  lcd.clear();
  lcd.print(F("sp: "));
  char cmd[4] = "L:";                       //cmd = {'L',':','#','#'}
  cmd[2] = char(loopNum);
  cmd[3] = "\0";
  responseCode = mdot.sendPairs(cmd);
  delay(5000);

  if (responseCode != 0) {
    lcd.print(F("err"));
    delay(1500);                              //delay for 1.5s so the msg can be seen
  } else {
    lcd.print(F("ok"));
    delay(1500);                              //delay for 1.5s so the msg can be seen
    loopNum++;
    mdot.ping();
    lcd.clear();

    lcd.print(F("rssi: "));
    lcd.print(mdot.rssi);
    lcd.setCursor(0, 2);
    lcd.print(F("snr: "));
    lcd.print(mdot.snr);
  }

  if (pos_old != pos) {                 //the rotary encoder has changed
    updateDataRate();
    pos_old = pos;
  }
  lcd.setCursor(0, 4);
  dispDataRate();
  delay(10000);                         //delay so lcd messages can be read
}

void updatePot() {
  digitalWrite(ledPin, (state) ? HIGH : LOW); 
  state = !state; 
  //read rotary encoder
  pos += digitalRead(dt_pin) ? 1 : -1 ;
  if (pos > 10) {
    pos = 10;
  } else if (pos < 0) {
    pos = 0;
  } 
}

void updateDataRate() {
  uint8_t cmd = 0;
  if (0 <= pos <= 4) {
    cmd = pos;
  } else if (5 <= pos <= 10) {
    cmd = pos + 3;                   //plus 3 to skip missing (not used) data rates 5-7
  } else {
    lcd.print(F("err3"));
    responseCode = -1;
  }

  responseCode = mdot.setDataRate(cmd);
  if (responseCode != 0) {
    lcd.print(F("err1"));
  } else {
    lcd.print(F("ok"));
  }
}

void dispDataRate() {
  lcd.print(F("dr: "));
  responseCode = mdot.getDataRate();
  if (responseCode != 0) {
    lcd.print(F("err2"));
  } else {
    lcd.print(mdot.dataRate);
  }
}


