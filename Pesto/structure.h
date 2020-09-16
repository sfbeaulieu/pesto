#ifndef STRUCTURE_H
#define STRUCTURE_H
#include <stdint.h>
#include <iostream>
struct camera{
    int xPxSize;
    int yPxSize;
    char serial[55];


};
struct initParam{

    std::string racinePath;
    std::string logPath;
    int nbBiasConv;
    int nbBiasEm;
    int maxGain;
    int minGain;
    std::string FW0;
    std::string FW1;
    std::string FW2;
    std::string FW3;
    std::string FW4;
    std::string FW5;
    int defROMODE;
    double defCCDTEMP;
    std::string pathPython;
};
struct camParam{
    double readoutTime;
    double waitingTime;
    double exposureTime;
    std::string object_name;
    std::string program_name;
    std::string path;
    int nbrExp;
    double ccdTemp;
    int gain;
    std::string current_filter;
    std::string observateur;
    std::string Operator;
    double rotAngle;
    double rotBras;


};

struct TCS{

char HA[15];
char RA[15];
char DEC[15];
char EPOCH[15];
char AIRMASS[15];
char ST[15];
char UT[15] ;
char YEAR[15];
char FOCUS[15];
char DOME[15];
char TROTATOR[15];
};
struct Meteo{
    char Hin[30];
    char Hout[30];
    char Tin[30];
    char Tout[30];
    char Tstruct[30];
    char Tm[30];

};
struct disp{
    bool zscale;// select zscale or minMax
    bool text;//display the number of increment on screen
    bool save;//write image on disk


};
struct display_roi{
    int offsetx[8];
    int offsety[8];
    int count;
    int ro_mode;
    int buff_width;
    int buff_height;
    int width[8];
    int height[8];
    uint32_t num;


};

#endif // STRUCTURE_H
