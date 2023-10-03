// Libraries to include
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266HTTPClient.h>
#include <Adafruit_NeoPixel.h>

// Defining LED pin
#define led_pin 13

/* Set these to your desired credentials. */
const char *ssid = "Derin's Phone";
const char *password = "1616161616";

// Web/Server address to read/write from
const char *host = "77.68.15.151";

// Parsing variables
int MyP = 0;
int MyI = 0;
String array[10];
int index_par = 0;

// WiFi Client
WiFiClient WiFiClient;

// Defining neopixels ring object
Adafruit_NeoPixel ring = Adafruit_NeoPixel(16, led_pin, NEO_GRB + NEO_KHZ800);

//=======================================================================
//                    Power on setup
//=======================================================================

void setup()
{
    delay(1000);

    // Neopixels
    ring.begin();
    ring.show();

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
        // delay(500);
        colorWipe(ring.Color(0, 204, 255), 40);
        colorWipe(ring.Color(0, 0, 0), 40);
        ring.clear();
        Serial.print(".");
    }

    ring.clear();
    colorWipe(ring.Color(255, 0, 0), 20);
    colorWipe(ring.Color(0, 255, 0), 20);
    colorWipe(ring.Color(0, 0, 255), 20);
    colorWipe(ring.Color(0, 0, 0), 20);

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

    String rgb_val, postData, getData, btn_state, led_state, led_brt, pattern;

    // Post Data
    getData = "?btn_state=" + btn_state + "&led_state=" + led_state + "&rgb_val=" + rgb_val + "&led_brt=" + led_brt + "&pattern=" + pattern;

    http.begin(WiFiClient, "77.68.15.151", 80, "http://calpardo.com/calparlamp/postdata.php" + getData); // Specify request destination
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");                                 // Specify content-type header

    int httpCode = http.GET();         // Send the request
    String payload = http.getString(); // Get the response payload

    // Serial.println(httpCode); // Print HTTP return code
    // Serial.println(payload);  // Print request response payload

    // Parsing the payload
    char payload_char[1024];
    strcpy(payload_char, payload.c_str());

    char *ptr = strtok(payload_char, "/");
    char *btn_state_char = ptr;

    ptr = strtok(NULL, "/");
    char *led_state_char = ptr;

    ptr = strtok(NULL, "/");
    char *rgb_val_char = ptr;

    ptr = strtok(NULL, "/");
    char *led_brt_char = ptr;

    ptr = strtok(NULL, "/");
    char *pattern_char = ptr;

    // Converting char variables to string
    String btn_state_str = String(btn_state_char);
    String led_state_str = String(led_state_char);
    String rgb_val_str = String(rgb_val_char);
    String led_brt_str = String(led_brt_char);
    String pattern_str = String(pattern_char);

    // Parsing RGB Values
    strcpy(rgb_val_char, rgb_val_str.c_str());

    char *psr = strtok(rgb_val_char, ",");
    char *red_char = psr;

    psr = strtok(NULL, ",");
    char *green_char = psr;

    psr = strtok(NULL, ",");
    char *blue_char = psr;

    // Converting RGB values to string
    String red_str = String(red_char);
    String green_str = String(green_char);
    String blue_str = String(blue_char);

    // Converting string variables to integer
    int led_brt_int = led_brt_str.toInt();
    int led_state_int = led_state_str.toInt();
    int pattern_int = pattern_str.toInt();
    int red_int = red_str.toInt();
    int green_int = green_str.toInt();
    int blue_int = blue_str.toInt();

    /*
        Serial.println(btn_state_par);
        Serial.println(led_state_par);
        Serial.println(rgb_val_par);
        Serial.println(led_brt_par);
    */

    // Neopixels Code Starts Here
    ring.clear();
    ring.setBrightness(led_brt_int);
    ring.show();

    if (led_state_int == 1)
    {
        if (pattern_int == 1) // Solid pattern
        {
            ring.fill(ring.Color(red_int, green_int, blue_int));
            ring.show();
        }

        else if (pattern_int == 2) // Rainbow pattern
        {
            rainbow(16);
        }

        else if (pattern_int == 3) // Wheel pattern
        {
            rainbowCycle(16);
        }
    }

    else
    {
        ring.clear();
        ring.show();
    }

    delay(100);
    // colorWipe(ring.Color(red_int, green_int, blue_int), 5000);
    http.end(); // Close connection

    // delay(5000); // Post Data at every 5 seconds
}

//=======================================================================
//                    Neopixels Functions
//=======================================================================

// ColorWipe (Fills the dots one after the other with a color)
void colorWipe(uint32_t c, uint8_t wait)
{
    for (uint16_t i = 0; i < ring.numPixels(); i++)
    {
        ring.setPixelColor(i, c);
        ring.show();
        delay(wait);
    }
}

// Rainbow function
void rainbow(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256; j++)
    {
        for (i = 0; i < ring.numPixels(); i++)
        {
            ring.setPixelColor(i, Wheel((i + j) & 255));
        }
        ring.show();
        delay(wait);
    }
}

// Rainbow Cycle (Makes the rainbow equally distributed throughout)
void rainbowCycle(uint8_t wait)
{
    uint16_t i, j;

    for (j = 0; j < 256 * 5; j++)
    { // 5 cycles of all colors on wheel
        for (i = 0; i < ring.numPixels(); i++)
        {
            ring.setPixelColor(i, Wheel(((i * 256 / ring.numPixels()) + j) & 255));
        }
        ring.show();
        delay(wait);
    }
}

/*Wheel
Input a value 0 to 255 to get a color value.
The colours are a transition r - g - b - back to r.*/
uint32_t Wheel(byte WheelPos)
{
    if (WheelPos < 85)
    {
        return ring.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
    }
    else if (WheelPos < 170)
    {
        WheelPos -= 85;
        return ring.Color(255 - WheelPos * 3, 0, WheelPos * 3);
    }
    else
    {
        WheelPos -= 170;
        return ring.Color(0, WheelPos * 3, 255 - WheelPos * 3);
    }
}

/*
  Some example procedures showing how to display to the pixels:
  colorWipe(ring.Color(255, 0, 0), 50); // Red
  colorWipe(ring.Color(0, 255, 0), 50); // Green
  colorWipe(ring.Color(0, 0, 255), 50); // Blue
  rainbow(16);
  rainbowCycle(16);
*/