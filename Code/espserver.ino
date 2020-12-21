#include <ESP8266WiFi.h>
#include <SimpleDHT.h>
// Este código implementa un servidor en el ESP8266
// Al acceder a el por HTML se muestra:
// <!DOCTYPE html>
// <head>
//  <metacharset="UTF-8">
//  <title> Servidor Web ESP8266</title>
// </head>
// <html>
//  <body>
//    <h1>Bienvenido al Servidor Web de mi ESP8266</h1>
//  </body>
// </html>

// SSID de la puerta de enlace WiFi a la que se conectará: ssid y contraseña
const char* ssid="dragino-1c0ad8";
const char* pswd="dragino-dragino";

int temperature = 0;
int humidity = 0;
int pinDHT11 = 2;
SimpleDHT11 dht11;

//Dirección IP dentro de la red
//IPAddress ip(xxx,xxx,x,xxx);
//Dirección de la puerta de enlace
//IPAddress gateway(xxx,xxx,x,xxx)
//Máscara de red
//IPAddress subnet(255,255,255,0)

//Nota: En un servidor, si se usa DNS habitualmente es una IP fija. Por lo que hay que poner las lineas superiores
// En las prácticas, sin embargo como estamos haciendo una prueba, la IP se asigna por DHCP dinámico. Por lo que
// no hay que hay que comentar las 3 lineas de arriba para que el ESP8266 configure la IP

// Crea objeto server para ESP8266
WiFiServer servidorWeb(80); // crea el objeto tanto del servidor como el cliente
// Crea objeto client para ESP8266 (Aunque el ESP8266 sea servidor, cuando envía datos realiza las mismas operaciones que un cliente web)
WiFiClient clientWeb;

void setup() {
  Serial.begin(9600);
  // Inicio de la conexión WiFi
  Serial.print("Conectando a "+ String(ssid) + " ");
  // Configuración y conexión por WiFi a IP fija (si es dinámica hay que comentar la linea "WiFi.config")
  //WiFi.config(ip,gateway,subnet);
  WiFi.begin(ssid,pswd);
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");

  //Arranque del servidor web
  servidorWeb.begin();
  Serial.println("Servidor Web arrancado");
  
}

void loop() {

  byte temperature = 0;
  byte humidity = 0;
  byte data[40] = {0};
  dht11.read(pinDHT11, &temperature, &humidity, data);
  Serial.print((int)temperature); Serial.print(","); Serial.println((int)humidity);
   // DHT11 sampling rate is 1HZ.
  delay(2000);
  
  clientWeb=servidorWeb.available(); // Mira si se ha conectado un cliente al servidor

  if (clientWeb){
    // Este flag se usa para comprobar la integridad del mensaje que envía el cliente. En principio, si el mensaje es correcto, empieza por
    // un comando (GET, PUT, etc..) seguido de las cabeceras. Las cabeceras, continene información sobre el contenido que el cliente quiere
    // obtener. Y, cuando finalizan las cabeceras, se envía una "linea en blanco" (o comando '\n') que es lo que tiene que detectar el servidor
    boolean lineaEnBlanco = true; 
    // Mientras el cliente esté conectado, el servidor lee todos los datos que envía el cliente (carácter a carácter)
    while (clientWeb.connected())
    {
      if (clientWeb.available())
      {
        char c = clientWeb.read(); // lee carácter por carácter de todo el texto
        if (c == '\n' && lineaEnBlanco) // se utiliza para detectar la cabecera de la petición
        {
          clientWeb.println("HTTP/1.1 200 OK"); // código html para enviar a una página web
          clientWeb.println("Content-Type: text/html"); // código html para enviar a una página web
          clientWeb.println("Connection: close"); // código html para enviar a una página web
          clientWeb.println();
          clientWeb.println("<!DOCTYPE HTML>");
          clientWeb.println("<head>");
          clientWeb.println("<meta charset='UTF-8'>");
          clientWeb.println("<title>Servidor Web ESP8266</title>");
          clientWeb.println("</head>");
          clientWeb.println("<html>");
          clientWeb.println("<body>");
          clientWeb.println("<h1>Bienvenido al servidor Web de mi ESP8266</h1>");
          clientWeb.print("<h2>Temperatura: ");
          clientWeb.print(temperature);
          clientWeb.print("°C");
          clientWeb.println("<h2>");
          clientWeb.print("<h3>Humedad: ");
          clientWeb.print(humidity);
          clientWeb.print("%");
          clientWeb.println("<h3>");
          clientWeb.println("</body>");
          clientWeb.println("</html>");
          break;                        
        }
        if (c == '\n') lineaEnBlanco = true;
        else if (c != '\r') lineaEnBlanco = false;
      }
    }

    //Cierre de la conexión del servidor con el cliente para asegurar que se envían los datos
    delay(10);
    clientWeb.stop();
  }
 
}
