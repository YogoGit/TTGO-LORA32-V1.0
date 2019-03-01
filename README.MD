TTGO V1.0 - Point-to-point Transmission
=======================================
As purchased from [here](https://www.banggood.com/2Pcs-Wemos-TTGO-LORA32-868915Mhz-ESP32-LoRa-OLED-0_96-Inch-Blue-Display-p-1239769.html).

## Install [PlatformIO](https://platformio.org/) (command line)

https://docs.platformio.org/en/latest/installation.html#installer-script

## Start
- In both the sender and receiver directories, the source code lives
  in the 'src' directory.  Inside each of the directories, the file
  *main.cpp* is the program for the appropriate task.  In *main.cpp*,
  ensure that the radio frequency you are using is commented/uncommented
  correctly.
    ```
    //#define LORA_BAND  433
    //#define LORA_BAND  868
    #define LORA_BAND  915
    ```

## Build
  To build is pretty easy (and fast), just run:
   ```
   % platformio run
   Processing ttgo-lora32-v1 (platform: espressif32; board: ttgo-lora32-v1; framework: arduino)
   ...
   Building .pioenvs/ttgo-lora32-v1/firmware.bin
   Checking size .pioenvs/ttgo-lora32-v1/firmware.elf
   Memory Usage -> http://bit.ly/pio-memory-usage
   DATA:    [          ]   4.5% (used 14900 bytes from 327680 bytes)
   PROGRAM: [==        ]  16.2% (used 212516 bytes from 1310720 bytes)
   ========================= [SUCCESS] Took 4.01 seconds =========================
   ```

## Upload
  Similar to build, just with an additional argument
   ```
   % platformio run -t upload
   ```
 
## Notes
You'll need two boards, one to send, and one to receive. Ensure the LORA_BAND
is the same in both sketches otherwise they won't be able to communicate.

## Case
I designed a case based on the designs of similar cases on Thingiverse, which
includes a spot for the antenna.  It's a tight fit, but it should work fine.

[3D printable case](https://www.thingiverse.com/thing:3443245)

## References
- https://github.com/lewisxhe/TTGO-LoRa-Series
- https://github.com/LilyGO/TTGO-LORA32-V2.0
- https://github.com/LilyGO/LORA-ESP32-OLED
- https://github.com/SensorsIot/LoRa-Board-Test-2

## Background (not necessary, as this is done already in this repo)

In PlatformIO, I hard-coded the configuration for the board.  You won't need
to do this, but it was simple and all I did was run the following command.
   ```
   % platformio init --board ttgo-lora32-v1
   ```

Then, I added the two libraries that this uses as library dependencies in the
same file.  If the routine doesn't automatically install them, use the commands.
   ```
   % platformio lib install"ESP8266_SSD1306"
   % platformio lib install"LoRa"
   ```

## Notes
This setup uses the LoRa radios to create a point-to-point link (this could
perhaps be used for mesh as well, but it's not designed for that). It does
NOT use the LoRaWAN protocol, just the basic LoRa radios and protocol, so
neither the Transmitter nor Receiver interact with LoRaWAN gateways.
