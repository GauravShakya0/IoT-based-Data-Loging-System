#include <SoftwareSerial.h>
#define RX 2
#define TX 3
const int trig = 6;
const int echo = 5;
long T, disM;
float temp;
String AP = "Galaxy";       // AP NAME
String PASS = "idontknow"; // AP PASSWORD
String API = "E1FMSLIG6236LM2D";   // Write API KEY
String HOST = "api.thingspeak.com";
String PORT = "80";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;

SoftwareSerial esp8266(RX, TX);

void setup() {
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  Serial.begin(115200);
  esp8266.begin(115200);
  sendCommand("AT", 5, "OK");
  sendCommand("AT+CWMODE=1", 5, "OK");
  sendCommand("AT+CWJAP=\"" + AP + "\",\"" + PASS + "\"", 20, "OK");
}

void loop() {
  digitalWrite(trig, 0);
  delay(1);
  digitalWrite(trig, 1);
  delayMicroseconds(10);
  digitalWrite(trig, 0);
  T = pulseIn(echo, 1);

  String getData = "GET /update?api_key=" + API + "&field1=" + getTemperatureValue() + "&field2=" + getUSValue();
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CIPSTART=0,\"TCP\",\"" + HOST + "\"," + PORT, 15, "OK");
  sendCommand("AT+CIPSEND=0," + String(getData.length() + 4), 4, ">");
  esp8266.println(getData); delay(1500); countTrueCommand++;
  sendCommand("AT+CIPCLOSE=0", 5, "OK");
  Serial.print("Temp=");
  Serial.print(temp);
  Serial.println();
   Serial.print("Distance=");
  Serial.print(disM);
  Serial.println();
}


String getTemperatureValue()
{

  int rea;

  rea = analogRead(A0);
  float voltage = rea * (5.0 / 1024.0);
   temp = voltage * 100;

  return String(temp);

}
String getUSValue() {


    disM = T * 0.034 / 2;
               // disM = disM / 2;
               return String(disM);
}




void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while (countTimeCommand < (maxTime * 1))
  {
    esp8266.println(command);//at+cipsend
    if (esp8266.find(readReplay)) //ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if (found == true)
  {
    Serial.println("OK");

    countTrueCommand++;
    countTimeCommand = 0;
  }

  if (found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
}

