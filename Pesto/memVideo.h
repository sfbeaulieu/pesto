#ifndef MEMVIDEO_H
#define MEMVIDEO_H
#include <opencv2/highgui.hpp>

cv::Mat memVidSetup( float *&im,const uint16_t size,std::string handle);
void display(std::string handle,cv::Mat matIm,unsigned short *im_src,float *&im_dst,struct display_roi *disp_roi);
void normalisation (unsigned short int *&A,float *&B ,uint32_t length, float z1,float z2);
void normalisation_ROI (unsigned short int *&A,float *&B ,int start_dst,int start_src,int width,int height, float z1,float z2);
void closeWindow(std::string handle);
void stats(unsigned short int A[],uint32_t length,double *mean,double *std);
void stats(float A[],uint32_t length,double *mean,double *std);
void copy_array(unsigned short int *im,unsigned short int *&im2,uint32_t length);
int setupROI(struct display_roi *disp_roi);
#endif // MEMVIDEO_H

