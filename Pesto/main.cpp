#include "nc_driver.h"
#include <iostream>
#include <pthread.h>
#include "thread.h"
#include <ctype.h>
#include "var.h"
#include "socket.h"
#include "utils.h"
#include <thread>
#include "fw.h"

using namespace std;



int main()
{
//:::::::::::::::::::::::::::::::: Mode Development :::::::::::::::::::::::::::;
developpement=1;//0-> release on lyra, 1->mode development
//:::::::::::::::::::::::::::::::: Mode Development :::::::::::::::::::::::::::;




//start everything
initializeSocket();//initialization des sockets
initVariable(&param,&detParam);//must be called before openCam()
openCam(&myCam,&param,&detParam);//open the connection with the camera




//create threads
std::thread tTCS(tcs,&tcs_var);
std::thread tCCDT(ccdTemp,&detParam);
std::thread tCCDLoop(getTempLoop,&detParam);
std::thread tMeteo(meteoThread,&meteo);
std::thread tisACQ(isAcqOnGoing,&isInAcq);
std::thread tStop(threadStop,&loop,&isInAcq);

//start the threads
tTCS.detach();
tCCDT.detach();
tCCDLoop.detach();
tMeteo.detach();
tisACQ.detach();
tStop.detach();


//cout<<param.maxGain<<endl;
while(1)
{   if(read_socket(&Case,caseID)!=0){break;}
    switch(atoi(Case.c_str())){
        case 1:
    {
            if(read_socket(&buff1,buff1ID)!=0)
            {   log.writetoVerbose("unable to read the argument buffer");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            if(read_socket(&buff2,buff2ID)!=0)
            {   log.writetoVerbose("unable to read the argument buffer");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            if(read_socket(&buff3,buff3ID)!=0)
            {   log.writetoVerbose("unable to read the argument buffer");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            if(read_socket(&buff4,buff4ID)!=0)
            {   log.writetoVerbose("unable to read the argument buffer");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }

            if (developpement==0)
            {
                preACQ(&detParam);
            }


            loop=1;//open the acquisition loop
            detParam.path=create_dossier(stoi(buff4),&detParam);



            mode=stoi(buff1);
            detParam.nbrExp=stoi(buff2);

            inc=get_inc(&param);
            std::thread tACQ(acquisition,&mode,&loop,&inc);

            if(stoi(buff3)==1)
            {
                tACQ.join();
            }
            else if(stoi(buff3)==2)
            {
                tACQ.detach();
            }
            else
            {
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
            }

            sprintf(cWRITE,"%d",0);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        case 2://set exposure time
        {
            if(read_socket(&buff1,buff1ID)!=0)
            {   log.writetoVerbose("unable to read the argument buffer");
                break;
            }
            detParam.exposureTime=stod(buff1);

            error+=ncCamSetExposureTime(myCam, detParam.exposureTime);
            error+=ncCamSetWaitingTime(myCam, 0);
            error+=ncCamGetReadoutTime(myCam, &detParam.readoutTime);
            error+=ncCamSetTimeout(myCam, (int)(0.1 * detParam.exposureTime + detParam.exposureTime + detParam.readoutTime ) );
           // if (error==0){sprintf(cWRITE,"%d",0);}
           // else {sprintf(cWRITE,"%d",-1);}
            sprintf(cWRITE,"%d",(error==0) ? 0 : -1);

            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
    case 3://request an exit of the acquisition thread loop
    {
        loop=0;
        while(isInAcq){
            cout<<"waiting for the end of the acquisition..."<<endl;
            delay(1000);
        }
        sprintf(cWRITE,"%d",0);
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);
        break;

    }
    case 4:// close the camera
    {
        if(ncCamSetShutterMode( myCam, CLOSE )!=0){log.writetoVerbose("Unable to close the camera shutter");}
        loop=0;//request an exit of the acquisition thread loop
        while(isInAcq)
        {
            cout<<"waiting for the end of the acquisition..."<<endl;
            delay(1000);
        }
        if (ncCamClose(myCam) != 0)
        {log.writetoVerbose("Unable to close the camera");
        //destroy all the thread

            return -1;}
        cout<<"Bye bye!"<<endl;
        //destroy all the thread
        return 0;

    }
    case 5://Set Temperature
    {   biasOK=0;
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (SET_CCDT)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        detParam.ccdTemp=stod(buff1);
        if (detParam.ccdTemp>=-20 || detParam.ccdTemp<=-90)
        {   log.writetoVerbose("The requested temperature is out of range");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {
            if (ncCamSetTargetDetectorTemp(myCam,detParam.ccdTemp)!=0){
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            else
            {   sprintf(cWRITE,"%d",0);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;

            }
        }
    }
    case 6://get temperature
    {   if (ncCamGetDetectorTemp(myCam,&detParam.ccdTemp)!=0)
        {
            log.writetoVerbose("Failed to fetch the ccd temperature (GET_CCDT)");
            break;
        }
        else
        {   sprintf(cWRITE,"%f",detParam.ccdTemp);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;

        }

    }
    case 7://set object name
    {
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writeto("unable to read the argument buffer (SET_OBJ)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {
            detParam.object_name=buff1;
            sprintf(cWRITE,"%d",0);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;
    }
    case 8://set Time stamp
    {   biasOK=0;
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writeto("unable to read the argument buffer (SET_TSTAMP)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {
            if (selectTimeStamp(&myCam,stoi(buff1))!=0)
            {
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            else
            {
                sprintf(cWRITE,"%d",0);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
        }
        break;
    }
    case 9://set the bias
    {   int out;

        out= applyBias(&myCam,&param,&detParam);
        sprintf(cWRITE,"%d",out);
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);
        break;
    }
    case 10://set the EM gain
    {   int gain;
        biasOK=0;
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writeto("unable to read the argument buffer (SET_GAIN)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        gain = stoi(buff1);
        if (gain <=param.maxGain && gain>=param.minGain)
        {
            if (ncCamSetCalibratedEmGain(myCam,gain)!=0)
            {
                log.writeto("unable to set the EM GAIN");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            else
            {

                sprintf(cWRITE,"%d",0);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
            }
        }
        else
        {
            log.writeto("The requested gain is out of range");
            sprintf(cWRITE,"%d",-2);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;
    }

    case 11://get the EM calibrated gain
    {   int gain;
        if (ncCamGetCalibratedEmGain(myCam,1,&gain)!=0)
        {
            log.writeto("Unable to retriev the gain");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {

            sprintf(cWRITE,"%d",gain);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);

        }
        break;
    }
    case 12://open the shutter
    {
        if(ncCamSetShutterMode(myCam,OPEN)!=0)
        {
            log.writeto("Unable to open the shutter");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {

            sprintf(cWRITE,"%d",0);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;
    }
    case 13://CLOSE the shutter
    {
        if(ncCamSetShutterMode(myCam,CLOSE)!=0)
        {
            log.writeto("Unable to open the shutter");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {

            sprintf(cWRITE,"%d",0);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;
    }
    case 14: //set the readout mode
    {   int ro;
        biasOK=0;
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writeto("unable to read the argument buffer (SET_RO)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        ro = stoi(buff1);
        if (ro<=20 && ro>=1)
        {
            if (ncCamSetReadoutMode(myCam, ro)!=0)
            {
                log.writeto("unable to set the readout mode");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
            }
            else
            {   ROIcount = 0;
                sprintf(cWRITE,"%d",0);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
            }
        }
        else
        {
            log.writeto("read out mode is out of range.\n");
            sprintf(cWRITE,"%d",-2);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;

    }
    case 15:// filter wheel
    {
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (filter wheel).\n");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }

        if (overideFW!=1){
        if (isdigit(buff1.c_str()[0]))
        {

            sprintf(cWRITE,"%d",fw.position(stoi(buff1)));
            if (atoi(cWRITE)==-1)
            {
                log.writetoVerbose("Failed to set the filter.\n");
                detParam.current_filter="N/A";
            }
            else
            {
                switch(atoi(cWRITE))
                {
                    case 1:
                    {
                        detParam.current_filter=param.FW1;
                        break;
                    }
                case 2:
                {
                    detParam.current_filter=param.FW2;
                    break;
                }
                case 3:
                {
                    detParam.current_filter=param.FW3;
                    break;
                }
                case 4:
                {
                    detParam.current_filter=param.FW4;
                    break;
                }
                case 5:
                {
                    detParam.current_filter=param.FW5;
                    break;
                }

                }//switch
            }


        }
        else if (!isdigit(buff1.c_str()[0]))
        {
            if (buff1=="H")
            {

                sprintf(cWRITE,"%d",fw.home());
                if (atoi(cWRITE)==-1)
                {
                    log.writetoVerbose("Failed to set the filter.\n");
                    detParam.current_filter="N/A";
                }
                else
                {
                    detParam.current_filter=param.FW0;
                }
            }
            if (buff1=="?")
            {
                sprintf(cWRITE,"%d",fw.get_position());
                if (atoi(cWRITE)==-1)
                {
                    log.writetoVerbose("Unable to get the position of the filter wheel.\n");
                    detParam.current_filter="N/A";
                }
            }
            if (buff1=="+")
            {

                sprintf(cWRITE,"%d",fw.increment());
                if (atoi(cWRITE)==-1)
                {
                    log.writetoVerbose("Failed to set the filter.\n");
                    detParam.current_filter="N/A";
                }
                else
                {
                    detParam.current_filter+="+";
                }
            }
        }
        else
        {
            sprintf(cWRITE,"%d",-1);
        }
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);
        }//if override
        else
        {
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;
    }
    case 16://connect a device FW()
    {   if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (connect device)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }


        sprintf(cWRITE,"%d",fw.connection(buff1));
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);

        if (WRITE=="0")
        {
            log.writeto("A new filter device has been connected succesfully");
        }
        else
        {
            log.writeto("Failed to connect a new filter device");
        }
        break;

    }
    case 17://close connection
    {
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (connect device)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;

        }


        sprintf(cWRITE,"%d",fw.closeConnection());
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);
        if (WRITE!="0")
        {
            log.writetoVerbose("Failed to close to disconnect the filter server");
        }
        break;
    }
    case 18://set the name of the observateur
    {
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (observateur)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;

        }
        else
        {
            detParam.observateur=buff1;
            log.writetoVerbose("new Observer set succesfully");
            sprintf(cWRITE,"%d",0);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }
        break;
    }
    case 19://set the telescope operator
    {
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (operator)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {
            detParam.Operator=buff1;
            log.writetoVerbose("new operator set succesfully");
            sprintf(cWRITE,"%d",0);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
        }

        break;
    }
    case 20://send readout mode
    {   int ro_mode,VerHz,HorHz;
        enum Ampli	ncAmpliNo;
        char Amp[32];
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (send readout mode)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        if (ncCamGetCurrentReadoutMode(myCam,&ro_mode,&ncAmpliNo,Amp,&VerHz,&HorHz)!=0)
        {
            log.writetoVerbose("Failed to retrieve the readout mode");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        else
        {
            sprintf(cWRITE,"%d",ro_mode);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            log.writeto("Readout mode is: "+std::to_string(ro_mode));
        }

        break;
    }
    case 21://test de tcs (print tcs variable)
    {
        sprintf(cWRITE,"%d",biasOK);
        if (biasOK)
        {
            //cout<<"The bias is up to date"<<endl;
        }
        else
        {
            //cout<<"The bias is out of date"<<endl;
        }
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);

        break;
    }
    case 22://ROI
    {   int ro_mode,VerHz,HorHz,fullWidth,fullHeight;
        enum Ampli	ncAmpliNo;
        char Amp[32];
        char dummy[10];
        int offsetY;
        int roiHeight;
        biasOK=0;
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (ROI)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        if(read_socket(&buff2,buff2ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (ROI)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        offsetY = stoi(buff1);
        roiHeight = stoi(buff2);


        if (isInAcq!=0)//the go loop should not be running while the roi is set
        {
            if (ncCamAbort(myCam)!=0)
            {
                log.writetoVerbose("Unable to abort the camera\n");
                sprintf(cWRITE,"%d",-1);
                WRITE = cWRITE;
                read2way(repID,&WRITE,WRITE);
                break;
            }
            else
            {
                isInAcq=0;
            }

        }

       if (ncCamGetCurrentReadoutMode(myCam,&ro_mode,&ncAmpliNo,Amp,&VerHz,&HorHz)!=0)
       {std::cout<<"Problème ligne 663"<<std::endl;
           sprintf(cWRITE,"%d",-1);
           WRITE = cWRITE;
           read2way(repID,&WRITE,WRITE);
           log.writetoVerbose("unable to query the camera");
           break;

       }
       //mode conventionel
       if (ro_mode>=4 && ro_mode<=11)
       {
            if (ncCamGetMaxSize(myCam, &fullWidth, &fullHeight)!=0){
                std::cout<<"Problème ligne 675"<<std::endl;
                sprintf(dummy,"-1");}
            else if (ncCamSetMRoiSize(myCam,0,fullWidth,roiHeight)!=0){
                std::cout<<"Problème ligne 678"<<std::endl;
                sprintf(dummy,"-1");}
            else if (ncCamSetMRoiPosition(myCam,0,0,offsetY)!=0){
                std::cout<<"Problème ligne 681"<<std::endl;
                sprintf(dummy,"-1");}
            else if (ncCamMRoiApply( myCam )!=0){
                std::cout<<"Problème ligne 684"<<std::endl;
                sprintf(dummy,"-1");}
            else {sprintf(dummy,"0");}
        }

       //mode EM
       else
           //cout<<"EM mode"<<endl;
       {
            if (ROIcount==0)
            {
                if (ncCamGetMaxSize(myCam, &fullWidth, &fullHeight)!=0){sprintf(dummy,"-1");}
                else if (ncCamSetMRoiSize(myCam,0,fullWidth,4)!=0){sprintf(dummy,"-1");}
                else if (ncCamMRoiApply( myCam )!=0){sprintf(dummy,"-1");}
                else {sprintf(dummy,"0");}
                ROIcount++;
            }


                if (ncCamAddMRoi( myCam,0,offsetY, fullWidth, roiHeight)!=0){sprintf(dummy,"-1");}
                else if (ncCamMRoiApply( myCam )!=0){sprintf(dummy,"-1");}
                else {ROIcount++;
                    sprintf(dummy,"0");}


       }

       WRITE = dummy;
       read2way(repID,&WRITE,WRITE);
       log.writetoVerbose("ROI created succesfully");


        break;
    }
    case 23://get exposure time
    {   double current_exposureT;
        double current_readoutT;
        if (ncCamGetExposureTime(myCam, 1, &current_exposureT)!=0 || ncCamGetReadoutTime(myCam, &current_readoutT)!=0)
        {
            sprintf(cWRITE,"%d",-1);
            log.writetoVerbose("unable to query the camera");
        }

        else
        {
            if (current_exposureT<current_readoutT)
            {
                sprintf(cWRITE,"%f",current_readoutT);
            }
            else
            {
                sprintf(cWRITE,"%f",current_exposureT);
            }

        }


        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);



        break;
    }
    case 24://get readout mode
    {

        int ro_mode,VerHz,HorHz;
                enum Ampli	ncAmpliNo;
                char Amp[32];

        if (ncCamGetCurrentReadoutMode(myCam,&ro_mode,&ncAmpliNo,Amp,&VerHz,&HorHz)!=0)
        {
            sprintf(cWRITE,"%d",-1);
        }
        else {
            sprintf(cWRITE,"%d",ro_mode);
        }
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);
        break;
    }
    case 25://overide filterposition
    {
        if(read_socket(&buff1,buff1ID)!=0)
        {   log.writetoVerbose("unable to read the argument buffer (ROI)");
            sprintf(cWRITE,"%d",-1);
            WRITE = cWRITE;
            read2way(repID,&WRITE,WRITE);
            break;
        }
        overideFW=1;
        detParam.current_filter=buff1;
        //detParam.current_filter
        sprintf(cWRITE,"%d",0);
        WRITE = cWRITE;
        read2way(repID,&WRITE,WRITE);
        break;
    }//case 25
    case 26://stop overriding the fw
    {
        overideFW=0;
        break;
    }
    }//switch statement
}

   return 0;
}
