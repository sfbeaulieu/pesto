#include <iostream>
#include "socket.h"

using namespace std;

int main(int argc, char *argv[])
{

    string cmd="6";
    string ANSW,WRITE="-1";

    if (argc!=1){
        cout<<"Wrong number of arguments"<<endl;
        cout<<"GET_CCDT <void>"<<endl;
        return -1;
    }

    //string arg1 = argv[1];//mode 1->continuous,
    //string arg2 = argv[2];//nbExp-> number of frame to acquire
    //string arg3 = argv[3];//mode d'enregistrement
    //string arg4 = argv[4];//mode 1->join thread, 2->detach thread
    write_socket(4000,&cmd);
    //write_socket(5001,&arg1);
    //write_socket(5002,&arg2);
    //write_socket(5003,&arg3);
    //write_socket(5004,&arg4);
    write2way(4001,WRITE,&ANSW);

    cout<<ANSW<<endl;

        return 0;
}

