#include <Wire.h>
#include "HT_SSD1306Wire.h" // legacy include: `#include "SSD1306.h"`
#include <RadioLib.h>
#define RADIO_BOARD_AUTO
#include <RadioBoards.h>
Radio radio = new RadioModule();

SSD1306Wire  display(0x3c, 500000, SDA, SCL, GEOMETRY_128_64, GPIO10); // addr , freq , SDA, SCL, resolution , rst

int transmissionState = RADIOLIB_ERR_NONE;
// flag to indicate transmission or reception state
bool transmitFlag = false;

// flag to indicate that a packet was sent or received
volatile bool operationDone = false;

void setFlag(void) {
  // we sent or received a packet, set the flag
  operationDone = true;
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

float currentFreq = 923.0;
String msg;
String out;

void setup() {
  VextON();
  Serial.begin(115200);
  Serial.println("Starting up");

  display.init();
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(64, 32-16/2, "Mock Sat Operational");
  display.display();

  int state = radio.begin(currentFreq, 250.0, 12,7, RADIOLIB_SX126X_SYNC_WORD_PRIVATE, 20);
  radio.setCurrentLimit(140);
  if (state == RADIOLIB_ERR_NONE) {
    Serial.println(F("success!"));
  } else {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true) { delay(10); }
  }

  
}

void loop() {
  if(Serial.available()){
    msg = Serial.readStringUntil('\n');
    msg.trim();
    
    radio.transmit(msg);

    // Start receive operation after sending out if the command is LORA. Else the module will continue transmission mode.
    // As shown, it must be explicitly stated when the module is in transmission, receiving or sleep mode
    if(msg == "LORA"){
    int state = radio.receive(out);
    if(state==RADIOLIB_ERR_NONE){
    radio.readData(out);
    Serial.print("You got mail, its:");
    Serial.println(out);

    Serial.print(F("RSSI:\t\t"));
    Serial.print(radio.getRSSI());
    Serial.println(F(" dBm"));

    // print the SNR (Signal-to-Noise Ratio)
    // of the last received packet
    Serial.print(F("SNR:\t\t"));
    Serial.print(radio.getSNR());
    Serial.println(F(" dB"));

    // print frequency error
    Serial.print(F("Frequency error (Yea Kai Jun):\t"));
    Serial.print(radio.getFrequencyError());
    Serial.println(F(" Hz"));
    delay(100);
    }
    }
    }
  
}
