#pragma once
#include "../message.h"
#include <WS2tcpip.h>
#include <string>
#include <thread>
#pragma comment(lib, "ws2_32.lib")

class Client;

typedef void (*MessageReceivedHandler)(std::string msg);

class Client {
public:
  Client();
  ~Client();
  bool initSocket();
  void connectSock();
  /*void sendMsg(std::string txt);*/
  void sendMessage(Message message);
  std::thread receievedThread;
  void threadReceived();
  std::string username;
  bool joinChat = true;


private:
  SOCKET createSocket();
  std::string serverIP = "127.0.0.1";
  int serverPort = 20002;
  sockaddr_in hint;
  SOCKET serverSocket;
  bool receievedThreadRunning;
};