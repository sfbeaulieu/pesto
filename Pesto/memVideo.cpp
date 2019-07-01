#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdint.h>
#include "imgproc.hpp"
#include<opencv2/opencv.hpp>
#include "zscale.h"
#include "nc_driver.h"
#include "structure.h"

using namespace cv;

extern NcCam myCam;
extern display_roi disp_roi;

cv::Mat memVidSetup(float *&im, size_t size, std::string handle)
{   //Description: Setup of the opencv mat object. Will return a Mat object
    cv::startWindowThread();//to fix the bug where the window does not resize if it's killed inbetween to image sequence
    cv::Mat matIm = cv::Mat(size, size, CV_32F  ,im);
    // we will eventually need to insert bits of code here to be able to resize the window.
    //It can be potentiolly problematic if the size of the detector increase or the resolution
    //of the screen is smaller
    cv::namedWindow(handle.c_str(),cv::WINDOW_FREERATIO);
    return matIm;
}

void stats(unsigned short int A[], size_t length, double *mean, double *std)
{   //Description: fast algorithm to compute the mean and the standard deviation in one loop for unsigned short array

    double sum=0;
    double sq_sum=0;
    for (auto i=0;i<length;i++)
    {
        sum+=A[i];
        sq_sum += A[i] * A[i];
    }
    *mean = sum/(double)length;
    double variance = sq_sum / length - *mean * *mean;
    *std = sqrt(variance);
}

void stats(float A[], size_t length, double *mean, double *std)
{   //Description: fast algorithm to compute the mean and the standard deviation in one loop for float arrray
    double sum=0;
    double sq_sum=0;
    for (auto i=0;i<length;i++)
    {
        sum+=A[i];
        sq_sum += A[i] * A[i];
    }

    *mean = sum/(double)length;
    double variance = sq_sum / length - *mean * *mean;
    *std = sqrt(variance);
}

void normalisation (unsigned short int *&A, float *&B ,  size_t length, float z1, float z2)
{   //Description: deprecated!!!

    for (auto i=0;i<length;i++)
    {
        B[i]=(static_cast<float>(A[i])-z1)/(z2-z1);
    }
    return;
}

void normalisation_ROI (unsigned short int *&A,float *&B ,int start_dst,int start_src,int width,int height, float z1,float z2)
{   //Description: This function applies the normalization to the unsigned short array to the float array. z1 and z2
    //are determine by cdl_zscale().
    for (auto i=0;i<width*height;i++)
    {
        B[start_dst+i]=(static_cast<float>(A[start_src+i])-z1)/(z2-z1);
    }
    return;
}

void display(std::string handle,cv::Mat matIm,unsigned short *im_src,float *&im_dst,struct display_roi *disp_roi)
{   //Description: this function will display an image buffer with an opencv window.
    //The handle of of the mat object is requiered hence memVidSetup() function must be called prior to this one.
    //the unsigned short array contains the image buffer that comes from the camera. The destination im_dst
    //is the array that will be displayed. The normalisation_ROI will be called to normalize using zscale (unsigned short become float)
    //The structure display_roi is needed to know how much of the image buffer we need to read.
    float z1,z2;

    //we calculate the zscale using iraf algorythm
    cdl_zscale(im_src,disp_roi->buff_width,disp_roi->buff_height,16,&z1,&z2,0.25,1000,1000);


    if (disp_roi->buff_height!=disp_roi->buff_width)//means that it is not a full frame image
    {
        int start_src = 0;//starting element, will be incremented in function of the size of the ROIs
        //we erase whatever was in the destination buffer (we don't want to display old image if something goes wrong)
        //Also, it assure that inbetween the ROI the value is set to zero.
        std::fill(im_dst, im_dst + disp_roi->buff_width*disp_roi->buff_width, 0);
        //we will normalize the ROI spearately to fill the space between the ROI with zero-value flux
        for (auto i=1;i<=disp_roi->count;i++)
        {
            normalisation_ROI(im_src,im_dst,disp_roi->offsety[i-1]*disp_roi->width[i-1],start_src,disp_roi->width[i-1],disp_roi->height[i-1],z1,z2);
            start_src+=disp_roi->height[i-1]*disp_roi->width[i-1];
        }
    }
    // it is a full frame
    else
    {   //we erase whtever was in the destination array
        std::fill(im_dst, im_dst + disp_roi->buff_height*disp_roi->buff_width, 0);
        normalisation(im_src,im_dst,disp_roi->buff_height*disp_roi->buff_width,z1,z2);
    }

    cv::imshow(handle.c_str(),matIm);
    //The function waitKey waits for a key event infinitely (when delay≤0 ) or for delay
    //milliseconds, when it is positive. Since the OS has a minimum time between switching
    //threads, the function will not wait exactly delay ms, it will wait at least delay ms,
    //depending on what else is running on your computer at that time.
    //It returns the code of the pressed key or -1 if no key was pressed before the specified
    //time had elapsed.
    cv::waitKey(1);//wait for 1ms
}

