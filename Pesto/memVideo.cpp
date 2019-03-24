#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdint.h>
#include "imgproc.hpp"
#include<opencv2/opencv.hpp>
#include "zscale.h"

using namespace cv;


cv::Mat memVidSetup(float *&im, const uint16_t size, std::string handle)
{
    cv::startWindowThread();//to fix the bug where the window does not resize if it's killed inbetween to image sequence
    cv::Mat matIm = cv::Mat(size, size, CV_32F  ,im);
    cv::namedWindow(handle.c_str(),cv::WINDOW_FREERATIO);
    return matIm;
}
void stats(unsigned short int A[], uint32_t length, double *mean, double *std){
    if (length<=0)
    {*mean=0;
    *std=0.0;}
    double sum=0;
    double sq_sum=0;
    for (uint32_t i=0;i<length;i++)
    {
        sum+=A[i];
        sq_sum += A[i] * A[i];
    }

    *mean = sum/(double)length;
    double variance = sq_sum / length - *mean * *mean;
    *std = sqrt(variance);
}
void stats(float A[], uint32_t length, double *mean, double *std){
    if (length<=0)
    {*mean=0;
    *std=0.0;}
    double sum=0;
    double sq_sum=0;
    for (uint32_t i=0;i<length;i++)
    {
        sum+=A[i];
        sq_sum += A[i] * A[i];
    }

    *mean = sum/(double)length;
    double variance = sq_sum / length - *mean * *mean;
    *std = sqrt(variance);
}

void normalisation (unsigned short int *&A, float *&B , const uint16_t size, float z1, float z2)
{   uint32_t length=(uint32_t)size*size;
    for (auto i=0;i<length;i++)
    {
        B[i]=((float)A[i]-z1)/(z2-z1);

    }
    return;
}
void display(std::string handle,cv::Mat matIm,uint32_t num,unsigned short *im_src,float *&im_dst,const uint16_t size,bool text=false,bool zscale=true)
{
    float z1,z2;
    if (text)
    {
        char text[50];
        sprintf(text,"Image: %d",num);
        putText(matIm, text,cv::Point(15,15),cv::FONT_HERSHEY_COMPLEX_SMALL, 4.0, cv::Scalar(200,200,250), 1);
    }
    if (zscale)
    {
        cdl_zscale(im_src,size,size,16,&z1,&z2,0.25,100,100);
        normalisation(im_src,im_dst,size,z1,z2);
    }
    else//min/max
    {
        cv::normalize(matIm,matIm,cv::NORM_MINMAX);
    }
    //std::cout<<z1<<" "<<z2<<std::endl;
    //double mean,st;
    //stats(im_dst,size,&mean,&st);
    //std::cout<<mean<<" "<<st<<std::endl;


    cv::imshow(handle.c_str(),matIm);
    cv::waitKey(1);
}

//void copyArr(unsigned short *A, unsigned short *B, int length)
//{
//    for (int i=0;i<length;i++)
//    {
//        B[i]=A[i];
//    }
//}
//int randomArray(uint32_t A[])
//{   int lim = 128*128;
//    for (int i=0 ; i<lim;i++)
//    {
//        A[i]=rand() % 65000;
//    }

//return 0;
//}


void closeWindow(std::string handle)
{

    cv::destroyWindow(handle);
}



