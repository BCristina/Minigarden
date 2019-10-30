#include <ESP8266WiFi.h>
#include <MQTTClient.h>
#include<ESP8266WebServer.h>
#include <DHT.h>
#include <ArduinoJson.h>

#define DHTPIN D6
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

 
const char* ssid     = "Wi-Fi_network_name";
const char* password = "Wi-Fi_network_password";
 
WiFiClient WiFiclient;
MQTTClient client;
 
unsigned long lastMillis = 0;

char data[80];
StaticJsonDocument<200> doc;
 
void setup() {
 Serial.begin(115200);
 delay(10);
 Serial.println();
 Serial.println();
 Serial.print("Connecting to ");
 Serial.println(ssid);

WiFi.hostname("ESP_YourName");//If you want your NodeMCU to have a specific name in the LAN network

// WIFI CONNECTION
 
 WiFi.begin(ssid, password);
 while (WiFi.status() != WL_CONNECTED) {
   delay(500);
   Serial.print(".");
 }
 
 Serial.println("");
 Serial.println("WiFi connected");  
 Serial.println("IP address: ");
 Serial.println(WiFi.localIP());
 delay(2000);

// Turn on DHT11 sensor
dht.begin();

// MQTT CONNECTION

 
 Serial.print("connecting to MQTT broker...");
 client.begin("mqtt_server_name", WiFiclient);//mqtt_server_name -> complete with the mqtt server name (where you want to upload your data)
 connect();
}
 
void connect() {
 while (!client.connect("grafana_topic", "try", "try")) { //grafana_topic -> complete with the topic you want to retrieve the data
   Serial.print(".");
 }
 
 Serial.println("\nconnected!");
 client.subscribe("grafana_topic");

}


// SENZOR DHT11


void loop(void)
{
  // We assign the variable "h" to be humidity value, read by the function "dht.readHumidity ()" 
  float h = dht.readHumidity();

  // We assign the variable "t" to be temperature value, read by the function "dht.readTemperature()"
  float t = dht.readTemperature();

    if (t>100 || t<0)
  {
    t=0;
  }
    if (h>100 || h<0)
  {
    h=0;
  }
  // Display the message: "Exterior Temperature (C) = "
  Serial.print("Exterior Temperature (C) = ");

  // Display the temperature value in the room, read using DHT11 sensor 
  Serial.println(t);

  // Display the message: "Exterior Humidity = "
  Serial.print("Exterior Humidity = ");

  // Display the humidity value in the room, read using the Dht11 sensor 
  Serial.println(h);

  String value = "\"HUMID\": " + String(h) ;
  String value2 = ", \"TEMP\": " + String(t) ;
  value = value + value2;
  delay(3000);



 client.loop();
 if(!client.connected()) {
   connect();
 }


// SEND VALUE TO MQTT

 
 if(millis() - lastMillis > 1000) {
   lastMillis = millis();
   String payload = "{" + value + "}";
   payload.toCharArray(data, (payload.length() + 1));
   client.publish("grafana_topic", data);
 }
}
