#include "client.h"
#include <iostream>
#include <string>
#include <thread>

Client::Client() { receievedThreadRunning = false; }


Client::~Client()
{
  closesocket(serverSocket);
  WSACleanup();
  if (receievedThreadRunning) {
    receievedThreadRunning = false;
    receievedThread.join();// Destroy safely to thread.
  }
}


bool Client::initSocket()
{
  WSADATA data;
  WORD ver = MAKEWORD(2, 2);
  int wsResult = WSAStartup(ver, &data);
  if (wsResult != 0) {
    std::cout << "Error: can't start Socket. \n";
    return false;
  }
  return true;
}

SOCKET Client::createSocket()
{

  SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
  if (sock == INVALID_SOCKET) {
    std::cout << "Error: can't create socket. \n";
    WSACleanup();
    return -1;
  }

  hint.sin_family = AF_INET;
  hint.sin_port = htons(serverPort);
  inet_pton(AF_INET, serverIP.c_str(), &hint.sin_addr);

  return sock;
}

void Client::threadReceived()
{

  receievedThreadRunning = true;
  while (receievedThreadRunning) {

    char buffer[4096];
    ZeroMemory(buffer, 4096);

    int bytesReceived = recv(serverSocket, buffer, 4096, 0);

    if (bytesReceived > 0) std::cout << std::string(buffer, 0, bytesReceived) << std::endl;
  }
}

void Client::connectSock()
{
  serverSocket = createSocket();

  int connResult = connect(serverSocket, (sockaddr*)&hint, sizeof(hint));
  if (connResult == SOCKET_ERROR) {
    std::cout << "Error: can't connect to server.\n";
    closesocket(serverSocket);
    WSACleanup();
    return;
  }
}

void Client::sendMessage(Message message)
{
  if (!message.textMessage.empty()) {
    std::string concatMessage = message.messageTime + " -- " + message.textMessage + '\n';
    send(serverSocket, concatMessage.c_str(), concatMessage.size() + 1, 0);
  }
}