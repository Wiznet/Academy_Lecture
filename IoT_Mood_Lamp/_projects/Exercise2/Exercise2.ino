#include "DHT.h"
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define DHTPIN 2     
#define PIN            6
#define NUMPIXELS      2
#define DHTTYPE DHT11   

DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
void setup() {
#if defined (__AVR_ATtiny85__)
  if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
#endif
  Serial.begin(9600);
  Serial.println("DHTxx test!");

  dht.begin();
  pixels.begin(); // This initializes the NeoPixel library.
}

void loop() {

  delay(2000);
  
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }

  float hif = dht.computeHeatIndex(f, h);
  float hic = dht.computeHeatIndex(t, h, false);

  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(" %\t");
  Serial.print("Temperature: ");
  Serial.print(t);
  Serial.print(" *C ");
  Serial.print(f);
  Serial.print(" *F\t");
  Serial.print("Heat index: ");
  Serial.print(hic);
  Serial.print(" *C ");
  Serial.print(hif);
  Serial.println(" *F");

  pixels.setPixelColor(0, pixels.Color(0, 0, map(h, 20, 90, 0, 255))); // bright blue color.
  pixels.setPixelColor(1, pixels.Color(map(t, 0, 50, 0, 255), 0, 0)); // bright red color.
  
  pixels.show(); // This sends the updated pixel color to the hardware.
}
