#ifndef UTILS_H
#define UTILS_H
#include "nc_driver.h"
#include <iostream>

int initializeSocket(void);
int openCam(NcCam *cam,struct initParam *param,struct camParam *detParam);
int initVariable(struct initParam *param,struct camParam *detParam);
std::string create_name(void);
void delay(int ms);
std::string create_dossier(int mode,struct camParam *detParam);
int selectTimeStamp(NcCam *cam,int mode);
int applyBias(NcCam *cam,struct initParam *param,struct camParam *detParam);
int get_inc(struct initParam *param);
double getPosRot(void);
double getPosBras(void);
int preACQ(struct camParam *cam);
#endif // UTILS_H
