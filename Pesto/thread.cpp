
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
extern int isInAcq;
extern int tcs_loop;
extern std::string adress_tcs;

//extern int loop;
//extern int inc;

//void *acquisition(void *arg){
void acquisition(int *mode,int *loop,int *inc){
    std::cout<<*mode<<std::endl;

    char objectnbr[15];
    std::string nameFile;
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

    int port=8380;
    char command[258]="EINF000000011\0";
    std::string write;
    std::string read;
    std::string str;
    char *token;
    int length;
    write = command;

std::cout<<"starting the thread"<<std::endl;
    while(tcs_loop)
    {   sleep(2);

        if (write2way_adress(port, adress_tcs, write, &read)==0)
        {
            if (strncmp(read.c_str(),"ANSW",4)!=0){
                 //do nothing
            }
            else
            {
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
        {
            sleep(2);
        }
    }




}
