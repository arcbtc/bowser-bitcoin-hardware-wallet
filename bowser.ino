#include "39words.h"
#include "Button2.h"
#include "Bitcoin.h"
#include "SPIFFS.h"
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
String seedgeneratestr;
String savedseed;
String seedgeneratesarr[24];
String sdcommand;
const char *bar = "good bye";
bool buttona = false;
bool buttonb = false;
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
  tft.setCursor(70, 50, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.print("Loading");
  buttonA.setClickHandler(click);
  buttonB.setClickHandler(click);
  buttonA.setLongClickHandler(click);
  
  if(!SPIFFS.begin(true)){
    return;
  }
  sdchecker();
  
  if (sdcommand == "HARDRESET"){
     tft.fillScreen(TFT_BLACK);
     tft.setCursor(0, 20, 4);
     tft.setTextSize(1);
     tft.setTextColor(TFT_RED,TFT_BLACK);
     tft.println("Device will be");
     tft.println("reset, are you sure?");
     tft.println("Press A to continue");
     while (buttona == false){
      buttonA.loop();
     }
     
     seedmaker();  
     pinmaker();
    
  }
  else{
    seedchecker();
    enterpin(false);
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
    buttona = true;
    } else if (btn == buttonB) {
    buttonb = true;
    }
}

void seedchecker(){
    
  File otherfile = SPIFFS.open("/key.txt");
  savedseed = otherfile.readStringUntil('\n');
  otherfile.close();
  int seedcount = 0;
  
  for (int x = 0; x < 24; x++){
    Serial.println(getValue(savedseed,' ',x));
    for (int z = 0; z < 2048; z++){
    if (getValue(savedseed,' ',x) == seedwords[z]){
      Serial.println(seedwords[z]);
      seedcount = seedcount + 1;
    }
    } 
  }
  
  Serial.print(int(seedcount));
  if(int(seedcount) != 24){
    
     tft.fillScreen(TFT_BLACK);
     tft.setCursor(0, 20, 4);
     tft.setTextSize(1);
     tft.setTextColor(TFT_RED,TFT_BLACK);
     tft.println("Error: Reset device");
     tft.println("or restore from seed");
     tft.println("(See documentation)");
     delay(99999999999999999999999);
  }
  else{
    return;
  }
}

void seedmaker(){
  
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 40, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.println("Write seed words");
  tft.println("somewhere safe!");
  delay(6000);
  buttona = false;
  
  for (int z = 0; z < 24; z++){
    seedgeneratestr += seedwords[random(0,2047)] + " ";
    seedgeneratesarr[z] = seedwords[random(0,2047)];
  }
  
  File file = SPIFFS.open("/key.txt", FILE_WRITE);
  file.print(seedgeneratestr.substring(0, seedgeneratestr.length() - 1) + "\n");
  file.close();

  for (int z = 0; z < 24; z++){
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 20, 4);
      tft.setTextSize(1);
      tft.println("Word " + String(z+1));
      tft.setTextSize(2);
      tft.setTextColor(TFT_GREEN,TFT_BLACK);
      tft.println(seedgeneratesarr[z]);
      tft.setTextSize(1);
      tft.println("Press A for next");
      while (buttona == false){
          buttonA.loop();
      }
      buttona = false;
  }
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 40, 4);
  tft.println("Time to check");
  tft.println("the words!");
  delay(6000);
  for (int z = 0; z < 24; z++){
      tft.fillScreen(TFT_BLACK);
      tft.setCursor(0, 20, 4);
      tft.setTextSize(1);
      tft.println("Word " + String(z+1));
      tft.setTextSize(2);
      tft.setTextColor(TFT_GREEN,TFT_BLACK);
      tft.println(seedgeneratesarr[z]);
      tft.setTextSize(1);
      tft.println("Press A for next");
      while (buttona == false){
          buttonA.loop();
      }
      buttona = false;
   
  }
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 40, 4);
  tft.println("Words will also");
  tft.println("be saved to SD");
  delay(6000);
}
void pinmaker(){
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(0, 30, 4);
  tft.println("Enter pin using");
  tft.println("morse code, 3-5");
  tft.println("letters");
  delay(6000);
  enterpin(true);
}

void enterpin(bool set){
  morseletter = "";
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(70, 50, 4);
  tft.setTextSize(1);
  tft.setTextColor(TFT_GREEN,TFT_BLACK);
  tft.print("Enter pin");
  
  while(confirm == false){
     buttonA.loop();
     buttonB.loop();
   if ((millis() - timy) > 2000) {
      if (buttona == true){
         for (int z = 0; z < 26; z++){
             if (morseletter == ref[0][z]){
                passkey += ref[1][z];
                passhide += "*";
                tft.fillScreen(TFT_BLACK);
                tft.setTextSize(2);
                tft.setCursor(40, 40, 4);
                tft.setTextColor(TFT_RED,TFT_BLACK);
                if(set == true){
                   tft.print(passkey);
                }
                else{
                   tft.print(passhide);
                }
             }
        }
      buttona = false;
      morseletter = "";
     }
   }
      if(buttonb == true){
       if(passkey == "POO" || set == true ){
        tft.fillScreen(TFT_BLACK);
          tft.setCursor(70, 50, 4);
          tft.setTextSize(1);
          tft.setTextColor(TFT_GREEN,TFT_BLACK);
          tft.print("Welcome");
          delay(3000);
          confirm = true;
       }
       else if (passkey != "POO" && set == false){
          tft.fillScreen(TFT_BLACK);
          tft.setCursor(0, 50, 4);
          tft.setTextSize(1);
          tft.setTextColor(TFT_RED,TFT_BLACK);
          tft.print("Reset and try again");
          passkey = "";
          passhide = "";
          delay(3000);
       }
   }
   buttonb == false;
  }
  confirm = false;
}

void sdchecker(){
  sdcommand = "HARDRESET";
}

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
}
