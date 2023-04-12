#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <SoftwareSerial.h>

#define WIFI_SSID "TP-LINK_92A6"
#define WIFI_PASS "secret123"

#define STOP 0
#define LEFT 1
#define RIGHT 2

#define GREEN_LED D0
#define RED_LED D1
#define MOTOR_E D2
#define MOTOR_1 D3
#define MOTOR_2 D4
#define RX D5
#define TX D6

#define WAIT_LED 2000
#define MOTOR_MOVE 2000
#define OPEN_TIME 5000

#define SERVER "http://192.168.0.101:8000/api/access/51"
/**
* Itt az 51 a zár ID-je. Ennek úgy kéne működnie, hogy bármilyen azonosítót elfogadni (pl. deviceID vagy MAC cím),
* de hibás a BE, szóval csak az adatbázisban tárolt primary key-t (ID-t) fogadja el.
* Ezt az ID-t meg tudjuk nézni az admin panel-en, és be kell égetni az eszközkódba egyelőre
*/

SoftwareSerial RFID(RX, TX);
String text;
String c;


void setup() {
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

  WiFi.disconnect();
  WiFi.begin(WIFI_SSID, WIFI_PASS);

  while(WiFi.status() != WL_CONNECTED){
    blink();
    delay(2000);
  }
  digitalWrite(LED_BUILTIN, HIGH);
}

void loop() {
  while (RFID.available() > 0) {
    delay(5);
    c = String(RFID.read());
    text += c;
  }

  text = text.substring(1, 11);
  int status = send(text);

  if(status == 202){
    digitalWrite(GREEN_LED, HIGH);
    motor(RIGHT);
    delay(MOTOR_MOVE);
    motor(STOP);
    delay(OPEN_TIME);
    motor(LEFT);
    delay(MOTOR_MOVE);
    motor(STOP);
    digitalWrite(GREEN_LED, LOW);
  }
  else if(status > 399){
    digitalWrite(RED_LED, HIGH);
    delay(WAIT_LED);
    digitalWrite(RED_LED, LOW);
  }

  text = "";
  delay(300);
}

void blink(){
  digitalWrite(LED_BUILTIN, HIGH);
  delay(WAIT_LED);
  digitalWrite(LED_BUILTIN, LOW);
}

int send(String rfid){
  if(!rfid.length() || WiFi.status() != WL_CONNECTED) {
    return -1;
  }
  
  HTTPClient http;
  WiFiClient client;
  String body = String("{\"worker_rfid\":\"")
    + rfid
    + "\"}";

  Serial.println(body); //debug

  http.begin(client, SERVER);
  http.addHeader("Content-Type", "application/json");
  int httpCode = http.PUT(body);

  http.end();

  Serial.println(httpCode); //debug
  return httpCode;
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