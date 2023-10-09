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
 * @file cameraDynamicSub.cpp
 *
 */

#include <iostream>
#include <chrono>
#include <thread>
#include <typeinfo>

#include "../dynddssub/ScanDynamicSub.h"

// #include <opencv2/highgui/highgui.hpp>
// #include <opencv2/imgproc/imgproc.hpp>
// #ifdef OPENCV2
// #include <opencv2/contrib/contrib.hpp>
// #endif



using namespace eprosima::fastdds::dds;

// using namespace cv;
using eprosima::fastdds::dds::Log;

int main()
{
    std::cout << "Starting subscriber." << std::endl;

    bool initilized = false;
    int stopafter = 10;

    ScanDynamicSub *mysub;
    mysub = new ScanDynamicSub();
    std::string mytopic = "FrameCameraTopic";
    std::vector<std::any> anyArray(4);

    const int frameHeight = static_cast<int>(480);
    const int frameWidth = static_cast<int>(640);

    initilized = mysub->init("camera.xml", "FrameCamera", mytopic);
    if(initilized){
        mysub->run(anyArray.data(), anyArray.size(), mytopic);
    }
    else{
        std::cout << "Error initilizing subscriber." << std::endl;
        return 0;
    }

    // while(1)
    // {              
        while(mysub->m_listener.n_samples<5)// ->listener_.newFrameFlag_ == 0) //TODO: check if n_samples is the right variable
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        // cv::namedWindow("Depth Image", cv::WINDOW_AUTOSIZE);
        // cv::namedWindow("Infrared Image", cv::WINDOW_AUTOSIZE);
        // // while (cv::waitKey(1) != 27 && getWindowProperty("Depth Image", cv::WND_PROP_AUTOSIZE) >= 0
        // //         && getWindowProperty("Infrared Image", cv::WND_PROP_AUTOSIZE) >= 0)
        // // {          
        //     if (mysub->m_listener.n_samples == 5) //TODO: check if n_samples is the right variable unten nicht vergessen
        //     {                      
        //         printf("here0\n");
        //         //Convert the frame into depth mat                        
        //         cv::Mat depth_mat;
        //         if(anyArray[2].type() == typeid(uint16_t*)){                
        //             depth_mat = cv::Mat(frameHeight, frameWidth, CV_16UC1, std::any_cast<std::uint16_t*>(anyArray[2])); //&mysub->listener_.adiFrame_.depthFrame());
        //         }      
        //         else{
        //             const std::type_info& typeInfo = anyArray[2].type();
        //             std::cout << "Type of the object: " << typeInfo.name() << std::endl;
        //             std::cout << "Error: depthFrame is not of type uint16_t*." << std::endl;
        //             return 0;
        //         }          
        //         printf("here1\n");
        //         //Calculate the distance factor 
        //         double distance_scale = 255.0 / (double)(std::any_cast<std::uint16_t>(anyArray[3])); //mysub->listener_.adiFrame_.cameraRange();
        //         printf("here2\n");
        //         //Convert from raw values to values that opencv can understand 
        //         depth_mat.convertTo(depth_mat, CV_8U, distance_scale);
                
        //         //Apply a rainbow color map to the depth mat to better visualize the depth data 
        //         applyColorMap(depth_mat, depth_mat, cv::COLORMAP_RAINBOW);
                                                    
        //         //Convert the frame into infrared mat
        //         cv::Mat ir_mat;
        //         ir_mat = cv::Mat(frameHeight, frameWidth, CV_16UC1, std::any_cast<std::uint16_t*>(anyArray[1]));//&mysub->listener_.adiFrame_.irFrame());
        //         printf("here3\n");
        //         //Display the depth and the infrared image
        //         imshow("Depth Image", depth_mat);
        //         imshow("Infrared Image", ir_mat);
                
        //         //Save the last depth and infrared image in the directory
        //         imwrite("depth.jpg", depth_mat);
        //         imwrite("ir.jpg", ir_mat);
                                
        //         mysub->m_listener.n_samples = 0;  //listener_.newFrameFlag_ = 0; //Flag zuruecksetzten //TODO: check if n_samples is the right variable wie oben
        //     }
        //}
    // }


    while(stopafter > 0)
    {
        if(mysub->m_listener.n_samples >= 1){
            if(anyArray[2].type() == typeid(uint16_t*)){                
                uint16_t* bufferarray =  std::any_cast<std::uint16_t*>(anyArray[2]);
                std::cout << "bufferarray: " << bufferarray[300] << std::endl;
            } 
            std::cout << "n_samples: " << mysub->m_listener.n_samples << std::endl;
        }
        mysub->m_listener.n_samples = 0;
        stopafter--;
    }
    delete mysub;
    Log::Reset();
    return 0;
}
