#include <M5Stack.h> 
#include "bowser.c"
#include "decoy.h"
#include "Bitcoin.h"
#include "Hash.h"
#include "SPIFFS.h"
#include "PSBT.h"

bool buttona = false;
bool buttonb = false;
bool buttonc = false;
bool confirm = false;
unsigned long timy;
String passkey;
String morseletter;
String passhide;
String seedgeneratestr;
String savedseed;
String seedgeneratesarr[24];
String sdcommand;
String hashed;
String savedpinhash;
String privatekey;
String pubkey;

char ref[2][26][5]={
{"10","0111","0101","011","1","1101","001","1111","11","1000","010","1011","00","01","000","1001", "0010","101","111","0","110","1110","100","0110","0100","0011"},
{"A", "B",   "C",   "D",  "E","F",   "G",  "H",   "I", "J",   "K",  "L",   "M",  "N","O",  "P",    "Q",   "R",  "S",  "T","U",  "V",   "W",  "X",   "Y",   "Z"}
};

void setup(void) {
  M5.begin();                   // M5STACK INITIALIZE
  M5.Lcd.setBrightness(200);    // BRIGHTNESS = MAX 255
  M5.Lcd.fillScreen(BLACK);     // CLEAR SCREEN
  decoysetup();
  M5.Lcd.setBrightness(100);
  M5.Lcd.drawBitmap(0, 0, 320, 240, (uint8_t *)Wallet_map);
  delay(3000);
  if(!SPIFFS.begin(true)){
    return;
  }
  sdchecker();

  if (sdcommand == "HARDRESET WIPE DEVICE"){
     M5.Lcd.fillScreen(BLACK);
     M5.Lcd.setCursor(0, 90);
     M5.Lcd.setTextColor(RED);
     M5.Lcd.setTextSize(2);
     M5.Lcd.println("   Device will be reset,");
     M5.Lcd.println("   are you sure?");
     M5.Lcd.println("");
     M5.Lcd.println("   Press A to continue");
     
     while (buttona == false){
        if (M5.BtnA.wasReleased()){buttona = true; M5.Lcd.fillScreen(BLACK);}
        M5.update(); 
     }

     buttona = false;
     
     seedmaker();  
     pinmaker();
    
  }
    if (sdcommand == "GETXPUB"){
     
    seedchecker();
    enterpin(false);
    int str_len = pubkey.length() + 1; 
    char char_array[str_len];
    pubkey.toCharArray(char_array, str_len);
    writeFile(SD, "/bowser.txt", char_array);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(0, 100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("   XPUB has been");
    M5.Lcd.println("   saved to SD");
    delay(2000);
    M5.Lcd.fillScreen(BLACK);
  }
    if (sdcommand == "GETSEED"){
     
    seedchecker();
    enterpin(false);
    savedseed = "Keep you seed phrase safe but dont lose them! \n" + savedseed + "\n To learn more about seed phrases visit https://en.bitcoin.it/wiki/Seed_phrase";

    int str_len = savedseed.length() + 1; 
    char char_array[str_len];
    savedseed.toCharArray(char_array, str_len);
    writeFile(SD, "/bowser.txt", char_array);
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setCursor(0, 100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("   Seed has been");
    M5.Lcd.println("   saved to SD");
    delay(2000);
    M5.Lcd.fillScreen(BLACK);
    
  
  }
  if (sdcommand.substring(0, 4) == "PSBT"){
    String psbttx = sdcommand.substring(5, sdcommand.length() + 1);
    seedchecker();
    enterpin(false);
    Serial.println(psbttx);
    
    HDPrivateKey hd(savedseed, passkey);
    PSBT tx;
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 90);
    M5.Lcd.setTextColor(GREEN);
    M5.Lcd.setTextSize(3);
    M5.Lcd.println("   Bwahahaha!");
    M5.Lcd.println("");
    M5.Lcd.setTextSize(3);
    M5.Lcd.println("   PSBT found");
    delay(3000);
    
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.setTextSize(2);
    
    //tx.parse(sdcommand.substring(5, sdcommand.length()-5));
    
    int str_lenn = psbttx.length() + 1; 
    char char_arrayy[str_lenn];
    psbttx.toCharArray(char_arrayy, str_lenn);
    tx.parse(char_arrayy);
      
    M5.Lcd.println("Outputs:");
    M5.Lcd.println("");
    M5.Lcd.setTextSize(1);
    for(int i=0; i<tx.tx.outputsNumber; i++){
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.print(tx.tx.txOuts[i].address(&Testnet));
      if(tx.txOutsMeta[i].derivationsLen > 0){ // there is derivation path
        // considering only single key for simplicity
        PSBTDerivation der = tx.txOutsMeta[i].derivations[0];
        if(hd.derive(der.derivation, der.derivationLen).publicKey() == der.pubkey){
          M5.Lcd.print(" (change) ");
        }
      }
      M5.Lcd.println("");
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.print(" -> ");
      M5.Lcd.print(tx.tx.txOuts[i].btcAmount()*1e3);
      M5.Lcd.println(" mBTC");
      M5.Lcd.println("");
     
      
    }
    M5.Lcd.setTextSize(2);
    M5.Lcd.print("Fee: ");
    M5.Lcd.print(float(tx.fee())/100); // Arduino can't print 64-bit ints
    M5.Lcd.println(" bits");
    M5.Lcd.println("");
    M5.Lcd.print("Press A to sign");
    
     while (buttona == false){
        if (M5.BtnA.wasReleased()){buttona = true; M5.Lcd.fillScreen(BLACK);}
        M5.update(); 
     }
     buttona = false;

    tx.sign(hd);
    String signedtx = tx;
    Serial.print(signedtx);
  
    int str_len = signedtx.length() + 1; 
    char char_array[str_len];
    signedtx.toCharArray(char_array, str_len);

    writeFile(SD, "/bowser.txt", char_array);

 
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 100);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("   Signed transaction");
    M5.Lcd.println("   saved to SD");
    delay(3000);
    sdcommand = "";
    
  }
  else{
    seedchecker();
    enterpin(false);
  }
}

