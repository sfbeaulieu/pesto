#include <iostream>
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <netdb.h>
#include <unistd.h> // for close
#include <arpa/inet.h>
#include "socket.h"



#define h_addr h_addr_list[0]//to be posix complient


void error(const char *msg){
//Description: needed for the 2 way buffer functions
    perror(msg);
    exit(1);
}



int create_socket(int port){
//Description: This function create a socket and
//return the sock file descriptor. Return -1 if failed to open socket

    struct sockaddr_in server;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0 ){
        perror("Failed to create socket");
        return -1;
        }
    int enable=1;
    //Si on redémare spirouserver rapidement on va pas avoir de problem au niveau d'ouvrir un port
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;}

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);

    if(bind(sock,(struct sockaddr *)&server,sizeof(server)) )
        {
        perror("Bind failed!!!");
        return -1;
        }
    listen(sock,10);

    return sock;}

int create_socket_adress(int port,std::string adress){
//Description: This function create a socket and
//return the sock file descriptor. Return -1 if failed to open socket

    struct sockaddr_in server;
    int sock;

    sock = socket(AF_INET, SOCK_STREAM, 0);

    if(sock < 0 ){
        perror("Failed to create socket");
        return -1;
        }
    int enable=1;
    //Si on redémare spirouserver rapidement on va pas avoir de problem au niveau d'ouvrir un port
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
        return -1;}

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(adress.c_str());
    if(bind(sock,(struct sockaddr *)&server,sizeof(server)) )
        {
        perror("Bind failed!!!");
        return -1;
        }
    listen(sock,10);

    return sock;}

int read_socket(std::string *buffer,int sock){
//Desciption: read a socket and place its value in
// the variable buffer. This function is meant to be used
//inside a while loop with switch. The function create_socket()
//must be called before this one. int sock, is a value
//returned by the function create_socket().
//if the buffer received is > 8char use memset to set 0's to char *buffer
    int mysock;
    int rval=1;
    char buff[1024];
    char tobecompare[2];
    long int nbchar;
    tobecompare[0]='L';
    tobecompare[1]='\0';

    mysock = accept(sock, (struct sockaddr *) 0, 0);

    if(mysock == -1 ){
        perror("accept failed");
        return -1;}
    else {memset(buff,0,sizeof(buffer->c_str()));}


    rval = recv(mysock, buff, 1,0);
    buffer->assign(buff,rval);
    std::string comp = *buffer;

    if (comp.compare(tobecompare)==0){

        recv(mysock, buff, 2,0);
        nbchar = atol(buff);

        recv(mysock, buff, nbchar,0);
        buffer->assign(buff,nbchar);
        }//if

    if((rval) < 0 ){
            perror("reading stream message error");
            return -1;}//if
    close(mysock);

    return 0;}//read_socket END



int write_socket(int port,std::string *command){
//Description: Send "command" to a server. Is meant to
//be use on the client side.
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char command_to_send[99];

    sock = socket(AF_INET, SOCK_STREAM,0);
    if(sock < 0){
        perror("socket failed");
       return -1;
        }
    server.sin_family = AF_INET;
    hp = gethostbyname("localhost");
    if(hp==NULL){//modification 2018-01-13
        perror("gethostbyname failed");
        close(sock);
        return -1;	}
    if((hp ==0))
        {
        perror("gethostbyname failed");
        return -1;
        }
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);

    server.sin_port = htons(port);

    if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
        {
        perror("connect failed");
        close(sock);
        return -1;
        }
    sprintf(command_to_send,"L%2.2lu%s\n",strlen(command->c_str()),command->c_str());


    send(sock, command_to_send,    strlen(command_to_send),0);

    memset(&command[0], 0, sizeof( command->c_str()));
    close(sock);

return sock;}//write_socket END

int write_socket_address(int port,std::string adress,std::string *command){
//Description: Send "command" to a server. Is meant to
//be use on the client side.
    int sock;
    struct sockaddr_in server;
    struct hostent *hp;
    char command_to_send[99];

    sock = socket(AF_INET, SOCK_STREAM,0);

    if(sock < 0){
        perror("socket failed");
       return -1;
        }
    server.sin_family = AF_INET;
    hp = gethostbyname("localhost");
    if(hp==NULL){//modification 2018-01-13
        perror("gethostbyname failed");
        close(sock);
        return -1;	}
    if((hp ==0))
        {
        perror("gethostbyname failed");
        return -1;
        }
    memcpy(&server.sin_addr, hp->h_addr, hp->h_length);

    server.sin_port = htons(port);
    server.sin_addr.s_addr = inet_addr(adress.c_str());
        if((connect(sock, (struct sockaddr *) &server, sizeof(server)) < 0) )
        {
        perror("connect failed");
        close(sock);
       return -1;
        }
    sprintf(command_to_send,"L%2.2lu%s\n",strlen(command->c_str()),command->c_str());


    send(sock, command_to_send,    strlen(command_to_send),0);

    memset(&command[0], 0, sizeof( command->c_str()));
    close(sock);

return sock;}//write_socket END

