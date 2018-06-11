#include <iostream>
#include "socket.h"

using namespace std;

int main(int argc, char *argv[])
{

    string cmd="10";
    string ANSW,WRITE="-1";

    if (argc!=2){
        cout<<"Wrong number of arguments"<<endl;
        cout<<"SET_GAIN <em gain>"<<endl;
        return -1;
    }

    string arg1 = argv[1];//1->em amplifier gain 
    //string arg2 = argv[2];//nbExp-> number of frame to acquire
    //string arg3 = argv[3];//mode 1->join thread, 2->detach thread
    //string arg4 = argv[4];//mode d'enregistrement
    write_socket(4000,&cmd);
    write_socket(5001,&arg1);
    //write_socket(5002,&arg2);
    //write_socket(5003,&arg3);
    //write_socket(5004,&arg4);
    write2way(4001,WRITE,&ANSW);

    cout<<ANSW<<endl;

        return 0;
}

