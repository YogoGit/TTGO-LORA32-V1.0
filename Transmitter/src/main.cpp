#include <SPI.h>
#include <Wire.h>
#include <SSD1306Wire.h>
#include <LoRa.h>
#include <pins_arduino.h>
#include "images.h"

//#define LORA_BAND    433
//#define LORA_BAND    868
#define LORA_BAND    915

// Global variable declarations
SSD1306Wire display(0x3c, OLED_SDA, OLED_SCL);

// Forward method declarations
void displayLoraData(String countStr);
void showLogo();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println("LoRa Transmitter");

  // Configure the LED an an output
  pinMode(LED_BUILTIN, OUTPUT);

  // Configure OLED by setting the OLED Reset HIGH, LOW, and then back HIGH
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, HIGH);
  delay(50);
  digitalWrite(OLED_RST, LOW);
  delay(50);
  digitalWrite(OLED_RST, HIGH);
  display.init();
  display.flipScreenVertically();

  // LoRa image
  showLogo();
  delay(2000);

  // Indicate which function this device is running
  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoRa Transmitter");
  display.display();
  delay(2000);

  // Configure the LoRA radio
  SPI.begin(LORA_SCK, LORA_MISO, LORA_MOSI, LORA_CS);
  LoRa.setPins(LORA_CS, LORA_RST, LORA_IRQ);
  if (!LoRa.begin(LORA_BAND * 1E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  LoRa.enableCrc();
  Serial.println("init ok");
}

void loop() {
  static int counter = 0;

  // send packet
  LoRa.beginPacket();
  LoRa.print("hello ");
  LoRa.print(counter);
  LoRa.endPacket();

  String countStr = String(counter, DEC);
  Serial.println(countStr);

  displayLoraData(countStr);

  // toggle the led to give a visual indication the packet was sent
  digitalWrite(LED_BUILTIN, HIGH);
  delay(250);
  digitalWrite(LED_BUILTIN, LOW);
  delay(250);

  counter++;
  delay(1500);
}

void displayLoraData(String countStr) {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  
  display.drawString(0, 0, "Sending packet: ");
  display.drawString(90, 0, countStr);
  display.display();
}

void showLogo() {
  uint8_t x_off = (display.getWidth() - logo_width) / 2;
  uint8_t y_off = (display.getHeight() - logo_height) / 2;

  display.clear();
  display.drawXbm(x_off, y_off, logo_width, logo_height, logo_bits);
  display.display();
}
