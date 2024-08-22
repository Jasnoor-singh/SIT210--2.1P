#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h"
char ssid[] = SSID;     // Your network SSID (name)
char pass[] = PASSWORD;     // Your network password

WiFiClient client;

unsigned long myChannelNumber = SECRET_CH_ID;
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;

#define DHTPIN 2          // Pin where the DHT sensor is connected (change if necessary)
#define DHTTYPE DHT22     // DHT 22

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);  
  dht.begin();          // Start the DHT sensor

  // Connect to Wi-Fi
  Serial.print("Connecting to WiFi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(5000);
    Serial.print(".");
  }
  Serial.println("Connected!");

  ThingSpeak.begin(client);  // Initialize ThingSpeak
}

void loop() {
  // Reading temperature data
  float temperature = dht.readTemperature();

  // Check if the read failed and exit early (to try again)
  if (isnan(temperature)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  // Print the temperature to the Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" °C");

  // Update ThingSpeak field 1 with the temperature data
  ThingSpeak.setField(1, temperature);

  // Write the data to ThingSpeak
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  if (x == 200) {
    Serial.println("Temperature successfully sent to ThingSpeak.");
  } else {
    Serial.println("Error sending temperature data to ThingSpeak. HTTP error code: " + String(x));
  }

  // Wait 60 seconds before sending data again
  delay(6000);
}

