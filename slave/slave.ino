
#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

RF24 radio(9,10);
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };
// setup our variables for our colors
String red, grn, blu;
void setup(void)
{
  Serial.begin(57600);
  printf_begin();
  radio.begin();
  radio.setRetries(15,15);
  radio.setPayloadSize(8);
  radio.openWritingPipe(pipes[1]);
  radio.openReadingPipe(1,pipes[0]);

  radio.startListening();

  radio.printDetails();
}

void loop(void)
{
  // if there is data ready
  if ( radio.available() )
  {
    // Dump the payloads until we've gotten everything
    String rgbStr;
    bool done = false;
    while(!done) {
      done = radio.read(&rgbStr,sizeof(rgbStr));
      delay(20);
    }
    radio.stopListening();
    radio.write(&rgbStr,11);
    printf("sent response\n\r");
    radio.startListening();
  }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
