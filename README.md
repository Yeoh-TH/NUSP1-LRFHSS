# NUSP1-LRFHSS Introduction of code firmware and usage
<img width="250" height="250" alt="image" align=”left”  src="https://github.com/user-attachments/assets/9c87c69d-6ad7-4aee-be0a-95b3782a0c44" />

Successful implementation of LRFHSS communication with the Semtech LRFHSS gateway V2. This sketch shows a mock Ground station and mock satellite system, where in the serial monitor of the satellite, if a command is sent by the user, the exact message is transmitted and processed in the ground station. All done on a Heltech Cubecell AB-02S module.

The process has been streamlined through the integration of multiple libraries in the same firmware. Code has been written for processing transmissions and sending and receiving data through the display and the serial monitor for the ground station and mock satellite program, respectively, in the Arduino IDE.

# Variables to look out for
The main variable to tweak if needed, are the following:

```
float currentFrequency = 932.0;
```
This is a floating-point variable for the frequency at which the user wants the modules to communicate at. The current default frequency is set at 923.0MHz, corresponding to the AS923 band, and the receiving frequency of the gateway. The user may change it to whatever frequency he or she desires, from 150MHz to 960MHz, which are the specifications of Semtech's SX1262 module, built-in in the Heltech Cubecell AB02S

## Other SX1262 Variables
These are the available bandwidths and coding rates for the LRFHSS function (currently there is a glitch with the module preventing it from using the bandwidth of 39.06kHz):
#define RADIOLIB_SX126X_LR_FHSS_CR_5_6                          (0x00UL << 0)   //  7     0     LR FHSS coding rate: 5/6
#define RADIOLIB_SX126X_LR_FHSS_CR_2_3                          (0x01UL << 0)   //  7     0                          2/3
#define RADIOLIB_SX126X_LR_FHSS_CR_1_2                          (0x02UL << 0)   //  7     0                          1/2
#define RADIOLIB_SX126X_LR_FHSS_CR_1_3                          (0x03UL << 0)   //  7     0                          1/3
#define RADIOLIB_SX126X_LR_FHSS_MOD_TYPE_GMSK                   (0x00UL << 0)   //  7     0     LR FHSS modulation: GMSK
#define RADIOLIB_SX126X_LR_FHSS_GRID_STEP_FCC                   (0x00UL << 0)   //  7     0     LR FHSS step size: 25.390625 kHz (FCC)
#define RADIOLIB_SX126X_LR_FHSS_GRID_STEP_NON_FCC               (0x01UL << 0)   //  7     0                        3.90625 kHz (non-FCC)
#define RADIOLIB_SX126X_LR_FHSS_HOPPING_DISABLED                (0x00UL << 0)   //  7     0     LR FHSS hopping: disabled
#define RADIOLIB_SX126X_LR_FHSS_HOPPING_ENABLED                 (0x01UL << 0)   //  7     0                      enabled
#define RADIOLIB_SX126X_LR_FHSS_BW_39_06                        (0x00UL << 0)   //  7     0     LR FHSS bandwidth: 39.06 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_85_94                        (0x01UL << 0)   //  7     0                        85.94 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_136_72                       (0x02UL << 0)   //  7     0                        136.72 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_183_59                       (0x03UL << 0)   //  7     0                        183.59 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_335_94                       (0x04UL << 0)   //  7     0                        335.94 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_386_72                       (0x05UL << 0)   //  7     0                        386.72 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_722_66                       (0x06UL << 0)   //  7     0                        722.66 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_773_44                       (0x07UL << 0)   //  7     0                        773.44 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_1523_4                       (0x08UL << 0)   //  7     0                        1523.4 kHz
#define RADIOLIB_SX126X_LR_FHSS_BW_1574_2                       (0x09UL << 0)   //  7     0                        1574.2 kHz

gnerally the smaller the bandwidth and coding rate, the greater signal strength of the received packet.

# GPS
The GPS module on this module is the Air530ZClass. Based on experimentation, this module will be able to receive GPS data in an open football field in around 2 to 3 minutes or less. Thus, it requires little fuss to operate and receive GPS. The following GPS data can be interfaced:

```
Air530ZClass GPS;
//This is a mathematical function to return any variable with a certain number of decimal points by the request of the user. This is needed for the precision of the GPS data, which is around 4 decimal points, even though Arduino rounds it to a whole number
int fracPart(double val, int n){
  return (int)((val - (int)(val))*pow(10,n));
}
```
The following measurements may be of use to you, you may pick and choose what variables you would like to display on  the OLED display. 
For our SGP4 satellite tracking implementation in the future, only altitude, latitude and longitude are needed variables.
```
GPS.altitude.meters() 
GPS.hdop.hdop() // Horizontal Dilution of Precision
GPS.location.lat()
GPS.location.lng()
GPS.speed.kmph()
```

