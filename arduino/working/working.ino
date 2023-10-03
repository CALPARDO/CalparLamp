#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>

/* Set these to your desired credentials. */
const char *ssid = "CALPARDO";
const char *password = "bursaspor16";

// Web/Server address to read/write from
const char *host = "77.68.15.151";

WiFiClient WiFiClient;

//=======================================================================
//                    Power on setup
//=======================================================================

void setup()
{
    delay(1000);
    Serial.begin(115200);
    WiFi.mode(WIFI_OFF); // Prevents reconnection issue (taking too long to connect)
    delay(1000);
    WiFi.mode(WIFI_STA); // This line hides the viewing of ESP as wifi hotspot

    WiFi.begin(ssid, password); // Connect to your WiFi router
    Serial.println("");

    Serial.print("Connecting");
    // Wait for connection
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }

    // If connection successful show IP address in serial monitor
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP()); // IP address assigned to your ESP
}

//=======================================================================
//                    Main Program Loop
//=======================================================================
void loop()
{
    HTTPClient http; // Declare object of class HTTPClient

    http.setUserAgent("Mozilla/5.0 (X11; Ubuntu; Linux x86_64; rv:78.0) Gecko/20100101 Firefox/78.0");

    String rgb_val, postData, getData, btn_state, led_state, led_brt;

    // Post Data
    getData = "?btn_state=" + btn_state + "&led_state=" + led_state + "&rgb_val=" + rgb_val + "&led_brt=" + led_brt;

    http.begin(WiFiClient, "77.68.15.151", 80, "http://calpardo.com/calparlamp/postdata.php" + getData); // Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");                                 // Specify content-type header

    int httpCode = http.GET();         // Send the request
    String payload = http.getString(); // Get the response payload

    Serial.println(httpCode); // Print HTTP return code
    Serial.println(payload);  // Print request response payload

    http.end(); // Close connection

    delay(5000); // Post Data at every 5 seconds
}
//=======================================================================