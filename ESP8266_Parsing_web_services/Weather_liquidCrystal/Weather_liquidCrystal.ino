//Written by Rupak Poddar
//www.youtube.com/RupakPoddar
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,20,4);

char ssid[] = "Change_this";       //Your SSID
char password[] = "Change_this";  //Your password

String web_url = "http://api.openweathermap.org/data/2.5/weather?q=New%20Delhi&appid=12841c714e99680a75ed314f17287745";  //Weather API cURL for New Delhi, India

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

  const size_t capacity = JSON_ARRAY_SIZE(1) + JSON_OBJECT_SIZE(1) + 2*JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(4) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(6) + JSON_OBJECT_SIZE(13) + 270;
  DynamicJsonDocument doc(capacity);

  deserializeJson(doc, payload);

  float coord_lon = doc["coord"]["lon"];
  float coord_lat = doc["coord"]["lat"];

  JsonObject weather_0 = doc["weather"][0];
  int weather_0_id = weather_0["id"];
  const char* weather_0_main = weather_0["main"]; // "Haze"
  const char* weather_0_description = weather_0["description"];
  const char* weather_0_icon = weather_0["icon"];

  const char* base = doc["base"];

  JsonObject main = doc["main"];
  float main_temp = main["temp"]; // 309.18
  float main_feels_like = main["feels_like"]; // 306.09
  float main_temp_min = main["temp_min"]; // 308.15
  float main_temp_max = main["temp_max"]; // 310.15
  int main_pressure = main["pressure"];
  int main_humidity = main["humidity"];

  int visibility = doc["visibility"];

  float wind_speed = doc["wind"]["speed"]; // 1.5
  int wind_deg = doc["wind"]["deg"];

  int clouds_all = doc["clouds"]["all"];

  long dt = doc["dt"]; // 1589814028

  JsonObject sys = doc["sys"];
  int sys_type = sys["type"];
  int sys_id = sys["id"];
  const char* sys_country = sys["country"]; // "IN"
  long sys_sunrise = sys["sunrise"];
  long sys_sunset = sys["sunset"];

  int timezone = doc["timezone"];
  long id = doc["id"];
  const char* name = doc["name"]; // "New Delhi"
  int cod = doc["cod"]; // 200

  lcd.clear();

  lcd.setCursor(0,0);
  lcd.print(main_temp-273);
  lcd.print(" C");

  lcd.setCursor(0,1);
  lcd.print(String(weather_0_main)+" "+String(main_temp_max-273)+"/"+String(main_temp_min-273)+" C");

  lcd.setCursor(0,2);
  lcd.print("Feels like: ");
  lcd.print(main_feels_like-273);

  lcd.setCursor(0,3);
  lcd.print(name);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  ESP.deepSleep(108e8, WAKE_RF_DEFAULT);  //Sleep for 3 hours
}

void loop() {}
