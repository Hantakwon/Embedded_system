#include "mbed.h"
#include "ESP8266.h"
#include "DHT.h"

Serial pc(USBTX,USBRX,115200);
 
//wifi UART port and baud rate
ESP8266 wifi(PA_9, PA_10, 9600); 

DHT dht22(PB_2, DHT22);
AnalogIn cds(A0);

//buffers for wifi library
char resp[1000];
char http_cmd[300], comm[300];
 
int timeout = 8000; //timeout for wifi commands
 
//SSID and password for connection
char SSID[] = "SILLA30"; 
char PASS[] = "silla1234";  
 
//Remote IP : thingspeak.com;
char IP[] = "184.106.153.149";
int port = 80;

char channelId[] = "2567999";
char writeApiKey[] = "7TR29DUSDWHFQPP7";
char readApiKey[] = "5LYRFWI5I0P14GPF";
	
float temperature, humidity, brightness;

void getBrightness(){
	brightness = 0;
	for (int i=0; i<10; i++) {
		brightness += cds.read();
	}
	brightness /= 10.0;
	brightness *= 100;
}

void getTempAndHumidity() {
	dht22.readData();
	temperature = dht22.ReadTemperature(CELCIUS);
	humidity = dht22.ReadHumidity();
}
	

//Wifi init function
void initializeWiFi(void){  
    pc.printf("******** Resetting wifi module ********\r\n");
    wifi.Reset();
    
    //wait for 5 seconds for response, else display no response receiveed
    if (wifi.RcvReply(resp, 5000))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response");
    
    pc.printf("******** Setting Station mode of wifi with AP ********\r\n");
    wifi.SetMode(1);    // set transparent  mode
    if (wifi.RcvReply(resp, timeout))    //receive a response from ESP
        pc.printf("%s",resp);    //Print the response onscreen
    else
        pc.printf("No response while setting mode. \r\n");
    
    pc.printf("******** Joining network with SSID and PASS ********\r\n");
    wifi.Join(SSID, PASS);     
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);   
    else
        pc.printf("No response while connecting to network \r\n");
        
    pc.printf("******** Getting IP and MAC of module ********\r\n");
    wifi.GetIP(resp);     
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while getting IP \r\n");
    
    pc.printf("******** Setting WIFI UART passthrough ********\r\n");
    wifi.setTransparent();          
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while setting wifi passthrough. \r\n");
    wait(1);    
    
    pc.printf("******** Setting single connection mode ********\r\n");
    wifi.SetSingle();             
    wifi.RcvReply(resp, timeout);
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while setting single connection \r\n");
    wait(1);
}

void updateThingspeakChannel(){
    
    pc.printf("******** Starting TCP connection on IP and port ********\r\n");
    wifi.startTCPConn(IP, port);    //cipstart
    wifi.RcvReply(resp, timeout);
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while starting TCP connection \r\n");
    wait(1);
    
    //create link 
    sprintf(http_cmd,"/update?api_key=%s&field1=%.2f&field2=%.1f&field3=%.1f", writeApiKey, temperature, humidity, brightness); 
    pc.printf("URI : %s\r\n", http_cmd);
		
    
    pc.printf("******** Sending URL to wifi ********\r\n");
    wifi.sendURL(http_cmd, comm);   //cipsend and get command
    if (wifi.RcvReply(resp, timeout))    
        pc.printf("%s",resp);    
    else
        pc.printf("No response while sending URL \r\n");
		
}
 
void setup(){
	wait(2.0);
	initializeWiFi();
}

int main()
{
	setup();
	
	while(1) {
		// update sensor data
		getBrightness();
		getTempAndHumidity();
		
		// sned sensor data to iot channel
		updateThingspeakChannel();
		
		wait(10.0);
	}
}