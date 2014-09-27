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
  radio.begin()
  // setup retries
  radio.setRetries(15,15);
  //payload size
  radio.setPayloadSize(8);
  
  // Setup 2 pipes
  radio.openWritingPipe(pipes[0]);
  radio.openReadingPipe(1,pipes[1]);
  
