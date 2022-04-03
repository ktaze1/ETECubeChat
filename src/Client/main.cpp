#include "client.h"
#include <WS2tcpip.h>
#include <iostream>
#include <sstream>
#include <string>
#pragma comment(lib, "ws2_32.lib")

int main()
{

  Client *client = new Client;
  std::string message{ "a" };
  std::string username;


  std::cout << "Enter your username.\n";
  std::cin >> username;
  client->username = username;

  if (client->initSocket()) {

    client->connectSock();

    client->receievedThread = std::thread([&] { client->threadReceived(); });

    while (true) {
      getline(std::cin, message);
      std::string messageToSend;
      if (client->joinChat == false) {
        std::ostringstream ss;
        ss << client->username << ": " << message;
        messageToSend = ss.str();
      } else if (client->joinChat == true) {
        std::ostringstream ss;
        ss << client->username << " joined the chat! \n";
        messageToSend = ss.str();
        client->joinChat = false;
      }
      Message message{};
      auto now = std::chrono::system_clock::now();
      std::time_t time = std::chrono::system_clock::to_time_t(now);

      message.messageTime = ctime(&time);
      message.textMessage = messageToSend;
      client->sendMessage(message);
    }
  }


  delete client;
  std::cin.get();
  return 0;
}