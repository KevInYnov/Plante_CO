#include "Adafruit_MQTT.h"
#include "Adafruit_MQTT_Client.h"

/************************* Configuration *************************************/
// Connexion Adafruit
#define IO_SERVER         "SECRET"
#define IO_SERVERPORT     1883
#define IO_USERNAME       "SECRET"
#define IO_KEY            "SECRET" 
// Feeds
#define FEED_POMPE        "/feeds/pompe"
#define FEED_MIN_HUMIDITY "/feeds/minhumidity"
#define FEED_MAX_HUMIDITY  "/feeds/maxhumidity"
#define FEED_HUMIDITY_1  "/feeds/humidity-1"
#define FEED_HUMIDITY_2  "/feeds/humidity-2"

/************************** Variables ****************************************/
// Instanciation du client WiFi qui servira à se connecter au broker Adafruit
WiFiClient client;
// Instanciation du client Adafruit avec les informations de connexion
Adafruit_MQTT_Client MyAdafruitMqtt(&client, IO_SERVER, IO_SERVERPORT, IO_USERNAME, IO_USERNAME, IO_KEY);
// Variable de stockage de la valeur du slider
int Humidity1 = 50;
int Humidity2 = 50;
uint32_t FEED_FREQ = 10;
bool Pompe = false;
Ticker MyAdafruitTicker;

/****************************** Feeds ****************************************/
// Création des Feed auxquels nous allons souscrire :
Adafruit_MQTT_Subscribe GetPompe = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_POMPE, MQTT_QOS_1);

Adafruit_MQTT_Subscribe MinHum = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_MIN_HUMIDITY, MQTT_QOS_1);

Adafruit_MQTT_Subscribe MaxHum = Adafruit_MQTT_Subscribe(&MyAdafruitMqtt, IO_USERNAME FEED_MAX_HUMIDITY, MQTT_QOS_1);

Adafruit_MQTT_Publish PushPompe = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_POMPE);

Adafruit_MQTT_Publish Hum1 = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_HUMIDITY_1);

Adafruit_MQTT_Publish Hum2 = Adafruit_MQTT_Publish(&MyAdafruitMqtt, IO_USERNAME FEED_HUMIDITY_2);

/*************************** Callback ************************************/

/**
 * Callback associée au Slider présent sur le dashboard
 */
void MaxHumidityCallBack(double uiMaxHum) {
  MYDEBUG_PRINT("-AdafruitIO : Humidity Max : ");
  MYDEBUG_PRINTLN(uiMaxHum);
  }

  void MinHumidityCallBack(double uiMinHum) {
  MYDEBUG_PRINT("-AdafruitIO : Humidity Min : ");
  MYDEBUG_PRINTLN(uiMinHum);
  }
/**
 * Callback associée à l'interrupteur sur le dashboard
 */
void PompeCallBack(char *data, uint16_t len) {
  MYDEBUG_PRINT("-AdafruitIO : Callback of Pompe : ");
  if (!strcmp(data, "ON")){
    MYDEBUG_PRINTLN("-AdafruitIO : J'allume");
    Pompe = true;
    
  }
  else { 
    MYDEBUG_PRINTLN("-AdafruitIO : J'éteins");
    Pompe = false;
  }
}

/************************** Simulation ****************************************/
void SendHumityToAdafruit(){
  // Convertissez le pointeur en double
  double minHumValue = atof((char *)MinHum.lastread); // Accédez à la dernière valeur lue de MinHum
  double maxHumValue = atof((char *)MaxHum.lastread); // Accédez à la dernière valeur lue de MaxHum
  
  if(!Pompe){
    FEED_FREQ = 10;
    Humidity1 -= 5;
    Humidity2 -= 5;
    if(Humidity1 < minHumValue || Humidity2 < minHumValue){
       MYDEBUG_PRINTLN("Sécuriter activé !");
       PushPompe.publish("ON"); 
       MYDEBUG_PRINTLN("Pompe ON");
    }
  }
  else{
    FEED_FREQ = 5;
    Humidity1 += 2;
    Humidity2 += 6;
    if(Humidity1 > maxHumValue || Humidity2 > maxHumValue){
       MYDEBUG_PRINTLN("Sécuriter activé !");
       PushPompe.publish("OFF");
       MYDEBUG_PRINTLN("Pompe OFF");
    }
  }

  // Envoi des données au Broker
  MYDEBUG_PRINTLN("AdafruitIO : SEND DATA");
  Hum1.publish(Humidity1);
  Hum2.publish(Humidity2);
}

/************************** Setup ****************************************/
void setupAdafruitIO() {
  PushPompe.publish("OFF");
  // Souscription aux FEEDs
  MyAdafruitMqtt.subscribe(&GetPompe);
  MyAdafruitMqtt.subscribe(&MinHum);
  MyAdafruitMqtt.subscribe(&MaxHum);
  // Configuration des callbacks pour les FEEDs auxquels on veut souscrire
  GetPompe.setCallback(PompeCallBack);
  MinHum.setCallback(MinHumidityCallBack);
  MaxHum.setCallback(MaxHumidityCallBack);

  MyAdafruitTicker.attach(FEED_FREQ, SendHumityToAdafruit);
}

/************************** Connection au broker ****************************************/
void connectAdafruitIO() {
  if (MyAdafruitMqtt.connected()) { return; }                      // Si déjà connecté, alors c'est tout bon

  MYDEBUG_PRINT("-AdafruitIO : Connexion au broker ... ");
  int8_t ret;
  while ((ret = MyAdafruitMqtt.connect()) != 0) {                  // Retourne 0 si déjà connecté
     MYDEBUG_PRINT("[ERREUR : ");
     MYDEBUG_PRINT(MyAdafruitMqtt.connectErrorString(ret));
     MYDEBUG_PRINT("] nouvelle tentative dans 10 secondes ...");
     MyAdafruitMqtt.disconnect();                                  // Deconnexion pour être propre
     delay(10000);                                                 // On attend 10 secondes avant de retenter le coup
  }
  MYDEBUG_PRINTLN("[OK]");
}

/************************** Loop ****************************************/
/**
 * Boucle Adafruit IO
 * - Vérification de l'état de la connexion
 * - Traitement des messages reçus
 * - Maintien de la connexion en vie avec un Ping si on aucun publish télémétrie n'est fait
 */
void loopAdafruitIO() {
  connectAdafruitIO(); // Vérification de l'état de la connexion
  MyAdafruitMqtt.processPackets(10000); // Traitement des messages reçus
  if(! MyAdafruitMqtt.ping()) { // Ping 
    MyAdafruitMqtt.disconnect(); // Deconnexion
  }
}
