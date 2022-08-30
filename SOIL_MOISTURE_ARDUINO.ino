//Including library files
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//declare variables 
char auth[] = "rEr5mvZukpb6ulqf_nonmlXoU09bjkp-";
char ssid[] = "DaRpan";
char pass[] = "9821239439";
BlynkTimer timer;
bool pirbutton = 0;

//declaring pins to sensors 
int sensor_pin = A0;            // moisture sensor pin
int output_value ;               
int relayPin = 13;              // pin for relay signal
int buzzPin = 12;               // pin for buzzer
int pirPin = 5;                 // input pin for PIR sensor
int pirDetect = 0;              // variable for reading the pin status
int calibrationTime = 30;

//Get buttons values
BLYNK_WRITE(V3) {
  pirbutton = param.asInt();
}


void setup() {
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  pinMode(buzzPin, OUTPUT);     // declare buzzPin as output
  pinMode(pirPin, INPUT);       // declare pirPin as output
  pinMode(LED_BUILTIN, OUTPUT); // declare LED as output
  pinMode(relayPin, OUTPUT);    // declare relayPin as output
  digitalWrite(buzzPin, LOW);
  digitalWrite(relayPin, HIGH);
  digitalWrite(pirPin, LOW); // provide time to calibrate sensor 

  //Calibrating Sensor
  Serial.print("Calibrating Sensor "); 
  for(int i = 0; i < calibrationTime; i++)
  { Serial.print("."); 
  delay(1000); } 
  Serial.println("Done Calibrating"); 
  Serial.println("SENSOR ACTIVE"); 
  delay(50);
  
//Call the functions
  timer.setInterval(100L, Soil_Moisture);
  timer.setInterval(100L, pirsensor);
  delay(5000);
  }

void Soil_Moisture(){
  output_value= analogRead(sensor_pin);
  output_value = map(output_value,1024,460,0,100);        //mapping values for moisture sensor
  Serial.print("Moisture : ");
  Serial.print(output_value);                             // print moisture value
  Serial.println("%");
  Blynk.virtualWrite(V0, output_value);
  if (output_value<=85)
  {
    digitalWrite(relayPin, LOW);                          // turn on relay circuit
    Serial.println("Motor is ON");
    Blynk.virtualWrite(V2, 1);
  }
  else
  {
    digitalWrite(relayPin, HIGH);                         // turn off relay ciruit
    Blynk.virtualWrite(V2, 0);
  }
  }


//Get the PIR sensor values
void pirsensor() {
  bool value = digitalRead(pirPin);
  if (pirbutton == 1) {
    if (value == 0) {
      digitalWrite(buzzPin, LOW);
      Serial.println("Motion ended!");  // print end of motion
      Blynk.virtualWrite(V1,0);
    } else if (value == 1) {
      Serial.println("Motion detected!");
      Blynk.notify("Warning! Intruder Detected.");
      digitalWrite(buzzPin, HIGH);
      Blynk.virtualWrite(V1,1);
    }
  }
}



  void loop()
  {
    Blynk.run(); //Running the Blynk library
    timer.run(); //Running the Blynk timer
  }
