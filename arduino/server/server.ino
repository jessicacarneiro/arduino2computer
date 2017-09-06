#include <string.h>

#include <SPI.h>
#include <WiFi.h>

#define BUFFER_SIZE 5

// variables for WiFi connection
char ssid[] = "ssid";
char pass[] = "pwd";
int status = WL_IDLE_STATUS;

WiFiClient con;
WiFiServer server(51515);

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

void rcv_msg(char *buffer, size_t len)
{
	memset(buffer,0,len);

	int i = 0;
	while (con.available())
		buffer[i++] = con.read();
        
	Serial.print("Received ");
  	Serial.print(i);
	Serial.println(" bytes from client.");
}

void snd_msg(char *msg)
{
	int n = con.write(msg,BUFFER_SIZE);
	Serial.print("I wrote ");
	Serial.print(n);
	Serial.println(" bytes for the client to read.");
	delay(9900);
}

bool run_server(char *buffer, size_t len_buf)
{
  con = server.available();

  if (con) {
    Serial.println("New incoming connection.");

    rcv_msg(buffer,len_buf);
    Serial.print("> ");
    Serial.println(buffer);

    snd_msg("ack");

    delay(1000);
    con.stop();

    return true;
  }

  delay(1000);
  return false;
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

	server.begin();
}

void loop()
{
	Serial.println("Waiting for client.");
	run_server(buffer,BUFFER_SIZE);
}
