// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.


/**
 * @file cameraAnglesDynamicPub.cpp
 *
 */

#include "../dynddspub/ScanDynamicPub.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/DataWriterListener.hpp>

#include <aditof/camera.h>
#include <aditof/frame.h>
#include <aditof/system.h>
#ifndef JS_BINDINGS
#include <glog/logging.h>
#else
#include <aditof/log_cout.h>
#endif

#include <iostream>
#include <string>

#include <ctime>
#include <chrono>

using namespace eprosima::fastdds::dds;

using namespace aditof;


#define INDEX_MemID 0
#define DEPTH67_ARRAY_MemID 1
#define CAMERA_RANGE_MemID 2

int main(int argc, char** argv)
{
    std::cout << "Starting publisher." << std::endl;

    const int frameHeight = static_cast<int>(480);
    const int frameWidth = static_cast<int>(640);
    const int pixelStart_HFOV = static_cast<int>(frameWidth * ((frameHeight/2)-1));//Pixelstart of center in the horizontal plane frame is 640x239
    const int pixelStart_VFOV = static_cast<int>((frameWidth/2)-1);//Pixelstart of center in the vertical plane frame is 319 = (640/2)-1
    const double angleOffset_HFOV = (87.0/static_cast<double>(frameWidth-1));//angle for each pixel in the horizontal plane
    const double angleOffset_VFOV = (67.0)/static_cast<double>(frameHeight-1);//angle for each pixel in the vertical plane

    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    uint32_t samples_index = 0;
    uint16_t depth_in_middle[frameHeight];
    double angle_in_middle[frameHeight];
    uint16_t depth_in_middle_67[67];
    unsigned int count = 1;
    unsigned int j = 0;
        
    Status status = Status::OK;

    System system;
    
    aditof::Frame frame;
    uint16_t *data_depth;
    uint16_t *data_ir;
    FrameDetails fDetails;
    std::string selectedMode;
    const int smallSignalThreshold = 50;
    
    std::vector<std::shared_ptr<Camera>> cameras;
    
    // Get the cameras
    system.getCameraList(cameras);
    
    // Initialize first TOF camera
    auto camera = cameras.front();
    status = camera->initialize();
    
    // Choose the frame type the camera should produce
    std::vector<std::string> frameTypes;
    camera->getAvailableFrameTypes(frameTypes);
    status = camera->setFrameType(frameTypes.front());
    
    //Get the available modes of the camera
    std::vector<std::string> modes;
    camera->getAvailableModes(modes);
    /*
    //Place the camera in a specific mode, that the user choosed
    std::cout << "Please enter the mode in which you want to operate the camera (near or medium)";
    std::cin >> selectedMode;
    
    if(selectedMode == "near" || selectedMode == "medium")
    { 
        std::cout << "The selected mode is: " << selectedMode << std::endl;
        status = camera->setMode(selectedMode);
    } else {  
        std::cout << "Incorrect mode. The default mode near is selected instead" << std::endl;
         status = camera->setMode("near");
    }*/
    
    //For now, we will only use medium mode
    status = camera->setMode("medium");
        
    mypub->init("../xmls/cameraAngle.xml", "FrameCameraAngle", "FrameCameraAngleTopic");
    
    camera->setControl("noise_reduction_threshold", std::to_string(smallSignalThreshold));

    while(1)
    {        
        auto startTime = std::chrono::steady_clock::now();
        
        auto startTime1 = std::chrono::steady_clock::now();
        status = camera->requestFrame(&frame);
        auto endTime1 = std::chrono::steady_clock::now();;
        auto differenceTime1 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime1 - startTime1).count();
        std::cout << "Time difference1: " << differenceTime1 << std::endl;
        
        //status = frame.getData(FrameDataType::IR, &data_ir);
        auto startTime2 = std::chrono::steady_clock::now();
        status = frame.getData(FrameDataType::DEPTH, &data_depth);
        auto endTime2 = std::chrono::steady_clock::now();;
        auto differenceTime2 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime2 - startTime2).count();
        std::cout << "Time difference2: " << differenceTime2 << std::endl;
        
        auto startTime3 = std::chrono::steady_clock::now();
        frame.getDetails(fDetails);
        auto endTime3 = std::chrono::steady_clock::now();;
        auto differenceTime3 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime3 - startTime3).count();
        std::cout << "Time difference3: " << differenceTime3 << std::endl;

        /*calculate central vertical frame (actually horizontal)*/
        angle_in_middle[0] = 0;
        depth_in_middle[0] = data_depth[pixelStart_VFOV];
        for (unsigned int i = 1; i < frameHeight; ++i){
            angle_in_middle[i] = angle_in_middle[i-1] + angleOffset_VFOV;
            depth_in_middle[i] = data_depth[(frameWidth * i) + (pixelStart_VFOV)];
            std::cout << "Angle information in VFOV: " << angle_in_middle[i] << " with index: " << i << " RECEIVED" << std::endl;
        }
        /*round to full degree*/

        angle_in_middle[0] = 0;
        depth_in_middle[0] = data_depth[pixelStart_VFOV];
        
        j = 0;
        depth_in_middle_67[j] = depth_in_middle[0];
        count = 1;//The of the pixel start must also be taken into account when calculating the mean value
        
        for (unsigned int i = 1; i < frameHeight; ++i){
            angle_in_middle[i] = angle_in_middle[i-1] + angleOffset_VFOV;
            depth_in_middle[i] = data_depth[(frameWidth * i) + (pixelStart_VFOV)];
            if (static_cast<int>(angle_in_middle[i]) == j)
            {
                //Calculate the sum of the depth frame data with the same pre-decimal number of the angle
                depth_in_middle_67[j] = depth_in_middle_67[j] + depth_in_middle[i];
                ++count;
            } else {
                //calcule the mean value
                std::cout << "Sum value: " << depth_in_middle_67[j] << " with count: " << count << std::endl;
                depth_in_middle_67[j] = static_cast<int>(static_cast<double>(depth_in_middle_67[j]) / static_cast<double>(count));
                std::cout << "Mean value depth frame: " << depth_in_middle_67[j] << " with index: " << j << " RECEIVED" << std::endl;
                ++j;
                count = 1;//reset the value count of the elements
                depth_in_middle_67[j] = depth_in_middle[i];//Save the next depth frame in the nextrow of the array
                std::cout << "Center frame: " << depth_in_middle_67[j] << " with count: " << count << " RECEIVED" << std::endl << std::endl;
            }
        }
        mypub->putData_uint16_array(depth_in_middle_67, 67, DEPTH67_ARRAY_MemID);

        
        auto startTime5 = std::chrono::steady_clock::now();
        aditof::CameraDetails cameraDetails;
        camera->getDetails(cameraDetails);
        mypub->putData_uint16_value(cameraDetails.depthParameters.maxDepth, CAMERA_RANGE_MemID);
        auto endTime5 = std::chrono::steady_clock::now();;
        auto differenceTime5 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime5 - startTime5).count();
        std::cout << "Time difference5: " << differenceTime5 << std::endl;
        
        auto startTime6 = std::chrono::steady_clock::now();                     
        if(mypub->publish())
        { 
            auto endTime6 = std::chrono::steady_clock::now();;
            auto differenceTime6 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime6 - startTime6).count();
            std::cout << "Time difference6: " << differenceTime6 << std::endl;
            
            auto startTime9 = std::chrono::steady_clock::now(); 
            //mypub->adiFrame_.index(mypub->adiFrame_.index() + 1);
            samples_index++;
            mypub->putData_uint32_value(samples_index, INDEX_MemID); //TODO: index is now being counted here
            auto endTime9 = std::chrono::steady_clock::now();;
            auto differenceTime9 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime9 - startTime9).count();
            std::cout << "Time difference9: " << differenceTime9 << std::endl;
        }
        
        
        auto endTime = std::chrono::steady_clock::now();;
        auto differenceTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        std::cout << "Time difference gesamt: " << differenceTime << std::endl << std::endl;

    }

    delete mypub;
    return 0;
}
