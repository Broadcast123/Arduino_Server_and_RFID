//RST           D9 //
//SDA(SS)      D10
//MOSI         D11
//MISO         D12
//SCK          D13

 
#include <SPI.h>  //Importamos librería comunicación SPI
#include "DHT.h"
//#include "DHT.h"
//#include <SPI.h>
//include <MySQL_Connection.h>
//include <MySQL_Cursor.h>
#include <MFRC522.h>
#include <Ethernet.h>  //Importamos librería Ethernet

const int RST_PIN = 9;            // Pin 9 para el reset del RC522
const int SS_PIN = 7;            // Pin 10 para el SS (SDA) del RC522, se cambió a 9, pero después a 7 para hacer el Pull Up, ya qué generaba confusión con Shield.
MFRC522 mfrc522(SS_PIN, RST_PIN);   // Crear instancia del MFRC522 
 
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };//Ponemos la dirección MAC de la Ethernet Shield que está con una etiqueta debajo la placa
//IPAddress ip(192,168,1,72 ); //Ip de modem , se dispone a ponerla de manera manual, se puede visualizar en terminal con ipconfig /all
IPAddress ip(192,168,1,90 ); //Asingamos la IP al Arduino
EthernetServer server(80); //Creamos un servidor Web con el puerto 80 que es el puerto HTTP por defecto

int led=6; //Pin del led
String estado="ON"; //Estado del Led inicialmente "ON"
byte *card[10];

//IPAddress ip_local(192,168,1,50 ); //ip local asignada a Router Hub.
IPAddress ip_local(157,230,53,247 ); //ip para servidor virtual digital ocean.

String id_tarjeta;
String classroomData = "A-208"; //variable para el aula de clase

void setup()
{
  Serial.begin(9600);
 
  // Inicializamos la comunicación Ethernet y el servidor
  Ethernet.begin(mac, ip);
  server.begin();
  Serial.print(" El servidor está en: ");
  Serial.println(Ethernet.localIP());
  //Serial.println(F("Card UID:")); //Para imprimir la card
 
  pinMode(led,OUTPUT);

   Serial.begin(9600);      //Inicializa la velocidad de Serial
   SPI.begin();         //Función que inicializa SPI
   mfrc522.PCD_Init();     //Función  que inicializa RFID
}

//
void printArray(byte *buffer, byte bufferSize) {
   for (byte i = 0; i < bufferSize; i++) {
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      //card[i]= buffer[i] < 0x10 ? " 0" : " ";
      Serial.print(buffer[i], HEX);
     // card[i]= buffer[i],HEX;
      
      
      
   }
}

//Convertir a String para visualizar en Interfaz de Servidor Arduino Ethernet
String dump_byte_array(byte *buffer, byte bufferSize) {
      String out = "";
    for (byte i = 0; i < bufferSize; i++) {
        //Serial.print(buffer[i] < 0x10 ? " 0" : " ");
        //Serial.print(buffer[i], HEX);
        out += String(buffer[i] < 0x10 ? " 0" : " ") + String(buffer[i], HEX);
    }
    out.toUpperCase();
    out.replace(" ", "");
    return out;
}

