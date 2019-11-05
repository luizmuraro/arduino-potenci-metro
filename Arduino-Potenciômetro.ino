#include <ESP8266WiFi.h>
#define potPin A0   // Define o pino analógico em que o potenciômetro vai ser conectado
#define ledPin D3   // Define o pino que terá um LED e um resistência ligada ao terra

int valPot = 0; //Variável que armazena o valor da leitura do potenciômetro
const char* ssid = "AndroidAP";
const char* password = "123456789";
 
WiFiServer server(80);
 
void setup() {

  pinMode(ledPin,OUTPUT); // Configura o pino do LED como saída

  Serial.begin(115200);
  delay(10);
 
 
  digitalWrite(ledPin, LOW);
 
  // Connect to WiFi network
  Serial.println();
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  // Start the server
  server.begin();
  Serial.println("Server started");
 
  // Print the IP address
  Serial.print("Use this URL : ");
  Serial.print("http://");
  Serial.print(WiFi.localIP());
  Serial.println("/");
 
}
 
void loop() {

  Serial.println(valPot);
  valPot =  analogRead(A0); //Faz a leitura analógica do pino em que o potenciômetro esta ligado
  valPot = map(valPot,0,1023,0,255); //Utilizando a função map() para transformar uma escala de 0-1023 em uma escala 0 a 255
  analogWrite(ledPin,valPot ); // Aciona o LED proporcionalmente ao valor da leitura analógica
  delay(500);
  render();
}

void render() {
  // Check if a client has connected
  WiFiClient client = server.available();
  if (!client) {
    return;
  }
   

 
  // Wait until the client sends some data
  Serial.println("new client");
  while(!client.available()){
    delay(1);
  }

  // Read the first line of the request
  String request = client.readStringUntil('\r');
  Serial.println(request);
 while (client.available()) {

  client.read();
 }

  // Match the request
  
  // Return the response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); //  do not forget this one
  client.println("<!DOCTYPE HTML>");
  client.println("<html>");
  client.print("<HEAD>");
  client.print("<meta http-equiv=\"refresh\" content=\"1\">");
  client.print("<meta charset=\"UTF-8\">");
  client.print("</head>");
    client.print("<body>");
        client.println("<h1>Trabalho DSOII</h1>");
        client.println("<h2>Lucas P. Feliciano e Luiz G. Muraro<h2/>");
  client.println("<br><br>");
  client.print("Intensidade do led: ");
  client.println(valPot);
  client.println("<br><br>");
  client.println((valPot == 255) ? F("Intensidade máxima!!!!") : F(""));
  client.println((valPot == 0) ? F("Intensidade mínima!!!!") : F(""));
    client.print("</body>");

  client.println("</html>");
 
  Serial.println("Client disconnected");
  Serial.println("");


 
}
