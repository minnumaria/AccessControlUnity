#include "mbed.h"
#include "DHT.h"
#include "MQTTEthernet.h"
#include "MQTTClient.h"
#define ECHO_SERVER_PORT   7
 
DigitalOut buzz(D5);
DigitalOut nyled(D4);
DigitalOut myled(D3);
int arrivedcount = 0;

void messageArrived(MQTT::MessageData& md)
{
    MQTT::Message &message = md.message;
    char result[10] = "\0";
    sprintf(result, "%.*s", message.payloadlen,(char*)message.payload);     
    if(strcmp(result,"cdi123") == 0)
    {
           nyled=1;
           wait(1);
           nyled=0;
        printf("LED blink \n");             
    }
      if(strcmp(result,"cdi123")!= 0)
    {
           buzz=1;
           wait(1);
           buzz=0;
        printf("buzz\n");             
    }
    printf("Message arrived: qos %d, retained %d, dup %d, packetid %d\n", message.qos, message.retained, message.dup, message.id);
    printf("Payload %.*s\n", message.payloadlen, (char*)message.payload);
    ++arrivedcount;
}
void baud(int baudrate) {
    Serial s(USBTX, USBRX);
    s.baud(baudrate);
}

int main (void)
{
     while(1)
    {
    
        
    baud(115200);
    printf("Wait a second...\r\n");
    char* topic = "Minnu";
    MQTTEthernet ipstack = MQTTEthernet();
   
    MQTT::Client<MQTTEthernet, Countdown> client = MQTT::Client<MQTTEthernet, Countdown>(ipstack);
    
    char* hostname = "172.16.73.4";
    int port = 1883;
    
    int rc = ipstack.connect(hostname, port);
    if (rc != 0)
        printf("rc from TCP connect is %d\n", rc);
        
    printf("Topic: %s\r\n",topic);
    
    MQTTPacket_connectData data = MQTTPacket_connectData_initializer;       
    data.MQTTVersion = 3;
    data.clientID.cstring = "parents";

    if ((rc = client.connect(data)) != 0)
        printf("rc from MQTT connect is %d\n", rc);
 
    if ((rc = client.subscribe(topic, MQTT::QOS1, messageArrived)) != 0)
        printf("rc from MQTT subscribe is %d\n", rc); 
    client.yield(60000);
    }
}

