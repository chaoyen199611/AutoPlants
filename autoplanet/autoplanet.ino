#include <SoftwareSerial.h>   // 引用程式庫
#include <LiquidCrystal_I2C.h>
#include "DHT.h"
#include <Wire.h> 
LiquidCrystal_I2C lcd(0x27,16,2);

#define DHTPIN 2
#define DHTTYPE DHT11
DHT dht(DHTPIN, DHTTYPE);

const int p_resistor=A1;
const int soil_moisture=A3;
const int In1 = 4;
const int In2 = 5;
const int In3 = 6;      
const int In4 = 7;

int bluePin=8;
int waterPin=3;  
int fancontrolPin=A0;
int fandigitalPin=12;

//sensor value
float h=0;
float t=0;
int light=0;
int soil=0;
// 定義連接藍牙模組的序列埠
SoftwareSerial BT(10, 11); // 接收腳, 傳送腳
char val;  // 儲存接收資料的變數
int count=0;
int fcount=0;
double fancontrolvalue=0;
bool openlight=false;
bool openfan=false;
bool wateruse=false;

void setup() {
  Serial.begin(9600);   // 與電腦序列埠連線
  Serial.println("BT is ready!");

  digitalWrite(bluePin,OUTPUT);
  lcd.init(); //初始化LCD 
  lcd.backlight(); //開啟背光
  lcd.print("Welcome!"); //顯示Hello, World!
  pinMode(fandigitalPin,OUTPUT);
  pinMode(waterPin,OUTPUT);
  pinMode(p_resistor,INPUT);
  pinMode(soil_moisture,INPUT);
  // 設定藍牙模組的連線速率
  // 如果是HC-05，請改成38400
  BT.begin(9600);
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(In3, OUTPUT);
  pinMode(In4, OUTPUT);  
  dht.begin();
}

void loop() {

  //analogWrite(In3,fancontrolvalue);
  //analogWrite(In4,0); 
  frun();
  
  light = analogRead(p_resistor);
  soil = analogRead(soil_moisture);
  h = dht.readHumidity();
  t = dht.readTemperature();
  soil = soil/10;
  if (BT.available()&&wateruse!=true) {
    val = BT.read();
 //   Serial.println(val);
    if(val=='l'){
      digitalWrite(bluePin,HIGH);
      openlight=true;   
      count=1;
    }
    if(val=='s'){
      digitalWrite(bluePin,LOW);
      openlight=false;
    }
    if(val=='o'){
      water();
    }
  }
  else {
      wateruse=false;
      if(count>10||count==0){
          openlight = false;
          if(light>700){
            digitalWrite(bluePin,HIGH);
          } else{
              digitalWrite(bluePin,LOW); 
          }
          count=0;
      }
  
      if(soil<50){
        digitalWrite(waterPin,HIGH);
        Serial.println("water pump open! ");
        delay(1000);
        digitalWrite(waterPin,LOW);
        Serial.println("water pump close! ");
      } 
  }
  showLCD();
  if(openlight){
    count++;
  }
  delay(2000);
}

void showLCD(){
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("S:");
    lcd.print(soil);
    lcd.setCursor(8,0);  
    lcd.print("T:");
    lcd.print(t);  
    lcd.setCursor(0,1);
    lcd.print("M:");
    lcd.print(h);  
    lcd.setCursor(8,1);
    lcd.print("L:");
    lcd.print(light);
}

void water(){
  digitalWrite(waterPin,HIGH);
  Serial.println("water pump open! ");
  delay(1000);
  digitalWrite(waterPin,LOW);
  Serial.println("water pump close! ");  
  
}

void frun(){
  digitalWrite(In1, LOW);
  digitalWrite(In2, HIGH);
  digitalWrite(In3, LOW);
  digitalWrite(In4, HIGH);
}

void fstop(){
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
  digitalWrite(In3, LOW);
  digitalWrite(In4, LOW);
}
