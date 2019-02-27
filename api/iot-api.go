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

package main

import (
	"math"
	"net/http"

	"github.com/gin-gonic/gin"
	"github.com/jinzhu/gorm"
	_ "github.com/jinzhu/gorm/dialects/sqlite"
)

type reading struct {
	gorm.Model
	Sensor      uint32  `json:"sensor" binding:"required"`
	Temperature float64 `json:"temperature" binding:"min=-40,max=85"` // min & max from sensor capabilities
	Humidity    float64 `json:"humidity" binding:"min=0,max=100"`
	Pressure    float64 `json:"pressure" binding:"min=300,max=1100"`
	DewPoint    float64
}

var db *gorm.DB

func getReading(c *gin.Context) {
	var allReadings []reading

	if err := db.Order("created_at desc").Limit(60).Find(&allReadings).Error; err != nil {
		c.JSON(http.StatusServiceUnavailable, gin.H{"message": err.Error()})
		return
	}
	c.JSON(200, allReadings)
}

func postReading(c *gin.Context) {
	var newReading reading

	if err := c.ShouldBindJSON(&newReading); err != nil {
		c.JSON(http.StatusBadRequest, gin.H{"message": err.Error()})
		return
	}
	newReading.DewPoint = math.Round((math.Pow(newReading.Humidity/100, 1.0/8.0)*
		(112.0+0.9*newReading.Temperature)+0.1*newReading.Temperature-112.0)*
		100.0) / 100.0
	if err := db.Create(&newReading).Error; err != nil {
		c.JSON(http.StatusServiceUnavailable, gin.H{"message": err.Error()})
		return
	}
	c.JSON(http.StatusCreated, newReading)
}

func main() {
	var err error

	db, err = gorm.Open("sqlite3", "api.sqlite3")
	if err != nil {
		panic("Failed to connect to database!")
	}
	defer db.Close()
	db.AutoMigrate(&reading{})
	gin.DisableConsoleColor()
	router := gin.Default()
	router.StaticFile("/", "./index.html")
	router.GET("/reading", getReading)
	router.POST("/reading", postReading)
	router.Run()
}
