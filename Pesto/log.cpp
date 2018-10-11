  #include "log.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <sys/stat.h>//mkdir
#include <libgen.h>
#include <string>
#include <algorithm>



Log::Log(std::string path)
{
Log::path=path;
Log::fileName=createfileName();

if (Log::isFolder(Log::path)!=1){Log::createFolder(Log::path);}

}

void Log::writeto(std::string text){


    std::ofstream ofs;
    Log::checkDate();
    ofs.open (Log::path+Log::fileName, std::ofstream::out | std::ofstream::app);
    ofs <<Log::Time()+'\t'+text<<'\n';
    ofs.flush();
    ofs.close();
}

bool Log::isLeap(int year)
{
    if (year%4==0){return true;}
    else if (year%100==0){return false;}
    else if (year%400==0){return true;}
    else {return false;}
}
void Log::writetoVerbose(std::string text){
    std::ofstream ofs;
    Log::checkDate();
    ofs.open (Log::path+Log::fileName, std::ofstream::out | std::ofstream::app);
    ofs <<Log::Time()+'\t'+text<<'\n';
    std::cout<<text<<std::endl;
    ofs.flush();
    ofs.close();
}

int Log::createFolder(std::string path){
    char *subpath, *fullpath;

    fullpath = strdup(path.c_str());
    subpath = dirname(fullpath);
    if (strlen(subpath) > 1)
        Log::createFolder(subpath);
    mkdir(path.c_str(),S_IRWXU | S_IRWXG | S_IRWXO);
    free(fullpath);
    return 0;
}
int Log::createfile(void){
    std::ofstream ofs;
    Log::checkDate();
    ofs.open (Log::path+Log::fileName, std::ofstream::out | std::ofstream::app);

    ofs.flush();
    ofs.close();
    return Log::isFile(Log::path+Log::fileName);
}

bool Log::isFolder(std::string path){
    return ( access( path.c_str(), F_OK ) != -1 );
}
bool Log::isFile(std::string path){

   return ( access( path.c_str(), F_OK ) != -1 );
}



void Log::checkDate(void){
    //check if the file name is up to date. If it is not up to date it will update the file name (YYMMDD)
    time_t t;
    t = time(NULL);
    char date[15];
    struct tm tm = *localtime(&t);
    static const int days[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}

    };
    if (tm.tm_hour<12){
        int year = tm.tm_year+1900;
        int month = tm.tm_mon+1;
        int day = tm.tm_mday;

        if (day==1)
        {
            day = days[Log::isLeap(year)][month];
        }
        else
        {
            day = day-1;
        }

        sprintf(date,"%d%.2d%.2d", year-2000,month,day);
        }
    if (tm.tm_hour>=12){
        sprintf(date,"%d%.2d%.2d", tm.tm_year+1900-2000,tm.tm_mon+1,tm.tm_mday);
        }

    if (strcmp(Log::fileName.c_str(),date)!=0){
        Log::fileName=date;
    }

}
std::string Log::createfileName(void){
    time_t t;
    t = time(NULL);
    char date[15];

    std::string fileName;
    struct tm tm = *localtime(&t);

    static const int days[2][13] = {
        {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31},
        {0, 31, 29, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}

    };

    if (tm.tm_hour<12){

        int year = tm.tm_year+1900;
        int month = tm.tm_mon+1;
        int day = tm.tm_mday;

        if (day==1)
        {
            day = days[Log::isLeap(year)][month];
        }
        else
        {
            day = day-1;
        }



        sprintf(date,"%d%.2d%.2d", year-2000,month,day);
        }

    if (tm.tm_hour>=12){
        sprintf(date,"%d%.2d%.2d", tm.tm_year+1900-2000,tm.tm_mon+1,tm.tm_mday);
        }
    fileName=date;
    return fileName;
}
std::string Log::Time(void)
{   std::string leTemps;
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    leTemps = asctime(timeinfo);
    leTemps.erase(std::remove(leTemps.begin(), leTemps.end(), '\n'), leTemps.end());
    return leTemps;
}
