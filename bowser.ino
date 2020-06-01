
#include "Button2.h";
#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>
TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h


bool confirm = false;
#define BUTTON_A_PIN  0
#define BUTTON_B_PIN  35
unsigned long timy;
String passkey;
String morseletter;
String passhide;
bool letterget = false;
char ref[2][26][5]={
{"10","0111","0101","011","1","1101","001","1111","11","1000","010","1011","00","01","000","1001", "0010","101","111","0","110","1110","100","0110","0100","0011"},
{"A", "B",   "C",   "D",  "E","F",   "G",  "H",   "I", "J",   "K",  "L",   "M",  "N","O",  "P",    "Q",   "R",  "S",  "T","U",  "V",   "W",  "X",   "Y",   "Z"}
};
Button2 buttonA = Button2(BUTTON_A_PIN);
Button2 buttonB = Button2(BUTTON_B_PIN);

void setup() {
  Serial.begin(115200);
  delay(50);
  tft.init();
  tft.fillScreen(TFT_BLACK);
  tft.setRotation(1);
  Serial.println("\n\nMultiple Buttons Demo");
  
  buttonA.setClickHandler(click);
  buttonB.setClickHandler(click);
  buttonA.setLongClickHandler(click);
  tft.setCursor(70, 50, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.print("Enter pin");
  
  while(confirm == false){
   buttonA.loop();
   buttonB.loop();
   if ((millis() - timy) > 2000) {
      if (letterget == true){
        
         for (int z = 0; z < 26; z++){
               if (morseletter == ref[0][z]){
                passkey += ref[1][z];
                passhide += "*";
                tft.fillScreen(TFT_BLACK);
                tft.setTextSize(2);
                tft.setCursor(40, 40, 4);
                tft.setTextColor(TFT_RED,TFT_BLACK);
                tft.print(passhide);
               }
             }

      letterget = false;
      morseletter = "";
      }
      
   }
  }
}


void loop() {

}

void click(Button2& btn) {

    if (btn == buttonA) {
      
      switch (btn.getClickType()) {
        case SINGLE_CLICK:
            morseletter = morseletter + "1";
            Serial.print("0");
            timy = millis();
            break;
        case LONG_CLICK:
            morseletter = morseletter + "0";
            Serial.print("1");
            timy = millis();
            break;
      }
    letterget = true;
    } else if (btn == buttonB) {
       if(passkey == "GOD"){
        tft.fillScreen(TFT_BLACK);
          tft.setCursor(70, 50, 4);
          tft.setTextSize(1);
          tft.setTextColor(TFT_GREEN,TFT_BLACK);
          tft.print("Welcome");
          delay(3000);
          confirm = true;
       }
       else{
          tft.fillScreen(TFT_BLACK);
          tft.setCursor(70, 50, 4);
          tft.setTextSize(1);
          tft.setTextColor(TFT_RED,TFT_BLACK);
          tft.print("Try again");
          passkey = "";
          passhide = "";
          delay(3000);
       }
    }
}
