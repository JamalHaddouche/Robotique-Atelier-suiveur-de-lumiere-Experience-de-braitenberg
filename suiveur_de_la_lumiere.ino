#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F,16,2);
enum MotorType{Fear,Lover,Aggressive,Explorer,Off};


#define left 0
#define right !left
String modes[5] = {"FEAR","LOVER","AGGRESSIVE","EXPLORER","OFF"};
#define photoResistorLeft A3
#define photoResistorRight A2
#define boutonPin 2
struct Motor{
  int brake,direction_,speed_;
};

struct Motor motor2,motor1;
MotorType typeDeSuiveur = Off;



void setup() {
  lcd.init();
  printMode();
  
  pinMode(photoResistorLeft,INPUT);
  pinMode(photoResistorRight,INPUT);
  Serial.begin(9600);
  lcd.backlight();
  initMotor(&motor2,9,12,3);
  initMotor(&motor1,8,13,11);

  pinMode(boutonPin,INPUT_PULLUP);
}
void printMode(){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("MASTER");
  lcd.setCursor(0,1);
  //lcd.print("Mode: "+modes[typeDeSuiveur]);
  lcd.print("IAAD");
}
#define minLuminosite 100
void loop() {
  int b = !digitalRead(boutonPin);
  delay(50);
  int delta = 50;
  int l = analogRead(photoResistorLeft)+delta;
  int r = analogRead(photoResistorRight);
  
  Serial.print("left: ");
  Serial.println(l);
  Serial.print("right: ");
  Serial.println(r);
  Serial.print("delta: ");
  Serial.println(l-r);
  if(b){
    swapType();
    delay(100);
  }
  
  l = max(l,minLuminosite);
  l = min(l,1024);
  r = max(r,minLuminosite);
  Serial.print("Mode:");
  switch(typeDeSuiveur){
  case Fear:
    fear(l,r);
    break;
  case Aggressive:
    aggressive(l,r);
    break;
  case Explorer:
    explorer(l,r);
    break;
  case Lover:
    lover(l,r);
    break;
  case Off:
    Serial.println("off");
    arreter();
    break;
  default:
      break;
  }
}

void fear(int l,int r){
  int vitesse1 = map(l,minLuminosite,1024,0,255);
  int vitesse2 = map(r,minLuminosite,1024,0,255);
  avancer(vitesse1,vitesse2);
  Serial.println("fear");
}
void aggressive(int l,int r){
  int vitesse1 = map(l,minLuminosite,1024,0,255);
  int vitesse2 = map(r,minLuminosite,1024,0,255);
  avancer(vitesse2,vitesse1);
  Serial.println("aggressive");
}
void lover(int l,int r){
  int vitesse1 = map(l,minLuminosite,1024,255,0);
  int vitesse2 = map(r,minLuminosite,1024,255,0);
  avancer(vitesse1,vitesse2);
  Serial.println("lover");
}
void explorer(int l,int r){
  int vitesse1 = map(l,minLuminosite,1024,255,0);
  int vitesse2 = map(r,minLuminosite,1024,255,0);
  avancer(vitesse2,vitesse1);
  Serial.println("explorer");
}

void swapType(){
  typeDeSuiveur = (MotorType)((typeDeSuiveur + 1) % 5);
  printMode();
}
void initMotor(Motor* motor,int br,int dir,int spd){
  motor->brake = br;
  motor->direction_= dir;
  motor->speed_ = spd;
  
  pinMode(motor->brake, OUTPUT); 
  pinMode(motor->direction_, OUTPUT);
  pinMode(motor->speed_, OUTPUT);
}


void avancer(int spd1,int spd2){
  digitalWrite(motor1.brake,LOW);
  digitalWrite(motor2.brake,LOW);
  digitalWrite(motor1.direction_,HIGH);
  digitalWrite(motor2.direction_,HIGH);
  analogWrite(motor1.speed_,spd1);
  analogWrite(motor2.speed_,spd2);
}
void arreter(){
  digitalWrite(motor1.brake,HIGH);
  digitalWrite(motor2.brake,HIGH);
}
