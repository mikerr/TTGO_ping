
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
  if (y == 0) tft.fillScreen(TFT_BLACK);
  tft.setCursor (0, y);
  tft.print(message);
  y += 9;
  if (y > MAXy) y = 0;
}

void clearline () {
  y -= 9;
  tft.fillRect(0,y,80,20,TFT_BLACK);
}

String ip_to_string(IPAddress ip) {
  char buf[16];
  sprintf(buf, "%d.%d.%d.%d", ip[0], ip[1], ip[2], ip[3] );
  return (String(buf));
}
void setup() {
int ROTATION=0;

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
    delay(500);
    tft.print(".");
  }
 
  y = 0;
  IPAddress ip = WiFi.localIP();
  printline("WiFi connected");
  tft.setTextColor(TFT_GREEN);
  printline ( ip_to_string(ip));    
}

void loop() {

  IPAddress ip = WiFi.localIP();

  tft.setTextColor(TFT_WHITE);
  printline("Pinging ip range");
  tft.setTextColor(TFT_GREEN);
  tft.fillCircle(64,210,30,TFT_RED);
  for (int i=1;i<255;i++) {
    ip[3] = i;
    printline(ip_to_string(ip));
    
      float rad = (2 * 3.14 * i) / 360; 
      int x = (int)(sin(rad) * 30 );
      int y = (int)(-cos(rad) * 30);
      
    if(Ping.ping(ip,1)) {
      tft.drawLine(64,210,64 + x,210 + y,TFT_GREEN); 
      tft.drawLine(120,i,128,i,TFT_GREEN);
    
    } else {
      clearline();
      tft.drawLine(120,i,128,i,TFT_BLUE);
    }
   }
  delay(5 * 60 * 1000); // wait 5 minutes
  y = 0;
  }
