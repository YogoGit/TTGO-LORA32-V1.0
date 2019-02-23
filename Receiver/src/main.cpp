#include <stdio.h>
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

TaskHandle_t pktDisplayTask;

typedef struct {
   int packetSize;
   char rssiStr[128];
   char packetBuff[4096];
} Packet;

Packet pkt;
volatile unsigned long lastRecvTime = millis();

// Forward method declarations
void onReceive(int packetSize);
void pktDisplay(void *parm);
void displayLoraPacket();
void showLogo();

void setup() {
  Serial.begin(115200);
  while (!Serial);

  Serial.println();
  Serial.println("LoRa Receiver");

  // Create a new task on the second core to blink the LED when packets
  // are received.  After creation, give it a bit of time to startup
  xTaskCreatePinnedToCore(
    pktDisplay,
    "pktDisplayTask",
    1000,
    NULL,
    1,
    &pktDisplayTask,
    1);
  delay(500);

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
  display.drawString(display.getWidth() / 2, display.getHeight() / 2, "LoRa Receiver");
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

  // Set the radio into receive mode
  LoRa.onReceive(onReceive);
  LoRa.receive();
}

void loop() {
  // If we don't get data in a timely fashion, indicate it on the screen
  if (millis() - lastRecvTime > 5 * 1000) {
    pkt.packetSize = 0;
    // Wakeup the blink task
    vTaskNotifyGiveFromISR(pktDisplayTask, NULL);
  }
  delay(100);
}

void onReceive(int packetSize) {
  for (int i = 0; i < packetSize; i++) {
    pkt.packetBuff[i] = (char)LoRa.read();
  }
  pkt.packetBuff[packetSize] = '\0';
  sprintf(pkt.rssiStr, "RSSI %d", LoRa.packetRssi());
  Serial.println(pkt.rssiStr);

  // Set this as the last thing since it's used to indicate a valid packet
  pkt.packetSize = packetSize;

  // Wakeup the blink task
  vTaskNotifyGiveFromISR(pktDisplayTask, NULL);

  lastRecvTime = millis();
}

// Use the second core to blink the LED when we receive a packet
void pktDisplay(void *parm) {
  static const int BLINK_TIME_MS = 250;

  // Run forever
  pinMode(LED_BUILTIN, OUTPUT);
  while (1) {
    // Wait for a notification
    ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

    // Show the data on the screen
    displayLoraPacket();

    if (pkt.packetSize > 0) {
        // toggle the led to give a visual indication the packet was sent
        digitalWrite(LED_BUILTIN, HIGH);
        delay(BLINK_TIME_MS);
        digitalWrite(LED_BUILTIN, LOW);
    }
  }
}

void displayLoraPacket() {
  display.clear();
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  display.setFont(ArialMT_Plain_10);
  if (pkt.packetSize > 0) {
    display.drawString(0, 0, pkt.rssiStr);
    display.drawString(0, 15, "Received " + String(pkt.packetSize, DEC) + " bytes");
    display.drawStringMaxWidth(0, 26, 128, pkt.packetBuff);
  } else {
      display.drawString(0, 0, "NoData: (timeout)");
      display.drawString(0 , 15 , "Received 0 bytes");
  }
  display.display();
}

void showLogo() {
  uint8_t x_off = (display.getWidth() - logo_width) / 2;
  uint8_t y_off = (display.getHeight() - logo_height) / 2;

  display.clear();
  display.drawXbm(x_off, y_off, logo_width, logo_height, logo_bits);
  display.display();
}
