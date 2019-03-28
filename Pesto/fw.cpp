#include "fw.h"
#include "socket.h"
#include <string>
#include <algorithm>
#include "log.h"

FW::FW()
{
FW::isHomed=0;//everytime the pesto is started the filter wheel is automatically not initialized
FW::adress = "132.204.61.142";// adress of the filter server at OMM

}
int FW::position(int pos)
{
    std::string cmd="1";
    std::string ANSW,WRITE="-1";
    std::string cmd2=std::to_string(pos);

    if (!FW::isHomed)
    {
        extern Log logg;
        logg.writetoVerbose("The filter wheel is not initialized. Initializing the filter first. Please wait...");
        FW::home();
    }

    write_socket_address(8300,FW::adress,&cmd);
    write_socket_address(8301,FW::adress,&cmd2);
    write2way_adress(8400,FW::adress,WRITE,&ANSW);
    ANSW.erase(std::remove(ANSW.begin(), ANSW.end(), '\n'), ANSW.end());

    if (pos==stoi(ANSW))
    {
        return stoi(ANSW);
    }
    else
    {
        return -1;
    }


}

int FW::increment(void)
{   FW::isHomed=0;
    std::string cmd="1";
    std::string ANSW,WRITE="-1";
    std::string cmd2="+";
    extern Log logg;
    logg.writetoVerbose("1+ incrementation with the filter wheel");
    write_socket_address(8300,FW::adress,&cmd);
    write_socket_address(8301,FW::adress,&cmd2);
    write2way_adress(8400,FW::adress,WRITE,&ANSW);
    ANSW.erase(std::remove(ANSW.begin(), ANSW.end(), '\n'), ANSW.end());
    //cout<<ANSW<<endl;
    return stoi(ANSW);
}
int FW::get_position(void)
{
    std::string cmd="1";
    std::string cmd2="?";
    std::string ANSW,WRITE="-1";
    write_socket_address(8300,FW::adress,&cmd);
    write_socket_address(8301,FW::adress,&cmd2);
    write2way_adress(8400,FW::adress,WRITE,&ANSW);
    ANSW.erase(std::remove(ANSW.begin(), ANSW.end(), '\n'), ANSW.end());
    //cout<<ANSW<<endl;
    return stoi(ANSW);
}

int FW::home(void)
{extern Log logg;
    std::string cmd="1";
    std::string ANSW,WRITE="-1";
    std::string cmd2="H";
    write_socket_address(8300,FW::adress,&cmd);
    write_socket_address(8301,FW::adress,&cmd2);
    write2way_adress(8400,FW::adress,WRITE,&ANSW);
    ANSW.erase(std::remove(ANSW.begin(), ANSW.end(), '\n'), ANSW.end());
    //cout<<ANSW<<endl;
    if (stoi(ANSW)==0)
    {
        FW::isHomed=1;

        logg.writetoVerbose("The filter wheel is homed");
    }

    return stoi(ANSW);
}
int FW::connection(std::string device)
{   FW::isHomed=0;
    std::string cmd="4";
    std::string ANSW,WRITE="-1";
    extern Log logg;
    logg.writetoVerbose("A new connection with a devive (FW) has been established");
    write_socket_address(8300,FW::adress,&cmd);
    write_socket_address(8301,FW::adress,&device);
    write2way_adress(8400,FW::adress,WRITE,&ANSW);
    ANSW.erase(std::remove(ANSW.begin(), ANSW.end(), '\n'), ANSW.end());
    //cout<<ANSW<<endl;
    return stoi(ANSW);

}
int FW::closeConnection(void)
{   FW::isHomed=0;
    std::string cmd="3";
    std::string ANSW,WRITE="-1";
    extern Log logg;
    logg.writetoVerbose("Disconnected from the filter server");
    write_socket_address(8300,FW::adress,&cmd);
    write2way_adress(8400,FW::adress,WRITE,&ANSW);
    ANSW.erase(std::remove(ANSW.begin(), ANSW.end(), '\n'), ANSW.end());
    //cout<<ANSW<<endl;
    return stoi(ANSW);
}
