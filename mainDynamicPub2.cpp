// Copyright 2019 Proyectos y Sistemas de Mantenimiento SL (eProsima).
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
 * @file HelloWorld_main.cpp
 *
 */

#include "ScanDynamicPub.h"

#include <fastrtps/log/Log.h>

using eprosima::fastdds::dds::Log;

void fillArray(uint16_t array[120]) {
    static uint16_t currentValue = 0;
    static int startIndex = 0;

    // Fill array with 0
    for (int i = 0; i < 120; i++) {
        array[i] = 0;
    }

    // Set the 20 adjacent values
    for (int i = startIndex; i < startIndex + 20; i++) {
        int index = i % 120;
        array[index] = currentValue;
        currentValue += 5;
        if (currentValue > 1500) {
            currentValue = 0;
        }
    }

    // Update the starting index for the next call
    startIndex = (startIndex + 1) % 120;
}


int main()
{ 
    //dummy data
    uint16_t distance_arr[120]={0};

    bool initialized = false;
    std::cout << "Starting " << std::endl;

    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    initialized = mypub->init("pub2.xml", "SensorData2", "Scanner2Topic");

    while(initialized){
        fillArray(distance_arr);
        mypub->putData_uint16_array(distance_arr, 120);
        mypub->publish();
        std::this_thread::sleep_for(std::chrono::milliseconds(180));
    }

    delete mypub;
    Log::Reset();
    return 0;
}
