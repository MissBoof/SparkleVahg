#include <Arduino.h>
#include <DSpotterSDK_MakerHL.h>


#include "CybLicense_1749396250.h"
#define DSPOTTER_LICENSE g_lpdwLicense

#if defined(TARGET_NANO_RP2040_CONNECT)
#include "Model_1749396250.h"
#endif
#define DSPOTTER_MODEL g_lpdwModel


static DSpotterSDKHL g_oDSpotterSDKHL;


/* 
    The command list at trigger stage:
        Hey Cortana, ID = 100

    The command list at command stage:
        Shields down, ID = 10000
        New Zombie, ID = 10001
        Overshield, ID = 10002
        Headshot, ID = 10003
        Betrayal, ID = 10004
        Kill a man jar oh, ID = 10005                                                                          
        Distraction, ID = 10006
        King of the hill, ID = 10007
        Boof gang, ID = 10008
*/

// LED timing control
bool ledActive = false;

void sendCommand(int commandID) {
  Serial1.println(commandID);
  Serial.print("📤 Sent command ID: ");
  Serial.println(commandID);
}


void VRCallback(int nFlag, int nID, int nScore, int nSG, int nEnergy) {
  if (nFlag == DSpotterSDKHL::GetResult) {
    Serial.print("🎙 Voice command ID: ");
    Serial.println(nID);

    sendCommand(nID); // Transmit any valid voice command
  } else if (nFlag == DSpotterSDKHL::GetError) {
    if (nID == DSpotterSDKHL::LicenseFailed) {
      Serial.print("❌ License failed. Serial #: ");
      Serial.println(DSpotterSDKHL::GetSerialNumber());
    }
    g_oDSpotterSDKHL.Release();
    while (1);
  }
}

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600);  // TX = GPIO0 → Connect to RX on receiver

  while (!Serial && millis() < 3000);

  Serial.println("🎧 Initializing voice recognition...");
  DSpotterSDKHL::ShowDebugInfo(true);
  g_oDSpotterSDKHL.Init(DSPOTTER_LICENSE, sizeof(DSPOTTER_LICENSE), DSPOTTER_MODEL, VRCallback);
  Serial.println("✅ Voice board ready!");

}

void loop() {
  g_oDSpotterSDKHL.DoVR();  // Continuously listen for voice
}


