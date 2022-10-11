#include <SPI.h>
#include <Adafruit_VS1053.h>
#include <SD.h>

// These are the pins used for the music maker shield
#define SHIELD_RESET  -1     // VS1053 reset pin (unused!)
#define SHIELD_CS     7      // VS1053 chip select pin (output)
#define SHIELD_DCS    6      // VS1053 Data/command select pin (output)
#define CARDCS        4      // SD Card chip select pin (output)
#define DREQ          3      // VS1053 Data request (interupt pin)
// For Reference the following pins are used for Hardware SPI
// CLK 24             SPI Clock, shared with SD card
// MISO 22            Input data, from VS1053/SD card
// MOSI 23            Output data, to VS1053/SD card

// These are the pins used for PIR sensors
#define RIGHT_EYE  5      // The Right Eye will detect someone arriving before the Left Eye does
#define LEFT_EYE   2

Adafruit_VS1053_FilePlayer audio = Adafruit_VS1053_FilePlayer(SHIELD_RESET, SHIELD_CS, SHIELD_DCS, DREQ, CARDCS);
bool previousDetectionState_Right = false;
bool previousDetectionState_Left = false;
  
void setup() {
  pinMode(RIGHT_EYE, INPUT);
  pinMode(LEFT_EYE, INPUT);
 
  delay(5000);
  Serial.begin(9600);
  Serial.println("Welcomator debugger messaging...");

  if (! audio.begin()) { // initialise the music player
     Serial.println(F("Couldn't find Music Shield, do you have the right pins defined?"));
     while (1);
  }
  Serial.println(F("Music Shield found"));
  
   if (!SD.begin(CARDCS)) {
    Serial.println(F("SD Card failed, or not present"));
    while (1);  // don't do anything more
  }
 
  // Set volume for left, right channels. lower numbers == louder volume!
  audio.setVolume(20,20);

  // If DREQ is on an interrupt pin (on uno, #2 or #3) we can do background
  // audio playing
  audio.useInterrupt(VS1053_FILEPLAYER_PIN_INT);  // DREQ int
}

void loop() {

  // Check Eye Detection State
  bool currentDetectionState_Right = digitalRead(RIGHT_EYE);
  bool currentDetectionState_Left = digitalRead(LEFT_EYE);
  
  if (currentDetectionState_Right && !previousDetectionState_Right)
  {
    audio.startPlayingFile("/ill_be.mp3");
  }

  if (currentDetectionState_Left && !previousDetectionState_Left)
  {
    audio.startPlayingFile("/back.mp3");
  } 

  previousDetectionState_Right = currentDetectionState_Right;
  previousDetectionState_Left = currentDetectionState_Left;

  delay(5000);
}