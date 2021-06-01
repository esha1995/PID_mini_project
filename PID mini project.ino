#include "rgb_lcd.h"
#include <Adafruit_NeoPixel.h> // import rgb circle library thing

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(20, 6, NEO_GRB + NEO_KHZ800); // create our circular light as a object
rgb_lcd lcd;

int colorR = 255;
int colorG = 0;
int colorB = 0; 

int encoderButton = 0;
int encoderA = 2;
int encoderB = 3;
int buttonState;
int oldButtonState;

#define slider A0

int highCut = 20;
int lowCut = 50;
int reverb = 0;
int echo = 0;
int chorus = 0; 

int volume; 
int volumeRead;
int oldVolume = 0;

int encoderStateA;
int encoderStateB;

int aFirst;
int bFirst;

void setup() {
  pinMode(encoderButton, INPUT_PULLUP);
  pinMode(encoderA, INPUT_PULLUP);
  pinMode(encoderB, INPUT_PULLUP);
  pinMode(slider, INPUT);
  Serial.begin(9600);
  attachInterrupt(0, EncoderA, RISING);
  attachInterrupt(1, EncoderB, RISING);
  lcd.begin(16,2);
  lcd.print("Volume: ");
  pixels.begin();
}

void loop() {
  buttonState = digitalRead(encoderButton);
  volumeRead = analogRead(slider);
  //Serial.println(volumeRead);

  if(buttonState != oldButtonState){
    if(buttonState == LOW){
      Serial.println("button");
    }
    oldButtonState = buttonState;
  }
  
  if (volumeRead < 585){
    volume = map(volumeRead, 0, 585,0, 50);
  }
  else{
    volume = map(volumeRead, 586, 733,50, 100);
  }

  if (oldVolume >= volume+5 or oldVolume <= volume-5){
    Serial.println(map(volume, 0, 100, 100, 200));
    lcd.setCursor(9, 0);
    lcd.print("    ");
    lcd.setCursor(9, 0);
    lcd.print(volume);
    oldVolume = volume;
  }

  
  ledRing(map(volume, 0,100,0,20)); // code for led ring 
}



void EncoderA(){
  cli();
  encoderStateA = digitalRead(encoderA);
  encoderStateB = digitalRead(encoderB);
  if (encoderStateA == 1 && encoderStateB == 0){
    aFirst = 1; 
  }
  if (encoderStateA == 1 && encoderStateB == 1 && bFirst == 1){
    Serial.println("right");
    aFirst = 0;
    bFirst = 0;
  }
  sei(); 
}

void EncoderB(){
  cli();
  encoderStateA = digitalRead(encoderA);
  encoderStateB = digitalRead(encoderB);
  if (encoderStateA == 0 && encoderStateB == 1){
    bFirst = 1; 
  }
  if (encoderStateA == 1 && encoderStateB == 1 && aFirst == 1){
    Serial.println("left");
    aFirst = 0;
    bFirst = 0;
  }
  sei(); 
}

void ledRing(int index){
  for(int i=0;i<20;i++)
      {
        if (i < index){ // if the LED is under the index(encoder) value, then the LEDS will light
        pixels.setPixelColor(i, pixels.Color(colorR,colorG,colorB)); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
        else{ // else it will not light
        pixels.setPixelColor(i, 0); // Moderately bright green color.
        pixels.show(); // This sends the updated pixel color to the hardware.
        }
    }
}
