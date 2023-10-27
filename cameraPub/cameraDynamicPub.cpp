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
 * @file cameraDynamicPub.cpp
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
#define IR_ARRAY_MemID 1
#define DEPTH_ARRAY_MemID 2
#define CAMERA_RANGE_MemID 3

int main(int argc, char** argv)
{
    std::cout << "Starting publisher." << std::endl;

    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    uint32_t samples_index = 0;
        
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
        
    mypub->init("../xmls/camera.xml", "FrameCamera", "FrameCameraTopic");
    
    camera->setControl("noise_reduction_threshold", std::to_string(smallSignalThreshold));

    while(1)
    {        
        auto startTime = std::chrono::steady_clock::now();
        
        auto startTime1 = std::chrono::steady_clock::now();
        status = camera->requestFrame(&frame);
        auto endTime1 = std::chrono::steady_clock::now();;
        auto differenceTime1 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime1 - startTime1).count();
        std::cout << "Time difference1: " << differenceTime1 << std::endl;
        
        status = frame.getData(FrameDataType::IR, &data_ir);
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
        
        auto startTime4 = std::chrono::steady_clock::now();
        mypub->putData_uint16_array(data_ir, fDetails.width * fDetails.height, IR_ARRAY_MemID);
        mypub->putData_uint16_array(data_depth, fDetails.width * fDetails.height, DEPTH_ARRAY_MemID);
        auto endTime4 = std::chrono::steady_clock::now();;
        auto differenceTime4 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime4 - startTime4).count();
        std::cout << "Time difference4: " << differenceTime4 << std::endl;
        
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
