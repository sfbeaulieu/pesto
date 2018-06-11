#ifndef SOCKET_H
#define SOCKET_H

int write2way(int portno, std::string Write, std::string *Read);
int write_socket(int port,std::string *command);

#endif // SOCKET_H
