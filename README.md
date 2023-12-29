# Microchip CLP USB Controled (2012)

Firmware for Microchip CLP Board USB Controled based on Microcontroler PIC 18F2455.

**OBS:** _All the drivers for USB communication were written based on datasheet reading._

## Hardware

A board with 8 opto-coupled relays, 14 Digital inputs, 2 analog inputs, serial communication module, and a usb communication module (HID).

![image](https://github.com/alexandredrefahl/Microchip_CLP_USB/assets/24326296/7bba2dae-f692-4e44-85e5-e4c2944b9a73)


## Software

**(Not included in this repository)**

It uses windows HID (Human Interface Device) to send some chars to board via USB where microcontroler interprets the commands sent and acts on the relays as programmed. 

It's very simple, for example: _"When the character "A" is sent, the board activates relay 1. When the character "a" is sent, it deactivates relay 1"_
