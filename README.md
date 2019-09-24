# Custom bluetooth keyboard for PROCREATE (ipad) and ClipStudio (win)


![image01](https://github.com/Roboxtools/Art_tools/blob/master/image03.jpg?raw=true)

**ABOUT THE CODE**

this code is based on examples provide by Adafruit for a Bluetooth keyboard 
Adafruit-Feather-nRF52- Bluefruit-nRF52832

it uses pins
A2, 11, A0, 15, 27, A3, 31, 30, 25, A5, A4, A1

it has two modes:
-windows mode (num==1)
-procreate mode (num==0)

A1 is the switch between Win and Ipad by default start in win mode


**LIST OF HARDWARE**
 
Adafruit-Feather-nRF52 arduino
[Adafruit-Feather-nRF52](https://www.amazon.co.uk/Adafruit-Feather-nRF52-Bluefruit-nRF52832/dp/B07DM1WVM3/ref=sr_1_1?s=electronics&ie=UTF8&qid=1548008745&sr=1-1&keywords=adafruit+nrf52)

protoboard (surface to solder the buttons)
[protoboard](https://www.adafruit.com/product/2884)

big buttons (good for main functions)
[big buttons](https://www.adafruit.com/product/1119)

small buttons (for secondary functions)
[small buttons](https://www.adafruit.com/product/367)

battery (I charge the keyboard less than one a month with this battery really nice)
[battery](https://www.amazon.co.uk/gp/product/B00NAOS8Z2?pf_rd_p=71cb17e9-f468-4d3f-94d5-a0de44c50a7e&pf_rd_r=0S9T4CG8A57GR8RSJ9ZD)


**KNOWN ISSUES**

-pairing windows and ipad at same time generated errors if this happen unpair all machine and only pair with one machine

-pin 16 after a year of use shortcircuit I'm not clear if is a problem on my soldering or with the pin if this happen just reroute to another pin


[![](http://img.youtube.com/vi/9TROEPfZXaY/0.jpg)](http://www.youtube.com/watch?v=9TROEPfZXaY "")
