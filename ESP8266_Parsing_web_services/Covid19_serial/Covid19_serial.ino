//Written by Rupak Poddar
//www.youtube.com/RupakPoddar
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>

char ssid[] = "Change_this";       //Your SSID
char password[] = "Change_this";  //Your password

String web_url = "http://covid2019-api.herokuapp.com/country/india";  //COVID 19 API cURL for India

void setup() {
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

      Serial.print("Confirmed cases: ");
      Serial.println(India_confirmed);

      Serial.print("Total deaths: ");
      Serial.println(India_deaths);

      Serial.print("Recovered patients: ");
      Serial.println(India_recovered);

      Serial.print("Last updated on ");
      Serial.println(dt);

      //Serial.println(ts);
    }
  } else {
    Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
  }
  http.end();

  ESP.deepSleep(108e8, WAKE_RF_DEFAULT);  //Sleep for 3 hours
}

void loop() {}
