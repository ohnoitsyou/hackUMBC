#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10);

const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

void setup(void) {
  Serial.begin(57600);
  printf_begin();

  radio.begin();
  radio.setRetries(15,15);
  radio.setPayloadSize(8);
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);

  radio.printDetails();
}

void loop(void) {
  String rgbStr;
  if(Serial.available()) {
    while(Serial.available() > 0) {
      char c = Serial.read();
      rgbStr += c;
      delay(3);
    }
    Serial.println(rgbStr);
    // I'm not going to do any verification
    // this time, i'm just going to forward
    // from the serial to the radio.
    radio.stopListening();
    printf("Sending payload... ");
    bool ok = radio.write(&rgbStr,sizeof(rgbStr));
    if(ok) {
      printf("ok...");
    } else {
      printf("failed...");
    }
    radio.startListening();
    unsigned long started_waiting_at = millis();
    bool timeout = false;
    while(! radio.available() && ! timeout) {
      if(millis() - started_waiting_at > 500) {
        timeout = true;
      }
    }

    if(timeout) {
      printf("Failed, timed out\n\r");
    } else {
      String resp;
      radio.read(&resp, 11);
      printf("Got response");
      Serial.println(resp);
    }
  }
}
