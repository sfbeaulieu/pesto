#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdint.h>
#include "imgproc.hpp"
#include<opencv2/opencv.hpp>
#include "zscale.h"
#include "nc_driver.h"
#include "structure.h"

using namespace cv;
#define EM 0
#define CONV 1
extern NcCam myCam;
extern display_roi disp_roi;
cv::Mat memVidSetup(float *&im, const uint16_t size, std::string handle)
{
    cv::startWindowThread();//to fix the bug where the window does not resize if it's killed inbetween to image sequence
    cv::Mat matIm = cv::Mat(size, size, CV_32F  ,im);
    cv::namedWindow(handle.c_str(),cv::WINDOW_FREERATIO);//WINDOW_FREERATIO previous config
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

void normalisation (unsigned short int *&A, float *&B ,  uint32_t length, float z1, float z2)
{

    for (uint32_t i=0;i<length;i++)
    {
        B[i]=((float)A[i]-z1)/(z2-z1);
    }
    return;
}
void normalisation_ROI (unsigned short int *&A,float *&B ,int start_dst,int start_src,int width,int height, float z1,float z2)
{
    for (auto i=0;i<width*height;i++)
    {
        B[start_dst+i]=((float)A[start_src+i]-z1)/(z2-z1);
    }
    return;
}
void display(std::string handle,cv::Mat matIm,unsigned short *im_src,float *&im_dst,struct display_roi *disp_roi)
{
    float z1,z2;
//    if (text)
//    {
//        char text[50];
//        sprintf(text,"Image: %d",disp_roi->num);
//        putText(matIm, text,cv::Point(15,15),cv::FONT_HERSHEY_COMPLEX_SMALL, 4.0, cv::Scalar(200,200,250), 1);
//    }
    cdl_zscale(im_src,disp_roi->buff_width,disp_roi->buff_height,16,&z1,&z2,0.25,1000,1000);
    //std::cout<<"z: "<<z1<<" "<<z2<<std::endl;



    if (disp_roi->buff_height!=disp_roi->buff_width)
    {
        int start_src = 0;
        std::fill(im_dst, im_dst + disp_roi->buff_width*disp_roi->buff_width, 0);
        for (auto i=1;i<=disp_roi->count;i++)
        {
            normalisation_ROI(im_src,im_dst,disp_roi->offsety[i-1]*disp_roi->width[i-1],start_src,disp_roi->width[i-1],disp_roi->height[i-1],z1,z2);
            start_src+=disp_roi->height[i-1]*disp_roi->width[i-1];

        }
    }

    else
    {   std::fill(im_dst, im_dst + disp_roi->buff_height*disp_roi->buff_width, 0);
        normalisation(im_src,im_dst,disp_roi->buff_height*disp_roi->buff_width,z1,z2);
    }

    cv::imshow(handle.c_str(),matIm);
    cv::waitKey(1);
}

int setupROI(struct display_roi *disp_roi)
{
    //get the size of the incomming buffer
    memset(disp_roi->offsetx,0,8);
    memset(disp_roi->height,0,8);
    memset(disp_roi->offsety,0,8);
    memset(disp_roi->width,0,8);

    disp_roi->count = 0;
    disp_roi->ro_mode = 0;
    if (ncCamGetSize(myCam,&disp_roi->buff_width,&disp_roi->buff_height)!=0)
    {
        std::cout<<"unable to retrieve the widht and height of the image buffer"<<std::endl;
        return -1;
    }
    else
    {
        std::cout<<"width: "<<disp_roi->buff_width<<", heigth: "<<disp_roi->buff_height<<std::endl;
    }
    //get the readout mode of the camera
    int VerHz,HorHz;
    enum Ampli	ncAmpliNo;
    char Amp[32];

    if (ncCamGetCurrentReadoutMode(myCam,&disp_roi->ro_mode,&ncAmpliNo,Amp,&VerHz,&HorHz)!=0){ std::cout<<"Unable to read the readout mode."<<std::endl; return -1;}
    else{std::cout<<"readout mode: "<<disp_roi->ro_mode<<std::endl;}
    if (disp_roi->ro_mode>=4 && disp_roi->ro_mode<=11)//conventioenl
    {
        disp_roi->count = 1;

        disp_roi->offsetx[0]=0;
        disp_roi->offsety[0]=0;
        disp_roi->height[0]=disp_roi->buff_height;
        disp_roi->width[0]=disp_roi->buff_width;
    }
    else//EM
    {
        if (ncCamGetMRoiCount(myCam,&disp_roi->count)!=0){std::cout<<"Unable to communicate with the camera"<<std::endl;}
        int w,h,offx,offy;
        for (auto i=0;i<disp_roi->count;i++)
        {
            if (ncCamGetMRoiSize(myCam,i,&w,&h)!=0){std::cout<<"Unable to communicate with the camera"<<std::endl;}
            if (ncCamGetMRoiPosition(myCam,i,&offx,&offy)!=0){std::cout<<"Unable to communicate with the camera"<<std::endl;}
            disp_roi->height[i]=h;
            disp_roi->width[i]=w;
            disp_roi->offsety[i]=offy;
            disp_roi->offsetx[i]=offx;

        }

    }
    std::cout<<"buffer size : height="<<disp_roi->buff_height<<" width="<<disp_roi->buff_width<<std::endl;
    std::cout<<"listing "<<disp_roi->count<<" roi"<<std::endl;
    for (int i=0;i<disp_roi->count;i++)
    {   std::cout<<""<<std::endl;
        std::cout<<"roi # "<<i+1<<std::endl;
        std::cout<<"offset (x,y) :"<<disp_roi->offsetx[i]<<" , "<< disp_roi->offsety[i]<<std::endl;
        std::cout<<"size (width x heigth)"<<disp_roi->width[i]<<" x "<<disp_roi->height[i]<<std::endl;
    }
    return 0;
}
void copy_array(unsigned short int *im,unsigned short int *&im2,uint32_t length,int row)
{
    for (uint32_t i=0;i<length;i++)
    {
        //im2[length-1-i]=im[i];//pour inverser l'image NS-EO
        //i%r+l-(r)*(i/r+1)
        im2[i%row+length-row*(i/row+1)]=im[i];//inversion NS
    }
}
void closeWindow(std::string handle)
{

    cv::destroyWindow(handle);
}




