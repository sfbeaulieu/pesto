#ifndef LOG_H
#define LOG_H

#include <iostream>
class Log
{std::string path;
 std::string fileName;

public:
    Log(std::string path);
    void writeto(std::string text);
    void writetoVerbose(std::string text);
    int createFolder(std::string path);
    int createfile(void);
    bool isFolder(std::string path);
    bool isFile(std::string path);
    bool isLeap(int year);

private:
    void checkDate(void);
    std::string createfileName(void);
    std::string Time(void);
};

#endif // LOG_H
