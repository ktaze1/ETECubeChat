#pragma once

#include <WS2tcpip.h>
#include <string>
#include <map>
#include "message.h"
#pragma comment(lib, "ws2_32.lib")

class Server;

// Callback fct = fct with fct as parameter.
typedef void (*MessageReceivedHandler)(Server *listener, uint16_t socketID, std::string msg);

class Server {
public:
  Server();
  Server(std::string ipAddress, uint16_t port);
  ~Server();

  Message tokenizeBuffer(char*);
  bool initSocket();
  void run();
  void cleanupWinsock();


private:
  SOCKET createSocket();
  std::string listenerIPAddress;
  uint16_t listenerPort;
  std::multimap<SOCKET, Message> messageList;
  // MessageReceivedHandler messageReceived;
};