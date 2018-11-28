#ifndef MEMVIDEO_H
#define MEMVIDEO_H

#include <opencv2/highgui.hpp>

cv::Mat memVidSetup( float im[],uint size,std::string handle);
void display(std::string handle,cv::Mat matIm,int num);
void copyArr(unsigned short int *A,unsigned short int *B,int length);
int randomArray(uint32_t A[]);
void normalisation (unsigned short int A[],float B [] ,uint size, float z1,float z2);
#endif // MEMVIDEO_H
