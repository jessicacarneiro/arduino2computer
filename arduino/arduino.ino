#include <string.h>

#include <SPI.h>
#include <WiFi.h>

#define BUFFER_SIZE 5

// variables for WiFi connection
char ssid[] = "ssid";
char pass[] = "pwd";
int status = WL_IDLE_STATUS;

IPAddress server_ip(192,168,1,146);
unsigned int server_port = 51515;
WiFiClient con;

char msg[BUFFER_SIZE];
char buffer[BUFFER_SIZE];

void printWifiData()
{
	// print your WiFi shield's IP address:
	IPAddress ip = WiFi.localIP();
	Serial.print("IP Address: ");
	Serial.println(ip);

	// print your MAC address:
	byte mac[6];  
	WiFi.macAddress(mac);
	Serial.print("MAC address: ");
	Serial.print(mac[5],HEX);
	Serial.print(":");
	Serial.print(mac[4],HEX);
	Serial.print(":");
	Serial.print(mac[3],HEX);
	Serial.print(":");
	Serial.print(mac[2],HEX);
	Serial.print(":");
	Serial.print(mac[1],HEX);
	Serial.print(":");
	Serial.println(mac[0],HEX);
}

bool connect(IPAddress addr, int port)
{
	if (con.connect(addr,port))
		return true;

	return false;
}

void rcv_msg(char *buffer, size_t len)
{
	memset(buffer,0,len);

	int i = 0;
	while (con.available())
		buffer[i++] = con.read();
        
	Serial.print("Received ");
  	Serial.print(i);
	Serial.println(" bytes from sender.");
}

void snd_msg(char *msg)
{
	int n = con.write(msg,BUFFER_SIZE);
	Serial.print("I wrote ");
	Serial.print(n);
	Serial.println(" bytes for the server to read.");
	delay(9900);
}

bool run_client(IPAddress addr, int port, char *buffer, char *msg, size_t buffer_sz)
{
	if (connect(addr,port)) {
		snd_msg(msg);
		rcv_msg(buffer,buffer_sz);
		delay(100);

		Serial.print("Response > ");
		Serial.println(buffer);
	}
	else {
		Serial.println("Failed to connect to server!");
		delay(1000);

		con.stop();
		return false;
	}

	con.stop();
	delay(1000);
	return true;
}

void setup()
{
	Serial.begin(115200);
	while (!Serial); // wait for serial port to connect

	// connect to WiFi network
	while (status != WL_CONNECTED) {
		Serial.print("Attempting to connect to WPA SSID: ");
		Serial.println(ssid);   
		status = WiFi.begin(ssid, pass);
		delay(1000);
	}

	// print connection details
	Serial.println("You're connected to the network");
	printWifiData();
}

void loop()
{
	memcpy(msg,"hello",5);
	run_client(server_ip,server_port,buffer,msg,BUFFER_SIZE);
}
