#include <WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_BMP085.h>

Adafruit_BMP085 bmp;

const char* ssid = "SUPERONLINE_WiFi_3477";      //wifi adı
const char* password = "RS200.krms";      //wifi şifre
const char* mqtt_server = "54.224.57.65";     //

int echo_pin = 12;
int trig_pin = 14;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void setup_bmp180(){
  if (!bmp.begin()) {
  
  }
  }

void setup_hcsr04(){
  pinMode(trig_pin, OUTPUT);
  pinMode(echo_pin, INPUT);
  }

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  setup_wifi();
  setup_bmp180();
  setup_hcsr04();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
}

void loop() {
  String press_str = String(bmp.readPressure());
  float distance = GetDistance();
  String distance_str = String(distance);
  if (!client.connected()) {
    reconnect();
  }
  client.subscribe("subscribe edilcek topic");
  client.loop();

  long now = millis();
  if (now - lastMsg > 200) {//200ms de bir mesaj atıcak
    lastMsg = now;
    //ısı nem uzaklık
    client.publish("group5/press", press_str.c_str() );
    client.publish("group5/distance", distance_str.c_str() );
  }

}


float GetDistance() {
  digitalWrite(trig_pin, LOW);
  delayMicroseconds(2);

  digitalWrite(trig_pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_pin, LOW);

  long duration = pulseIn(echo_pin, HIGH);

  float distance = duration * 0.034 / 2;
  return distance;
}


void callback(char* topic, byte* message, unsigned int length) {
  Serial.print(topic);

  String messageTemp;

  for (int i = 0; i < length; i++) {
    Serial.print((char)message[i]);
    messageTemp += (char)message[i];
  }
  Serial.println();

  if (String(topic) == "subscribe edilen topic") {

  }else if(String(topic) == "subscribe edilen topic") {

  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("hcbmp")) {
      Serial.println("connected");
      // Subscribe

    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
