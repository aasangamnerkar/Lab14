SYSTEM_THREAD(ENABLED);
#include "MQTT.h"
#include "oled-wing-adafruit.h"
#include "blynk.h"

String auth_token = "cnjXDn_t7vcs3I-PR-jbSobGd__j2qM9";
String topic = "slatt";
bool valid = false;

void callback(char* topic, byte* payload, unsigned int length) {
    Serial.print("Received message on testTopic/wbk: \n");
    for (int i = 0; i < length; i++) {
      Serial.print((char)payload[i]);
    }
    char p[length + 1];
    memcpy(p, payload, length);
    p[length] = NULL;
    if (!strcmp(p, "1")) {
      RGB.control(true);
      RGB.color(255, 255, 255);
      RGB.brightness(255);
      digitalWrite(D7, HIGH);
    }
    if (!strcmp(p, "0")) {
      RGB.control(true);
      RGB.color(0, 0, 0);
      RGB.brightness(255);
      digitalWrite(D7, LOW);
    }
    Serial.println();
}

MQTT client("lab.thewcl.com", 1883, callback);
OledWingAdafruit display;



void printToDisplay(String output)
{
  //reset display
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);

  //output to display
  display.println(output);
  display.display();
}

BLYNK_WRITE(V0)
{
  String blynkString = param.asStr();
  client.publish(topic, blynkString);
  Serial.println(blynkString);
  printToDisplay(blynkString);
}

void setup()
{
  pinMode(D7, OUTPUT);
  
  display.setup();
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0,0);
  display.println("");
  display.display();
  Serial.begin(9600);

  
  client.connect(System.deviceID());
  if (client.isConnected())
  {
    client.subscribe(topic);
    client.publish(topic,"hello world");
  }
  Blynk.begin(auth_token, IPAddress(167, 172, 234, 162), 9090);
  Blynk.run();
  RGB.control(true);
  RGB.control(true);
  RGB.color(0, 0, 0);
  RGB.brightness(255);

  //valid = false;
}

// loop() runs over and over again, as quickly as it can execute.
void loop()
{
  display.loop();
  client.connect(System.deviceID());

  if (client.isConnected())
  {
    //valid = true;
    
  
    client.loop();
  }
  else
  {
    client.connect(System.deviceID());
    client.subscribe(topic);
    //valid = false;
  }

  if (client.isConnected())
  {
    if (!valid)
    {
      client.subscribe(topic);
    client.publish(topic,"slatt");
      valid = true;
    }
    if(display.pressedA()){
        client.publish(topic, "Button A was pressed");
    }
    if(display.pressedB()){
        client.publish(topic, "Button B was pressed");
    }
    if(display.pressedC()){
        client.publish(topic, "Button C was pressed");
    }
  }

 delay(1000);
}