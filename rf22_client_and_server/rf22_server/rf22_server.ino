// rf22_server.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing server
// with the RF22 class. RF22 class does not provide for addressing or reliability.
// It is designed to work with the other example rf22_client

#include <SPI.h>
#include <RF22.h>

// Singleton instance of the radio
RF22 rf22(8);
/*  SDI  :  11
 *  SDO  :  12
 *  SCK  :  13
 *  CSN  :  8
 *  IRQ  :  2
 */

void setup() 
{
  Serial.begin(115200);
  while (!rf22.init())
  {
    Serial.println("SERVER: RF22 init failed");
    // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
    delay(1000);
  }  
  Serial.println("SERVER: RF22 init succeeded");
  if (!rf22.setFrequency(434.0))
  {
    Serial.println("test_rx: setFrequency failed");
  } 
  if (!rf22.setModemConfig(RF22::GFSK_Rb2Fd5))
  {
    Serial.println("test_rx: setModemConfig failed");
  }
}

void loop()
{
  Serial.println("######################################");
  Serial.println("SERVER: Waiting for data from CLIENT...");
  
  rf22.waitAvailable();    // wait for RF22 to be available
  
  Serial.println("SERVER: Got data");
  //if (rf22.available())
  {
    // Should be a message for us now
    uint8_t buf[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);
    if (rf22.recv(buf, &len))
    {
      Serial.print("SERVER: got request: ");
      Serial.println((char*)buf);
      
      // Send a reply
      uint8_t data[] = "And hello back to you";
      rf22.send(data, sizeof(data));
      Serial.println("here");
      rf22.waitPacketSent();
      Serial.println("here2");
      Serial.println("SERVER: Sent a reply");
    }
    else
    {
      Serial.println("SERVER: recv failed");
    }
  }
}

