// rf22_client.pde
// -*- mode: C++ -*-
// Example sketch showing how to create a simple messageing client
// with the RF22 class. RF22 class does not provide for addressing or reliability.
// It is designed to work with the other example rf22_server

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
    Serial.println("CLIENT: RF22 init failed");
    // Defaults after init are 434.0MHz, 0.05MHz AFC pull-in, modulation FSK_Rb2_4Fd36
    delay(1000);
  }  
  Serial.println("CLIENT: RF22 init succeeded");
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
  while (1)
  {
    delay(500);
    Serial.println("CLIENT: Sending to SERVER");
    // Send a message to rf22_server
    uint8_t data[25] = "Hello World!";
    //rf22.send(data, sizeof(data));
    rf22.send(data, strlen((char*)data));
    rf22.waitPacketSent();
    Serial.println("CLIENT: Sent to SERVER");
    
    // Now wait for a reply
    uint8_t buf[RF22_MAX_MESSAGE_LEN];
    uint8_t len = sizeof(buf);

    Serial.println("CLIENT: Waiting for reply from SERVER...");
    rf22.waitAvailable();
    //if (rf22.available())
    { 
      // Should be a message for us now   
      if (rf22.recv(buf, &len))
      {
        Serial.print("CLIENT: got reply: ");
        Serial.println((char*)buf);
      }
      else
      {
        Serial.println("CLIENT: recv failed");
      }
    }
    /*
    else
    {
      Serial.println("CLIENT: No reply, is SERVER running?");
    }
    */
  }
}

