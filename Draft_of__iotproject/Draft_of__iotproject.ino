#include "DHT.h" 
#include "ThingSpeak.h"
#include "secrets.h"
#include <WiFi.h>

#define DHTPIN 15 // DHT22 on GPIO15 Pin
#define DHTTYPE DHT22
char ssid[] = SECRET_SSID;   // your network SSID (name) 
char pass[] = SECRET_PASS;   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

DHT dht(DHTPIN, DHTTYPE);
unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;
float h,t, ev;
// Initialize our values

void setup() {
  Serial.begin(115200);  //Initialize serial
  dht.begin();
 WiFi.mode(WIFI_STA);   
  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
   h = dht.readHumidity();
    t = dht.readTemperature(); 
    if ( h < 65){
      ev = 1;}
    else{
      ev = 0;}
    if (isnan(h) || isnan(t))
    { 
      Serial.println("Failed to read from DHT sensor!");
      return;
    }
    Serial.print("Air humidity: ");
    Serial.print(h);
    Serial.println(" %");
    Serial.print("Air Temperature: ");
    Serial.print(t);
    Serial.println(" Celcius");
    Serial.print("Status of electric valve: ");
    Serial.print(ev);


  // set the fields with the values
  ThingSpeak.setField(1, h);
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3, ev);

  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
}
delay(15000); // Wait 15 seconds to update the channel again
