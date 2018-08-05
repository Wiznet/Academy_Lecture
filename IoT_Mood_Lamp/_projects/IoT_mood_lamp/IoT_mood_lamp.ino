#define BLYNK_PRINT  Serial
#define SERIAL_WIFI  Serial3
#define NEOPIXEL_PIN 6
#define NEOPIXEL_LED_MAX 2
/* For DHT11 Sensor */
#define DHTPIN            2     
#define DHTTYPE           DHT11  

#include "BlynkSimpleWizFi310.h"
#include <Adafruit_NeoPixel.h> 
#include "DHT.h"

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NEOPIXEL_LED_MAX, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800);

char auth[] = "";
char ssid[] = "";
char pass[] = "";

BLYNK_WRITE(V2)
{
  int r = param[0].asInt();
  int g = param[1].asInt();
  int b = param[2].asInt();

  for (int i = 0; i < strip.numPixels(); i++)
  {
    strip.setPixelColor(i, r, g, b);
  }
  strip.show(); 
}
void sendSensor()
{
  float h = dht.readHumidity();
  float t = dht.readTemperature(); // or dht.readTemperature(true) for Fahrenheit

  if (isnan(h) || isnan(t)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // You can send any value at any time.
  // Please don't send more that 10 values per second.
  Blynk.virtualWrite(V0, t);
  Blynk.virtualWrite(V1, h);
}

void setup()
{
  strip.begin();                                      
  strip.show();
  // Debug console
  Serial.begin(9600);
  SERIAL_WIFI.begin(115200);

  WiFi.init(&SERIAL_WIFI);
  
  Blynk.begin(auth, ssid, pass);
  //Blynk.begin(auth, ssid, pass, "blynk-cloud.com", 80);
  dht.begin();
  // Setup a function to be called every second
  timer.setInterval(1000L, sendSensor);
}

void loop()
{
  Blynk.run();
  timer.run();
}

