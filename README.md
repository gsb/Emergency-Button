# Emergency-Button
ESP32 (ESP8266) paired with 433MHz wearable button to make a cell phone call as an emergency notification.


## Motivation
My wife is 75, weak and unstable. I am her 24/7 caregiver but must leave her alone occasionally; groceries mostly. This causes me anxiety and I shorten my trips as much as possible. So I decided to make for her a wearable button that, when activated, would call my cell phone, just in-case she had trouble during my absence.

Research led to a few commercial solutions. But they seemed costly with recurring monthly charges. Other DIY solutions that I found seemed bulky and overly complicated; except for one - “The Smallest WiFi Button in the World” https://www.youtube.com/watch?v=ImVK5cGVrpQ. I was inspired by Bitluni’s work but I have no access to a 3-D printer; printing my own button was off-the-table.

So I decided to use a very simple and cheap hybrid solution, using one of my extra ESP32s, although the ESP8266 works just as well. This ESP is the core of a ‘base station’ configuration that processes the button press events when received. The physical button and receiver is a commercial, paired 433MHz single channel switch by eMylo:

![eMylo](/docs/eMylo.png)

Using a couple of jumper wires, just two - 3v3 poewer goes into the eMylo switch, red. Then one, green from the switch back to the ESP. For the ESP32, the green is plugged into GPIO34 (D34) as the device input signal. For the ESP8266, the green is plugged into ADC0 (A0). 

![eMylo](/docs/wiring.png)

Compact, inexpensive and works well.


## IFTTT
The cell phone call is generated via IFTTT’s Webhooks app which takes the ESP’s button press event packaged as an HTTP GET request and passes it to the Woopla app, a third party app that makes the actual cell phone call.

I tested using Email, SMS Messaging and even a simple Notification, also free. Each of these methods provide a single beep/vibration upon arrival to my cell phone. But I have bad hearing and mostly do not ware my hearing aids. I opted for the cell phone call with an obnoxious ring tone that does stop ringing/vibrating until I hear it and answer. Tada! Mission accomplished. Help message received by me.

IFTTT and Woopla accounts are free to setup and **required**. The third party app, Woopla, charges my account less than $0.04 per call. I can handle 4¢ per call given that I hope to never get a call at all, except during testing. And Woopla makes it simple. I used $6.50 Paypal cash to get some 167 phone calls, credited to my account, and even I don't expect I'll need that much testing.


## Final Solution
I integrated the final code into my home-spun ESP32 NodeNet Broker, a hub of sorts linking all of my inuse ESPs, freeing back up my testing ESP32. The base hub (as I call it) was place back on the top of a cabinet next to the WiFi router and is now "out of site and out of mind." So now, my outings are a little less stressful.
