#include <opencv2/highgui.hpp>
#include <iostream>
#include <stdint.h>
#include "imgproc.hpp"
#include<opencv2/opencv.hpp>
using namespace cv;


cv::Mat memVidSetup(float im[], uint size, std::string handle)
{
    cv::Mat matIm = cv::Mat(size, size, CV_32F  ,im);
    cv::namedWindow(handle.c_str(),cv::WINDOW_FREERATIO);
    return matIm;
}

void display(std::string handle,cv::Mat matIm,int num)
{   cv::normalize(matIm,matIm,cv::NORM_MINMAX);
    char text[50];
    sprintf(text,"Image: %d",num);
    putText(matIm, text,cv::Point(15,15),cv::FONT_HERSHEY_COMPLEX_SMALL, 0.8, cv::Scalar(200,200,250), 1);
    cv::imshow(handle.c_str(),matIm);
    cv::waitKey(1);

    //std::cout<<matIm<<std::endl;

}

void copyArr(unsigned short *A, unsigned short *B, int length)
{
    for (int i=0;i<length;i++)
    {
        B[i]=A[i];
    }



}
int randomArray(uint32_t A[])
{   int lim = 128*128;
    for (int i=0 ; i<lim;i++)
    {
        A[i]=rand() % 65000;
    }

return 0;
}

void normalisation (unsigned short int A[],float B [] ,uint size, float z1,float z2)
{
    for (int i=0;i<size;i++)
    {
        B[i]=(A[i]-z1)/(z2-z1);
    }
    return;
}


