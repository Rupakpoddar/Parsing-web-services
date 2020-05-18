//Written by Rupak Poddar
//www.youtube.com/RupakPoddar
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

char ssid[] = "Change_this";       //Your SSID
char password[] = "Change_this";  //Your password

String web_url = "http://covid2019-api.herokuapp.com/country/india";  //COVID 19 API cURL for India

void setup() {
  lcd.init();
  lcd.backlight();
  Serial.begin(115200);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(1000);

  //Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
   
  WiFi.begin(ssid, password);
   
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print("-");
  }
  Serial.println("");
  Serial.println("WiFi connected");

  HTTPClient http;
  http.begin(web_url);
  int httpCode = http.GET();

  if(httpCode > 0) {
    // file found at server
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();

      //Serial.println(payload);

     const size_t capacity = 2*JSON_OBJECT_SIZE(3) + 60;
        DynamicJsonDocument doc(capacity);
     
      deserializeJson(doc, payload);

      JsonObject India = doc["India"];
      int India_confirmed = India["confirmed"];
      int India_deaths = India["deaths"];
      int India_recovered = India["recovered"];

      const char* dt = doc["dt"];
      long ts = doc["ts"];

      lcd.clear();

      lcd.setCursor(0,0);
      lcd.print("Confirmed: ");
      lcd.print(India_confirmed);

      lcd.setCursor(0,1);
      lcd.print("Deaths: ");
      lcd.print(India_deaths);

      lcd.setCursor(0,2);
      lcd.print("Recovered: ");
      lcd.print(India_recovered);

      lcd.setCursor(0,3);
      lcd.print("Date: ");
      lcd.print(dt);      
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  ESP.deepSleep(108e8, WAKE_RF_DEFAULT);  //Sleep for 3 hours
}

void loop() {}
