// librairies nécessaires 
#include <WebServer.h>

/************************** Variable ****************************************/
WebServer monWebServeur(80);           // Serveur web sur le port 80

/************************** Page HTML ****************************************/
/*
 * Index
 */
void handleRoot() {
  String out = "";
  out += "<html><head><meta http-equiv='refresh' content='30'/>";
  out += "<title>Wifi Scanner</title>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  out += "</head><body>";
  out += "<h1>Wifi Scanner</h1><br>";
  out += "<ul>";
  out += "<li><a href=\"/scan\">Scanner le WiFi</a></li>";
  out += "</ul>";
  out += "</body></html>";
  monWebServeur.send(200, "text/html", out);
}
/*
 * Scan Wifi
 */
void handleScan() {
  int n = WiFi.scanNetworks();
  String out = "";
  out += "<html><head><meta http-equiv='refresh' content='5'/>";
  out += "<title>Liste des réseaux WiFi</title>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  out += "</head><body>";
  out += "<h1>Liste des réseaux WiFi</h1><br>";
  
  if (n == 0) {
    out += "<p>Aucun réseau WiFi trouvé</p>";
  } else {
    out += "<ul>";
    for (int i = 0; i < n; ++i) {
      out += "<li><a href=\"/ssid?ssid=" + WiFi.SSID(i) + "\">" + WiFi.SSID(i) + "</a></li>";
    }
    out += "</ul>";
  }
  
  out += "</body></html>";
  monWebServeur.send(200, "text/html", out);
}
/*
 * Formulaire de connexion
 */
void handleSSID() {
  String ssid = monWebServeur.arg("ssid");
  String out = "";
  out += "<html><head><meta http-equiv='refresh' content='30'/>";
  out += "<title>Infos WiFi</title>";
  out += "<style>body { background-color: #cccccc; font-family: Arial, Helvetica, Sans-Serif; Color: #000088; }</style>";
  out += "</head><body>";
  out += "<h1>Infos du réseau WiFi</h1><br>";
  out += "<p>SSID: " + ssid + "</p>";
  out += "<label for=\"password\">Mot de passe:</label>";
  out += "<input type=\"text\" id=\"password\" name=\"password\">";
  out += "<br><button onclick=\"connectWiFi()\">Connexion</button>";
  out += "<script>function connectWiFi() {";
  out += "var password = document.getElementById('password').value;";
  out += "window.location.href = '/connect?ssid=" + ssid + "&password=' + password;";
  out += "}</script>";
  out += "</body></html>";
  monWebServeur.send(200, "text/html", out);
}
 /*
  *  Page 404
  */
void handleNotFound() {
  String message = "File Not Found\n\n";
  message += "URI: ";
  message += monWebServeur.uri();
  message += "\nMethod: ";
  message += (monWebServeur.method() == HTTP_GET) ? "GET" : "POST";
  message += "\nArguments: ";
  message += monWebServeur.args();
  message += "\n";
  for (uint8_t i = 0; i < monWebServeur.args(); i++) {
    message += " " + monWebServeur.argName(i) + ": " + monWebServeur.arg(i) + "\n";
  }
  monWebServeur.send(404, "text/plain", message);
}

/************************** Fonction ****************************************/

/*
 * Connexion au wifi en mode station
 */
void handleConnect() {
  String station_ssid = monWebServeur.arg("ssid"); // Récupération du SSID
  String station_password = monWebServeur.arg("password"); // Récupération du MDP
  WiFi.begin(station_ssid.c_str(), station_password.c_str()); // Connexion en mode station
  while (WiFi.status() != WL_CONNECTED) { // Tant que la connexion n'est pas établie
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  monWebServeur.send(200, "text/plain", "Connected to WiFi"); // Renvoie une page HTML pour indiqué depuis le serveur web que la connexion est bien éffectuer
}

/**
 * Initialisation du serveur web
 */
void setupWebServer(){
  MYDEBUG_PRINTLN("-WEBSERVER : Démarrage");
  // Construction des pages du web server
  monWebServeur.on("/", handleRoot);
  monWebServeur.on("/scan", handleScan);
  monWebServeur.on("/ssid", handleSSID);
  monWebServeur.on("/connect", handleConnect); 
  monWebServeur.onNotFound(handleNotFound);
  monWebServeur.begin(); // Démarage du web server
  MYDEBUG_PRINTLN("-WEBSERVER : Serveur Web démarré");
}


/************************** Loop ****************************************/
void loopWebServer(void) {
  monWebServeur.handleClient();
}
