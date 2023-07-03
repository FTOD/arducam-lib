#include <cam.h>

const static int CS = 7;
Cam cam(CS);

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;
  Serial.println("Serial ready");
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

// uint8_t read_fifo_burst(ArduCAM myCAM)
// {
//   bool is_header = false;
//   uint8_t temp = 0, temp_last = 0;
//   uint32_t length = 0;
//   length = myCAM.read_fifo_length();
//   Serial.println(length, DEC);
//   if (length >= IMG_BUF_SIZE) // 512 kb
//   {
//     Serial.println(F("Data Over sized. Abort!"));
//     return -1;
//   }
//   if (length == 0) // 0 kb
//   {
//     Serial.println(F("FIFO buffer empty. Ignoring"));
//     return 0;
//   }
//   myCAM.CS_LOW();
//   myCAM.set_fifo_burst(); // Set fifo burst mode
//   temp = SPI.transfer(0x00);
//   length--;
//   int i = 0;
//   while (length--)
//   {
//     temp_last = temp;
//     temp = SPI.transfer(0x00);
//     if (is_header == true)
//     {
//       img_buf[i++] = temp;
//     }
//     else if ((temp == 0xD8) & (temp_last == 0xFF))
//     {
//       is_header = true;
//       img_buf[i++] = temp_last;
//       img_buf[i++] = temp;
//       Serial.println(F("Image Start Mark detected, transmission continues"));
//     }
//     if ((temp == 0xD9) && (temp_last == 0xFF))
//     {
//       Serial.println("Image End Mark detected, transimission completed");
//       img_buf[i++] = temp_last;
//       img_buf[i++] = temp;
//       break;
//     }
//     delayMicroseconds(15);
//   }
//   myCAM.CS_HIGH();
//   is_header = false;
//   return 1;
// }