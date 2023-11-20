#include <WiFi.h>
#include <PubSubClient.h>
#include <LiquidCrystal_I2C.h>
#include <DHTesp.h>

LiquidCrystal_I2C LCD = LiquidCrystal_I2C(0x27, 16, 2);


const int DHT_PIN = 15;
const int trigPin = 5;
const int echoPin = 18;
int potPin = 2;
int valor;
long duration;

// WIFI
const char* SSID = "Wokwi-GUEST"; // SSID / nome da rede WI-FI que deseja se conectar
const char* PASSWORD = ""; // Senha da rede WI-FI que deseja se conectar

// MQTT
const char* BROKER_MQTT = "46.17.108.113"; //URL do broker MQTT que se deseja utilizar
int BROKER_PORT = 1883; // Porta do Broker MQTT
//definindo id mqtt
#define ID_MQTT  "hosp_201"

//Tópicos
#define TOPICO_SUBSCRIBE "/TEF/hosp201/cmd"
#define TOPICO_PUBLISH   "/TEF/hosp201/attrs/d"
#define TOPICO_PUBLISH_2   "/TEF/hosp201/attrs/t"
#define TOPICO_PUBLISH_3   "/TEF/hosp201/attrs/h"

DHTesp dhtSensor;
WiFiClient espClient; // Cria o objeto espClient
PubSubClient MQTT(espClient); // Instancia o Cliente MQTT passando o objeto espClient

#define SOUND_SPEED 0.034

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  Serial.println("Hello, ESP32!");
  dhtSensor.setup(DHT_PIN, DHTesp::DHT22);
  LCD.init();
  pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
  pinMode(echoPin, INPUT); // Sets the echoPin as an Input
  initWiFi();
  initMQTT();
}

void initWiFi() 
{
    delay(10);
    Serial.println("------Conexao WI-FI------");
    Serial.print("Conectando-se na rede: ");
    Serial.println(SSID);
    Serial.println("Aguarde");
     
    reconectWiFi();
}

void reconectWiFi() 
{
    //se já está conectado a rede WI-FI, nada é feito. 
    //Caso contrário, são efetuadas tentativas de conexão
    if (WiFi.status() == WL_CONNECTED)
        return;
         
    WiFi.begin(SSID, PASSWORD, 6); // Conecta na rede WI-FI
     
    while (WiFi.status() != WL_CONNECTED) 
    {
        delay(100);
        Serial.print(".");
    }
   
    Serial.println();
    Serial.print("Conectado com sucesso na rede ");
    Serial.print(SSID);
    Serial.println("IP obtido: ");
    Serial.println(WiFi.localIP());
}

void initMQTT() 
{
    MQTT.setServer(BROKER_MQTT, BROKER_PORT);   //informa qual broker e porta deve ser conectado
    MQTT.setCallback(mqtt_callback);            //atribui função de callback (função chamada quando qualquer informação de um dos tópicos subescritos chega)
}

void reconnectMQTT() 
{
    while (!MQTT.connected()) 
    {
        Serial.print("* Tentando se conectar ao Broker MQTT: ");
        Serial.println(BROKER_MQTT);
        if (MQTT.connect(ID_MQTT)) 
        {
            Serial.println("Conectado com sucesso ao broker MQTT!");
            //MQTT.subscribe(TOPICO_SUBSCRIBE); 
        } 
        else
        {
            Serial.println("Falha ao reconectar no broker.");
            Serial.println("Havera nova tentatica de conexao em 2s");
            delay(2000);
        }
    }
}

void mqtt_callback(char* topic, byte* payload, unsigned int length) 
{
  //juro que tentei fazer esse negócio funcionar mas n foi
    String msg;
     
    //obtem a string do payload recebido
    for(int i = 0; i < length; i++) 
    {
       char c = (char)payload[i];
       msg += c;
    }
    
    Serial.print("- Mensagem recebida: ");
    Serial.println(msg);
}

void VerificaConexoesWiFIEMQTT(void)
{
    if (!MQTT.connected()) 
        reconnectMQTT(); //se não há conexão com o Broker, a conexão é refeita
     
     reconectWiFi(); //se não há conexão com o WiFI, a conexão é refeita
}

void verificarDist(){
  //distancia
  // Clears the trigPin
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  float distanceCm = duration * SOUND_SPEED/2;
  char msgBuffer[4];
  // Prints the distance in the Serial Monitor
  Serial.print("Distance (cm): ");
  Serial.println(distanceCm);
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Distancia:");
  LCD.setCursor(0,1);
  LCD.print(distanceCm);
  dtostrf(distanceCm, 4, 2, msgBuffer);
  MQTT.publish(TOPICO_PUBLISH,msgBuffer);
}

void verificarTemp(){
  char msgBuffer[4];
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  float temp = data.temperature;
  //Serial.println("Temp: " + String(data.temperature, 2) + "°C");
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Temperatura:");
  LCD.setCursor(0,1);
  LCD.print(temp);
  Serial.println(temp);
  dtostrf(temp, 4, 2, msgBuffer);
  MQTT.publish(TOPICO_PUBLISH_2,msgBuffer);
}

void verificarHumi(){
  char msgBuffer[4];
  TempAndHumidity  data = dhtSensor.getTempAndHumidity();
  float humi = data.humidity;
  //Serial.println("Humidity: " + String(data.humidity, 1) + "%");
  LCD.clear();
  LCD.setCursor(0,0);
  LCD.print("Humidade:");
  LCD.setCursor(0,1);
  LCD.print(humi);
  Serial.println(humi);
  dtostrf(humi, 4, 2, msgBuffer);
  MQTT.publish(TOPICO_PUBLISH_3,msgBuffer);
}

void loop() {
  // put your main code here, to run repeatedly:
  VerificaConexoesWiFIEMQTT();
  /*LCD.clear();
  LCD.setCursor(0,0);
  LCD.print(valor);*/
  verificarDist();
  delay(2000);
  verificarTemp();
  delay(2000);
  verificarHumi();
  delay(2000); // this speeds up the simulation
  MQTT.loop();
}