void loop() {

}

void seedchecker(){
  File otherfile = SPIFFS.open("/key.txt");
  savedseed = otherfile.readStringUntil('\n');
  otherfile.close();
  int seedcount = 0;
  
  for (int x = 0; x < 24; x++){
    for (int z = 0; z < 2048; z++){
      if (getValue(savedseed,' ',x) == seedwords[z]){
        seedcount = seedcount + 1;
      }
    } 
  }
  
  if(int(seedcount) != 24){
    M5.Lcd.fillScreen(BLACK);
    M5.Lcd.setCursor(0, 90);
    M5.Lcd.setTextColor(RED);
    M5.Lcd.setTextSize(2);
    M5.Lcd.println("   Error: Reset device");
    M5.Lcd.println("   or restore from seed");
    M5.Lcd.println("   (See documentation)");
    delay(99999999999999999999999);
  }
  else{
    return;
  }
}

void seedmaker(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("   Write seed words");
  M5.Lcd.println("   somewhere safe!");
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
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 70);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("   Word " + String(z+1));
      M5.Lcd.println("");
      M5.Lcd.setTextSize(5);
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.println("  " + seedgeneratesarr[z]);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("");
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.println("   Press A for next");
      
     while (buttona == false){
        if (M5.BtnA.wasReleased()){buttona = true;}
        M5.update(); 
     }
     buttona = false;
     
  }
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("   Time to check");
  M5.Lcd.println("   the words!");
  delay(6000);
  for (int z = 0; z < 24; z++){
      M5.Lcd.fillScreen(BLACK);
      M5.Lcd.setCursor(0, 70);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("   Word " + String(z+1));
      M5.Lcd.println("");
      M5.Lcd.setTextSize(5);
      M5.Lcd.setTextColor(BLUE);
      M5.Lcd.println("  " + seedgeneratesarr[z]);
      M5.Lcd.setTextSize(2);
      M5.Lcd.println("");
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.println("   Press A for next");

     while (buttona == false){
        if (M5.BtnA.wasReleased()){buttona = true;}
        M5.update(); 
     }
     buttona = false;
  }
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.println("   Words will also");
  M5.Lcd.println("   be saved to SD");


  seedgeneratestr = "Keep you seed phrase safe but dont lose them! \n" + seedgeneratestr + "\n To learn more about seed phrases visit https://en.bitcoin.it/wiki/Seed_phrase";
  int str_len = seedgeneratestr.length() + 1; 
  char char_array[str_len];
  seedgeneratestr.toCharArray(char_array, str_len);


  writeFile(SD, "/bowser.txt", char_array);
  
  delay(6000);
}
void pinmaker(){
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 90);
  M5.Lcd.println("   Enter pin using");
  M5.Lcd.println("   use morse code,");
  M5.Lcd.println("   3 letters at least");
  delay(6000);
  enterpin(true);
}

