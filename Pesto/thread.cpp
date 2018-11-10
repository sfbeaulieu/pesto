
#include "nc_driver.h"
#include "utils.h"

#include "structure.h"
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>
#include "log.h"
#include "socket.h"


extern NcCam myCam;
extern NcImage	*myNcImage;
extern struct camParam detParam;
extern struct initParam param;
extern Log log;
extern int isInAcq,buffAcQ_port,buffAcQ,buffStop_port,buffStop,buffInc,buffInc_port;
extern int tcs_loop,meteo_loop;
extern std::string adress_tcs;
extern std::string nameFile;
extern int threadInc;
//extern int loop;
//extern int inc;

//void *acquisition(void *arg){
void acquisition(int *mode,int *loop,int *inc){
    //std::cout<<*mode<<std::endl;

    char objectnbr[15];
    //std::string nameFile;
    //detParam.object_name= create_name();
    std::string racineFN = create_name();
    int error;
    error = ncCamStart(myCam,0);
    if (error){
        log.writetoVerbose("Unable to start the acquisition");
        return;
    }
    std::cout<<"Acquisition started..."<<std::endl;

    if (!log.isFolder(param.racinePath+detParam.path)){
        log.createFolder(param.racinePath+detParam.path);

    }

    switch (*mode){

    case 1:
    {
        isInAcq=1;
        while(*loop)
        {
            *inc+=1;
            sprintf(objectnbr,"%.10d",*inc);
            sscanf(objectnbr,"%d",&threadInc);
            ncCamRead(myCam, &myNcImage);
            nameFile = param.racinePath+detParam.path+racineFN+std::string(objectnbr);
            ncCamSaveImage(myCam, myNcImage,nameFile.c_str(), FITS," " , 1);

        }
        break;
    }

    case 2:
    {
        isInAcq=1;

        for (int i=0;i<=detParam.nbrExp;i++)
        {
            *inc+=1;
            sprintf(objectnbr,"%.10d",*inc);
            ncCamRead(myCam, &myNcImage);
            nameFile = param.racinePath+detParam.path+racineFN+std::string(objectnbr);
            ncCamSaveImage(myCam, myNcImage,nameFile.c_str(), FITS," " , 1);
            if (*loop==0){break;}
        }
        break;
    }

    }//switch case
    isInAcq=0;
    *loop=0;
    std::cout<<"Acquisition stopped"<<std::endl;
    ncCamAbort(myCam);


}

//tel meteo

void tcs(struct TCS *tcs_var)
{
    int conteur=0;
    int port=8380;
    char command[258]="EINF000000011\0";
    std::string write,read,str;
    char *token;
    int length;
    write = command;

//std::cout<<"starting the thread tcs"<<std::endl;
    while(tcs_loop)
    {   sleep(4);

        if (write2way_adress(port, adress_tcs, write, &read)==0)
        {
            if (strncmp(read.c_str(),"ANSW",4)!=0){
                 conteur+=1;
                 if (conteur>30){
                 log.writetoVerbose("BonOMM is not responsive to Pesto's querries for more than 120 sec. Please make sure BonOMM is open.");
                 log.writetoVerbose("If the problem persist, restart the PineNuts and Pesto software.");
                }
            }
            else
            {   conteur=0;
                length = stoi(read.substr(4,8));
                str = read.substr(12,length);

                token = strtok((char *)str.c_str(),"\r");
                memset(&tcs_var->HA[0],0,sizeof(tcs_var->HA));
                sprintf(tcs_var->HA,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->RA[0],0,sizeof(tcs_var->RA));
                sprintf(tcs_var->RA,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->DEC[0],0,sizeof(tcs_var->DEC));
                sprintf(tcs_var->DEC,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->EPOCH[0],0,sizeof(tcs_var->EPOCH));
                sprintf(tcs_var->EPOCH,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->AIRMASS[0],0,sizeof(tcs_var->AIRMASS));
                sprintf(tcs_var->AIRMASS,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->ST[0],0,sizeof(tcs_var->ST));
                sprintf(tcs_var->ST,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->UT[0],0,sizeof(tcs_var->UT));
                sprintf(tcs_var->UT,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->YEAR[0],0,sizeof(tcs_var->YEAR));
                sprintf(tcs_var->YEAR,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->FOCUS[0],0,sizeof(tcs_var->FOCUS));
                sprintf(tcs_var->FOCUS,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->DOME[0],0,sizeof(tcs_var->DOME));
                sprintf(tcs_var->DOME,"%s\n",token);
                token = strtok(NULL,"\r");
                memset(&tcs_var->TROTATOR[0],0,sizeof(tcs_var->TROTATOR));
                sprintf(tcs_var->TROTATOR,"%s\n",token);
                token = strtok(NULL,"\r");




            }
        }
        else
        {   conteur+=1;
            sleep(2);
            if (conteur>30)
            {
                log.writetoVerbose("Unable to communicate with TCS for more than 120 sec. Please make sure BonOMM is open.");
                log.writetoVerbose("If the problem persist, restart the PineNuts and Pesto software.");
            }
        }
    }




}

