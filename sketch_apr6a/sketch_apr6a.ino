#include "Arduino.h"
#include "WiFi.h"
#include "Audio.h"
#include <array>
#include <RotaryEncoder.h>


#define I2S_DOUT 25
#define I2S_BCLK 27
#define I2S_LRC 26

Audio audio;

String ssid = "NETGEAR09";
String password = "icytuba581";

#define PIN_IN1 33
#define PIN_IN2 32

#define PIN_IN3 22
#define PIN_IN4 23

RotaryEncoder encoder(PIN_IN1, PIN_IN2, RotaryEncoder::LatchMode::TWO03);
RotaryEncoder encoder2(PIN_IN3, PIN_IN4, RotaryEncoder::LatchMode::TWO03);


#define MAX7219_DATA 21
#define MAX7219_CLK 18
#define MAX7219_LOAD 19

// Declare and initialize an array of string literals
const char *channels[] = {
  "https://stream-relay-geo.ntslive.net/stream",
  "https://stream-relay-geo.ntslive.net/stream2",
  "https://stream-mixtape-geo.ntslive.net/mixtape4",   // poolside
  "https://stream-mixtape-geo.ntslive.net/mixtape6",   // memory lane
  "https://stream-mixtape-geo.ntslive.net/mixtape2",   // low key
  "https://stream-mixtape-geo.ntslive.net/mixtape27",  // feelings
  "https://stream-mixtape-geo.ntslive.net/mixtape35",  // sheet music
  "https://stream-mixtape-geo.ntslive.net/mixtape3",   // expansions
  "https://stream-mixtape-geo.ntslive.net/mixtape34",  // the pit
  "https://stream-mixtape-geo.ntslive.net/mixtape",    // slow focus
  "https://stream-mixtape-geo.ntslive.net/mixtape22",  // rap house
  "https://stream-mixtape-geo.ntslive.net/mixtape24",  // sweat
  "https://stream-mixtape-geo.ntslive.net/mixtape23",  // field recordings
  "https://stream-mixtape-geo.ntslive.net/mixtape36",  // otaku
  "https://stream-mixtape-geo.ntslive.net/mixtape21",  // island time
  "https://stream-mixtape-geo.ntslive.net/mixtape5",   // 4 to the floor
  "https://stream-mixtape-geo.ntslive.net/mixtape31",  // labyrinth
  "https://stream-mixtape-geo.ntslive.net/mixtape26",  // the tube
  "https://streamer-uk.rinse.fm:8443/stream",
  "https://balamii.out.airtime.pro/balamii_a",
  "https://stream0.wfmu.org/freeform-128k",
  "https://fm939.wnyc.org/wnycfm",
};

// Number of elements in the array
int numChannels = sizeof(channels) / sizeof(channels[0]);

int currentChannel = 0;
void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid.c_str(), password.c_str());

  while (WiFi.status() != WL_CONNECTED) {
    Serial.println("wifi....!");

    delay(1500);
  }

  Serial.println("wifi connected!");

  audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);

  audio.setVolume(5);

  Serial.println("connecting to audio");


  audio.connecttohost(channels[currentChannel]);

  Serial.println("connectieD!!!! audio");
}

void loop() {
  audio.loop();
  static int pos = 0;
  static int pos2 = 0;
  encoder.tick();
  encoder2.tick();

  int newPos = encoder.getPosition();
  //Serial.println(newPos);
  if (pos != newPos) {
    Serial.print("pos:");
    Serial.print(newPos);
    Serial.print(" dir:");
    int8_t dir = (int8_t)(encoder.getDirection());
    Serial.println(dir);
    pos = newPos;
    uint8_t vol = audio.getVolume();
    audio.setVolume(vol + dir);
    Serial.println(vol + dir);
  }  // if

  int newPos2 = encoder2.getPosition();

  if (pos2 != newPos2) {
    Serial.print("pos2:");
    Serial.print(newPos2);
    Serial.print(" dir2:");
    int8_t dir2 = (int8_t)(encoder2.getDirection());
    Serial.println(dir2);
    pos2 = newPos2;
    currentChannel = currentChannel + dir2;
    if (currentChannel < 0) {
      currentChannel = numChannels - 1;
    } else if (currentChannel >= numChannels) {
      currentChannel = 0;
      //      audio.connecttohost(channels)
    }
    Serial.print("selecting channel: ");
    Serial.println(currentChannel);
    audio.connecttohost(channels[currentChannel]);
  }
}
