#include <Arduino.h>
#include <Servo.h>
//#include <time.h>


const int pinPump[] = {4,7,8};
const int pinServo = 9;
const int pinLed1[] = {3,5,6};
const int trigPin = A1;
const int echoPin = A0;


Servo myservo;  // create Servo object to control a servo
void pumpPWM(int i, int time);
void pump(int i, bool on);
void led1(int r, int g, int b);
void colorLed();
void trasiem();

int state = 0;

void led1(int r, int g, int b){
  analogWrite(pinLed1[0], r);
  analogWrite(pinLed1[1], g);
  analogWrite(pinLed1[2], b);
}

void pump(int i, bool on){
  digitalWrite(pinPump[i], on ? HIGH : LOW);
}


// void pumpPWM(int i, int time){
//   if(time < 100) {
//     digitalWrite(pinPump[i], LOW);
//   } else {
//     digitalWrite(pinPump[i], HIGH);
//     delay(time);
//     digitalWrite(pinPump[i], LOW);
//     delay(time);
//   }
// }

float readDistance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  unsigned long duration = pulseIn(echoPin, HIGH);
  float distance = (duration * 0.0343) / 2;
  return distance;
}



unsigned long previousMicros = 0;
bool distanceReadFlagA = false;
bool distanceReadFlagB = false;
float distanceRead(){
  if(!distanceReadFlagA){
    previousMicros = micros();
    digitalWrite(trigPin, LOW);
    if(micros() - previousMicros < 2){
      return -1;
    }
    distanceReadFlagA = true;
    previousMicros = 0;
  }
  digitalWrite(trigPin, HIGH);
  if(micros() - previousMicros < 10){
    return -1;
  }
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  float duration = pulseIn(echoPin, HIGH);
  return ((duration*.0343)/2);
}
void trasiem(){
  myservo.write(80);
  delay(200);
  myservo.write(120);
  delay(200);
}
void setup() {
  myservo.attach(pinServo);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(13, OUTPUT);
  pinMode(pinLed1[0], OUTPUT);
  pinMode(pinLed1[1], OUTPUT);
  pinMode(pinLed1[2], OUTPUT);
  for(int i = 0; i < 4; i++){
    pinMode(pinPump[i], OUTPUT);
  }
  pump(3, false);
  pump(2, false);
  pump(1, false);
  Serial.begin(9600);
  while(distanceRead() > 80){
    led1(255, 0, 0);
    delay(100);
    //Serial.println(distanceRead());
  }
  
  //led1(50,50,50);
}


static byte endColor[3];
static byte startColor[3];
static bool transitioning = false;
int transitionTime;
unsigned long transitionStart;

void transitionColor(byte r, byte g, byte b, int ms) {
  startColor[0] = endColor[0];
  startColor[1] = endColor[1];
  startColor[2] = endColor[2];
  endColor[0] = r;
  endColor[1] = g;
  endColor[2] = b;
  transitionStart = millis();
  transitionTime = ms;
  transitioning = true;


}

void colorRun() {
  if (transitioning) {
    float progress = (float)(millis() - transitionStart) / transitionTime;
    if (progress >= 1.0) {
      progress = 1.0;
      transitioning = false;
    }
    
    int r = startColor[0] + progress * (endColor[0] - startColor[0]);
    int g = startColor[1] + progress * (endColor[1] - startColor[1]);
    int b = startColor[2] + progress * (endColor[2] - startColor[2]);
    led1(r, g, b);
  }
}

long lastTransition = -5000;

void loop() {
  pump(2, true);
  pump(1, true);
  pump(3, true);
  // if (millis() - lastTransition >= 1500) {
  //   if (state == 0) {
  //     transitionColor(10, 10, 10, 2000);
  //   } else {
  //     transitionColor(250, 10, 10, 2000);
  //   }
    
  //   lastTransition = millis();
  //   state = !state;
  //   Serial.println("Transitioning");
  // }
  // colorRun();
  //Serial.println(distanceRead());
  if(distanceRead() < 50 && distanceRead() > 30){
    led1(0, 255, 0);
    delay(100);
    led1(255, 255, 0);
    delay(100);
  }
  if(distanceRead() < 30 ){
    led1(0, 0, 255);
    delay(100);
    led1(0, 255, 0);
    delay(100);
    led1(255, 0, 0);
    delay(100);
    
    trasiem();
  }

  
  
  
  // pump(3, true);
  // delay(5000);
  // pump(1, true);
  // delay(5000);
  // pump(3, false);S
  
  // pump(1, false);
  // delay(5000);
  

}

