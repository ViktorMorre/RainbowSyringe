#include <Arduino.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
 
#define S0 5
#define S1 21
#define S2 22
#define S3 4
#define EO 14
#define sensorOut 2

#define inPin 7

int frequency = 0;
int kleur=6;

int val=0;

//Omzetten gemeten RGB-waarden naar een kleur.
int colorChecker(int redFrequency, int greenFrequency, int blueFrequency){
  if(redFrequency>90 && greenFrequency<90 && blueFrequency>80){
    Serial.println("Groen");
    return kleur=0;}

  if(redFrequency<90 && greenFrequency>250 && blueFrequency>250){
    Serial.println("Rood");
    return kleur=1;}
  
  if(redFrequency<40 && greenFrequency>90 && blueFrequency>80){
    Serial.println("Blauw");
    return kleur=2;}

  if(redFrequency<40 && greenFrequency>90 && blueFrequency>80){
    Serial.println("Geel");
    return kleur=3;}

  if(redFrequency<40 && greenFrequency>90 && blueFrequency>80){
    Serial.println("Paars");
    return kleur=4;}

  if(redFrequency<40 && greenFrequency>90 && blueFrequency>80){
    Serial.println("Cyaan");
    return kleur=5;}
  else
    Serial.println("Geen kleur");
    return kleur=6;
}

int measure(){
  digitalWrite(EO,LOW);
  // Setting red filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,LOW);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  int red = map(frequency, 25,72,255,0);
  // Printing the value on the serial monitor
  Serial.print("R= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

  // Setting Green filtered photodiodes to be read
  digitalWrite(S2,HIGH);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  int green = map(frequency, 30,90,255,0);
  // Printing the value on the serial monitor
  Serial.print("G= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.print("  ");
  delay(100);

  // Setting Blue filtered photodiodes to be read
  digitalWrite(S2,LOW);
  digitalWrite(S3,HIGH);
  // Reading the output frequency
  frequency = pulseIn(sensorOut, LOW);
  //Remaping the value of the frequency to the RGB Model of 0 to 255
  int blue = map(frequency, 25,70,255,0);
  // Printing the value on the serial monitor
  Serial.print("B= ");//printing name
  Serial.print(frequency);//printing RED color frequency
  Serial.println("  ");
  delay(100);
  digitalWrite(EO,HIGH);
  return colorChecker(red, green, blue);
}

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void radioSetup() {
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}

void radioSend(int color) {
  radio.write(&color, sizeof(color));
  delay(1000);
}

void setup() {
  pinMode(S0, OUTPUT);
  pinMode(S1, OUTPUT);
  pinMode(S2, OUTPUT);
  pinMode(S3, OUTPUT);
  pinMode(sensorOut, INPUT);

  pinMode(inPin, INPUT);
  
  // Setting frequency-scaling to 20%
  digitalWrite(S0,HIGH);
  digitalWrite(S1,LOW);
  
  Serial.begin(9600);

  radioSetup();
}

void loop() {
  val = digitalRead(inPin);  // read input value
  if (val == HIGH) { 
    int measColor = measure();
    radioSend(measColor);
    }
}





