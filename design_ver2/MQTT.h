#ifndef _MQTT_H
#define _MQTT_H

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>

void MQTT_Callback(StaticJsonDocument<256> *msg);

void MQTT_onMessageListener(char *topic, byte *payload, unsigned int length)
{
    StaticJsonDocument<256> msg;
    deserializeJson(msg, payload, length);
    MQTT_Callback(&msg);
}

/**
 * @brief MQTT的辣鸡封装
 * @author lyk
 */
class MQTT
{
private:
    WiFiClient espClient;
    PubSubClient client;
    const char *mqtt_server;
    String subTopic;
    String pubTopic;
    void reconnect()
    {
        // Loop until we're reconnected
        while (!client.connected())
        {
            Serial.print("Attempting MQTT connection...");
            // Create a random client ID
            String clientId = "ESP8266Client-";
            clientId += String(random(0xffff), HEX);
            // Attempt to connect
            if (client.connect(clientId.c_str()))
            {
                Serial.println("connected");
                // Once connected, publish an announcement...
                // client.publish("outTopic", "hello world");
                // ... and resubscribe
                client.subscribe(this->subTopic.c_str());
            }
            else
            {
                Serial.print("failed, rc=");
                Serial.print(client.state());
                Serial.println(" try again in 5 seconds");
                // Wait 5 seconds before retrying
                delay(5000);
            }
        }
    }
    void callback(char *topic, byte *payload, unsigned int length)
    {
    }

public:
    MQTT(const char *mqtt_server)
    {
        this->mqtt_server = mqtt_server;
        this->subTopic = "cn/edu/cuz/scj/cmd/" + WiFi.macAddress();
        this->pubTopic = "cn/edu/cuz/scj/data/" + WiFi.macAddress();
    }

    ~MQTT()
    {
    }

    void begin()
    {
        this->client.setClient(this->espClient);
        client.setServer(mqtt_server, 1883);
        client.setCallback(MQTT_onMessageListener);
    }

    void loop()
    {
        if (!this->client.connected())
        {
            this->reconnect();
        }
        this->client.loop();
    }

    void send(bool ledState, uint32_t ledColor, int temperature, int humidity, float light, bool buzzerState)
    {
        StaticJsonDocument<256> json;
        json["led"]["state"] = ledState;
        json["led"]["color"] = ledColor;
        json["temperature"] = temperature;
        json["humidity"] = humidity;
        json["light"] = light;
        json["buzzer"] = buzzerState;

        String msg;
        serializeJson(json, msg);
        this->client.publish(this->pubTopic.c_str(), msg.c_str());
    }
};

#endif
