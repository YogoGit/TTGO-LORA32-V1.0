#include <SPI.h>
#include <Wire.h>
#include <SSD1306.h>
#include <LoRa.h>
#include "images.h"

//#define LORA_BAND    433
//#define LORA_BAND    868
#define LORA_BAND    915

#define OLED_SDA    4
#define OLED_SCL    15
#define OLED_RST    16

#define SCK     5    // GPIO5  -- SX1278's SCK
#define MISO    19   // GPIO19 -- SX1278's MISO
#define MOSI    27   // GPIO27 -- SX1278's MOSI
#define SS      18   // GPIO18 -- SX1278's CS
#define RST     14   // GPIO14 -- SX1278's RESET
#define DI0     26   // GPIO26 -- SX1278's IRQ(Interrupt Request)

SSD1306 display(0x3c, OLED_SDA, OLED_SCL);

// Forward declarations
void displayLoraData(int packetSize, String packet, String rssi);
void showLogo();

void setup() {
  Serial.begin(115200);
  while (!Serial);
  Serial.println();
  Serial.println("LoRa Receiver");

  // Configure OLED by setting the OLED Reset HIGH, LOW, and then back HIGH
  pinMode(OLED_RST, OUTPUT);
  digitalWrite(OLED_RST, HIGH);
  delay(100);
  digitalWrite(OLED_RST, LOW);
  delay(100);
  digitalWrite(OLED_RST, HIGH);

  display.init();
  display.flipScreenVertically();

  showLogo();
  delay(2000);

  display.clear();
  display.setFont(ArialMT_Plain_16);
  display.setTextAlignment(TEXT_ALIGN_CENTER);
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoRa Receiver");
  display.display();
  delay(2000);

  // Configure the LoRA radio
  SPI.begin(SCK, MISO, MOSI, SS);
  LoRa.setPins(SS, RST, DI0);
  if (!LoRa.begin(LORA_BAND * 1E6)) {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("init ok");

  // Set the radio into receive mode
  LoRa.receive();
  delay(1500);
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    String packet = "";
    for (int i = 0; i < packetSize; i++) {
      packet += (char)LoRa.read();
    }
    String rssi = "RSSI " + String(LoRa.packetRssi(), DEC) ;
    Serial.println(rssi);

    displayLoraData(packetSize, packet, rssi);
  }
  delay(10);
}

void displayLoraData(int packetSize, String packet, String rssi) {
  String packSize = String(packetSize, DEC);

  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  display.drawString(0 , 15 , "Received " + packSize + " bytes");
  display.drawStringMaxWidth(0 , 26 , 128, packet);
  display.drawString(0, 0, rssi);
  display.display();
}

void showLogo() {
  uint8_t x_off = (display.getWidth() - logo_width) / 2;
  uint8_t y_off = (display.getHeight() - logo_height) / 2;

  display.clear();
  display.drawXbm(x_off, y_off, logo_width, logo_height, logo_bits);
  display.display();
}
