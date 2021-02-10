#include <WebSocketServer.h>
using namespace net;

#  define _SERIAL Serial

#if NETWORK_CONTROLLER == NETWORK_CONTROLLER_WIFI
constexpr char kSSID[]{ "SKYNET" };
constexpr char kPassword[]{ "***" };
#else
byte mac[]{ 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEC };
IPAddress ip(192, 168, 100, 111);
#endif

constexpr uint16_t port = 3000;
WebSocketServer wss(port);

uint32_t previousTime;

//-----------------------------------------------------------------------------
void wsOnMessageCallback(WebSocket &ws,
						const WebSocket::DataType &dataType,
						const char *message,
						uint16_t length)
{
	switch (dataType) {
	case WebSocket::DataType::TEXT:
		_SERIAL.print(F("Received: "));
		_SERIAL.println(message);
		break;
	case WebSocket::DataType::BINARY:
		_SERIAL.println(F("Received binary data"));
		break;
	}

	ws.send(dataType, message, length);

	previousTime = millis(); // reset heartbit timer
}
//-----------------------------------------------------------------------------
void wsOnCloseCallback(WebSocket &ws,
					const WebSocket::CloseCode &code,
					const char *reason,
					uint16_t length)
{
	_SERIAL.print("Disconnected, reason: ");
	_SERIAL.println(reason);
}
//-----------------------------------------------------------------------------
void wssOnConnectionCallback(WebSocket &ws)
{
    _SERIAL.print(F("New client: "));
    _SERIAL.println(ws.getRemoteIP());

	ws.onMessage(wsOnMessageCallback);
	ws.onClose(wsOnCloseCallback);

    const char message[]{ "Hello from Arduino server!" };
    ws.send(WebSocket::DataType::TEXT, message, strlen(message));

	previousTime = millis(); // reset heartbit timer
}
//-----------------------------------------------------------------------------
void setup()
{
  _SERIAL.begin(115200);
  while (!_SERIAL); delay(10);

#if NETWORK_CONTROLLER == NETWORK_CONTROLLER_WIFI
  //_SERIAL.setDebugOutput(true);
  _SERIAL.printf("\nConnecting to %s ", kSSID);

  WiFi.mode(WIFI_STA);
  WiFi.begin(kSSID, kPassword);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    _SERIAL.print(F("."));
  }

  _SERIAL.println(F(" connected"));

  WiFi.printDiag(_SERIAL);

  _SERIAL.print(F("Device IP: "));
  _SERIAL.println(WiFi.localIP());
#else
  _SERIAL.println("Initializing ... ");

 #if NETWORK_CONTROLLER == ETHERNET_CONTROLLER_W5X00
  // Ethernet.init(PB12); // needed for CS pin other than PA4 
 #endif

  Ethernet.begin(mac, ip);

  _SERIAL.print(F("Server running at "));
  _SERIAL.print(Ethernet.localIP());
  _SERIAL.print(F(":"));
  _SERIAL.println(port);
#endif

  wss.onConnection(wssOnConnectionCallback);

  wss.begin();
}

#define TIME_INTERVAL 3000
const char hb[] = "-heart-bit-";
#define MAX_LEN 128
char in_str[MAX_LEN];
//-----------------------------------------------------------------------------
void loop()
{
	wss.listen();

	if ( ! wss.countClients() ) return;

	int cnt;
	if ( (cnt = _SERIAL.available())>0 )
	{
		while (cnt>=MAX_LEN)
		{
			_SERIAL.read(in_str, MAX_LEN);
			wss.broadcast(WebSocket::DataType::TEXT, in_str, MAX_LEN);
			cnt -= MAX_LEN;
		}
		// send here the remaining bytes if any
		if ( cnt>0 )
			wss.broadcast(WebSocket::DataType::TEXT, in_str, cnt);
	}
	// send periodical heartbit message if any client is connected
	if ( (millis()-previousTime)>=TIME_INTERVAL )
	{
		previousTime = millis();
		wss.broadcast(WebSocket::DataType::TEXT, hb, sizeof(hb)-1);
		_SERIAL.println(hb);
	}
		
}