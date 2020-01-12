#include <Adafruit_NeoPixel.h>


#define LED_PIN    1
#define LED_COUNT 30
Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}
void loop(){
ledDesign(30,0,25,0,0);
ledDesign(30,25,0,0,1);
ledDesign(30,0,0,25,0);
ledDesign(30,25,25,25,1);
}

void ledDesign(int speed, int r, int g, int b, int direction){
  if(direction == 0){
    for(int i = 0; i < 30; i++){
      strip.setPixelColor(i,r, g, b);
      strip.show();
      delay(speed);
      }
    }
  else{
    for(int i = 30; i > 0; i--){
      strip.setPixelColor(i,r, g, b);
      strip.show();
      delay(speed);
      }
    }
  
}

void setFill(int r, int g, int b){
  for(int i = 0; i < 30; i++){
  strip.setPixelColor(i,r, g, b);
  }
  strip.show();
}
