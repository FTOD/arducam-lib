#include <cam.h>
#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_ST7735.h> // Hardware-specific library for ST7789

const static int CAM_CS = 7;
const static int SHUTTLE_PIN = 8;

#define TFT_RST 2
#define TFT_DC 3
#define TFT_CS 4
Adafruit_ST7735 tft = Adafruit_ST7735(TFT_CS, TFT_DC, TFT_RST);
Cam cam(CAM_CS);
uint8_t img_buf[64 * 64 * 2];
void testdrawtext(char *text, uint16_t color);
void send_pic_via_serial(size_t size, uint8_t *buf);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Serial ready");
  pinMode(SHUTTLE_PIN, INPUT_PULLUP);

  Serial.print(F("Hello! ST77xx TFT Test"));
  tft.initR(INITR_BLACKTAB);
  uint16_t time = millis();
  // large block of text
  tft.fillScreen(ST77XX_BLACK);
  tft.fillScreen(ST77XX_BLACK);
  testdrawtext("Lorem ipsum dolor sit amet, consectetur adipiscing elit. Curabitur adipiscing ante sed nibh tincidunt feugiat. Maecenas enim massa, fringilla sed malesuada et, malesuada sit amet turpis. Sed porttitor neque ut ante pretium vitae malesuada nunc bibendum. Nullam aliquet ultrices massa eu hendrerit. Ut sed nisi lorem. In vestibulum purus a tortor imperdiet posuere. ", ST77XX_WHITE);
  digitalWrite(TFT_CS, HIGH);
  delay(1000);

  Serial.println("done");
  delay(1000);
  cam.setup();
  cam.configure_and_init();
}

void loop()
{
  cam.flush_fifo();
  delay(5);
  cam.start_capture();
  while (!cam.is_capture_done())
    ;
  // Serial.println("Capture done");
  cam.read_fifo_burst(64 * 64 * 2, img_buf);
  digitalWrite(CAM_CS, HIGH); // Deselect the camera.

  digitalWrite(TFT_CS, LOW); // Select the TFT.
  tft.drawRGBBitmap(0, 0, (uint16_t *)img_buf, 64, 64);
  digitalWrite(TFT_CS, HIGH); // Deselect the TFT.
  if (digitalRead(SHUTTLE_PIN) == LOW)
    send_pic_via_serial(64 * 64 * 2, img_buf);
  return;
}

void testdrawtext(char *text, uint16_t color)
{
  tft.setCursor(0, 0);
  tft.setTextColor(color);
  tft.setTextWrap(true);
  tft.print(text);
}

void send_pic_via_serial(size_t size, uint8_t *buf)
{
  Serial.print("IMG\n");
  Serial.write((uint8_t *)buf, size);
  Serial.print("\nEND\n");
}