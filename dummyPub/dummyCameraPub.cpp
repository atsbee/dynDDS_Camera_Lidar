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


#include <iostream>
#include <string>

#include <ctime>
#include <chrono>

using namespace eprosima::fastdds::dds;

void fill_array(uint16_t *array, int incrementInterval, uint16_t startVal , int size)
{
    uint16_t value = startVal*10;
    for (int i = 0; i < size; ++i) {
        if (i % incrementInterval == 0){
            value = value + 1;
        }   
        array[i] = value;
    }
}


#define INDEX_MemID 0
#define IR_ARRAY_MemID 1
#define DEPTH_ARRAY_MemID 2
#define CAMERA_RANGE_MemID 3

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
    
    uint16_t data_depth[307200]={0};
    uint16_t data_ir[307200]={0};
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

    mypub->init("camera.xml", "FrameCamera", "FrameCameraTopic");

    while(1)
    {        
        auto startTime = std::chrono::steady_clock::now();
        
        fill_array(data_ir, 640, static_cast<uint16_t>(samples_index%100),307200);  
        auto startTime4 = std::chrono::steady_clock::now();
        //std::memcpy(&mypub->adiFrame_.irFrame(), data_ir, sizeof(unsigned short) * fDetails.width * fDetails.height);
        mypub->putData_uint16_array(data_ir, frameWidth * frameHeight, IR_ARRAY_MemID);
        //std::memcpy(&mypub->adiFrame_.depthFrame(), data_depth, sizeof(unsigned short) * fDetails.width * fDetails.height);
        mypub->putData_uint16_array(data_depth, frameWidth * frameHeight , DEPTH_ARRAY_MemID);
        auto endTime4 = std::chrono::steady_clock::now();;
        auto differenceTime4 = std::chrono::duration_cast<std::chrono::milliseconds>(endTime4 - startTime4).count();
        std::cout << "Time difference4: " << differenceTime4 << std::endl;
        
        auto startTime5 = std::chrono::steady_clock::now();
    
        //mypub->adiFrame_.cameraRange() = cameraDetails.depthParameters.maxDepth;
        mypub->putData_uint16_value( 67 , CAMERA_RANGE_MemID);
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
