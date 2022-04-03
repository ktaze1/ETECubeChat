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

void Client::connectSocket()
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
    // Encrypt Message
    send(serverSocket, concatMessage.c_str(), concatMessage.size() + 1, 0);
  }
}


/*
* Encrypt Message before sending to server using Server's public key
*/
//void Client::encryptMessage(Message message) {
    //  unsigned char client_pk[crypto_kx_PUBLICKEYBYTES], client_sk[crypto_kx_SECRETKEYBYTES];
    //  unsigned char client_rx[crypto_kx_SESSIONKEYBYTES], client_tx[crypto_kx_SESSIONKEYBYTES];
    //
    //  /* Generate the client's key pair */
    //  crypto_kx_keypair(client_pk, client_sk);
    //
    //  /* Prerequisite after this point: the server's public key must be known by the client */
    //
    //  /* Compute two shared keys using the server's public key and the client's secret key.
    //     client_rx will be used by the client to receive data from the server,
    //     client_tx will be used by the client to send data to the server. */
    //  if (crypto_kx_client_session_keys(client_rx, client_tx, client_pk, client_sk, server_pk) != 0) {
    //    /* Suspicious server public key, bail out */
    //  }
//}