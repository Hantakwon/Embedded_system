#ifndef ESP8266_H
#define ESP8266_H

#include <string>
#include "mbed.h"

class ESP8266
{
public:
/**
  * ESP8266 constructor
  *
  * @param tx TX pin
  * @param rx RX pin
  * @param br Baud Rate
  */
  ESP8266(PinName tx, PinName rx, int br);

  /**
  * ESP8266 destructor
  */
  ~ESP8266();

	void SendCMD(char * s);
	void Reset(void);
	bool RcvReply(char * r, int to);
	void GetList(char * l);
	void Join(char * id, char * pwd);
	void GetIP(char * ip);
	void SetMode(char mode);
	void Quit(void);
	void SetSingle(void);
	void SetMultiple(void);
	void GetConnStatus(char * st);
	void StartServerMode(int port);
	void CloseServerMode(void);
	void setTransparent(void);
	void startTCPConn(char * IP, int port);
	void sendURL(char *URL, char *command);

private:
	Serial comm;
	void AddEOL(char * s);
	void AddChar(char * s, char c);
	void itoa(int c, char s[]);
};
  
#endif