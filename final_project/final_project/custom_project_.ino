#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_LEDBackpack.h"

Adafruit_BicolorMatrix matrix = Adafruit_BicolorMatrix();
int ran;
int ran2;
int ran3;
int ran4;
int ran5;
int ran6;
int ran3_1;
int ran3_2;
int ran3_3;

int x = 0;

int ran_l;
int i;
int block=0;
int incoming = 0;

int timing = 0;
int real_L = 0;
int uptime = 2;

int s = 0;
int cnt_d = 0;
int timer[] = { 80, 65, 45};
int on = 0;
void setup() {
  Serial.begin(9600);
  Serial.println("8x8 LED Matrix Test");
  
  matrix.begin(0x70);  // pass in the address
  matrix.setRotation(2);

}    

void loop() {
  if(on == 0) {
    ON();
    
  }
  matrix.clear();
  matrix.drawPixel(0, 1, LED_GREEN);
  matrix.writeDisplay();
  
  rand_nums();
  //level1();

    game();
  
}
void game() {
  i=0;
  while(i<4) {
    rand_nums();
    level1();
    i++;
  }
  i=0;
  while(i<6) {
    rand_nums();
    level2();
    i++;
  }
  i=0;
  while(i>= 0) {
    rand_nums();
    level3();
    i++;
  }
}

void ON(){
  matrix.clear();
  matrix.writeDisplay();
  if(Serial.available()>0) {
    incoming = Serial.read();
      if(incoming == 2 || incoming == 1) {
        on = 1;
      }
  }
  else {
    ON();
  }
}

void jump(){
  if(uptime ==1 || uptime == 0) {
    return;
  }
  else if(Serial.available()>0) {
    incoming = Serial.read();
    //Serial.println();
    //Serial.print("I received: ");
    //Serial.println(incoming, DEC);
    if(incoming == 1) {
      uptime = 1;
      s++;
    }
    else if(incoming == 2) {
      reset();
    }
  }
  else{
    uptime = 2;
    block = 0;
  }
}

void up(){
    if(uptime == 1) {
      block++;
      if(block == 3) {
        uptime = 0;
      }
    }
   else if(uptime == 0) {
     block--;
     if(block == 0) {
      uptime = 2;
     }
   }
}

void lose(int lose_lv) {
  int ll = lose_lv;
switch(ll){
    case 0:
      if ((block == 0) && (2 == (x+ran4))) {
        matrix.drawPixel(block, x+ran4, LED_YELLOW);
        matrix.writeDisplay();
        checkL(); 
      }
       if ((block == 0) && (2 == (x+ran5))) {
        matrix.drawPixel(block, x+ran5, LED_YELLOW);
        matrix.writeDisplay();
        checkL(); 
      }   
    case 1:

      if ((block == ran) && (2 == (x+ran4))) {
        matrix.drawPixel(block, x+ran4, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      else if ((block == ran2) && (2 == (x+ran5))) {
        matrix.drawPixel(block, x+ran5, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      else if ((block == ran3) && (2 == (x+ran6))) {
        matrix.drawPixel(block, x+ran6, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      else if ((block == ran) && (2 == x)) {
        matrix.drawPixel(block, x, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      break;
    case 2:
      if ((block == ran) && (2 == (x+ran4))) {
        matrix.drawPixel(ran, x+ran4, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      else if ((block == ran2) && (2 == (x+ran5)))  {
        matrix.drawPixel(ran2, x+ran5, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      else if ((block == ran3) && (2 == (x+ran6))) {
        matrix.drawPixel(ran3, x+ran6, LED_YELLOW);
        matrix.writeDisplay();
        checkL();
      }
      break;
    default:
      break;
}
  } 


void delayz(int lv) {
  int l = lv;
  while(cnt_d < timer[l]) {  //timer[l]
        cnt_d++;
        Serial.print("ddd");
  }
  s++;
    Serial.println(s);
   cnt_d = 0;
}

void sendscore(){
  //int t = s/350;
  Serial.write(s);
}

void checkL() {
  if(Serial.available()>0) {
    incoming = Serial.read();
    //Serial.print("I received: ");
    //Serial.println(incoming, DEC);
    if(incoming == 2) {
      reset();
    }
    if (incoming == 1) {
      reset();
    }
  }
  else {
    sendscore();
      checkL();
  }
}

void reset() {
  s = 0;
  ran_l=0;
  block=0;
  timing = 0;
  real_L = 0;
  uptime = 2;
  cnt_d = 0;
  //game();
  loop();
  
}

void level1(){
  for ( x=10; x>=-1; x--) {
    matrix.clear();
    jump(); 
    up();
    matrix.drawPixel(block, 2, LED_GREEN);
    matrix.setCursor(x,0);
    matrix.drawPixel(0, x+ran4, LED_RED);
    matrix.drawPixel(0, x+ran5, LED_RED);
    matrix.writeDisplay();
    lose(0);
    delayz(0);
        sendscore();

  }
}

void level2(){
  ran_l = random(1,4);
    for ( x=7; x>=-1; x--) {
      matrix.clear();
      jump(); 
      up();
      matrix.drawPixel(block, 2, LED_GREEN);
      matrix.setCursor(x,0);
//      jump(); 
//      up();
      if (ran_l == 3) {
        matrix.drawPixel(ran, x+ran4, LED_RED);
        matrix.drawPixel(ran2, x+ran5, LED_RED);
        matrix.drawPixel(ran3, x+ran6, LED_RED);
      }
      else if(ran_l == 2) {
        matrix.drawPixel(ran, x+ran4, LED_RED);
        matrix.drawPixel(ran2, x+ran5, LED_RED);
      }
      else if(ran_l == 1) {
        matrix.drawPixel(ran, x, LED_RED);
      }
      else { matrix.drawPixel(0, 0, LED_RED); }
    
    matrix.writeDisplay();
    lose(1); 
    delayz(1);
    sendscore();
    }
}
    
void level3(){
  ran_l = random(1,5);
  for ( x=7; x>=-1; x--) {
    matrix.clear();
    matrix.drawPixel(block, 2, LED_GREEN);
    matrix.setCursor(x,0);
    jump(); 
    up();
    if (ran_l == 3) {
        matrix.drawPixel(ran, x+ran4, LED_RED);
        matrix.drawPixel(ran2, x+ran5, LED_RED);
        matrix.drawPixel(ran3, x+ran6, LED_RED);
        matrix.drawPixel(ran2, x+ran, LED_RED);
      }
      else if(ran_l == 2) {
        matrix.drawPixel(ran, x+ran4, LED_RED);
        matrix.drawPixel(ran2, x+ran5, LED_RED);
      }
      else if(ran_l == 1) {
        matrix.drawPixel(ran, x+ran4, LED_RED);
      }
      else if(ran_l == 4) {
        matrix.drawPixel(0, x-1, LED_RED);
        matrix.drawPixel(1, x, LED_RED);
        matrix.drawPixel(0, x+1, LED_RED);
      }
      else { matrix.drawPixel(0, 0, LED_RED); }
    matrix.writeDisplay();    
    lose(2);
    delayz(2);
    sendscore();
    
  }
}

void rand_nums(){
  ran = random(0,2);
  ran2 = random(0,2);
  ran3 = random(0,2);
  ran4 = random(0,2);
  ran5 = random(0,2);
  ran6 = random(0,2);
  ran3_1 = random(0,3);
  ran3_2 = random(0,3);
  ran3_3 = random(0,3);
}
