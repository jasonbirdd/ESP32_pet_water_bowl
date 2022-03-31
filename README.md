---
title: 'ESP32_pet_water_bowl'
disqus: hackmd
---

How much water is enough for your dog
===

## Introduction

It’s hard to measure how much your dog or cat drink water a day. However, the daily water intake may signal a healthy problem.  According to some research, dogs and cats need to get 40~60cc/kg per day. But how can we calculate from the pet water bowl?  
This device is my own design for recording the consumption of water from my dog today and yesterday. And you can trace the water level of the water bowl from your connected device, moreover, if the level is too low, the buzzer under the bowl will remind you to fill it.
![746699BE-F97E-4A4E-9028-2C39B8B354C5](https://user-images.githubusercontent.com/59694163/160978360-32f3da60-77fa-46b6-b376-3e568b2df99b.jpg)



This IoT device is get much help from https://randomnerdtutorials.com/ & [https://reurl.cc/9Ol2kj](https://)
There are many material about chip design using ESP32. Thanks a lot for them.

Requirements:
---
* Hardware:  
#### ESP32  
![](https://i.imgur.com/KpRh6fQ.png)

#### HX711 & weight measurements  
![](https://i.imgur.com/Qn3Gp8F.png)

* Software:  
#### Arduino IDE  

When using this project, you should upload the html file( "/data/index.html") to ESP32 flash memory. You could follow this website to set[https://randomnerdtutorials.com/install-esp32-filesystem-uploader-arduino-ide/](https://)  
After that, you have built a UI on ESP32 for monitoring the water bowl.

User modify:
---
```c=45
// Replace with your network credentials
const char* ssid = "XXX";
const char* password = "XXX";
```
change wifi attibute as your own network credentials.

```c=99
timeClient.begin();
// Set offset time in seconds to adjust for your timezone, for example:
// GMT +1 = 3600
// GMT +8 = 28800
// GMT -1 = -3600
// GMT 0 = 0
timeClient.setTimeOffset(28800);
getTimeStamp();
```
set correct timezone for your region

---

### reference
[https://randomnerdtutorials.com/esp32-esp8266-plot-chart-web-server/](https://)  
[https://reurl.cc/9Ol2kj](https://)  
[https://www.dogcatstar.com/blog-petwater/](https://)  


