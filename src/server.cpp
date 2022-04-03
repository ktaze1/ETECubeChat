#include "server.h"
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

const int MAX_BUFFER_SIZE = 4096;

Server::Server() {}

Server::Server(std::string ipAddress, uint16_t port) : listenerIPAddress(ipAddress), listenerPort(port) {}

Server::~Server()
{
  cleanupWinsock();
}

bool Server::initSocket()
{
  WSADATA data;
  WORD ver = MAKEWORD(2, 2);

  int wsInit = WSAStartup(ver, &data);
  if (wsInit != 0) {
    std::cout << "Error: can't initialize the socket. \n";
    return false;
  }
  return true;
}

SOCKET Server::createSocket()
{

  SOCKET listeningSocket = socket(AF_INET, SOCK_STREAM, 0);

  if (listeningSocket != INVALID_SOCKET) {

    sockaddr_in hint;// Structure used to bind IP address & port to specific socket.
    hint.sin_family = AF_INET;
    hint.sin_port = htons(listenerPort);
    inet_pton(AF_INET,
      listenerIPAddress.c_str(),
      &hint.sin_addr);

    int bindCheck = bind(listeningSocket,(sockaddr *)&hint,sizeof(hint));

    if (bindCheck != SOCKET_ERROR) {
      int listenCheck = listen(listeningSocket, SOMAXCONN);// Tell the socket is for listening.
      if (listenCheck == SOCKET_ERROR) { return -1; }
    }

    else {
      return -1;
    }

    return listeningSocket;
  }
}


void Server::run()
{

  char buf[MAX_BUFFER_SIZE];
  SOCKET listeningSocket = createSocket();

  while (true) {

    if (listeningSocket == INVALID_SOCKET) { break; }

    fd_set master;
    FD_ZERO(&master);

    FD_SET(listeningSocket, &master);

    while (true) {
      fd_set copy = master;// Create new file descriptor bc the file descriptor gets destroyed every time.
      int socketCount = select(0,
        &copy,
        nullptr,
        nullptr,
        nullptr);// Select() determines status of sockets & returns the sockets doing "work".

      for (int i = 0; i < socketCount; i++) {

        SOCKET sock = copy.fd_array[i];

        if (sock == listeningSocket) { // accept new connection.

          SOCKET client =
            accept(listeningSocket, nullptr, nullptr);
          FD_SET(client, &master);// Add new connection to list of sockets.
          std::string connSuccessfulMessage = "You have successfully joined the server!\n";
          send(client, connSuccessfulMessage.c_str(), connSuccessfulMessage.size() + 1, 0);
          std::cout << "New user joined the chat. \n";

        } else { // message received from a client

          ZeroMemory(buf, MAX_BUFFER_SIZE);
          int bytesReceived = recv(sock, buf, MAX_BUFFER_SIZE, 0);

          if (bytesReceived <= 0) { // No message
            closesocket(sock);
            FD_CLR(sock, &master);
          } else {
            for (int i = 0; i < master.fd_count; i++) {
              SOCKET outSock = master.fd_array[i];

              if (outSock != listeningSocket) {

                if (outSock == sock) {
                  //std::string msgSent = "Message delivered.";
                  //send(outSock, msgSent.c_str(), msgSent.size() + 1, 0);// Notify the client that the msg was delivered.
                } else {
                  send(outSock, buf, bytesReceived, 0);// Send the message to the current socket.
                }
              }
            }
            
            messageList.insert(std::pair<SOCKET, Message>(sock,tokenizeBuffer(buf))); // TODO: log this multimap to a file instead of
                                                                                      // allocating memory from RAM
            std::cout << std::string(buf, 0, bytesReceived); // Log the message
          }
        }
      }
    }
  }
}

void Server::cleanupWinsock() { WSACleanup(); }

Message Server::tokenizeBuffer(char* buffer) {
  Message message;
  std::string s = std::string(buffer);
  const std::string delimiter = " -- ";
  message.messageTime = s.substr(0, s.find(delimiter)) + "\n";
  message.textMessage = s.substr(s.find(delimiter)+ 3, s.size()) + "\n";

  return message;
}