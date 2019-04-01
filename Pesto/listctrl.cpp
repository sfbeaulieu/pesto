#include "nc_driver.h"
#include <iostream>
#include <string.h>
using namespace std;

int initCtrl(NcCam *myCam)
{   NcCtrlList ctrlList;
    //NcCam cam;
    int size=0;
    size_t serialSize=100;
    char serial[serialSize];
    char model[serialSize];
    int unit=0;
    int channel=0;
    char interface[serialSize];
    char unID[serialSize];
    char detector[serialSize];

    int sizeX=0;
    int sizeY=0;
if (ncControllerListOpen(&ctrlList)!=0){cout<<"error"<<endl; return -1;}
if (ncControllerListGetSize(ctrlList,&size)!=0){cout<<"error"<<endl; return -1;}


for (int i=0;i<1;i++)
{
    memset(serial,0,serialSize);
    memset(model,0,serialSize);
    memset(interface,0,serialSize);
    memset(unID,0,serialSize);
    memset(detector,0,serialSize);

    if (ncControllerListGetSerial(ctrlList,i,serial,serialSize)!=0){cout<<"error"<<endl;return -1;}
    if (ncControllerListGetModel(ctrlList,i,model,serialSize)!=0){cout<<"error"<<endl; return -1;}
    if (ncControllerListGetPortUnit(ctrlList,i,&unit)!=0){cout<<"error"<<endl; return -1;}
    if (ncControllerListGetPortChannel(ctrlList,i,&channel)!=0){cout<<"error"<<endl; return -1;}
    if (ncControllerListGetPortInterface(ctrlList,i,interface,serialSize)!=0){cout<<"error"<<endl; return -1;}
    if (ncControllerListGetUniqueID(ctrlList,i,unID,serialSize)!=0){cout<<"error"<<endl; return -1;}
    if (ncControllerListGetFullSizeSize(ctrlList,i,&sizeX,&sizeY)){cout<<"error"<<endl; return -1;}
    if (ncControllerListGetDetectorType(ctrlList,i,detector,serialSize)!=0){cout<<"error"<<endl; return -1;}
    cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
    cout<<"Index: ("<<i<<")"<<endl;
    cout<<"Serial: "<<serial<<endl;
    cout<<"Model: "<<model<<endl;
    cout<<"Unit: "<<unit<<endl;
    cout<<"Channel: "<<endl;
    cout<<"Interface: "<<interface<<endl;
    cout<<"Unique ID: "<<unID<<endl;
    cout<<"Detector size: "<<sizeX<<"X"<<sizeY<<endl;
    cout<<"Detector Type: "<<detector<<endl;
}
cout<<"::::::::::::::::::::::::::::::::::::::::::::::::::::::::::"<<endl;
cout<<"Select a camera and enter the index number:"<<endl;
int indexChoice;
cin>>indexChoice;
if (indexChoice<0 || indexChoice>size)
{
    cout<<"Your choice is wrong. If the camera still connect, please contact the developper. (jonathan@astro.umontreal.ca)"<<endl;
}

if (ncCamOpenFromList(ctrlList,indexChoice,128,myCam)){cout<<"error"<<endl; return -1;}
if (ncControllerListFree(ctrlList)!=0){cout<<"error"<<endl; return -1;}


return 0;
}
