#ifndef SOCKET_H
#define SOCKET_H

int create_socket(int port);
int createSocket2way(int portno);
int read_socket(std::string *buffer,int sock);
int read2way(int sockfd,std::string *buffer, std::string Write);
int write_socket_address(int port,std::string adress,std::string *command);
int write2way_adress(int portno,std::string adress, std::string Write, std::string *Read);
//int create_socket_adress(int port,std::string adress);
#endif // SOCKET_H
