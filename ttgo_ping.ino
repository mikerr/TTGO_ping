
#include <WiFi.h>
#include <ESP32Ping.h> // https://github.com/marian-craciunescu/ESP32Ping
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
                      //https://github.com/Xinyuan-LilyGO/TTGO-T-Display
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const char* ssid     = "wifi";
const char* password = "password";

int y,MAXy;

void printline (String message) {
  tft.setCursor (0, y);
  tft.print(message);
  y += 9;
  if (y > MAXy) {
    y = 0;
    tft.fillScreen(TFT_BLACK);
  }
}

void clearline () {
  y -= 9;
  tft.fillRect(0,y,240,20,TFT_BLACK);
}

String ip_to_string(IPAddress ip) {
  char buf[16];
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );
  return (String(buf));
}
void setup() {
int ROTATION;

  if (ROTATION == 0) MAXy = 240;
  else MAXy = 128;

  tft.init();
  tft.setRotation(ROTATION);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  printline("Connecting to WiFi");
  
  WiFi.begin(ssid, password);

  tft.setCursor (125, 5);
  while (WiFi.status() != WL_CONNECTED) {
    delay(r00);
    tft.print(".");
  }

  IPAddress ip = WiFi.localIP(); 
  clearline();
  printline("WiFi connected with ip ");
  printline ( ip_to_string(ip));
    
}

void loop() {
  IPAddress ip = WiFi.localIP();

  printline("Pinging ip range");
  
  for (int i=1;i<255;i++) {
    ip[3] = i;
    printline(ip_to_string(ip));
    if(!Ping.ping(ip,1)) {
        clearline();
    }
  }
  delay(5 * 60 * 1000); // wait 5 minutes
  y = 0;
  tft.fillScreen(TFT_BLACK);
  }
