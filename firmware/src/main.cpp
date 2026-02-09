/***************************************************************************
  Αισθητήρας υγρασίας χώματος, θερμοκρασίας αέρα και βαρομετρικής πίεσης.
  Αποθήκευση των μετρήσεων σε SD card.
  Δημιουργία ασύρματου δικτύου WiFi με HTML server. Οι συσκευές πελάτες  μπορούν
  να συνδεθούν στο SSID ESP8266 Access Point και να εντοπίσουν
  τη συσκευή του Access Point στη διεύθυνση 192.168.2.1
  και να κατεβάσουν την σελίδα του HTML server
 ***************************************************************************/
/*'''
  BMA280     | ESP8266 NodeMCU
Breakout Brd |  (I2C)
_____________|________________________
VCC -----------> 3V
GND -----------> G
SCL -----------> D1
SDA -----------> D2
CSB ----------> NC
SDO ----------> NC

     DS1307  | ESP8266 NodeMCU
  RTC module |  (I2C)
_____________|________________________
GND -----------> G
VCC -----------> 3V
SDA -----------> D2
SCL -----------> D1
SD ------------> NC

Micro SD   |  ESP8266 NodeMCU
 Adaptor   |  (SPI)
___________|________________________
VCC -----------> 3V
GND -----------> G
CS  -----------> D8
MOSI ----------> D7
SCK  ----------> D5
MISO ----------> D6

Soil Humidity |  ESP8266 NodeMCU
   Sensor     |
______________|________________________
VCC -----------> 3V
GND -----------> G
A0  -----------> A0
'''*/
 //#define With_WiFi
#define SLEEP_TIME_US 900000000UL // expressed as μs --->> 15 minutes

#include <Arduino.h>
#include <ESP8266WiFi.h> // Include the Wi-Fi library
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SD.h> // Include the SD card library

#include "BMP280.h" // library for BMP280 TEMPERATURE BAROMETRIC PRESSURE
#include "DS1307.h" // library for RTC module
#include "html_logger.h"

#define LED 2        /* ESP8266 D4 ON BOARD LED*/
#define analogPin A0 /* ESP8266 Analog Pin ADC0 = A0 used by Soil Humidity Sensor*/
#define P0 1013.25

double T, P, H;            // Temperature, Barometric pressure, Soil Humidity
const int chipSelect = D8; // chip select pin for the SD card
char buffer[64];           // To be used in writing data to SD card (bigger to avoid overflow)
int adcValue = 0;          /* Variable to store Output of ADC used by Soil Humidity Sensor*/

const char *ssid = "ESP8266_WiFi"; // The name of the Wi-Fi network that will be created
const char *password = "12345678"; // The password required to connect to it (min 8 characters), leave blank for an open network
const int channel = 4;             // WiFi Channel to transmit

BMP280 bmp;       // BMP280 sensor object instance
DS1307 rtc_clock; // define an object of DS1307 class

IPAddress ip(192, 168, 2, 1);
IPAddress gateway(192, 168, 2, 2);
IPAddress subnet(255, 255, 255, 0);

ESP8266WebServer server(80);

void handleRoot()
{ // When URI / is requested, send html page_logger to client
  server.send(200, "text/html", page_logger);
  Serial.println("HTTP / client request");
}

void readData()
{ // When URI /readData is requested, send data to client
  String data = "{\"Temperature\":\"" + String(T) + "\", \"Pressure\":\"" + String(P) + "\", \"Humidity\":\"" + String(H) + "\"}";
  digitalWrite(LED, !digitalRead(LED)); // Toggle LED
  server.send(200, "text/plane", data); // Server reply with HTTP status code of 200 (Ok) and the text data
  delay(100);
}

/*Function: Display time on the serial monitor*/
void serialPrintTime()
{
  rtc_clock.getTime();
  sprintf(buffer, "%.2d:%.2d  %.2d:%.2d:%d", rtc_clock.hour, rtc_clock.minute, rtc_clock.dayOfMonth, rtc_clock.month, rtc_clock.year + 2000);
  Serial.println(buffer);
}

