#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_ADDR 0x3C
#define RedLED 16
#define GreenLED 17
#define BlueLED 18
const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};
const char Redbtn = 'A';
const char Greenbtn = 'B';
const char Bluebtn = 'C';
int CurrentRedValue = 255;
int CurrentGreenValue = 255;
int CurrentBlueValue = 255;
const int LEDPins[] ={RedLED, BlueLED, GreenLED};
int LEDValues[] ={CurrentRedValue, CurrentGreenValue, CurrentBlueValue};
bool LightSwitch = true;
byte rowPins[ROWS] = {13, 14, 27, 26}; 
byte colPins[COLS] = {25,33,32,19};

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned long lastDisplayUpdate = 0;
const unsigned long DISPLAY_INTERVAL = 200; // ms between OLED redraws
int counterVal = 0;
Keypad keypad = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
String LastButtonPressed ="";
String wholeString = "";

void LightsOut(){
 for (const auto& pin: LEDPins) 
 analogWrite(pin, 0);
  CurrentRedValue = 0;
  CurrentGreenValue = 0;
  CurrentBlueValue = 0;
}

void LightsOn(){
 for (const auto& pin: LEDPins) 
 analogWrite(pin, 255);
   CurrentRedValue = 255;
  CurrentGreenValue = 255;
  CurrentBlueValue = 255;
}

void setup_lights() {
  display.println("Starting LED Setup");
  display.display();
  pinMode(RedLED, OUTPUT);
  pinMode(GreenLED, OUTPUT);
  pinMode(BlueLED, OUTPUT);
  display.println("Red LED Test");
  display.display();
  delay(500);
  analogWrite(RedLED, CurrentRedValue);
  delay(2000);
  analogWrite(RedLED, 0);
  delay(2000);
  display.println("Green LED Test");
  display.display();
  delay(500);
  analogWrite(GreenLED, CurrentGreenValue);
  delay(2000);
  analogWrite(GreenLED, 0);
  delay(2000);
  display.println("Blue LED Test");
  display.display();
  delay(500);
  analogWrite(BlueLED, CurrentBlueValue);
  delay(2000);
  analogWrite(BlueLED, 0);
  delay(2000);
  display.clearDisplay();
  display.setCursor(0,0);
  
  for (size_t i = 0; i < 35; i++)
  {
    analogWrite(RedLED, CurrentRedValue);
    analogWrite(GreenLED, CurrentGreenValue);
    analogWrite(BlueLED, CurrentBlueValue);
    
    display.println("Full Powa Time!!!");
    display.display();
  }
  
  delay(2000);
  display.clearDisplay();
  int16_t x1, y1;
  uint16_t w, h;
  const char* doneMsg = "LED SETUP Done!";
  display.getTextBounds(doneMsg, 0, 0, &x1, &y1, &w, &h);

  int16_t centeredX = (SCREEN_WIDTH - w) / 2 - x1;
  int16_t centeredY = (SCREEN_HEIGHT - h) / 2 - y1;

  display.setCursor(centeredX, centeredY);
  display.println(doneMsg);
  display.display();

}

void setup() {
  Wire.begin(21, 22);
  Serial.begin(115200);
  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR)) {
    while (true) { delay(1000); }
  }
  analogWriteFrequency(60000);


  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Hello, ESP32!");
  display.display();
  setup_lights();
  display.println("Boot up done");
  delay(5000);
  Serial.println("Booting finished"); 
}


void loop() {
  

  char key = keypad.getKey();
  if(key){
    
    wholeString += key;

    switch(key){
      case Redbtn:
        LastButtonPressed = "Red BUTTON";
        if(CurrentRedValue == 255){
          CurrentRedValue = 0;
          if(!CurrentBlueValue && !CurrentGreenValue){
            LightSwitch = false;
          }
        }else{
          CurrentRedValue = 255;
          if(!LightSwitch){LightSwitch=true;}
        }
        analogWrite(RedLED, CurrentRedValue);
        break;
      case Greenbtn:
        LastButtonPressed = "Green BUTTON";
        if(CurrentGreenValue == 255){
          CurrentGreenValue = 0;
          if(!CurrentBlueValue && !CurrentRedValue){
            LightSwitch = false;
          }
        }else{
          CurrentGreenValue = 255;
          if(!LightSwitch){LightSwitch=true;}
        }
        analogWrite(GreenLED, CurrentGreenValue);
        break;
      case Bluebtn:
        LastButtonPressed = "BLUE BUTTON";
        if(CurrentBlueValue == 255){
          CurrentBlueValue = 0;
          if(!CurrentGreenValue && !CurrentRedValue){LightSwitch=false;}
        }else{
          CurrentBlueValue = 255;
          LightSwitch=true;
        }
        analogWrite(BlueLED, CurrentBlueValue);
        break;
      case '*':
        LightsOut();
        LightSwitch = false;
        break;
      case '#':
        LightsOn();
        LightSwitch = true;
        break;
      case 'D':
        if (LightSwitch == true) {
          LightsOut();
          LightSwitch = false;
        } else {
          LightsOn();
          LightSwitch = true;
        }
        break;
      default:
        LastButtonPressed = key;
        break;


    }
    
    
    
    
  }

  // "Loop" #2 — only actually fires every DISPLAY_INTERVAL ms
  unsigned long now = millis();
  if (now - lastDisplayUpdate >= DISPLAY_INTERVAL) {
    lastDisplayUpdate = now;
    counterVal++;

    display.clearDisplay();
    display.setCursor(0, 0);
    display.print("Counter ");
    display.println(counterVal);
    display.println(LastButtonPressed);
    display.println(wholeString);
    display.display();
  }


}