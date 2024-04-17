#define MYDEBUG         1

/************************** Variables ****************************************/
int i = 0;            // pour la loop
bool NtpInitialized = false; 
bool adafruitIOInitialized = false;

/************************** Modules ****************************************/
// MODULES
#include "MyDebug.h"        // Debug
#include "MyWiFi.h"         // WiFi
#include "MyWebServer.h"    // Serveur Web
#include "MyNTP.h"          // Network Time Protocol
#include "MyTicker.h"       // Tickers
#include "MyAdafruitIO.h"   // Adafruit MQTT
// ------------------------------------------------------------------------------------------------


/************************** Setup ****************************************/
//  J'adore le cours d'IoT
//  Moi aussi <3
/*************************************************************************/
void setup() {
  setupDebug();
  MYDEBUG_PRINTLN("------------------- SETUP");
  setupWiFi();        // Initialisation du WiFi
  setupWebServer();   // Initialisation du Serveur Web
  setupTicker();      // Initialisation d'un ticker
}

/************************** Loupe ****************************************/
void loop() {
  loopWebServer();
  loopAdafruitIO();
  if (WiFi.status() == WL_CONNECTED){ // Verification de la connexion
    if (!adafruitIOInitialized) { // Verification de l'initialisation du broker
      setupAdafruitIO();
      adafruitIOInitialized = true;
    }
    if(!NtpInitialized){  // Verification de l'initialisation du NTP
      setupNTP();         // Initialisation de la connexion avec le serveur NTP (heure)
      getNTP();           // Récupération de l'heure
      NtpInitialized = true;
    }
  }
  delay(10);        // Délai pour que le CPU puisse passer à d'éventuelles autres tâches
}
