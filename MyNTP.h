#include <NTPClient.h>
#include <WiFiUdp.h>

/************************** Variable ****************************************/
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP);

/************************** Fonction ****************************************/
void getNTP(){ // Récuperer l'heure
  MYDEBUG_PRINT("-NTP : ");
  timeClient.update(); // mise à jour de l'heure
  MYDEBUG_PRINTLN(timeClient.getFormattedTime()); // Affichage de l'heure

}

/************************** Setup ****************************************/
void setupNTP(){
  timeClient.begin();
  timeClient.setTimeOffset(7200); // Configuration heure (7200s = 2h) GMT+2
}
