#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

const char* ssid = "";   // Replace with your WiFi credentials
const char* password = "";

ESP8266WebServer server(80);  // Webserver on port 80
#define MOTOR_PIN D5  // Motor control pin (GPIO14)

bool motorState = false;  // Motor state variable

void setup() {
    pinMode(MOTOR_PIN, OUTPUT);
    digitalWrite(MOTOR_PIN, LOW); // Motor initially off
    Serial.begin(115200);

    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }
    Serial.println("WiFi Connected!");
    Serial.println(WiFi.localIP()); // Print IP Address

    server.on("/", HTTP_GET, []() {
        String html = "<html><body style='text-align:center'>";
        html += "<h2>Motor Control</h2>";
        html += "<p>Motor Status: " + String(motorState ? "ON" : "OFF") + "</p>";
        html += "<a href='/start'><button style='padding:10px 20px;'>Start</button></a>";
        html += "<a href='/stop'><button style='padding:10px 20px;'>Stop</button></a>";
        html += "</body></html>";
        server.send(200, "text/html", html);
    });

    server.on("/start", HTTP_GET, []() {
        motorState = true;
        digitalWrite(MOTOR_PIN, HIGH);
        server.sendHeader("Location", "/", true);
        server.send(302, "text/plain", "");
    });

    server.on("/stop", HTTP_GET, []() {
        motorState = false;
        digitalWrite(MOTOR_PIN, LOW);
        server.sendHeader("Location", "/", true);
        server.send(302, "text/plain", "");
    });

    server.begin();
}

void loop() {
    server.handleClient();
}
