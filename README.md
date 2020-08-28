# IoT Weather Station

The following project is a simple implementation of a weather monitoring station based on the ESP8266 powered device. It runs as a client for platform-independent server web service written using Go and a little bit of JavaScript.

## Built with

This project would not be possible without following great open source libraries.

Client code for IOT device:

* [ESP8266 core for Arduino](https://github.com/esp8266/Arduino)
* [Arduino Library for BME280 sensors](https://github.com/adafruit/Adafruit_BME280_Library)
* [ArduinoJson](https://github.com/bblanchon/ArduinoJson)

Server web service:

* [Gin Web Framework](https://github.com/gin-gonic/gin)
* [GORM](https://github.com/jinzhu/gorm)
* [Milligram](https://github.com/milligram/milligram)
* [Chart.js](https://github.com/chartjs/Chart.js)
* [lodash](https://github.com/lodash/lodash)
* [axios](https://github.com/axios/axios)
* [Moment.js](https://github.com/moment/moment)

## More information

There is a [blog post](https://kiesiu.com/internet-of-things-lets-build-something/) describing what the purpose of this repository is.

## License

This project is licensed under the Apache License - see the [LICENSE](LICENSE) file for details.
