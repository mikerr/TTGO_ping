
#include <WiFi.h>
#include <ESP32Ping.h> // https://github.com/marian-craciunescu/ESP32Ping

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
                      //https://github.com/Xinyuan-LilyGO/TTGO-T-Display
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const char* ssid     = "wifi";
const char* password = "password";

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

void clearline () {
  y -= 10;
  tft.fillRect(12,y,240,20,TFT_BLACK);
}

String ip_to_string(IPAddress ip) {
  char buf[16];
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );
  return (String(buf));
}
void setup() {

  delay(10);

  tft.init();
  tft.setRotation(1);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  printline("Connecting to WiFi");
  
  WiFi.begin(ssid, password);

  int i=0;
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);

    tft.setCursor (120 + i, 5);
    tft.print(".");
    i += 10;
  }

  IPAddress ip = WiFi.localIP();
  
  printline("WiFi connected with ip " + ip_to_string(ip));  

  printline("Pinging ip range");

  for (i=1;i<255;i++) {
    ip[3] = i;
    printline(ip_to_string(ip));
    if(!Ping.ping(ip)) {
        clearline();
    }
  }
}

void loop() {
  }
