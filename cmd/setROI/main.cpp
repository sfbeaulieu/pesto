#include <iostream>
#include "socket.h"

using namespace std;

int main(int argc, char *argv[])
{

    string cmd="22";
    string ANSW,WRITE="-1";

    if (argc!=3){
        cout<<"Wrong number of arguments"<<endl;
        cout<<"SET_ROI <offset Y> <ROI height>"<<endl;
        return -1;
    }

    string arg1 = argv[1];//Offset-> offset en Y du ROI,
    string arg2 = argv[2];//ROI height-> height (Y) of the ROI
    //string arg3 = argv[3];//mode 1->join thread, 2->detach thread

    write_socket(4000,&cmd);
    write_socket(5001,&arg1);
    write_socket(5002,&arg2);
    //write_socket(5003,&arg3);

    write2way(4001,WRITE,&ANSW);

    cout<<ANSW<<endl;

        return 0;
}

