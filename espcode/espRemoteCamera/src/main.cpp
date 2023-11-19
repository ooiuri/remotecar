#include <Arduino.h>

// Include file with required WIFI Credentials
#include "config.h"

// Import required libraries
// Import required libraries
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>

#define LED 2

// declaration of motors pins
#define PWMA 16
#define IN1 17
#define IN2 5
#define IN3 19
#define IN4 18
#define PWMB 21

const int freq = 30000;
const int pwmChannelA = 0;
const int pwmChannelB = 1;
// Setting the resolution to 8Bits, gives you a duty cycle range [0 – 255]. While setting it to 10Bits, gives you a range of [ 0 – 1023 ]. And so on!
const int resolution = 8;
int dutyCycle = 200;

unsigned long previousMillis = 0;

const char *PARAM_INPUT_1 = "vela";
const char *PARAM_INPUT_2 = "velb";

unsigned int timerMove = 0;
int motorVelA = 0, motorVelB = 0; 

void moveMotors(int velA, int velB)
{
  if (velA == 0 && velB == 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, HIGH);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, HIGH);
    ledcWrite(pwmChannelB, velB);
  }
  if (velA > 0 && velB > 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(pwmChannelB, velB);
  }
  else if (velA >= 0 && velB < 0)
  {
    digitalWrite(IN1, HIGH);
    digitalWrite(IN2, LOW);
    ledcWrite(pwmChannelA, velA);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(pwmChannelB, -velB);
  }
  else if (velA < 0 && velB >= 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(pwmChannelA, -velA);
    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    ledcWrite(pwmChannelB, velB);
  }
  else if (velA < 0 && velB < 0)
  {
    digitalWrite(IN1, LOW);
    digitalWrite(IN2, HIGH);
    ledcWrite(pwmChannelA, -velA);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    ledcWrite(pwmChannelB, -velB);
  }
}

void requestMove(int velA, int velB)
{
  motorVelA = velA;
  motorVelB = velB;
  timerMove = millis();
}

char *ssid = WIFI_SSID;
char *password = WIFI_PASSWORD;

AsyncWebServer server(80);

const int led_WIFI = 16;

// Set your Static IP address
IPAddress local_IP(192, 168, 100, 101);
// Set your Gateway IP address
IPAddress gateway(192, 168, 100, 1);

IPAddress subnet(255, 255, 0, 0);
IPAddress primaryDNS(8, 8, 8, 8);   // optional
IPAddress secondaryDNS(8, 8, 4, 4); // optional

void setup()
{

  Serial.begin(115200);
  delay(1000);
  // Configures static IP address
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS))
  {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println(WiFi.localIP());

  server.on("/move", HTTP_ANY, [](AsyncWebServerRequest *request)
            {
    String motorVelA;
    String motorVelB;

    if (request->hasParam(PARAM_INPUT_1) && request->hasParam(PARAM_INPUT_2)) {
      motorVelA = request->getParam(PARAM_INPUT_1)->value();
      motorVelB = request->getParam(PARAM_INPUT_2)->value();

      requestMove(motorVelA.toInt(), motorVelB.toInt());
    }
    else {
      motorVelA = "No message sent";
      motorVelB = "No message sent";
    }
    Serial.print("motorVelA: ");
    Serial.print(motorVelA.toInt());
    Serial.print(" motorVelB: ");
    Serial.println(motorVelB.toInt());
    request->send(200, "text/plain", "OK"); });

  server.onNotFound([](AsyncWebServerRequest *request)
                    {
    if (request->method() == HTTP_OPTIONS) {
      request->send(200);
    } else {
      request->send(404);
    } });

  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Origin", "*");
  DefaultHeaders::Instance().addHeader("Access-Control-Max-Age", "10000");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Methods", "PUT,POST,GET,OPTIONS,PATCH");
  DefaultHeaders::Instance().addHeader("Access-Control-Allow-Headers", "*");
  server.begin();

  pinMode(PWMA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(PWMB, OUTPUT);

  // PWM Setup
  ledcSetup(pwmChannelA, freq, resolution);
  ledcSetup(pwmChannelB, freq, resolution);
  ledcAttachPin(PWMA, pwmChannelA);
  ledcAttachPin(PWMB, pwmChannelB);

  Serial.println("Pronto para a Ação!");

  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);

}

void loop()
{
  if (millis() - timerMove < 80) {
    moveMotors(motorVelA, motorVelB);
  }
  else {
    moveMotors(0,0);
  }
}
