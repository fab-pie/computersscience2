#include <WiFi.h>
#include <WebSocketsServer.h>
#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp;
int xAccl = 0, yAccl = 0, zAccl = 0;

#define ACCL_ADDR 0x1D
#define LED_PIN 9

const char* ssid = "PoleDeVinci_IFT";
const char* password = "*c.r4UV@VfPn_0";

WebSocketsServer webSocket = WebSocketsServer(80);

unsigned long previousMillis = 0;
const long interval = 5000;

void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t * payload, size_t length) {
  if (type == WStype_TEXT) {
    String message = (char *)payload;
    Serial.println("Message reçu : " + message);

    if (message == "getBMP280") {
      float temperature = bmp.readTemperature();
      float pressure = bmp.readPressure();
      float altitude = bmp.readAltitude(1013.25);

      String bmpData = String(temperature) + "," + String(pressure) + "," + String(altitude);
      webSocket.sendTXT(num, bmpData);
    } 
    else if (message == "getAccelerometer") {
      String acclData = String(xAccl) + "," + String(yAccl) + "," + String(zAccl);
      webSocket.sendTXT(num, acclData);
    }
    else if (message.startsWith("setLED:")) {
      int intensity = message.substring(7).toInt();
      analogWrite(LED_PIN, intensity);
      webSocket.sendTXT(num, "LED intensity set to " + String(intensity));
    }
  }
}
void setup() {
  Serial.begin(9600);

  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
  Serial.println("Starting WebSocket server...");
  webSocket.begin();
  webSocket.onEvent(onWebSocketEvent);
  Serial.println("completed");
  if (!bmp.begin(0x76)) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  pinMode(LED_PIN, OUTPUT);
}
void loop() {
  webSocket.loop();
  unsigned int data[7];
  Wire.requestFrom(ACCL_ADDR, 7);
  if (Wire.available() == 7) {
    data[0] = Wire.read();
    data[1] = Wire.read();
    data[2] = Wire.read();
    data[3] = Wire.read();
    data[4] = Wire.read();
    data[5] = Wire.read();
    data[6] = Wire.read();
  }
  xAccl = ((data[1] * 256) + data[2]) / 16;
  if (xAccl > 2047) xAccl -= 4096;
  yAccl = ((data[3] * 256) + data[4]) / 16;
  if (yAccl > 2047) yAccl -= 4096;
  zAccl = ((data[5] * 256) + data[6]) / 16;
  if (zAccl > 2047) zAccl -= 4096;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    previousMillis = currentMillis;
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure();
    float altitude = bmp.readAltitude(1013.25);
    Serial.print("Temperature = ");
    Serial.print(temperature);
    Serial.println(" °C");
    Serial.print("Pressure = ");
    Serial.print(pressure);
    Serial.println(" Pa");
    Serial.print("Approx altitude = ");
    Serial.print(altitude);
    Serial.println(" m");
    Serial.print("Acceleration in X-Axis : ");
    Serial.println(xAccl);
    Serial.print("Acceleration in Y-Axis : ");
    Serial.println(yAccl);
    Serial.print("Acceleration in Z-Axis : ");
    Serial.println(zAccl);
  }
  if (Serial.available() > 0) { 
    int intensity = Serial.parseInt(); 
    if (intensity >= 0 && intensity <= 255) {
      analogWrite(LED_PIN, intensity); 
      Serial.print("Intensité réglée à : ");
      Serial.println(intensity);
    } else {
      Serial.println("Veuillez entrer une valeur entre 0 et 255."); 
    }
  }
}

