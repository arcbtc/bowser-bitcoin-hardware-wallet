  <p align="center">
<img src="https://i.imgur.com/PJXob0B.png" />
</p>

<h1>BOWSER, DIY BITCOIN HARDWARE WALLET V1</h1>

Building your own hardware wallet is a bit like building a lightsaber, only really necessary if you want to be a Jedi.

Now building hardware wallets is easy with the help of <a href="https://github.com/micro-bitcoin/uBitcoin">uBitcoin</a> an Arduino library created by Stepan Snigirev.

Building hardware wallets not only gives greater autonomy/liberty, it is an excellent excuse to get a deeper understanding of how bitcoin keys, seeds and signing works.

## Disclaimer

Project is out of beta and ready for use, but use at your own risk.

## Latest version

https://github.com/arcbtc/bowser-bitcoin-hardware-wallet/releases/tag/v0.01

## Tutorial

https://www.youtube.com/watch?v=DG1zrlAVdys

## SD COMMANDS

Commands to enter into bowser.txt on the SD card.

Wipe device completely, setup new wallet:<br/>
`HARD RESET`

Restore from a seed backup:<br/>
`RESTORE husband behind shallow promote....`

Sign an Electrum transaction:<br/>
`SIGN 45505446ff00020000000001016cb....`

## Morse-code cheat sheet

<img src="https://i.imgur.com/atlxPn1.png">

## Installing Arduino

Install Aruino IDE
https://www.arduino.cc/en/Guide/HomePage

Install ESP32 Boards
https://github.com/espressif/arduino-esp32/blob/master/docs/arduino-ide/boards_manager.md

## Libraries Used

- <a href="https://github.com/micro-bitcoin/uBitcoin">uBitcoin</a> (Download library from github and "Add Zip Library" in Arduino Library Manager...Tested with v1.0.2)
- <a href="https://github.com/m5stack/M5Stack">M5Stack</a> (Install from "Library Manager" in Arduino)
- [lvgl](https://lvgl.io/) (Install from "Library Manager" in Arduino, search as "lvgl", appears in the list as "lv_arduino")

## Materials

- M5Stack (grey one is better)
- SD card (not sure if size is an issue for larger SD, 16GB works fine)
