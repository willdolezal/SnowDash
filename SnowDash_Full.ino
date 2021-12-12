#include <Arduino.h>
#include <rpcWiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include <ArduinoJson.h>
#include"Free_Fonts.h"
#include"TFT_eSPI.h"
TFT_eSPI tft;

// Set up array of resort options
int resortID_num = 0;
int resortID_list[8] = {427,2,314,403,10,493,466,221};
int total_resorts = 8 - 1;

// SSL certificate for HTTPS access to the OnTheSnow API
const char* test_root_ca = \
                           "-----BEGIN CERTIFICATE-----\n"
                           "MIIEdTCCA12gAwIBAgIJAKcOSkw0grd/MA0GCSqGSIb3DQEBCwUAMGgxCzAJBgNV\n"
                           "BAYTAlVTMSUwIwYDVQQKExxTdGFyZmllbGQgVGVjaG5vbG9naWVzLCBJbmMuMTIw\n"
                           "MAYDVQQLEylTdGFyZmllbGQgQ2xhc3MgMiBDZXJ0aWZpY2F0aW9uIEF1dGhvcml0\n"
                           "eTAeFw0wOTA5MDIwMDAwMDBaFw0zNDA2MjgxNzM5MTZaMIGYMQswCQYDVQQGEwJV\n"
                           "UzEQMA4GA1UECBMHQXJpem9uYTETMBEGA1UEBxMKU2NvdHRzZGFsZTElMCMGA1UE\n"
                           "ChMcU3RhcmZpZWxkIFRlY2hub2xvZ2llcywgSW5jLjE7MDkGA1UEAxMyU3RhcmZp\n"
                           "ZWxkIFNlcnZpY2VzIFJvb3QgQ2VydGlmaWNhdGUgQXV0aG9yaXR5IC0gRzIwggEi\n"
                           "MA0GCSqGSIb3DQEBAQUAA4IBDwAwggEKAoIBAQDVDDrEKvlO4vW+GZdfjohTsR8/\n"
                           "y8+fIBNtKTrID30892t2OGPZNmCom15cAICyL1l/9of5JUOG52kbUpqQ4XHj2C0N\n"
                           "Tm/2yEnZtvMaVq4rtnQU68/7JuMauh2WLmo7WJSJR1b/JaCTcFOD2oR0FMNnngRo\n"
                           "Ot+OQFodSk7PQ5E751bWAHDLUu57fa4657wx+UX2wmDPE1kCK4DMNEffud6QZW0C\n"
                           "zyyRpqbn3oUYSXxmTqM6bam17jQuug0DuDPfR+uxa40l2ZvOgdFFRjKWcIfeAg5J\n"
                           "Q4W2bHO7ZOphQazJ1FTfhy/HIrImzJ9ZVGif/L4qL8RVHHVAYBeFAlU5i38FAgMB\n"
                           "AAGjgfAwge0wDwYDVR0TAQH/BAUwAwEB/zAOBgNVHQ8BAf8EBAMCAYYwHQYDVR0O\n"
                           "BBYEFJxfAN+qAdcwKziIorhtSpzyEZGDMB8GA1UdIwQYMBaAFL9ft9HO3R+G9FtV\n"
                           "rNzXEMIOqYjnME8GCCsGAQUFBwEBBEMwQTAcBggrBgEFBQcwAYYQaHR0cDovL28u\n"
                           "c3MyLnVzLzAhBggrBgEFBQcwAoYVaHR0cDovL3guc3MyLnVzL3guY2VyMCYGA1Ud\n"
                           "HwQfMB0wG6AZoBeGFWh0dHA6Ly9zLnNzMi51cy9yLmNybDARBgNVHSAECjAIMAYG\n"
                           "BFUdIAAwDQYJKoZIhvcNAQELBQADggEBACMd44pXyn3pF3lM8R5V/cxTbj5HD9/G\n"
                           "VfKyBDbtgB9TxF00KGu+x1X8Z+rLP3+QsjPNG1gQggL4+C/1E2DUBc7xgQjB3ad1\n"
                           "l08YuW3e95ORCLp+QCztweq7dp4zBncdDQh/U90bZKuCJ/Fp1U1ervShw3WnWEQt\n"
                           "8jxwmKy6abaVd38PMV4s/KCHOkdp8Hlf9BRUpJVeEXgSYCfOn8J3/yNTd126/+pZ\n"
                           "59vPr5KW7ySaNRB6nJHGDn2Z9j8Z3/VyVOEVqQdZe4O/Ui5GjLIAZHYcSNPYeehu\n"
                           "VsyuLAOQ1xk4meTKCRlb/weWsKh/NEnfVqn3sF/tM+2MR7cwA130A4w=\n"
                           "-----END CERTIFICATE-----\n";


