#include <ESP8266WiFi.h>
// Este código de ESP8266 se conecta a www.vermiip.es y muestra la IP por el terminal
// SSID de la red WiFi a la que se conectará: ssig y contraseña
const char* ssid="*********";
const char* pswd="*********";

//Dirección web del servidor al que accederá
const char* servidorWeb="www.vermiip.es";

void setup() {
  Serial.begin(9600);
  // Inicio de la conexión WiFi
  Serial.print("Conectando a "+ String(ssid) + " ");
  WiFi.begin(ssid,pswd);
  while (WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Conectado");
}

void loop() {

  // Crea objeto clienteweb para ESP8266
  WiFiClient clientWeb;

  // Establece conexión con servidor

  if (clientWeb.connect(servidorWeb,80))
  {
    // Petición de la página HTML
    clientWeb.print(String("GET /") + "HTTP/1.1\n" + 
                "Host: " + servidorWeb + "\n" + 
                "Connection: close\r\n" + "Connection: close\n\n");
  
    // Lee la respuesta del servidor linea a linea
    while (clientWeb.connected())
    {
      if (clientWeb.available())
      {
        String linea = clientWeb.readStringUntil('\n');
        if(linea.indexOf("<h2>Tu IP p&uacute;blica es: ") != -1) 
        Serial.println("Su IP pública es: " +
            linea.substring(linea.indexOf("<h2>Tu IP p&uacute;blica es: ")+28, //Ojo: Puede que el número correcto no sea 28.
            //En ese caso, habrá que determinar por prueba y error en qué índice de la línea empieza el texto con la IP
                           (linea.indexOf("</h2>"))));
    }
  }

  // Fin de la conexión con el servidor
  clientWeb.stop();
  delay(10000);
  
  }
  else Serial.println("Problemas al conectar con el servidor");
  delay(10000);
}
