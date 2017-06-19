/*
 * Aula Scada Br
 * 
 * Baseado nos exemplos de  Modbus-Arduino Example
 * http://github.com/andresarmento/modbus-arduino
 * Modbus-Arduino Example - Test Led (Modbus IP ESP8266)
*  Modbus-Arduino Example - Test Holding Register (Modbus IP ESP8266)
*   Autor: João Campos
 * 
*/

#include <ESP8266WiFi.h>
#include <Modbus.h>
#include <ModbusIP_ESP8266.h>

//Modbus Registers Offsets (0-9999)
const int SENSOR_IREG = 100;

//Modbus Registers Offsets (0-9999)
const int LED_COIL = 100;
//Used Pins
const int ledPin = 0; //GPIO0

//Objeto ModbusIP 
ModbusIP mb;
/*
 * Temporizador usado para executar as tarefas a cada tempo predefinido sem usar a função delay
 * 
 */
long temporizador;
/**
 * 
 * Poll em ms em que o ESP8266 atualiza o registro Analógico
 */
int poll;

void setup() {
  // Iniciar ligação Serial
    Serial.begin(115200);
 
    // Configurando a ligação à rede
    // Coloque as credenciais da rede wifi 
    mb.config("MS Net", "ianvictor2007");
  
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
    }
 
    Serial.println("");
    Serial.println("WiFi connected");  
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());

    // Adiciona SENSOR_IREG register - Usar addIreg() para entradas analógicas
    // O ESP8266 só tem um sensor analógico no A0
    mb.addIreg(SENSOR_IREG);
    /*
     * Adcicionar Led na saída 0 do NodeMcu
     * A Bobina simboliza um atuador no Modbus
     * 
     */
     pinMode(ledPin, OUTPUT);
     mb.addCoil(LED_COIL);
  
    temporizador = millis();
}

void loop() {
   // Chamar rotina de Modbus Task que executa a tarefa de tratar os dados recebidos do master
   // Chamar esta rotina em cada Loop. No modo Modbus Ip, esta rotina recebe o pacote TCP na porta 503 e trata de 
   // acordo com o protocolo Modbus
   mb.task();

    //Lê o estado atual da bobina do registro
   digitalWrite(ledPin, mb.Coil(LED_COIL));

   // Atualiza o valor do sensor a cada 2 s
   // Nota: Configurar o scadaBr a realizar poll a cada 2 s para obter o valor atualizado
   if (millis() > temporizador + 2000) {
      temporizador = millis();
       //Setting raw value (0-1024)
       mb.Ireg(SENSOR_IREG, analogRead(A0));
   }
}
