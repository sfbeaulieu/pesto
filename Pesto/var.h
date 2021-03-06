#ifndef VAR_H
#define VAR_H
#include <pthread.h>
#include "log.h"
#include "nc_driver.h"
#include "structure.h"
#include "fw.h"
#include "string"


int caseID,repID,buff1ID,buff2ID,buff3ID,buff4ID,pathID,buff2wayID1,buff2wayID2,buffAcQ,buffStop,buffInc;//tout les ID des socket
pthread_t pth1,pth2;//pense pas qu'on a en encore dde besoin
pthread_t pth3;//the go thread loop
pthread_t pth4;//tcs loop to update struct tcs_var
pthread_t pth5;//temperature loop
pthread_t pth6;//meteo loop
pthread_t pth7;//thread for returning the incrementation

std::string Case;
std::string WRITE;
std::string buff1,buff2,buff3,buff4;
std::string nameFile ;


char cWRITE[100];

int loop,biasOK,mode,inc,error;
int ROIcount;
int isInAcq;//1 -> in ACQ, 0->not in acquisition
int display_on;//0 do not start the memory video display, 1-> start mvd
int saveIm;//0-> do not save images, 1-> save images
int tcs_loop,meteo_loop;
int threadInc;
int developpement;
int overideFW;

//parameter de la camera
int nbrBuffer;//This parameter represents the number of buffers that will receive acquired images from the camera. These buffers will receive images one after the other such that one or more buffers can be used/processed while another one receives a new image from the camera
std::string adress_tcs;
NcCam myCam;
NcImage	*myNcImage;

//start the log class
Log logg("/home/pesto/jonathan/log/");

FW fw;

struct camera camera;
struct initParam param;
struct camParam detParam;
struct TCS tcs_var;
struct Meteo meteo;
struct disp display_struct;
#endif // VAR_H
