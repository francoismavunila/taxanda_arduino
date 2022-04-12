#include "base64.hpp"

typedef uint8_t u8;
u8 output_buffer[10];
u8 input_buffer[10]={123,127,45,52,71,0,87,6,87,1};



void setup() {
  Serial.begin(115200);
}

void loop() {

      encode_base64(input_buffer, 10, output_buffer);
      Serial.println("converted");
      Serial.println((char *) output_buffer);
delay(2000);
}
