#include<DHT.h>
#include <SoftwareSerial.h>
#define DHTPIN 5
#define DHTTYPE DHT11 // DHT 11
SoftwareSerial espSerial(8, 9); //Nodemcu Esp8266 interfacing pin with Arduino
DHT dht(DHTPIN, DHTTYPE);
String str;

int sensor = A0;	/* Soil moisture sensor O/P pin */
int ldr = 2;      //ldr sensor
int LED = 0;      //LED
int sensor_MQ = A1; // MQ7 sensor
int flame = 3; //Flame Sensor
int PIR = 4; //PIR sensor

void setup() {
  espSerial.begin(115200);	/* Define baud rate for serial communication */
  pinMode(sensor,INPUT);
  pinMode(ldr,INPUT);    //Ldr input pin define
  pinMode(LED, OUTPUT);
  delay(100);
  pinMode(sensor_MQ,INPUT);  //MQ7 input pin define
  pinMode(flame,INPUT);      //Flame sensor input pin define
  pinMode(LED_BUILTIN,OUTPUT);
  pinMode(PIR,INPUT);       //PIR sensor input pin
  delay(2000);
  dht.begin();
  delay(2000);
}

void loop() {
  float moisture_percentage;
  int sensor_analog;
  sensor_analog = analogRead(sensor);
  moisture_percentage = map(moisture_percentage, 1023, 400, 0, 100); //( 100 - ( (sensor_analog/1023.00) * 100 ) );
  Serial.print("Moisture Percentage = %\n");
  espSerial.println(moisture_percentage);
  //Serial.println("%\n\n");
  delay(1000);
  int ldr = digitalRead (ldr);
  if(ldr == 1){
    digitalWrite(LED,HIGH);
    ldr = map(ldr,0,1023,0,100);
  }
  else{
    digitalWrite(LED,LOW);
  }
  //ldr = map(ldr,0,1023,0,100);
  espSerial.println(ldr);
  delay(200);
  int SensorData;
  SensorData = analogRead(sensor_MQ);
  SensorData = map(SensorData, 400, 1023, 0, 100); 
  espSerial.println(SensorData);
  delay(100);
  int flame_sensor;
  flame_sensor = digitalRead(flame);
  //flame_sensor = map(flame_sensor,0,1023,0,100);
  if(digitalRead (flame) == 1)
  {
    digitalWrite(LED_BUILTIN,HIGH);
    Serial.println("***WARNING!!! FIRE DETECTED!!!***");
    flame_sensor = map(flame_sensor,30,1023,0,100);
  }
  else{
    digitalWrite(LED_BUILTIN,LOW);
    Serial.println("No Fire Detected");
  }
  espSerial.println(flame_sensor);
  delay(100);
  int warm_up;
  int SensorOtp;
  SensorOtp = digitalRead(PIR);
  //SensorOtp = map(SensorOtp,200,1023,0,100);
  if(SensorOtp == LOW)
  {
    if(warm_up == 1)
    {
      Serial.print("Warming Up\n");
      warm_up = 0;
      delay(2000);
    }
    Serial.print("No Motion Detected\n");
    delay(1000);
  }
  else{
    Serial.print("Motion Detected\n");
    warm_up = 1;
    SensorOtp = map(SensorOtp,200,1023,0,100);
    delay(1000);
  }
  espSerial.println(SensorOtp);
  float humidity = dht.readHumidity();
  float temperature = dht.readTemperature();
  Serial.print("Humidity: ");
  Serial.print(humidity); 
  Serial.print("% ");
  Serial.print(" Temperature: ");
  Serial.print(temperature); 
  Serial.println("C");
  str =String("coming from arduino: ")+String("humidity= ")+String(humidity)+String("temperature= ")+String(temperature);
  espSerial.println(str);
  delay(1000);
}


