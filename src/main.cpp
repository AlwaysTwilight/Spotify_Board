#include <Arduino.h>
#include <SPI.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ST7735.h>

// VC-02 on UART2: VC-02 TX1 -> ESP32 D16 (RX), VC-02 RX1 -> ESP32 D17 (TX)
static const int VC02_RX = 16;
static const int VC02_TX = 17;
static const uint32_t VC02_BAUD = 9600;  // change here if the VC-02 firmware uses another rate

// TFT (1.8" 128x160 ST7735): SCK=D18, SDA(MOSI)=D23, CS=D5, A0(DC)=D4, RESET=D2
static const int TFT_CS = 5;
static const int TFT_DC = 4;
static const int TFT_RST = 2;

HardwareSerial vc02(2);
Adafruit_ST7735 tft(TFT_CS, TFT_DC, TFT_RST);  // uses hardware SPI (SCK D18, MOSI D23)

const char *replyForByte(uint8_t b) {
  switch (b) {
    case 0x01: return "Hi, how are you?";
    case 0x02: return "I'm good, thanks!";
    case 0x03: return "Goodbye!";
    default: return nullptr;
  }
}

const char *replyFor(String cmd) {
  cmd.trim();
  cmd.toLowerCase();
  if (cmd == "hi" || cmd == "hello") return "Hi, how are you?";
  if (cmd == "how are you") return "I'm good, thanks!";
  if (cmd == "bye") return "Goodbye!";
  return nullptr;
}

void showText(const char *title, const char *text, uint16_t color) {
  tft.fillScreen(ST77XX_BLACK);
  tft.setTextWrap(true);
  tft.setTextColor(ST77XX_CYAN);
  tft.setTextSize(1);
  tft.setCursor(4, 4);
  tft.print(title);
  tft.setTextColor(color);
  tft.setTextSize(2);
  tft.setCursor(4, 30);
  tft.print(text);
}

void setup() {
  Serial.begin(115200);
  vc02.begin(VC02_BAUD, SERIAL_8N1, VC02_RX, VC02_TX);

  tft.initR(INITR_BLACKTAB);  // if colours/offsets look wrong, try INITR_GREENTAB or INITR_REDTAB
  tft.setRotation(1);         // landscape
  showText("Voice assistant", "Say hi...", ST77XX_WHITE);

  Serial.println("Ready. Say a command to the VC-02, or type hi / how are you / bye here.");
}

void loop() {
  while (vc02.available()) {
    uint8_t b = vc02.read();
    Serial.printf("VC-02 -> 0x%02X\n", b);
    const char *r = replyForByte(b);
    if (r) {
      showText("You said:", r, ST77XX_GREEN);
    } else {
      char buf[24];
      snprintf(buf, sizeof(buf), "Unknown 0x%02X", b);
      showText("VC-02 sent:", buf, ST77XX_YELLOW);
    }
  }

  if (Serial.available()) {
    String line = Serial.readStringUntil('\n');
    line.trim();
    if (line.length()) {
      const char *r = replyFor(line);
      showText(line.c_str(), r ? r : "Unknown command", r ? ST77XX_GREEN : ST77XX_YELLOW);
    }
  }
}
