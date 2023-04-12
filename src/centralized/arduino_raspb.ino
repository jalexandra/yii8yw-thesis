#include <SoftwareSerial.h>

#define STOP 0
#define LEFT 1
#define RIGHT 2

#define GREEN_LED 8
#define RED_LED 7
#define MOTOR_E 4
#define MOTOR_1 5
#define MOTOR_2 6
#define RX 2
#define TX 3

#define WAIT_LED 2000
#define MOTOR_MOVE 2000
#define OPEN_TIME 5000

#define DEVICE_ID "51"

SoftwareSerial RFID(RX, TX);
String text;
String c;

void setup()
{
  Serial.begin(9600);
  RFID.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(RED_LED, OUTPUT);
  pinMode(MOTOR_E, OUTPUT);
  pinMode(MOTOR_1, OUTPUT);
  pinMode(MOTOR_2, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  digitalWrite(GREEN_LED, LOW);
  digitalWrite(RED_LED, LOW);
  motor(STOP);
}

void loop()
{
  while (RFID.available() > 0) {
    delay(5);
    c = RFID.read();
    text += c;
  }
  text = text.substring(1, 11);
  String status = send(text);

  if(status == "1"){
   digitalWrite(GREEN_LED, HIGH);
    motor(RIGHT);
    delay(MOTOR_MOVE);
    motor(STOP);
    delay(OPEN_TIME);
    motor(LEFT);
    delay(MOTOR_MOVE);
    motor(STOP);
    digitalWrite(GREEN_LED, LOW);
  } else if(status != "") {
    digitalWrite(RED_LED, HIGH);
    delay(WAIT_LED);
    digitalWrite(RED_LED, LOW);
  }

  text = "";
  delay(300);
}

String send(String rfid){
  if(!rfid.length()){
    return "";
  }
  String body = String("{\"deviceId\": \"") 
    + DEVICE_ID 
    + "\", \"rfid\": \"" 
    + text
    + "\"}";
  Serial.println(body);
  return wait_for_response();
}

String wait_for_response(){
  RFID.end();
  while(Serial.available()) {
    Serial.read();
  }
  while(!Serial.available()){
    delay(50);
  }

  String input = Serial.readString();
  input.trim();

  RFID.begin(9600);

  return input;
}

void motor(int direction){
  switch(direction){
    case 0:
      digitalWrite(MOTOR_E, LOW);
      digitalWrite(MOTOR_1, LOW);
      digitalWrite(MOTOR_2, LOW);
      break;
    case 1:
      digitalWrite(MOTOR_E, HIGH);
      digitalWrite(MOTOR_1, HIGH);
      digitalWrite(MOTOR_2, LOW);
      break;
    case 2:
      digitalWrite(MOTOR_E, HIGH);
      digitalWrite(MOTOR_1, LOW);
      digitalWrite(MOTOR_2, HIGH);
      break;
  }
}
