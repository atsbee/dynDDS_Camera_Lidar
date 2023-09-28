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

#include <iostream>
#include <string>

#include <ctime>
#include <chrono>

using namespace eprosima::fastdds::dds;

#define INDEX_MemID 0
#define DEPTH67_ARRAY_MemID 1
#define CAMERA_RANGE_MemID 2


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
        
    uint16_t data_depth[307200]={0};
        
    mypub->init("cameraAngle.xml", "FrameCameraAngle", "FrameCameraAngleTopic");

    while(1)
    {        
        fill_array(data_depth, 640, static_cast<uint16_t>(samples_index%100),307200);

        /*calculate central vertical frame (actually horizontal)*/
        angle_in_middle[0] = 0;
        depth_in_middle[0] = data_depth[pixelStart_VFOV];
        for (unsigned int i = 1; i < frameHeight; ++i){
            angle_in_middle[i] = angle_in_middle[i-1] + angleOffset_VFOV;
            depth_in_middle[i] = data_depth[(frameWidth * i) + (pixelStart_VFOV)];
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
                depth_in_middle_67[j] = static_cast<int>(static_cast<double>(depth_in_middle_67[j]) / static_cast<double>(count));
                ++j;
                count = 1;//reset the value count of the elements
                depth_in_middle_67[j] = depth_in_middle[i];//Save the next depth frame in the nextrow of the array
            }
        }
        mypub->putData_uint16_array(depth_in_middle_67, 67, DEPTH67_ARRAY_MemID);
        
        auto startTime6 = std::chrono::steady_clock::now();                     
        if(mypub->publish())
        {  
            samples_index++;
            mypub->putData_uint32_value(samples_index, INDEX_MemID); //TODO: index is now being counted here
        }
        //sleep for 1 second
        std::this_thread::sleep_for(std::chrono::milliseconds(182));

    }

    delete mypub;
    return 0;
}