/*Function: Display time on the serial monitor and return as String*/
String getRtcTime()
{
  char buffer[32];
  rtc_clock.getTime();
  snprintf(buffer, sizeof(buffer), "%.2d:%.2d  %.2d:%.2d:%d", rtc_clock.hour, rtc_clock.minute, rtc_clock.dayOfMonth, rtc_clock.month, rtc_clock.year + 2000);
//  Serial.println(buff);
  return String(buffer);
}


/*Safe append to SD: open->write->flush->close*/
bool appendToSD(const char *line)
{
  File dataFile = SD.open("data.txt", FILE_WRITE); // FILE_WRITE appends
  if (!dataFile)
  {
    Serial.println("Error opening file for append");
    return false;
  }
  dataFile.println(line);
  dataFile.flush();
  dataFile.close();
  return true;
} 

void setup()
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  Serial.begin(115200);
  delay(100);

  rtc_clock.begin();
  // rtc_clock.fillByYMD(2022, 12, 31); //Jan 19,2013
  // rtc_clock.fillByHMS(23, 19, 30); //15:28 30"
  // rtc_clock.fillDayOfWeek(SAT);//Saturday
  // rtc_clock.setTime();//write time to the RTC chip

  Serial.println("RTC time:");
  serialPrintTime();
#ifndef With_WiFi
  WiFi.mode(WIFI_OFF);
  WiFi.forceSleepBegin();
  delay(1);
#endif

#ifdef With_WiFi
  Serial.println("RTC time:");
  serialPrintTime();
  WiFi.softAPConfig(ip, gateway, subnet);
  WiFi.softAP(ssid, password, channel); // Start the access point
  Serial.print("Access Point \"");
  Serial.print(ssid);
  Serial.println("\" started");

  Serial.print("IP address:\t");
  Serial.println(WiFi.softAPIP()); // Send the IP address of the ESP8266 to the computer

  server.on("/", handleRoot);
  server.on("/readData", HTTP_GET, readData); // Call the 'readData' function when a client requests URI "/readData"
  server.begin();
  Serial.println("HTTP server started");
#endif

  if (!bmp.begin())
  {
    Serial.println("BMP init failed!");
    while (1)
      ;
  }
  else
    Serial.println("BMP init success!");

  bmp.setOversampling(4);

  // initialize the SD card and check for errors
  if (!SD.begin(chipSelect))
  {
    Serial.println("SD card initialization failed!");
    return;
  }
  Serial.println("SD card initialization done.");
}

void loop()
{
#ifdef With_WiFi
  server.handleClient();
#endif
  // read soil sensor
  adcValue = analogRead(analogPin);     /* Read the Analog Input value */
  H = (double)adcValue;                 /* mirror ADC as H for existing code paths */
  H = map(adcValue, 300, 1024, 100, 1); /* Convert ADC value to percentage */

  char result = bmp.startMeasurment();

  if (result != 0)
  {
    delay(result);
    result = bmp.getTemperatureAndPressure(T, P);

    if (result != 0)
    {
      // format a line with timestamp, temperature, pressure, soil ADC
      String ts = getRtcTime();
      int n = snprintf(buffer, sizeof(buffer), "%s\t%.1f\t%.1f\t%.1f\t%d", ts.c_str(), T, P, H, adcValue);
      if (n > 0 && n < (int)sizeof(buffer))
      {
        Serial.println(buffer);
        if (appendToSD(buffer))
        {
          Serial.println("Data saved to file.");
        }
        else
        {
          Serial.println("Error writing to SD.");
          digitalWrite(LED, LOW); // Turn on on Board LED
        }
      }
      else
      {
        Serial.println("Formatting error - buffer overflow");
      }
    }
    else
    {
      Serial.println("Error reading BMP sensor.");
    }
  }
  else
  {
    Serial.println("Error starting BMP sensor.");
  }

  //digitalWrite(LED, !digitalRead(LED)); // Toggle LED
  // add a 2 second delay to slow down the output
  delay(1000);
#ifdef With_WiFi
  server.handleClient();
  delay(1000);
  server.handleClient();
  delay(1000);
  server.handleClient();
  delay(1000);
  server.handleClient();
  delay(1000);
  server.handleClient();
  delay(1000);
  server.handleClient();
  delay(1000);
  server.handleClient();
#endif

  delay(1000);
#ifndef With_WiFi
  Serial.println("Going For Sleep");
  delay(2000);
  ESP.deepSleep(SLEEP_TIME_US);
#endif
}
