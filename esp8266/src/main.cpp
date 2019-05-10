/**
 *  Copyright 2019 Łukasz Kieś
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <Arduino.h>
#include <Adafruit_BME280.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "main.h"

bool connectWiFi()
{
    uint8_t wifiTimeout = 0;

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASS);
    while (!WiFi.isConnected()) {
        delay(100);
        wifiTimeout++;
        if (wifiTimeout > 100) {
            return false;
        }
    }
    Serial.print("IP: ");
    Serial.println(WiFi.localIP());

    return true;
}

void goDeepSleep()
{
    ESP.deepSleep(60000000);
}

void sendReadings()
{
    const uint16_t szBuffer = JSON_OBJECT_SIZE(4);
    StaticJsonDocument<szBuffer> document;
    char jsonMessage[szBuffer];
    HTTPClient client;

    bme.takeForcedMeasurement();
    document["sensor"] = ESP.getChipId();
    document["temperature"] = bme.readTemperature();
    document["humidity"] = bme.readHumidity();
    document["pressure"] = bme.readPressure() / 100.0f;
    serializeJson(document, Serial);
    serializeJson(document, jsonMessage);
    client.begin(API_PATH);
    client.addHeader("Content-Type", "application/json");
    client.POST(jsonMessage);
    Serial.println(client.getString());
    client.end();
}

void setup()
{
    Serial.begin(74880);
    while (!Serial) {
        yield();
    }
    if (!bme.begin()) {
        Serial.println("Cannot connect to BME280!");
        goDeepSleep();
    }
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1);
    if (!connectWiFi()) {
        Serial.println("Cannot connect to WiFI!");
        goDeepSleep();
    }
    sendReadings();
    goDeepSleep();
}

void loop()
{
}
