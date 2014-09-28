/*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>

 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example RF Radio Ping Pair
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two different nodes,
 * connect the role_pin to ground on one.  The ping node sends the current time to the pong node,
 * which responds by sending the value back.  The ping node can then see how long the whole cycle
 * took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"

//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0xF0F0F0F0E1LL, 0xF0F0F0F0D2LL };

// setup our variables for our colors
String red, grn, blu;
void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(57600);
  printf_begin();

  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  radio.setPayloadSize(15);

  //
  // Open pipes to other nodes for communication
  //
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);

  //
  // Start listening
  //

  radio.startListening();

  //
  // Dump the configuration of the rf unit for debugging
  //

  radio.printDetails();

  // setup the led's
  pinMode(3,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);

  // initilize them to off
  analogWrite(3,255);
  analogWrite(5,255);
  analogWrite(6,255);
}

void loop(void)
{
  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      String rgbStr;
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &rgbStr,11);

        // Spew it
        Serial.print("Got payload...");
        Serial.println(rgbStr);

        int redSep = rgbStr.indexOf(':');
        int grnSep = rgbStr.indexOf(':',redSep + 1);

        // Make sure that there are at least two seperators,
        // I should make sure there isn't a 3rd.
        // For now I'll trust my input
        if(redSep != -1 && grnSep != -1) {
          red = rgbStr.substring(0,redSep);
          grn = rgbStr.substring(redSep + 1,grnSep);
          blu = rgbStr.substring(grnSep + 1);

          analogWrite(3, red.toInt());
          analogWrite(5, grn.toInt());
          analogWrite(6, blu.toInt());
        }

	      // Delay just a little bit to let the other unit
	      // make the transition to receiver
	      delay(20);
      }

      // First, stop listening so we can talk
      radio.stopListening();

      // Send the final one back.
      radio.write( &rgbStr, 11);
      printf("Sent response.\n\r");

      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
}
// vim:cin:ai:sts=2 sw=2 ft=cpp
