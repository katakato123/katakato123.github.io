/********************###库文件声###********************/
#include <ESP8266WiFi.h>
#include <Ticker.h>
#include "MQTT.h"
#include "SSD1306Wire.h"
#include "ColorLED.h"
#include "dht11.h"
#include "BH1750FVI.h"

const char *ssid = "katakato";
const char *password = "12345678";
const char *mqtt_server = "mqtt.eclipseprojects.io";

MQTT mqtt(mqtt_server);

// oled屏幕的I2c地址
const int I2C_ADDR = 0x3c;

Ticker ticker;
unsigned int countMQ = 0;
unsigned int countSensor = 0;
unsigned int countBuzzer = 0;
float lux = 0;
bool buzzerState = false;

SSD1306Wire oled(I2C_ADDR, D2, D1);
dht11 DHT11;
BH1750FVI bh1750fvi;
ColorLED colorLED(D5, D6, D7);

/********************###程序初始化###********************/
void setup()
{
    // 蜂鸣器
    pinMode(D8, OUTPUT);

    /* 1. 初始化串口通讯波特率为115200*/
    Serial.begin(115200);
    setup_wifi();
    mqtt.begin();

    oled.init();
    oled.flipScreenVertically();
    oled.setContrast(255);
    oled.clear();
    oled.display();

    /* BH1750FVI初始化 */
    bh1750fvi.begin();
    colorLED.setState(false);

    ticker.attach(1, timer);
}

void loop()
{
    mqtt.loop();
    if (countMQ > 2)
    {
        mqtt.send(colorLED.getState(), colorLED.getColor(), DHT11.temperature, DHT11.humidity, lux, buzzerState);
        countMQ = 0;
    } 

    if (countSensor > 0)
    {
        lux = bh1750fvi.getLux();
        DHT11.read(D4); // 读取温湿度
        countSensor = 0;
    }

    if (DHT11.temperature >= 40 || DHT11.temperature <= 10)
    {
        colorLED.setState(true);
        colorLED.setColor(0x0000FF);
    }
    else if (DHT11.humidity < 30 && DHT11.humidity > 80)
    {
        colorLED.setState(true);
        colorLED.setColor(0x00FF00);
    }
    else if (lux > 3000 || lux < 100)
    {
        colorLED.setState(true);
        colorLED.setColor(0xFFFFFF);
    }
    else if (DHT11.humidity < 5 && DHT11.temperature > 50 && lux > 3500)
    {
        colorLED.setState(true);
        colorLED.setColor(0xFF0000);
        buzzerState = true;
    }
    else
    {
        colorLED.setState(false);
        buzzerState = false;
    }

    if (countBuzzer > 0)
    {
        if (buzzerState)
        {
            digitalWrite(D8, !digitalRead(D8)); // 叫
        }
        else
        {
            digitalWrite(D8, LOW); // 不叫
        }
        countBuzzer = 0;
    }

    String temStr = "temperature:" + String(DHT11.temperature) + "°C";
    String humStr = "humidity:" + String(DHT11.humidity) + "%";
    String luxStr = "Lux:" + String(lux);

    oled.clear();
    oled.setFont(ArialMT_Plain_16);
    oled.drawString(0, 0, temStr);
    oled.drawString(0, 16, humStr);
    oled.drawString(0, 32, luxStr);
    oled.display();
}
void timer()
{
    countSensor++;
    countBuzzer++;
    countMQ++;
}

/**
 * @brief Set the up wifi object
 * @author lyk
 *
 */
void setup_wifi()
{

    delay(10);
    // We start by connecting to a WiFi network
    Serial.println();
    Serial.print("Connecting to ");
    Serial.println(ssid);

    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    randomSeed(micros());

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    Serial.println(WiFi.macAddress());
}

void MQTT_Callback(StaticJsonDocument<256> *msg)
{
}