void meteoThread(struct Meteo *meteo)
{
    int length,port=8380;
    std::string write,read,str;
    char *token;
    char command[258]="ETMP000000011\0";

    write = command;
    while (meteo_loop)
    {
    sleep(30);

    if (write2way_adress(port, adress_tcs, write, &read)==0)
    {
        if (strncmp(read.c_str(),"ANSW",4)!=0)
        {
            //std::cout<<"Unable to communicate with TCS"<<std::endl;
            log.writeto("In meteo()-- Unable to communicate with TCS");
        }
        else{
        length = stoi(read.substr(4,8));
        str = read.substr(12,length);

        if (strcmp(str.c_str(),"Commande non reconnue")!=0)
        {
            token = strtok((char *)str.c_str(),"\r");
            memset(&meteo->Hin[0],0,sizeof(meteo->Hin));
            sprintf(meteo->Hin,"%s\n",token);
            token = strtok(NULL,"\r");
            memset(&meteo->Hout[0],0,sizeof(meteo->Hout));
            sprintf(meteo->Hout,"%s",token);
            token = strtok(NULL,"\r");
            memset(&meteo->Tin[0],0,sizeof(meteo->Tin));
            sprintf(meteo->Tin,"%s\n",token);
            token = strtok(NULL,"\r");
            memset(&meteo->Tout[0],0,sizeof(meteo->Tout));
            sprintf(meteo->Tout,"%s\n",token);
            token = strtok(NULL,"\r");
            memset(&meteo->Tstruct[0],0,sizeof(meteo->Tstruct));
            sprintf(meteo->Tstruct,"%s\n",token);
            token = strtok(NULL,"\r");
            memset(&meteo->Tm[0],0,sizeof(meteo->Tm));
            sprintf(meteo->Tm,"%s\n",token);
            token = strtok(NULL,"\r");


        }
    }//else
    }

    }//while loop

}

void ccdTemp(struct camParam *detParam){

    double Tccd;
    int count=0;
    while(1)
    {   sleep(4);
        if (ncCamGetDetectorTemp(myCam,&Tccd)!=0)
        {   count++;
            if (count>10)
            {
                log.writetoVerbose("Unable to read the emccd temp");
            }
        }

        else
        {   count=0;
            detParam->ccdTemp=Tccd;
        }
    }


}

void getTempLoop(struct camParam *detParam)
{
    int threadTempCCD=6000;
    int IDccdTEMP = createSocket2way(threadTempCCD);
    std::string READ;
    while(1)
    {
        read2way(IDccdTEMP,&READ,std::to_string(detParam->ccdTemp));


    }

}

void isAcqOnGoing(int *ongoing)
{
    buffAcQ = create_socket(buffAcQ_port);
    if(buffAcQ==-1)
    {
        log.writetoVerbose("Unable to create a socket on port "+std::to_string(buffAcQ_port));
        log.writetoVerbose("Shutting down");
        exit(1);
    }
    std::string REP,dummy;
    char WRITE[15];
    while(1)
    {
        if(read_socket(&REP,buffAcQ)!=0){break;}
        if(atoi(REP.c_str())==0)
        {
            sprintf(WRITE,"%d",*ongoing);
            REP = WRITE;
            read2way(buffAcQ,&dummy,REP);
        }


    }

}

void threadStop(int *close,int *isInAcq)
{
    buffStop = create_socket(buffStop_port);
    if(buffStop==-1)
    {
        log.writetoVerbose("Unable to create a socket on port "+std::to_string(buffStop_port));
        log.writetoVerbose("Shutting down");
        exit(1);
    }
    std::string REP,dummy;
    char WRITE[15];
    while(1)
    {
        if(read_socket(&REP,buffStop)!=0){break;}
        if(atoi(REP.c_str())==0)
        {
            *close=0;
            while(*isInAcq){
                std::cout<<"waiting for the end of the acquisition..."<<std::endl;
                delay(1000);
            }
            sprintf(WRITE,"%d",0);
            REP = WRITE;
            read2way(buffStop,&dummy,REP);
        }




    }
}
void getInc(int *incVar)
{
    buffInc = create_socket(buffInc_port);
    if(buffInc==-1)
    {
        log.writetoVerbose("Unable to create a socket on port "+std::to_string(buffInc_port));
        log.writetoVerbose("Shutting down");
        exit(1);
    }
    std::string REP,dummy;
    char WRITE[15];
    while(1)
    {
        if(read_socket(&REP,buffInc)!=0){break;}
        if(atoi(REP.c_str())==0)
        {
            sprintf(WRITE,"%d",*incVar);
            REP = WRITE;
            read2way(buffInc,&dummy,REP);
        }




    }
}
