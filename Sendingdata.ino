#include <DHT.h>
#include <WiFiNINA.h>
#include <ThingSpeak.h>
#include "secrets.h"

// Wi-Fi credentials from secrets.h
char ssid[] = SSID;     // Your Wi-Fi network name (SSID)
char pass[] = PASSWORD; // Your Wi-Fi network password

WiFiClient client;

// ThingSpeak channel details from secrets.h
unsigned long myChannelNumber = SECRET_CH_ID;        // ThingSpeak Channel ID
const char * myWriteAPIKey = SECRET_WRITE_APIKEY;    // ThingSpeak Write API Key

// Define DHT sensor configuration
#define DHTPIN 2          // Pin where the DHT22 sensor is connected (update if needed)
#define DHTTYPE DHT22     // Using DHT22 sensor

DHT dhtSensor(DHTPIN, DHTTYPE); // Create a DHT object

void setup() {
  Serial.begin(9600);  
  dhtSensor.begin();          // Initialize the DHT sensor

  // Connect to Wi-Fi network
  Serial.print("Attempting to connect to Wi-Fi...");
  while (WiFi.begin(ssid, pass) != WL_CONNECTED) {
    delay(5000); // Retry every 5 seconds
    Serial.print(".");
  }
  Serial.println("Connected to Wi-Fi!");

  ThingSpeak.begin(client);  // Initialize connection to ThingSpeak
}
void loop() {
  // Read temperature data from the sensor
  float currentTemperature = dhtSensor.readTemperature();
  // Check if reading is valid
  if (isnan(currentTemperature)) {
    Serial.println("Error: Could not read from DHT sensor.");
    return; // Retry reading in the next loop
  }
  // Display the temperature reading in the Serial Monitor
  Serial.print("Current Temperature: ");
  Serial.print(currentTemperature);
  Serial.println(" °C");

  // Send temperature data to ThingSpeak (Field 1)
  ThingSpeak.setField(1, currentTemperature);

  // Write data to ThingSpeak
  int statusCode = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);

  // Check if data was successfully sent
  if (statusCode == 200) {
    Serial.println("Temperature data sent to ThingSpeak successfully.");
  } else {
    Serial.println("Failed to send data to ThingSpeak. HTTP error code: " + String(statusCode));
  }
  // Wait for 5 seconds before the next reading
  delay(5000);
}
