#include "server.h"
#include <iostream>
#include <string>

const std::string serverIP = "127.0.0.1";
const uint16_t port = 20002;

int main()
{

  Server server(serverIP, port);

  if (server.initSocket()) { server.run(); }
}