TTGO V1.0
==============================
As purchased from [here](https://www.banggood.com/2Pcs-Wemos-TTGO-LORA32-868915Mhz-ESP32-LoRa-OLED-0_96-Inch-Blue-Display-p-1239769.html).

## Install the following Arduino libraries:
- [arduino-LoRa](https://github.com/sandeepmistry/arduino-LoRa)
- [oled-ssd1306](https://github.com/ThingPulse/esp8266-oled-ssd1306)

## Start
- In the Arduino sketches, ensure that the radio frequency you are using is commented/uncommented correctly
    ```
    //#define LORA_BAND  433
    //#define LORA_BAND  868
    #define LORA_BAND  915
    ```

In the Arduino IDE, I chose the board 'TTGO LoRa32-OLED V1', compiled the sketch
to verify it works, and then uploaded to the board.  You'll need two boards,
one to send, and one to receive. Ensure the LORA_BAND matches in both sketches
otherwise they won't be able to communicate.

## Case
I designed a case based on the designs of similar cases on Thingiverse, which
includes a spot for the antenna.  It's a tight fit, but it should work fine.

[case](https://www.thingiverse.com/thing:3443245)

## References
- https://github.com/lewisxhe/TTGO-LoRa-Series
- https://github.com/LilyGO/TTGO-LORA32-V2.0
- https://github.com/LilyGO/LORA-ESP32-OLED
- https://github.com/SensorsIot/LoRa-Board-Test-2
