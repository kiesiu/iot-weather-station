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

#include <Adafruit_BME280.h>

#define WIFI_SSID "ssid"
#define WIFI_PASS "password"
#define API_PATH "192.168.1.1:8080/reading"

bool connectWiFi();
void goDeepSleep();
void sendReadings();

Adafruit_BME280 bme(D1);
