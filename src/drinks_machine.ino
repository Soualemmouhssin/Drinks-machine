#include <Wire.h>  // Comes with Arduino IDE
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define OLED_ADDR   0x3C

Adafruit_SSD1306 display(-1);  

//------------Defining pins to use for relays 
int pins[16] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17} ; // pins of relays 
//------------Defining pins to use for rotary encoder
 #define outputA 31
 #define outputB 33
 #define button_pin 19 
String  names[10] = { "   Rum Punch     " , " Rum and Coke  "  , "Tequila Sunrise  " , "   Daiquiri     ", " Gin and Tonic   ", "  Screwdriver    ",
  "Vodka Cranberry  ", "  Vodka Sour    ","  Vodka Tonic    ", "      AMF        "};

 int aState;
 int aLastState;  

int g = sizeof(int) ; 
struct program {
  int Size ; 
  int relay_pins[16] ; 
  int times[16] ;
};
//-----------Programs------------
 
program drinks[10] ; 


/*--------Relays of every prgram------
 * here you can configure the time of activation for each relay for every drink
 * times are in milliseconds 
 * for pins numbers are from 0 to 11
 * you can change the pins to use for relays in int pins[16] = {2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17} ; // pins of relays 
 * then you can select relays for each drink from the pins table
 */

int Rum_Punch_pins [6] = { pins[0],pins[1],pins[9],pins[10],pins[11],pins[15]} ;
int Rum_Punch_times [6] = { 1996,1996,2038,4435,887,2660} ; 

int Rum_Coke_pins [3] = { pins[0],pins[7],pins[13]}; 
int Rum_Coke_times [6] = { 2660,10646,887} ; 

int Tequila_Sunrise_pins [3] = { pins[3],pins[9],pins[15]}; 
int Tequila_Sunrise_times [6] = { 2660,10646,1331} ; 

int Daiquiri_pins [3] = { pins[0],pins[13],pins[14]}; 
int Daiquiri_times [3] = { 3549,2076,887}; 

int Gin_Tonic_pins [3] = { pins[2],pins[6],pins[13]}; 
int Gin_Tonic_times [3] = { 3549 ,7097,887}; 

int Screwdriver_pins [3] = { pins[4],pins[9],pins[14]}; 
int Screwdriver_times[3] = { 2660 ,7097,887}; 

int Vodka_Cranberry_pins [3] = { pins[4],pins[11],pins[15]}; 
int Vodka_Cranberry_times[3] = { 2660 ,10646,1331}; 

int Vodka_Sour_pins [3] = { pins[4],pins[12],pins[14]}; 
int Vodka_Sour_times[3] = { 3549 ,1331,302}; 

int Vodka_Tonic_pins [3] = { pins[4],pins[6],pins[12]}; 
int Vodka_Tonic_times[3] = { 3549 ,7097,887}; 

int AMF_pins [9] = { pins[0],pins[2],pins[3],pins[4],pins[5],pins[8],pins[12],pins[13],pins[14]} ; 
int AMF_times [9] = { 1774,1774,1774,1774,1774,1774,745,745,2147} ; 
////////////////

int current_drink = 0 ; // counter of drinks 
bool test = true ; 
void setup() {
  // put your setup code here, to run once:
  display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDR);
  display.clearDisplay() ; 
  display.display();  
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(20,15);
  display.print(names[0]);
  display.display();
  
  Serial.begin(115200) ; 
   pinMode (outputA,INPUT);
   pinMode (outputB,INPUT);

    for (int i = 0 ; i<16; i++) {
      pinMode (pins[i] , OUTPUT); 
      digitalWrite(pins[i],LOW) ; 
    }
    pinMode(button_pin, INPUT_PULLUP) ; 
    populating() ; 
 aLastState = digitalRead(outputA); 
  }

void loop() {
rotary() ; 
 if (digitalRead(button_pin) ==LOW) execute(current_drink) ; 
}

