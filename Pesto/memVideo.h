#ifndef MEMVIDEO_H
#define MEMVIDEO_H

#include <opencv2/highgui.hpp>

cv::Mat memVidSetup( float *&im,const uint16_t size,std::string handle);
void display(std::string handle,cv::Mat matIm,uint32_t num,unsigned short *im_src,float *&im_dst,const uint16_t size,bool text=false,bool zscale=true);
//void copyArr(unsigned short int *A,unsigned short int *B,int length);
//int randomArray(uint32_t A[]);
void normalisation (unsigned short int *&A,float *&B ,const uint16_t size, float z1,float z2);
void closeWindow(std::string handle);
void stats(unsigned short int A[],uint32_t length,double *mean,double *std);
void stats(float A[],uint32_t length,double *mean,double *std);

#endif // MEMVIDEO_H
