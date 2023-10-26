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
 * @file dummyCameraPub.cpp
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
    uint16_t value = startVal*100;
    for (int i = 0; i < size; ++i) {
        if (i % incrementInterval == 0){
            value = value + 30;
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

    mypub->init("../xmls/camera.xml", "FrameCamera", "FrameCameraTopic");

    while(1)
    {        
        auto startTime = std::chrono::steady_clock::now();
        
        fill_array(data_ir, 640, static_cast<uint16_t>(samples_index%200),307200);  
        fill_array(data_depth, 640, static_cast<uint16_t>(samples_index%200),307200);

        mypub->putData_uint16_array(data_ir, frameWidth * frameHeight, IR_ARRAY_MemID);
        mypub->putData_uint16_array(data_depth, frameWidth * frameHeight , DEPTH_ARRAY_MemID);

        mypub->putData_uint16_value( static_cast<uint16_t>(12000) , CAMERA_RANGE_MemID);
                   
        if(mypub->publish())
        { 
            samples_index++;
            mypub->putData_uint32_value(samples_index, INDEX_MemID); 
        }
        
        auto endTime = std::chrono::steady_clock::now();
        auto differenceTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - startTime).count();
        std::cout << "Time difference gesamt: " << differenceTime << std::endl << std::endl;
    }

    delete mypub;
    return 0;
}
