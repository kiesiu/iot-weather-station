/**
 *  Copyright 2019-2020 Łukasz Kieś
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
    DEBUG_PRINT("IP: ");
    DEBUG_PRINTLN(WiFi.localIP());

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
    WiFiClient client;
    HTTPClient clientHttp;

    bme.takeForcedMeasurement();
    document["sensor"] = ESP.getChipId();
    document["temperature"] = bme.readTemperature();
    document["humidity"] = bme.readHumidity();
    document["pressure"] = bme.readPressure() / 100.0f;
    #ifdef DEBUG
        serializeJson(document, Serial);
    #endif
    serializeJson(document, jsonMessage);
    clientHttp.begin(client, API_PATH);
    clientHttp.addHeader("Content-Type", "application/json");
    clientHttp.POST(jsonMessage);
    DEBUG_PRINTLN(clientHttp.getString());
    clientHttp.end();
}

void setup()
{
    #ifdef DEBUG
        Serial.begin(74880);
        while (!Serial) {
            yield();
        }
    #endif
    if (!bme.begin()) {
        DEBUG_PRINTLN("Cannot connect to BME280!");
        goDeepSleep();
    }
    bme.setSampling(Adafruit_BME280::MODE_FORCED,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1,
        Adafruit_BME280::SAMPLING_X1);
    if (!connectWiFi()) {
        DEBUG_PRINTLN("Cannot connect to WiFI!");
        goDeepSleep();
    }
    sendReadings();
    goDeepSleep();
}

void loop()
{
}