void loop()
{
    EthernetClient client = server.available(); //Creamos un cliente Web

  // Detectar tarjeta
   if (mfrc522.PICC_IsNewCardPresent())
   {
      if (mfrc522.PICC_ReadCardSerial())
      {
         Serial.print(F("Card UID:"));
         printArray(mfrc522.uid.uidByte, mfrc522.uid.size);
         Serial.println();
         Serial.println("Copia");
         //Se obtiene el identificador de la tarjeta, y se almacena en una variable de tipo String
         id_tarjeta = dump_byte_array(mfrc522.uid.uidByte, mfrc522.uid.size);
         Serial.println(id_tarjeta);
         //Comienza codigo de prueba para conectar a mysql
           if (client.connect(ip_local, 80)){
              Serial.println("connected");
              // Make a HTTP request:
              //Serial.print("GET /WebServer_and_RFID/dht.php?identifier=");
              //client.print("GET /WebServer_and_RFID/dht.php?identifier=");     //YOUR URL //URL que manda a la carpeta en localhost
              Serial.print("GET http://157.230.53.247/arduino/dht.php?identifier=");
              client.print("GET http://157.230.53.247/arduino/dht.php?identifier=");     //YOUR URL //URL que manda a servidor virtal web
              
              Serial.println(id_tarjeta);
              client.print(id_tarjeta);
              
              client.print("&classroom="); //se conecta con el parametro de php
              Serial.println("&classroom=");
              client.print(classroomData); //Para imprimir el aula
              Serial.println(classroomData);

              digitalWrite(led,HIGH);
              delay(2000); //Para que el foco LED prenda durante dos segundos al hacer la lectura de código RFID
              digitalWrite(led, LOW); 
              
              client.print(" ");      //SPACE BEFORE HTTP/1.1
              client.print("HTTP/1.1");
              client.println();
              client.println("Host: 192.168.1.72");
              client.println("Connection: close");
              client.println();
            } else {
              // if you didn't get a connection to the server:
            }

         //Termina codigo de prueba de conexion a mysql
        /*
         for(byte i = 0; i < mfrc522.uid.size; i++){
            Serial.println(card[i], HEX);
         }
       */  
         //Card[1] = (mfrc522.uid.uidByte, mfrc522.uid.size); 
         //Serial.println(Card[1]);
         // Finalizar lectura actual
         mfrc522.PICC_HaltA();
      }
   }
  //Cuando detecte un cliente a través de una petición HTTP
  if (client) {
    Serial.println("new client");
    boolean currentLineIsBlank = true; //Una petición HTTP acaba con una línea en blanco
    String cadena=""; //Creamos una cadena de caracteres vacía
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();//Leemos la petición HTTP carácter por carácter
        Serial.write(c);//Visualizamos la petición HTTP por el Monitor Serial
        cadena.concat(c);//Unimos el String 'cadena' con la petición HTTP (c). De esta manera convertimos la petición HTTP a un String
 
         //Ya que hemos convertido la petición HTTP a una cadena de caracteres, ahora podremos buscar partes del texto.
         int posicion=cadena.indexOf("LED="); //Guardamos la posición de la instancia "LED=" a la variable 'posicion'
 
          if(cadena.substring(posicion)=="LED=ON")//Si a la posición 'posicion' hay "LED=ON"
          {
            //digitalWrite(led,HIGH);
            //estado="ON";
          }
 
          if(cadena.substring(posicion)=="LED=OFF")//Si a la posición 'posicion' hay "LED=OFF"
          {
            //digitalWrite(led,LOW);
            //estado="OFF";
          }
          
        //Cuando reciba una línea en blanco, quiere decir que la petición HTTP ha acabado y el servidor Web está listo para enviar una respuesta
        if (c == '\n' && currentLineIsBlank) {
 
            // Enviamos al cliente una respuesta HTTP
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println();
 
            //Página web en formato HTML
            client.println("<html>");
            client.println("<head>");
            client.println("</head>");
            client.println("<body>");
            client.println("<h1 align='center'>Interfaz Web Server</h1><h3 align='center'>LED controlado por Servidor Web con Arduino</h3>");
            
            //Creamos los botones. Para enviar parametros a través de HTML se utiliza el metodo URL encode. Los parámetros se envian a través del símbolo '?'
            client.println("<div style='text-align:center;'>");
            client.println("<button onClick=location.href='./?LED=ON\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("ON");
            client.println("</button>");
            //client.println (printArray(mfrc522.uid.uidByte, mfrc522.uid.size);//IMPRIMIR EL CODOGO DE LA TARJETA
            client.println("<button onClick=location.href='./?LED=OFF\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            client.println("OFF");
            //client.println("<div style='text-align:center;'>");
            //client.println("<button onClick=location.href='./?TAG=ON\' style='margin:auto;background-color: #84B1FF;color: snow;padding: 10px;border: 1px solid #3F7CFF;width:65px;'>");
            //client.println("ON");
            client.println("</button>");
            client.println("<br /><br />");
            client.println("<b>LED = ");
            client.print(estado);
            client.println("</b><br />");
            
            //Serial.print("GET /192.168.1.90/dht.php?codigo="'./?id_tarjeta\');
            
            //Para imprimir el Aula del alumno en el navegador
            client.println("<b>Aula = ");
            client.println(classroomData);
            
            client.println("</b><br />");
            client.println("</b><br />");
            
            //Para imprimir el id de la tarjeta en el navegador
            client.println("<b>UID Tarjeta = ");
            client.println(id_tarjeta);
            
            client.println("</b></body>");
            client.println("</html>");
            break;
        }
        if (c == '\n') {
          currentLineIsBlank = true;
        }
        else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    
    //Dar tiempo al navegador para recibir los datos
    delay(1);
    client.stop();// Cierra la conexión
  }
   delay(250);
}