void rotary () {

   aState = digitalRead(outputA); // Reads the "current" state of the outputA
   // If the previous and the current state of the outputA are different, that means a pulPinse has occured
   if (aState != aLastState ){     
     // If the outputB state is different to the outputA state, that means the encoder is rotating clockwise

     // forward mode
     if (digitalRead(outputB) != aState && test) { 
         if (current_drink == 9) {current_drink = 0 ;}
         else {current_drink = current_drink +1  ;} 
         Serial.println("forw") ; }
    /// backward mode  
     else if (test) {
         if (current_drink == 0){ current_drink = 9 ;}
         else {current_drink= current_drink -1  ; }
     }
       display.clearDisplay() ; 
       display.setTextColor(WHITE);
       display.setTextSize(1);
       display.setCursor(20,15);
       display.print(names[current_drink]);
       Serial.println(names[current_drink]);

       display.display();
       test  = !test ; 

   }

    aLastState = aState;

}

void execute(int  k) {
  display.clearDisplay() ; 
  display.setTextColor(WHITE);
  display.setTextSize(1);
  display.setCursor(20,15);
  display.print(names[current_drink]);
  display.display() ; 
unsigned long now  = millis() ; 
int t = 0 ; 
unsigned long max_time = getmax(drinks[k]) ; 
for ( t = 0 ; t<drinks[k].Size ; t++) {
     digitalWrite(drinks[k].relay_pins[t], HIGH) ; 
    }
while (1) {
    for (t = 0 ; t<drinks[k].Size ; t++) {
      if (millis() - now >drinks[k].times[t]) { digitalWrite(drinks[k].relay_pins[t], LOW) ; Serial.println(String(drinks[k].relay_pins[t])+ "  OFF") ; }
    }
    if (millis() - now > (max_time+100)) {Serial.println ("finished"); 
       display.clearDisplay() ; 
       display.setTextColor(WHITE);
      display.setTextSize(1);
       display.setCursor(20,15);
       display.print(names[current_drink]);
       display.display();
        return ; }    
    }
}
int getmax (program drink) {
 int o = drink.times[0] ; 
    for (int i = 0 ; i<drink.Size; i++) { o = max(drink.times[i],o) ;Serial.println(String(drink.times[i])); }
 return  o ; 
}
void populating() {
populate (0, Rum_Punch_pins,Rum_Punch_times,sizeof(Rum_Punch_pins)/g);
populate (1, Rum_Coke_pins,Rum_Coke_times,sizeof(Rum_Coke_pins)/g);
populate (2, Tequila_Sunrise_pins,Tequila_Sunrise_times,sizeof(Tequila_Sunrise_pins)/g);
populate (3, Daiquiri_pins,Daiquiri_times,sizeof(Daiquiri_pins)/g);
populate (4, Gin_Tonic_pins,Gin_Tonic_times,sizeof(Gin_Tonic_pins)/g);
populate (5, Screwdriver_pins,Screwdriver_times,sizeof(Screwdriver_pins)/g);
populate (6, Vodka_Cranberry_pins,Vodka_Cranberry_times,sizeof(Vodka_Cranberry_pins)/g);
populate (7, Vodka_Sour_pins,Vodka_Sour_times,sizeof(Vodka_Sour_pins)/g);
populate (8, Vodka_Tonic_pins,Vodka_Tonic_times,sizeof(Vodka_Tonic_pins)/g);
populate (9, AMF_pins,AMF_times,sizeof(AMF_pins)/g);
}
/*
 * 
 */
 void populate  (int k , int input[] , int durations[],int x) {
drinks[k].Size = x ; 

for (int i = 0 ; i <16 ; i++) {
    if (i<x)  {
        drinks[k].relay_pins[i] = input[i] ; 
        drinks[k].times[i] = durations[i] ; }
    else {
        drinks[k].relay_pins[i] = 20 ; 
        drinks[k].times[i] = 0; }
  }
}
