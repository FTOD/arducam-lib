#include <cam.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7789.h> // Hardware-specific library for ST7789

const static int CS = 7;

#define TFT_RST 2
#define TFT_DC 3
#define TFT_CS 4
Adafruit_ST7789 tft = Adafruit_ST7789(TFT_CS, TFT_DC, TFT_RST);
Cam cam(CS);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Serial ready");

  Serial.print(F("Hello! ST77xx TFT Test"));
  tft.init(160, 128);
  uint16_t time = millis();
  tft.fillScreen(ST77XX_BLACK);
  time = millis() - time;

  Serial.println(time, DEC);
  delay(500);

  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  delay(1000);

  Serial.println("done");
  digitalWrite(TFT_CS, HIGH);
  delay(1000);
  cam.setup();
  cam.configure_and_init();
}

void loop()
{
  // DEBUG
  Serial.println("LOOP");
  cam.flush_fifo();
  // DEBUG
  cam.flush_fifo();
  delay(5);
  cam.start_capture();
  while (!cam.is_capture_done())
    ;
  cam.read_fifo_burst();
  delay(100);
  return;
}
