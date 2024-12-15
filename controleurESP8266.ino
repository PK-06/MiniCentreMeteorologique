#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <NTPClient.h>
#include <WiFiUdp.h>

const char* ssid = "ICI NOM DU WIFI AUQUEL LA CARTE ESP8266 PEUT SE CONNECTER"; // TODO
const char* password = "ICI MOT DE PASSE DU WIFI"; // TODO

const long utcOffsetInSeconds = 3600;
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

#define SDA_PIN D2
#define SCL_PIN D1
const int luminosityPin = A0;

ESP8266WebServer server(80);
Adafruit_BME280 bme;


void setup() {
  /*Serial.begin(115200);*/
  WiFi.begin(ssid, password);
  
  /*while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connexion au Wi-Fi...");
  }

  Serial.print("Connecté, IP : ");
  Serial.println(WiFi.localIP());*/

  // Initialisation du BME280
  Wire.begin(SDA_PIN, SCL_PIN);
  if (!bme.begin(0x76)) {
    Serial.println("Erreur : impossible de trouver le capteur BME280 !");
    while (1);
  }

  pinMode(luminosityPin, INPUT);
  


  server.on("/", HTTP_GET, handleHome);
  server.begin();

  Serial.println("Serveur HTTP démarré");
  timeClient.begin();
}

void loop() {
  server.handleClient();
}


// Route : / (page d'accueil)
void handleHome() {
  timeClient.update();
  
  time_t rawTime = timeClient.getEpochTime(); // Obtenir le temps en epoch
  struct tm* timeinfo = localtime(&rawTime);  // Convertir en structure tm
  
  char timeString[50];
  strftime(timeString, sizeof(timeString), "%F %H:%M:%S", timeinfo); // Formater la date

  String json = "{";

  json += "\"time\": \"";
  json += String(timeString); // Ajouter la chaîne formatée
  json += "\",";
  
  // Luminosité
  json += "\"luminosity\":";
  json += String(analogRead(luminosityPin));
  json += ",";

  // Température
  json += "\"temperature\":";
  json += String(bme.readTemperature());
  json += ",";

  // Pression
  json += "\"pressure\":";
  json += String(bme.readPressure() / 100.0F);
  json += ",";

  // Humidité
  json += "\"humidity\":";
  json += String(bme.readHumidity());
  json += "}"; // Fin du JSON

  server.send(200, "application/json", json);
}

