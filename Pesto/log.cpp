#include "log.h"
#include <unistd.h>
#include <time.h>
#include <stdio.h>
#include <cstdio>
#include <fstream>
#include <string.h>
#include <sys/stat.h>//mkdir
#include <libgen.h>



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
    ofs <<text<<'\n';
    ofs.flush();
    ofs.close();
}
void Log::writetoVerbose(std::string text){
    std::ofstream ofs;
    Log::checkDate();
    ofs.open (Log::path+Log::fileName, std::ofstream::out | std::ofstream::app);
    ofs <<text<<'\n';
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

    if (tm.tm_hour<=12){
        sprintf(date,"%d%.2d%.2d", tm.tm_year+1900-2000,tm.tm_mon+1,tm.tm_mday-1);
        }
    if (tm.tm_hour>12){
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
    if (tm.tm_hour<=12){
        sprintf(date,"%d%.2d%.2d", tm.tm_year+1900-2000,tm.tm_mon+1,tm.tm_mday-1);
        }

    if (tm.tm_hour>12){
        sprintf(date,"%d%.2d%.2d", tm.tm_year+1900-2000,tm.tm_mon+1,tm.tm_mday);
        }
    fileName=date;
    return fileName;
}