WiFiClientSecure client;



void setup() {

  // Initiate onboard buttons
  pinMode(WIO_KEY_A, INPUT_PULLUP);
  pinMode(WIO_KEY_B, INPUT_PULLUP);
  pinMode(WIO_KEY_C, INPUT_PULLUP);

  // Connect to board serial port
  Serial.begin(115200);
  delay(1000);

  // Load TFT library and set up LCD screen
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  
  // Connect to WiFi 
  WiFi.mode(WIFI_STA);
  // wait for WiFi connection
  Serial.println("Connecting to WiFi...");
  tft.setFreeFont(FSI12);
  tft.fillRect(0,0,360,56,TFT_BLACK);
  tft.drawString("Connecting to WiFi...",10,10);
  
  while ((WiFi.status() != WL_CONNECTED)) {
    WiFi.begin("Bill Wi, the Science Fi", "6128771216");
    delay(3000);
    Serial.println("Taking another try at the WiFi.");
    tft.fillRect(0,0,360,56,TFT_BLACK);
    tft.drawString("Taking another try at the WiFi.",10,10);
  }
  
  Serial.println("Success! WiFi connected at IP:");
  Serial.println(WiFi.localIP());
  tft.drawString("Success! WiFi connected.",10,10);
  tft.fillRect(0,0,360,56,TFT_BLACK);
  
  client.setCACert(test_root_ca);


  // Pie chart of lifts open

  // Recent snowfall at base and summit
  tft.setFreeFont(FSS12);
  tft.drawString("Recent Snow",20,57);
  tft.drawFastHLine(15,80,150,TFT_RED);
  tft.drawFastHLine(15,81,150,TFT_RED);
  tft.drawString("24H",25,90);
  tft.drawString("72H",25,120);
  tft.setFreeFont(FSI9);
  tft.drawString("Base:",25,146);
  tft.drawString("Top:",90,146);

  // Current temperature and wind conditions
  tft.setFreeFont(FSS12);
  tft.drawString("WeatherAt:",175,57);
  tft.setFreeFont(FSS9);
  tft.drawString("Type",175,108);
  tft.drawString("Temp",170,130);
  tft.drawString("Wind",175,152);
  tft.drawFastVLine(220,108,60,TFT_RED);
  
  
  // 7 day forecast as a bar graph
  tft.setFreeFont(FSS12);
  tft.drawString("7 Day",15,185);
  tft.drawString("Forecast",15,210);
  tft.drawFastHLine(125,235,180,TFT_DARKGREY);
  
  //Initial run to populate dashboard
  weatherData(427);
}

void loop() {
  int resortID = resortID_list[resortID_num];
  // put your main code here, to run repeatedly:
   if (digitalRead(WIO_KEY_A) == LOW) {
    Serial.println("Right Key pressed");
    resortID_num = resortID_num + 1;
    resortID_num = min(resortID_num,total_resorts);
    int resortID = resortID_list[resortID_num];
    eraseCurrentData();
    weatherData(resortID);
   }
   else if (digitalRead(WIO_KEY_B) == LOW) {
    Serial.println("Mid Key pressed");
    eraseCurrentData();
    weatherData(resortID);
   }
   else if (digitalRead(WIO_KEY_C) == LOW) {
    Serial.println("Left Key pressed");
    resortID_num = resortID_num - 1;
    resortID_num = max(resortID_num,0);
    int resortID = resortID_list[resortID_num];
    eraseCurrentData();
    weatherData(resortID);
   }
   delay(200);

}


