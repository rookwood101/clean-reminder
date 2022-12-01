# clean-reminder
 
You have an ESP-12E
https://en.wikipedia.org/wiki/ESP8266
https://www.mischianti.org/2021/05/04/esp-12-high-resolution-pinout-and-specs/
https://components101.com/sites/default/files/component_datasheet/ESP12E%20Datasheet.pdf
https://www.instructables.com/Ultimate-ESP8266-01-Programmer-USB-Adapter/

usb ESP-8266 programmer:
https://smile.amazon.co.uk/gp/product/B078J7LDLY/ref=ppx_yo_dt_b_asin_title_o05_s00?ie=UTF8&psc=1
can't find schematics for it
you have a couple of photos in google photos that show how to plug it in
red x x blue
green black orange white ⬆

how to run:
plug into usb
open serial monitor in arduino ide
Select Generic ESP8266 module for port
Both NL & CR, 115200 baud I think
Pressing buton on breadboard seems to reset

How to program:
Connect yellow GPIO0 wire that dangles to negative (ground)
hold down button on usb-to-serial while plugging it in and let go after 1 second
Press the button on breadboard (reset) and then release and disconnect yellow GPIO0 wire
Press upload
https://www.instructables.com/3-Simple-Ways-of-Programming-an-ESP8266-12X-Module/
when done disconnect yellow wire, unplug usb and plug back in

# TODO
 - [ ] Use performant types (not uint64_t)
 - [ ] Wrap serial output in ifdef to avoid overhead when not debugging
 - [ ] Measure current draw
 - [ ] Measure performance and see if any optimisations needed
 - [ ] Get whatsapp messages from dad for datasheets and schematics