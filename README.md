# Emergency-Button
ESP32 (ESP8266) paired with 433MHz wearable button to make a cell phone call as an emergency notification.

## Motivation
My wife is 75, weak and unstable. I am her 24/7 caregiver but must leave her alone occasionally; groceries mostly. This causes me anxiety and I shorten my trips as much as possible. So I decided to make for her a wearable button that, when activated, would call my cell phone, just in-case she had trouble during my absence.

Research led to a few commercial solutions. But they seemed costly with recurring monthly charges. Other DIY solutions that I found seemed bulky and overly complicated; except for one - “The Smallest WiFi Button in the World” https://www.youtube.com/watch?v=ImVK5cGVrpQ. I was inspired by Bitluni’s work but I have no access to a 3-D printer; printing my own button was off-the-table.

So I decided to use a very simple and cheap hybrid solution, using one of my extra ESP32s, although the ESP8266 works just as well. This ESP is the core of a ‘base station’ configuration that processes the button press events when received. The physical button and receiver is a commercial, paired 433MHz single channel switch by eMylo:

![eMylo](/docs/eMylo.png)
Format: ![Alt Text](url)

Compact and inexpensive and works well.

The cell phone call is generated via IFTTT’s Webhooks app which takes the ESP’s button press event packaged as an HTTP GET request and passes it to Woopla, a third party application that makes the actual cell phone call.
