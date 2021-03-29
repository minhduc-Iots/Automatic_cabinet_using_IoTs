#include <ESP8266WiFi.h>
#include <PubSubClient.h>
//#include <String.h>
// Update these with values suitable for your network.
#define ssid "Vietkey"
#define password "12345678"
#define mqtt_server "192.168.43.34"
//#define mqtt_topic_pub "door_state"
//#define mqtt_topic_sub "door_command"
#define mqtt_user "Client1"
#define mqtt_password "123456"
const uint16_t mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;


void setup() {
  Serial.begin(115200);
  pinMode(16, OUTPUT);
  digitalWrite(16, LOW);
  setup_wifi();
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

}

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
  client.publish("MatKhau_state", "MAT_KHAU_ON");
  client.publish("RFID_state", "RF_ID_ON");
}

void callback(char* topic, byte* payload, unsigned int length) {
 // Serial.print("Message arrived [");
 // Serial.print(topic);
 // Serial.print("] ");
  String s = "";

  for ( int i = 0; i < length; i++)
  {
    s += (char)payload[i];
  }
  //char p=(char)payload[0];
  //Serial.print(s);


  if (s == "RF_ID_OFF")
  {
    // gửi dữ liệu xuống arduino bắt đầu lấy thẻ RFID

    Serial.write('3');
  }
  else if (s == "RF_ID_ON")
  {
    Serial.write('4');
  }
  if (s == "MAT_KHAU_OFF")
  {
    // gửi dữ liệu xuống arduino bắt đầu cài mật khẩu
    Serial.write('5');
  }
  else if (s == "MAT_KHAU_ON")
  {
    Serial.write('6');
  }
  // else Serial.println(" Khong hop le");
  Serial.println();
}

void Uart()
{
  if (Serial.available())
  {
     char data = Serial.read();
    if (data == '1')
    {
      client.publish("door_state", "DOOR_ON");
      Serial.println("mo");
    }
    else if (data == '0')
    {
      client.publish("door_state", "DOOR_OFF");
      Serial.println("dong");
    }
  }
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client", mqtt_user, mqtt_password)) {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("door_state", "ESP_reconnected");
      client.publish("MatKhau_state", "ESP_reconnected");
      client.publish("RFID_command", "ESP_reconnected");
     

      // ... and resubscribe
      client.subscribe("door_command");
      client.subscribe("MatKhau_command");
      client.subscribe("RFID_command");
   
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}
void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
  //NhanNut();
  Uart();

}
