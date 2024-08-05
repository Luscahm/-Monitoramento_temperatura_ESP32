// Temperature Sensor - Groove
// Adapted for ESP32 using Arduino IDE

#include <math.h>
#include <WiFi.h>
const char* ssid = "Kayawifi";
const char* password = "ecwn3884";
WiFiServer server(80);

const int B = 4275;               // B value of the thermistor
const int R0 = 100000;            // R0 = 100k
const int pinTempSensor = 15;     // Grove - Temperature Sensor connected to GPIO34 (ESP32)
float temperature = 0;
#define LED 4 //Pino D4 ESP32



void setup()
{
    pinMode(LED, OUTPUT);

    Serial.begin(115200);         // Use a higher baud rate for ESP32
    
    for(int i =0; i<30;i++){
      int ADC = analogRead(pinTempSensor);
      float R = 4095.0 / ADC - 1.0;
      Serial.println("Lendo temperatura");
      R = R0 * R;
      temperature += 1.0 / (log(R / R0) / B + 1 / 298.15) - 273.15;
      delay(1000);                   // Delay of 1 second

    }
    
    temperature /= 30;
  Serial.println(temperature);
  Serial.println();
  Serial.print("Conectando-se a ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi conectada.");
  Serial.println("Endereço de IP: ");
  Serial.println(WiFi.localIP());

  server.begin();
}

void loop()
{
  if(temperature> 20){
    digitalWrite(LED, HIGH);
  }

  else{
    digitalWrite(LED, LOW);
  }

  WiFiClient client = server.available();
    if (client) {
    Serial.println("New Client.");
    String currentLine = "";
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        Serial.write(c);
        if (c == '\n') {
          if (currentLine.length() == 0) {
            client.println("HTTP/1.1 200 OK");
            client.println("Content-type:text/html");
            client.println();
            client.println("<h1>Sistemas Computacionais embarcado - ESP32 wifi</h1>");
            client.print("<p>Temperatura atual: ");
            client.print(temperature);
            client.println(" &deg;C</p>");

            client.println();
            break;
          } else {
            currentLine = "";
          }
        } else if (c != '\r') {
          currentLine += c;
        }
      }
    }
    client.stop();
    Serial.println("Client Disconnected.");
  }


}
    
