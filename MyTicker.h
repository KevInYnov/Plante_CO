#include <Ticker.h>

/************************** Variable ****************************************/
Ticker myTicker;
int count = 0;
unsigned long myTickerTime;

/************************** Fonction ****************************************/
void tickerFunction() {
  MYDEBUG_PRINT("-TICKER [");
  MYDEBUG_PRINT(count);
  MYDEBUG_PRINT("] Depuis la dernière fois :");
  MYDEBUG_PRINT(micros()-myTickerTime);
  MYDEBUG_PRINTLN(" us (micro secondes)");
  myTickerTime = micros();
  count++;
}

/************************** Setup ****************************************/
void setupTicker(){
  MYDEBUG_PRINTLN("-TICKER : Initialisation d'un ticker toutes les 2 minutes");
  myTicker.attach(120, tickerFunction);         // Association du Ticker avec une fonction appelée toutes les 2 minutes
  myTickerTime = micros();                     // J'enregistre l'heure en nombre de us
}
