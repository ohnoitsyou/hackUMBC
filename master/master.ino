#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Configure hardware
//

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void) {
  // print Preamble
  Serial.begin(57600);
  printf_begin();


  // Setup the radio
  radio.begin();
  // setup retries
  radio.setRetries(15,15);
  //payload size
  radio.setPayloadSize(15);

  // Setup 2 pipes
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.printDetails();
}

void loop(void) {
  // We don't need to be listening, normally.
  radio.stopListening();
  String rgbStr;
  char character;
  if(Serial.available() > 0) {
    while(Serial.available() > 0) {
      character = Serial.read();
      rgbStr += character;
      delay(3);
    }
    Serial.println(rgbStr);
    // Serial.println(rgbStr);
    // break apart the string we got to be R:G:B
    String red, grn, blu;
    int redSep = rgbStr.indexOf(':');
    int grnSep = rgbStr.indexOf(':',redSep + 1);

    // Make sure that there are at least two seperators,
    // I should make sure there isn't a 3rd.
    // For now I'll trust my input
    if(redSep != -1 && grnSep != -1) {
      red = rgbStr.substring(0,redSep);
      grn = rgbStr.substring(redSep + 1,grnSep);
      blu = rgbStr.substring(grnSep + 1);

      while(red.length() < 3){
        red = '0' + red;
      }

      while(grn.length() < 3) {
        grn = '0' + grn;
      }
      while(blu.length() < 3) {
        blu = '0' + blu;
      }

      Serial.print(red);
      Serial.print(grn);
      Serial.println(blu);

      rgbStr = red + ":" + grn + ":" + blu;

      radio.stopListening();
//      printf("writing string to radio... ");
      bool ok = radio.write(&rgbStr,11);
      if(ok) {
        printf("ok...");
      } else {
//        printf("failed.\n\r");
      }
      radio.startListening();

      unsigned long started_waiting_at = millis();
      bool timeout = false;
      while(! radio.available() && ! timeout) {
        if(millis() - started_waiting_at > 500) {
          timeout = true;
        }
 //       Serial.print("in timeout Loop");
      }
      if(timeout) {
   //     printf("Failed, response timed out.\n\r");
      } else {
        String resp;
        radio.read(&resp, 11);
        Serial.print("Got response ");
  //      Serial.println(resp);
        if(resp == rgbStr) {
//          Serial.println("they are the same");
        }
      }
    }
  }
}