int createSocket2way(int portno){
//Description: Create a socket on the server side that enable
//a client to send information to the server and receive back an
//answer from the server.
    int sockfd;
    struct sockaddr_in serv_addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    int enable=1;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof( int)) < 0){
        perror("setsockopt(SO_REUSEADDR) failed");
    return -1;}
    if (sockfd < 0){
        perror("ERROR opening socket");
    return -1;}
     memset((char *) &serv_addr, 0,sizeof(serv_addr));
     serv_addr.sin_family = AF_INET;
     serv_addr.sin_addr.s_addr = INADDR_ANY;
     serv_addr.sin_port = htons(portno);
    if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
         perror("ERROR on binding");
    return -1;}
    listen(sockfd,10);
    return sockfd;
}//createSocket2way END

int write2way_adress(int portno,std::string adress, std::string Write, std::string *Read){
//Description: This function is called on the client side
//to send information though Write to a server. The server should then
//send back an answer which is place in Read. The arguments
//include the port number of the socket. The is no need to create
// a socket on the client side. The socket is created using
// using the function "createSocket2way(int portno)" on the server side
    int sockfd, n;

    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("ERROR opening socket");
        return -1;
    }//if

    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr," no such host\n");
        return -1;}//if
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy( (char *)&serv_addr.sin_addr.s_addr,(char *)server->h_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = inet_addr(adress.c_str());


    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
        return -1;
    }//if

    n = write(sockfd,Write.c_str(),strlen(Write.c_str()));
    if (n < 0) {
         perror("ERROR writing to socket");
        return -1;
    }//if
    memset(buffer,0,1024);
    n = read(sockfd,buffer,1023);
    //strcpy(Read,buffer);
    //Read->copy(buffer,sizeof(buffer));
    *Read=buffer;

    if (n < 0){
         perror("ERROR reading from socket");
        return -1;}

    close(sockfd);
    return 0;
}//write2way end

int read2way(int sockfd,std::string *buffer, std::string Write){
//Description: this function read the socket "sockfd" and place
//the value in buffer. After readijng the buffer it send to the
//client the value Write in the same socket. This function is meant
//to be used on the server side. The function "createSocket2way()"
//must be called before "read2way()".


    int  newsockfd;
    int n;
    struct sockaddr_in cli_addr;
    socklen_t clilen;
    char buff[1024];
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd,(struct sockaddr *) &cli_addr, &clilen);
    if (newsockfd < 0){
        perror("ERROR on accept");
        return -1;}
    memset(buff,0,1024);
    n = read(newsockfd,buff,1024);
    if (n < 0) error("ERROR reading from socket");
    n = write(newsockfd,Write.c_str(),strlen(Write.c_str()));
    if (n < 0) error("ERROR writing to socket");
    *buffer=buff;
    close(newsockfd);
    return sockfd;
}//read2way end


int write2way(int portno, std::string Write, std::string *Read){
//Description: This function is called on the client side
//to send information though Write to a server. The server should then
//send back an answer which is place in Read. The arguments
//include the port number of the socket. The is no need to create
// a socket on the client side. The socket is created using
// using the function "createSocket2way(int portno)" on the server side

    int sockfd, n;
    struct sockaddr_in serv_addr;
    struct hostent *server;
    char buffer[1024];
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0){
        perror("ERROR opening socket");
    return -1;}//if
    server = gethostbyname("localhost");
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        return -1;}//if
    memset((char *) &serv_addr,0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy( (char *)&serv_addr.sin_addr.s_addr,(char *)server->h_addr,server->h_length);
    serv_addr.sin_port = htons(portno);
    if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0){
        perror("ERROR connecting");
    return -1;}//if
    n = write(sockfd,Write.c_str(),strlen(Write.c_str()));
    if (n < 0) {
         perror("ERROR writing to socket");
    return -1;}//if
    memset(buffer,0,1024);
    n = read(sockfd,buffer,1023);
    //strcpy(Read,buffer);
    //Read->copy(buffer,sizeof(buffer));
    *Read=buffer;

    if (n < 0){
         perror("ERROR reading from socket");
    return -1;}
    close(sockfd);
    return 0;
}//write2way end

