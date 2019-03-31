#include "nc_driver.h"
#include <iostream>
using namespace std;

int main()
{   const NcCtrlList ctrlList;
    const int size;
    if (ncControllerListGetSize(ctrlList,&size)!=0){cout<<"error"<<endl;}

    for (int i=0;i<size;i++)
    {
        cout<<ctrlList[i]<<endl;
    }
    return 0;
}
