#include <Wire.h>
#include "HT_SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <RadioLib.h>
#include "GPS_Air530Z.h"
#define RADIO_BOARD_AUTO
#include <RadioBoards.h>
Radio radio = new RadioModule();

Air530ZClass GPS;

SSD1306Wire  display(0x3c, 500000, SDA, SCL, GEOMETRY_128_64, GPIO10); // addr , freq , SDA, SCL, resolution , rst

int transmissionState = RADIOLIB_ERR_NONE;
//Change syncword like you change password for different websites - TH
uint8_t lrfhssSyncWord[4] = {0x2C, 0x0F, 0x79, 0x95};

// flag to indicate transmission or reception state
bool transmitFlag = false;

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;

// flag to indicate that a packet was received
volatile bool receivedFlag = false;

//This is used to get coordinates to 4 Decimal places from the GPS module on the mock Ground Station module
int fracPart(double val, int n)
{
  return (int)((val - (int)(val))*pow(10,n));
}

void setFlag(void) {
  // we sent or received a packet, set the flag
  operationDone = true;
  receivedFlag = true;
}

void VextON(void)
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, LOW);
}

void VextOFF(void) //Vext default OFF
{
  pinMode(Vext,OUTPUT);
  digitalWrite(Vext, HIGH);
}
// Global public variable for easy setting of frequency in case of multiple functions wishing to access this variable. 
// For instance, if a doppler effect calculation function wishes to determine offset needed, or if another function wishes to access the current frequency 
float currentFreq = 923.0;

void setup() {
  VextON();
  Serial.begin(115200);
  Serial.print(F("[TH Device] Starting up..."));

  // Starting the display mechanism and GPS, display.display() is used to set the cache of the build-in OLED display
  display.init();
  display.clear();
  display.display();
  
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 32-16/2, "GPS Initing");
  display.display();
  GPS.begin();
  
  delay(1000);

  
  // Order of variables: Frequency, bandwidth, spreading factor, coding rate, sync word and power (max is 22) other varialbes onward include preamble length [not needed ]
  int state = radio.begin(currentFreq, 250.0, 12,7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 20);
  
  
  radio.setFrequency(currentFreq);
  radio.setCurrentLimit(140);
  
  // set the function that will be called
  // when new packet is received
  radio.setPacketReceivedAction(setFlag);
  display.clear();
  display.drawString(64, 32-16/2, "Listening");
  display.display();
  state = radio.startReceive();

}

void loop() {
  while (GPS.available() > 0)
    {
      GPS.encode(GPS.read());
    }
  
  if(receivedFlag){
    receivedFlag = false;

    String msg;
    int state = radio.readData(msg);

    char UI[30];
    sprintf(UI, "Rx: %s", msg);

    char rssi[30];
    sprintf(rssi, "RSSI: %f", radio.getRSSI());

    char snr[30];
    sprintf(snr, "SNR: %f", radio.getSNR());

    

    if(state == RADIOLIB_ERR_NONE){
      display.clear();
      display.drawString(30,0,msg);
      display.drawString(10,10,rssi);
      display.drawString(10,20,snr);
      display.display();
      }

      if(msg == "LRFHSS"){
        radio.reset();
        radio.beginLRFHSS(currentFreq, RADIOLIB_SX126X_LR_FHSS_BW_722_66, RADIOLIB_SX126X_LR_FHSS_CR_1_3, false, 20);
        radio.setSyncWord(lrfhssSyncWord, 4);
        radio.setCurrentLimit(140);
        char lati[30];
        sprintf(lati, "Latitude: %d, Longitude:%d",fracPart(GPS.location.lat(),4), fracPart(GPS.location.lng(),4));
        
        radio.transmit(lati);
        delay(100);

        display.clear();
        display.drawString(10,0,"Sending GPS yay");
        display.display();
        radio.reset();
        radio.begin(currentFreq, 250.0, 12,7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 16);
        radio.setCurrentLimit(140);
        radio.startReceive();
        
        }else if(msg == "LORA"){
        char lati[60];
        sprintf(lati, "Latitude: %d, Longitude: %d",fracPart(GPS.location.lat(),4),fracPart(GPS.location.lat(),4));

        
        radio.transmit(lati);
        delay(100);

        display.clear();
        display.drawString(10,0,"Sending GPS LoRa");
        display.display();

        delay(100);
         radio.startReceive();
          }
    
    }
  
}
