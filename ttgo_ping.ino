
#include <WiFi.h>
#include <ESP32Ping.h> // https://github.com/marian-craciunescu/ESP32Ping
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
                      //https://github.com/Xinyuan-LilyGO/TTGO-T-Display
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

const char* ssid     = "wifi";
const char* password = "password";

int button = 35;

int y,MAXy;
bool hosts[256];

void printline (String message) {
  tft.setCursor (0, y);
  tft.fillRect(0,y,80,20,TFT_BLACK);
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

  tft.setCursor (0, y);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    tft.print(".");
  }  
  pinMode(button, INPUT);  
  scan();
}

void sidegraph() {
for (int i=0;i<256;i++)
    if (hosts[i]) tft.drawLine(120,i,128,i,TFT_GREEN);
    else tft.drawLine(120,i,128,i,TFT_BLUE);
}

void circlegraph() {
tft.fillCircle(64,210,30,TFT_RED);
for (int i=0;i<256;i++) 
    if (hosts[i]) {
      float rad = (2 * 3.14 * i) / 360; 
      int cx = (int)(sin(rad) * 30 );
      int cy = (int)(-cos(rad) * 30);
      tft.drawLine(64,210,64 + cx,210 + cy,TFT_GREEN); 
    }
}
void scan() {
  IPAddress ip = WiFi.localIP();
  y = 0;
  tft.fillScreen(TFT_BLACK);
  printline("WiFi : " + String(ssid));
  tft.setTextColor(TFT_GREEN);
  printline ( ip_to_string(ip)); 
  tft.fillRect(0,27,80,133,TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  printline("Live IP addresses:");
  
  for (int i=1;i<255;i++) {
    ip[3] = i;
    tft.setTextColor(TFT_RED);
    printline(ip_to_string(ip));
    
    if(Ping.ping(ip,1)) { 
      hosts[i] = 1; 
      circlegraph();
      sidegraph();
      clearline();
      tft.setTextColor(TFT_GREEN);
      printline(ip_to_string(ip));
      } else { 
      hosts[i] = 0;
      clearline();
      }
    if (y > 160) {
      y = 27;
      tft.fillRect(0,27,80,133,TFT_BLACK);
    }
   }
}
int print_hosts(int page) {
  // print live hosts page by page
  
  IPAddress ip = WiFi.localIP();
  int thispage = 0;
  bool printed = false;
  
      y = 27;
      tft.fillRect(0,27,80,133,TFT_BLACK);
      tft.setTextColor(TFT_GREEN);
      
      for (int i=1;i<255;i++) {
          ip[3] = i;
          if (hosts[i]) {
             if (thispage == page) {
              printline(ip_to_string(ip));
              printed = true;
             } 
          else y += 9;
                          
          if (y > 160) { 
              thispage++;
              y = 27;
             }
          }
       }
       return (printed);
}

void loop() {
  static int page = 0;
   
    if (digitalRead(button) == LOW) {
      int printed = print_hosts(page++);
      if (!printed) {
        page = 0; 
        print_hosts(page);
      }
    }
    delay (200);
  }
