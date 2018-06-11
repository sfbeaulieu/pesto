#ifndef FW_H
#define FW_H
#include <iostream>

class FW
{
    int isHomed;
    std::string adress;
public:
    FW();
    int position(int pos);
    int increment(void);
    int get_position(void);
    int home(void);
    int closeConnection(void);
    int connection(std::string device);


};

#endif // FW_H
