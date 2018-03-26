# Neopixel-Dueldisk
Animating some Adafruit NeoPixel strips

This repository contains a small scripts to animate a bunch of short Adafruit NeoPixel strips which are embedded in the cosplay prop of my girlfriend. The prop is a replication of the [duel disk](http://yugioh.wikia.com/wiki/Duel_Disk#Duel_Disk_.28version_3.29) from the Yu-Gi-Oh! movie 'Dark Side of Dimensions':

![Duel Disk v3][disk]

The system consists of 6 LED-strips in star formation which are controlled by an [Adafruit Pro Trinket 5V/16MHz][trinketpro], running at 8 MHz.
The MCU clock speed has been manually reduced by changing the hardware settings used for compilation in the Arduino IDE. 
The MCU animates a gamma-corrected cosine wave over these strips in parallel:

<a href="http://www.youtube.com/watch?feature=player_embedded&v=9gw3B-Dr4Mk
" target="_blank"><img src="http://img.youtube.com/vi/9gw3B-Dr4Mk/0.jpg" 
alt="IMAGE ALT TEXT HERE" width="240" height="180" border="10" /></a>

The script is built on top of the [Adafruit NeoPixel library](https://github.com/adafruit/Adafruit_NeoPixel) and was developed using the Arduino IDE.

# References
- [Adafruit NeoPixel Überguide](https://learn.adafruit.com/adafruit-neopixel-uberguide/neopixel-strips)
- [Adafruit Pro Trinket Introduction][trinketpro]
- [Gamma Correction](https://learn.adafruit.com/led-tricks-gamma-correction/the-issue)

[disk]: https://vignette.wikia.nocookie.net/yugioh/images/0/07/The_Lustrous_Kit.jpg/revision/latest/scale-to-width-down/200?cb=20171110155033 "Duel Disk v3"
[trinketpro]: https://learn.adafruit.com/introducing-pro-trinket?view=all
