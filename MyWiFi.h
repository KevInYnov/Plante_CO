#include <WiFi.h>        
#include "esp_wpa2.h"


/************************** Variables ****************************************/
// pour le mode ACCESS POINT
const char* ap_ssid     = "SECRET";      // Nom du réseau WiFi pour accéder à mon serveur web
const char* ap_password = "SECRET";        // Mot de passe du réseau WiFi pour accéder à mon serveur web


/************************** Setup ****************************************/
void setupWiFi(){
  MYDEBUG_PRINTLN();
  MYDEBUG_PRINT("-WIFI : Configuration");
  WiFi.mode(WIFI_AP_STA); // Configuration de la carte en mode Access Point ET Station
  WiFi.softAP(ap_ssid, ap_password); // Démarrage du mode Access Point
  MYDEBUG_PRINT("-WIFI : Access Point mis à disposition : "); 
  MYDEBUG_PRINTLN(WiFi.softAPIP()); // Affichage de l'adresse IP de mon point d'accès

}
