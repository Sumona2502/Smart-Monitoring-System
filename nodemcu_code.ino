#include <ESP8266WiFi.h>
#include <ThingSpeak.h>


const int trigPin1 = D1;
const int echoPin1 = D2;
#define redled D3
#define grnled D4
#define BUZZER D5 //buzzer pin
int redVal, greenVal;
int field;
unsigned int field1 = 1;
unsigned int field2 = 2;

const char *ssid = "XXX";     //dummy id
const char *password = "****";  //dummy password
const long myChannelNumber = 2334686;  //dummy channel id
const char *myWriteAPIKey = "TPEUBXU5MG1MEYGW";  //dummy  api key

WiFiClient client;

void setup() {
   pinMode(trigPin1, OUTPUT);
  pinMode(echoPin1, INPUT);
  pinMode(redled, OUTPUT);
  pinMode(grnled, OUTPUT);
  digitalWrite(redled, LOW);
  digitalWrite(grnled, LOW);
  Serial.begin(115200);
  connectToWiFi();
  ThingSpeak.begin(client);
}
void connectToWiFi() {
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
}



void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    Serial.println("Received data: " + data);
    int val = ThingSpeak.readFloatField(myChannelNumber, field);
     int Received_data = ThingSpeak.getLastReadStatus();

    // Assuming the data received is in the format "humidity= [value] temperature= [value]"
    float humidity = data.substring(data.indexOf("humidity= ") + 10, data.indexOf(" temperature=")).toFloat();
    float temperature = data.substring(data.indexOf("temperature= ") + 13).toFloat();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("% Temperature: ");
    Serial.print(temperature);
    Serial.println("C");
    if(Received_data == 200 && field==field1){
    redVal= val;
    //Serial.println("Red Status: " + String(val));
    if(val>0){
      digitalWrite(D3, HIGH);
      tone(BUZZER, 300);
      delay(1500);
    }else{
      digitalWrite(D3, LOW);
      delay(1500);
    noTone(BUZZER);
    delay(1000);
    }
  }
  else if(Received_data == 200 && field==field2){
    greenVal= val;
    //Serial.println("Green Status: " + String(val));
    if(val>0){
      digitalWrite(D4, HIGH);
    }else{
      digitalWrite(D4, LOW);
    }
  }

    // Send data to ThingSpeak
    if (ThingSpeak.writeField(myChannelNumber, 1, humidity, myWriteAPIKey) && 
        ThingSpeak.writeField(myChannelNumber, 2, temperature, myWriteAPIKey)) {
      Serial.println("Data sent to ThingSpeak");
    } else {
      Serial.println("Error sending data to ThingSpeak");
    }
  }
    if (Serial.available() > 0) { 
    String data = Serial.readStringUntil('\n'); 
 
    // Parse the received data 
    int commaIndex1 = data.indexOf(','); 
    int commaIndex2 = data.indexOf(',', commaIndex1 + 1); 
    int commaIndex3 = data.indexOf(',', commaIndex2 + 1); 
    int commaIndex4 = data.indexOf(',', commaIndex3 + 1); 
    int commaIndex5 = data.indexOf(',', commaIndex4 + 1); 
 
    String moisture_percentage = data.substring(0, commaIndex1);  //soil moisture sensor data
    int soil_data = ThingSpeak.getLastReadStatus();
    if(soil_data == LOW)
    {
      digitalWrite(D3, HIGH);
      tone(BUZZER, 300);
     digitalWrite(D4, LOW);
     noTone(BUZZER);
     delay(1500);
    
    }
    else{
      digitalWrite(D3, LOW);
     digitalWrite(D4, HIGH);
     delay(1500);
    }



    String ldr = data.substring(commaIndex1 + 1, commaIndex2);  // ldr sensor data
    String SensorData  = data.substring(commaIndex2 + 1, commaIndex3); //MQ7 sensor data
    String SensorOtp  = data.substring(commaIndex3 + 1, commaIndex4); //PIR sensor data
    int warm_up;
    if(SensorOtp == LOW){
    if(warm_up == 1)
    {
      //Serial.print("Warming Up\n");
      warm_up = 0;
      digitalWrite(D3, LOW);
     digitalWrite(D4, HIGH);
     delay(1500);
    
    }
    //Serial.print("No Motion Detected\n");
    //delay(1000);
  
  else{
    Serial.print("Motion Detected\n");
    warm_up = 1;
    digitalWrite(D3, HIGH);
    tone(BUZZER, 300);
    digitalWrite(D4, LOW);
    delay(1500);
    noTone(BUZZER);
    delay(1000);
  }
    }

    String flame_sensor  = data.substring(commaIndex4 + 1, commaIndex5);  // Flame sensor data
    int flame;
    if(digitalRead (flame) == 1)
  {
    digitalWrite(D3, HIGH);
    tone(BUZZER, 300);
    digitalWrite(D4, LOW);
    delay(1500);
    noTone(BUZZER);
    delay(1000);
    //flame_sensor = map(flame_sensor,0,1023,0,100);
  }
  else{
    digitalWrite(D3, LOW);
     digitalWrite(D4, HIGH);
     delay(1500);
    
  }

       // int moisture_percentage = moisture_percentageStr.toInt(); 
        //int SensorData = SensorDataStr.toInt(); 

        ThingSpeak.writeField(myChannelNumber, 3, moisture_percentage, myWriteAPIKey); 
    ThingSpeak.writeField(myChannelNumber, 4, SensorData, myWriteAPIKey); 
    ThingSpeak.writeField(myChannelNumber, 5, ldr, myWriteAPIKey); 
    ThingSpeak.writeField(myChannelNumber, 4, SensorOtp, myWriteAPIKey); 
    ThingSpeak.writeField(myChannelNumber, 5, flame_sensor, myWriteAPIKey); 

    }
}