void enterpin(bool set){
  morseletter = "";
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.print("    Enter pin");
  
  while(confirm == false){
   if (M5.BtnA.wasReleased()){
    buttona = true; 
    morseletter = morseletter + "1";
    timy = millis();
   }
   if (M5.BtnB.wasReleased()){
    buttonb = true; 
    morseletter = morseletter + "0";
    timy = millis();
   }
   if (M5.BtnC.wasReleased()){
     
     if (set == true){
       uint8_t newpasskeyresult[32];
       sha256(passkey, newpasskeyresult);
       hashed = toHex(newpasskeyresult,32);
          
       File file = SPIFFS.open("/pass.txt", FILE_WRITE);
       file.print(hashed + "\n");
       file.close();
     }
     
     File otherfile = SPIFFS.open("/pass.txt");
     savedpinhash = otherfile.readStringUntil('\n');
     otherfile.close();
         
     uint8_t passkeyresult[32];
     sha256(passkey, passkeyresult);
     hashed = toHex(passkeyresult,32);
     Serial.println(savedpinhash);
     Serial.println(hashed);
     
     if(savedpinhash == hashed || set == true ){
        getKeys(savedseed, passkey);
        passkey = "";
        passhide = "";
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 100);
        M5.Lcd.setTextSize(3);
        M5.Lcd.setTextColor(GREEN);
        M5.Lcd.print("     Welcome");
        delay(3000);
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setCursor(0, 60);
        M5.Lcd.println("   Feed me something");
        M5.Lcd.println("   to sign");
        M5.Lcd.println("");
        M5.Lcd.println("");
        M5.Lcd.setTextColor(BLUE);
        M5.Lcd.println(pubkey);
        M5.Lcd.setTextColor(GREEN);
        delay(1000);
        confirm = true;
     }
     else if (savedpinhash != hashed && set == false){
        M5.Lcd.fillScreen(BLACK);
        M5.Lcd.setCursor(0, 110);
        M5.Lcd.setTextSize(2);
        M5.Lcd.setTextColor(RED);
        M5.Lcd.print("   Reset and try again");
        passkey = "";
        passhide = "";
        delay(3000);
     }
   }
   M5.update(); 
   if ((millis() - timy) > 2000) {
      if (buttona == true || buttonb == true){
         for (int z = 0; z < 26; z++){
             if (morseletter == ref[0][z]){
                passkey += ref[1][z];
                passhide += "* ";
                M5.Lcd.fillScreen(BLACK);
                M5.Lcd.setTextSize(3);
                M5.Lcd.setCursor(0, 90);
                M5.Lcd.setTextColor(GREEN);
                if(set == true){
                   M5.Lcd.print("   " + passkey);
                }
                else{
                   M5.Lcd.print("   " + passhide);
       }}}
      buttona = false;
      buttonb = false;
      morseletter = "";
   }}}
  confirm = false;
}

void sdchecker(){
  readFile(SD, "/bowser.txt");

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



void getKeys(String mnemonic, String password)
{

  HDPrivateKey hd(mnemonic, password);

  if(!hd){ // check if it is valid
    Serial.println("   Invalid xpub");
    return;
  }
  
  HDPrivateKey account = hd.derive("m/84'/0'/0'/");
  privatekey = account;
  pubkey = account.xpub();
 
}



void readFile(fs::FS &fs, const char * path) {
    File file = fs.open(path);
    if(!file){
        M5.Lcd.println("   Failed to open file for reading");
        return;
    }
    
    while(file.available()){
        sdcommand = file.readStringUntil('\n');

    }
    Serial.println(sdcommand);
}

void writeFile(fs::FS &fs, const char * path, const char * message){


    File file = fs.open(path, FILE_WRITE);
    if(!file){

        M5.Lcd.println("   Failed to open file for writing");
        return;
    }
    if(file.print(message)){

    } else {
        M5.Lcd.println("   Write failed");
    }
}