int setupROI(struct display_roi *disp_roi)
{   //Description: This function setup the display_roi structure for display. One of the perticullarities
    //of the NuVu camera is that it can read sub-section of the detector. This my be problematic for the
    //display loop since the size of the image buffer depend on the ROI. In this function, we read several
    //internal parameters of the NuVu camera in order to know the size of the incoming image buffer.
    //get the size of the incomming buffer
    //Return 0 if successfull, -1 if an error occurs.

    //erase the integers array of the display_roi structure.
    memset(disp_roi->offsetx,0,8);
    memset(disp_roi->height,0,8);
    memset(disp_roi->offsety,0,8);
    memset(disp_roi->width,0,8);
    //set the roi count and read out mode to 0
    disp_roi->count = 0;
    disp_roi->ro_mode = 0;

    if (ncCamGetSize(myCam,&disp_roi->buff_width,&disp_roi->buff_height)!=0)
    {
        std::cout<<"unable to retrieve the width and height of the image buffer"<<std::endl;
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
    //if the readout mode is conventioenl
    if (disp_roi->ro_mode>=4 && disp_roi->ro_mode<=11)
    {
        disp_roi->count = 1;
        disp_roi->offsetx[0]=0;
        disp_roi->offsety[0]=0;
        disp_roi->height[0]=disp_roi->buff_height;
        disp_roi->width[0]=disp_roi->buff_width;
    }
    else//The readout mode is EM
    {
        if (ncCamGetMRoiCount(myCam,&disp_roi->count)!=0){std::cout<<"Unable to communicate with the camera"<<std::endl;return -1;}
        int w,h,offx,offy;
        for (auto i=0;i<disp_roi->count;i++)
        {
            if (ncCamGetMRoiSize(myCam,i,&w,&h)!=0){std::cout<<"Unable to communicate with the camera"<<std::endl;return -1;}
            if (ncCamGetMRoiPosition(myCam,i,&offx,&offy)!=0){std::cout<<"Unable to communicate with the camera"<<std::endl; return -1;}
            disp_roi->height[i]=h;
            disp_roi->width[i]=w;
            disp_roi->offsety[i]=offy;
            disp_roi->offsetx[i]=offx;
         }
    }

    std::cout<<"buffer size : height="<<disp_roi->buff_height<<" width="<<disp_roi->buff_width<<std::endl;
    std::cout<<"listing "<<disp_roi->count<<" roi"<<std::endl;

    //listing all the ROI
    for (int i=0;i<disp_roi->count;i++)
    {   std::cout<<""<<std::endl;
        std::cout<<"roi # "<<i+1<<std::endl;
        std::cout<<"offset (x,y) :"<<disp_roi->offsetx[i]<<" , "<< disp_roi->offsety[i]<<std::endl;
        std::cout<<"size (width x heigth)"<<disp_roi->width[i]<<" x "<<disp_roi->height[i]<<std::endl;
    }
    return 0;
}

void copy_array(unsigned short int *im,unsigned short int *&im2,size_t length)
{   //Description: This function copy and array of type unsigned short int.
    //This is used to copy the image buffer into another array for processing.
    for (uint32_t i=0;i<length;i++)
    {
        //im2[length-1-i]=im[i];//pour inverser l'image NS-EO
        //i%r+l-(r)*(i/r+1)
        im2[i%row+length-row*(i/row+1)]=im[i];//inversion NS
    }
}

void closeWindow(std::string handle)
{//Description: Close a openCv MAT handle.

    cv::destroyWindow(handle);
}




