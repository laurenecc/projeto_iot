/***************************************************************************
  This is a library for the BME280 humidity, temperature & pressure sensor.

  Designed specifically to work with the Adafruit BME280 Breakout
  ----> http://www.adafruit.com/products/2650

  These sensors use I2C or SPI to communicate, 2 or 4 pins are required.
  to interface. The device's I2C address is either 0x76 or 0x77.

  Adafruit invests time and resources providing this open-source code,
  please support Adafruit and open-source hardware by purchasing products
  from Adafruit!

  Written by Limor Fried & Kevin Townsend for Adafruit Industries.
  BSD license, all text above must be included in any redistribution.
  See the LICENSE file for details.
 ***************************************************************************/

/*
Universidade Presbiteriana Mackenzie
Código adaptado para o projeto: "Sistemas IoT para Medir Temperatura e Umidade Local"
Autora: Laurene Cristina Caldeira
Data: 11/11/2023
Local: Uberlândia-MG
*/

// Declaração de bibliotecas necessárias
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define BME_SCK 13
#define BME_MISO 12
#define BME_MOSI 11
#define BME_CS 10

#define SEALEVELPRESSURE_HPA (1013.25)

#define RELAY_PIN 13  // *Define em qual pino o Relay está conectado com a placa ESP32, neste caso GPIO13

// *Valores pré-definidos para ligar ou desligar um umidificador de ar (ventilador)

#define TEMP_PARA_LIGAR 30     // *Acima de 30 graus Celsius, o ventilador deve ser acionado
#define TEMP_PARA_DESLIGAR 20  // *Abaixo de 20 graus Celsius, o ventilador deve ser desligado

#define UMID_PARA_LIGAR 50     // *Ou caso a umidade esteja abaixo de 50%, o ventilador deve ser acionado
#define UMID_PARA_DESLIGAR 70  // *Ou caso a umidade esteja acima de 70%, o ventilador deve ser desligado

Adafruit_BME280 bme;  // I2C
//Adafruit_BME280 bme(BME_CS); // hardware SPI
//Adafruit_BME280 bme(BME_CS, BME_MOSI, BME_MISO, BME_SCK); // software SPI

unsigned long delayTime;

// WiFi
const char *ssid = "CRISTY";         // Enter your WiFi name
const char *password = "128052188";  // Enter WiFi password

// MQTT Broker
const char *mqtt_broker = "broker.emqx.io";
const char *topic = "topicoBME/Temperatura";
const char *topicUmid = "topicoBME/Umidade";
const char *mqtt_username = "emqx";
const char *mqtt_password = "public";
const int   mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);


void setup() {
  Serial.begin(9600);
  while (!Serial);  // time to get serial running;
  Serial.println(F("BME280 test"));

  unsigned status;

  // default settings
  status = bme.begin();
  status = bme.begin(0x76);

  if (!status) {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x");
    Serial.println(bme.sensorID(), 16);
    Serial.print("   ID of 0xFF probably means a bad address, a BMP 180 or BMP 085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP 280,\n");
    Serial.print("   ID of 0x60 represents a BME 280.\n");
    Serial.print("   ID of 0x61 represents a BME 680.\n");
    while (1) delay(10);
  }

  Serial.println("-- Default Test --");
  delayTime = 10000;  // *Espera de 20 segundos

  pinMode(RELAY_PIN, OUTPUT);  // *Configura o Relay para se comportar como um output

  Serial.println();

  // connecting to a WiFi network
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to the WiFi network.");
  //connecting to a mqtt broker
  client.setServer(mqtt_broker, mqtt_port);
  client.setCallback(callback);

  while (!client.connected()) {
    String client_id = "esp32-client-";
    client_id += String(WiFi.macAddress());
    Serial.printf("The client %s connects to the public mqtt broker\n", client_id.c_str());

    if (client.connect(client_id.c_str(), mqtt_username, mqtt_password)) {
      Serial.println("Public emqx mqtt broker connected.");
    } else {
      Serial.print("failed with state ");
      Serial.print(client.state());
      delay(2000);
    }
  }
}


void loop() {
  float temperature = bme.readTemperature();  // *Faz a leitura de temperatura e armazena na variável
  float humidity = bme.readHumidity();        // *Faz a leitura da umidade e armanzena na variável

  char temp[8]; // Buffer big enough for 7-character float
  dtostrf(temperature, 3, 2, temp);

  char humid[8];
  dtostrf(humidity, 3, 2, humid);

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("Falha na leitura do sensor BME280!");
  } else {
    if ((temperature >= TEMP_PARA_LIGAR) || (humidity <= UMID_PARA_LIGAR)) {
      Serial.println("Acionar Relay! (Ligar ventilador ou umidificador de ar)");
      printValues();

      // publish and subscribe
      client.publish(topic, temp);
      client.publish(topicUmid, humid);
      client.subscribe(topic);
      client.subscribe(topicUmid);

      digitalWrite(RELAY_PIN, HIGH);  // *O Relay libera a energia para acionar o dispositivo (ventilador, umidificador de ar, ar-condicionado)
      //
    } else if ((temperature <= TEMP_PARA_DESLIGAR) || (humidity >= UMID_PARA_DESLIGAR)) {
      Serial.println("Desativar Relay! (Desligar ventilador ou umidificador de ar)");
      printValues();

      // publish and subscribe
      client.publish(topic, temp);
      client.publish(topicUmid, humid);
      client.subscribe(topic);
      client.subscribe(topicUmid);
      
      digitalWrite(RELAY_PIN, LOW);  // *O Relay bloqueia a energia para desligar o dispositivo (ventilador, umidificador de ar, ar-condicionado)
    }
  }

  client.loop();
  delay(delayTime);  // *Intervalo de 10 segundos até a próxima leitura
}


void printValues() {
  Serial.print("Temperatura = ");
  Serial.print(bme.readTemperature());
  Serial.println(" °C");

  Serial.print("Pressão = ");

  Serial.print(bme.readPressure() / 100.0F);
  Serial.println(" hPa");

  Serial.print("Altitude aproximada = ");
  Serial.print(bme.readAltitude(SEALEVELPRESSURE_HPA));
  Serial.println(" m");

  Serial.print("Umidade = ");
  Serial.print(bme.readHumidity());
  Serial.println(" %");

  Serial.println();
}

void callback(char *topic, byte *payload, unsigned int length) {
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Temperature: ");

  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  Serial.println("============================");
}
