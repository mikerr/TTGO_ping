
#include <WiFi.h>
#include <ESP32Ping.h>

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

unsigned long targetTime = 0;

const char* ssid     = "wifi";
const char* password = "password";

const IPAddress remote_ip(192, 168, 0, 1);

int y = 5;
void printline (String message) {
  tft.setCursor (12, y);
  tft.print(message);
  y += 10;
  if (y > 128) {
    y = 0;
    tft.fillScreen(TFT_BLACK);
  }
}

void setup() {

  int i=0;
  char buf[16];
  
  delay(10);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  printline("Connecting to WiFi");
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.setCursor (120 + i, 5);
    tft.print(".");
    i += 10;
  }

  IPAddress ip = WiFi.localIP();
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );

  printline("WiFi connected with ip " + String( buf));  
  printline("Pinging ip range");

  ip = remote_ip;

  for (i=1;i<255;i++) {
    ip[3] = i;
    sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );
    printline(buf);
    if(!Ping.ping(ip)) {
        y -= 10;
        tft.fillRect(12,y,240,20,TFT_BLACK);
    }
  }
}

void loop() {
  }