void weatherData(int resortID) {
  Serial.println(resortID);
  String weather_data_url = "https://api.onthesnow.com/api/v2/resort/" + String(resortID) + "/snowreport";

  if (&client) {
    {
      // Add a scoping block for HTTPClient https to make sure it is destroyed before WiFiClientSecure *client is
      HTTPClient https;
      
      Serial.print("[HTTPS] begin...\n");
      if (https.begin(client, weather_data_url)) {  // HTTPS
        Serial.print("[HTTPS] GET...\n");
        // start connection and send HTTP header
        int httpCode = https.GET();

        // httpCode will be negative on error
        if (httpCode > 0) {
          // HTTP header has been send and Server response header has been handled
          Serial.printf("[HTTPS] GET... code: %d\n", httpCode);

          // file found at server
          if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
            String payload = https.getString();
            Serial.println(payload);

            // convert HTTP payload into JSON and extract into variables
            DynamicJsonDocument doc(6144);
            deserializeJson(doc, payload);

            // 7 day forecase variables
            JsonArray forecast = doc["forecast"];
            JsonObject day_1 = forecast[0];
            int day_1_snow = day_1["snow"];
            JsonObject day_2 = forecast[1];
            int day_2_snow = day_2["snow"];
            JsonObject day_3 = forecast[2];
            int day_3_snow = day_3["snow"];
            JsonObject day_4 = forecast[3];
            int day_4_snow = day_4["snow"];
            JsonObject day_5 = forecast[4];
            int day_5_snow = day_5["snow"];
            JsonObject day_6 = forecast[5];
            int day_6_snow = day_6["snow"];
            JsonObject day_7 = forecast[6];
            int day_7_snow = day_7["snow"];

            // Recent snow fall at base and summit
            JsonObject snow = doc["snow"];
            int last24 = snow["last24"];
            int base = snow["base"];
            int summit = snow["summit"];
            int last72 = snow["last72"];


            // Weather conditions from when data pushed
            JsonObject currentWeather = doc["currentWeather"];
            double wind = currentWeather["wind"];
            double base_min = currentWeather["base"]["min"];
            double base_max = currentWeather["base"]["max"];
            double summit_min = currentWeather["summit"]["min"];
            double summit_max = currentWeather["base"]["max"];
            double current_min = min(base_min,summit_min);
            double current_max = max(base_max,summit_max);
            String condition_type = currentWeather["type"];
            String recent_date = currentWeather["date"];         
            recent_date.remove(recent_date.lastIndexOf(":00"));

            // Lift status
            JsonObject lift_status = doc["lifts"];
            int open_lifts = lift_status["open"];
            int total_lifts = lift_status["total"];

            //Acquire resort name
            String resort_name = doc["corporateName"];
            // Header text and underline
            tft.setFreeFont(FSSB18);
            tft.drawString(resort_name,10,10);


            // Insert weather data 
            tft.setFreeFont(FSSB9);
            tft.drawNumber(last24,85,95);
            tft.drawString("inches",115,95);
            tft.drawNumber(last72,85,125);
            tft.drawString("inches",115,125);
            
            tft.drawNumber(base,75,146);
            tft.drawNumber(summit,130,146);
            
            tft.setFreeFont(FSI9);
            tft.drawString(recent_date,180,80);
            tft.setFreeFont(FSS9);
            tft.drawString(condition_type,230,108);
            tft.drawFloat(current_min,0,230,130);
            tft.drawString(":",265,130);
            tft.drawFloat(current_max,0,270,130);
            tft.drawString("C",310,130);
            tft.drawNumber(wind,230,152);
            tft.drawString("MPH",255,152);

            // Build rectangles to form bar graph
            tft.fillRect(125,(235 - day_1_snow),20,(0 + day_1_snow),TFT_LIGHTGREY);
            tft.fillRect(150,(235 - day_2_snow),20,(0 + day_2_snow),TFT_LIGHTGREY);
            tft.fillRect(175,(235 - day_3_snow),20,(0 + day_3_snow),TFT_LIGHTGREY);
            tft.fillRect(200,(235 - day_4_snow),20,(0 + day_4_snow),TFT_LIGHTGREY);
            tft.fillRect(225,(235 - day_5_snow),20,(0 + day_5_snow),TFT_LIGHTGREY);
            tft.fillRect(250,(235 - day_6_snow),20,(0 + day_6_snow),TFT_LIGHTGREY);
            tft.fillRect(275,(235 - day_7_snow),20,(0 + day_7_snow),TFT_LIGHTGREY);
          }
        } else {
          Serial.printf("[HTTPS] GET... failed, error: %s\n", https.errorToString(httpCode).c_str());
          
          ResetToBoot();
          
        }

        https.end();
      } else {
        Serial.printf("[HTTPS] Unable to connect\n");
        delay(1);
        ResetToBoot();
      }
      // End extra scoping block
    }

  } else {
    Serial.println("Unable to create client");
    delay(1);
    ResetToBoot();
  }
}

void ResetToBoot()
{
    Serial.println("Rebooting Arduino. Better luck next time.");
    delay(1);
    NVIC_SystemReset();
    Serial.println("Reboot unsuccessful. Now we're goofed.");
}

void eraseCurrentData() {

  //Wipe out resort header
  tft.fillRect(0,0,360,56,TFT_BLACK);
  //Wipe out recent snow left column
  tft.fillRect(85,95,30,50,TFT_BLACK);
  //Wipe out recent snow right column
  tft.fillRect(130,95,30,50,TFT_BLACK);
  //Wipe out datetime
  tft.fillRect(180,80,180,25,TFT_BLACK);
  //Wipe out weather conditions
  tft.fillRect(240,108,120,20,TFT_BLACK);
  tft.fillRect(240,130,70,25,TFT_BLACK);
  tft.fillRect(240,152,180,23,TFT_BLACK);
  //Wipe out bar graph
  tft.fillRect(125,170,195,65,TFT_BLACK);
}
