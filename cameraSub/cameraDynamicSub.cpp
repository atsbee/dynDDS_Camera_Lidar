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

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#ifdef OPENCV2
#include <opencv2/contrib/contrib.hpp>
#endif



using namespace eprosima::fastdds::dds;

using eprosima::fastdds::dds::Log;

int main()
{
    std::cout << "Starting subscriber." << std::endl;

    bool initilized = false;

    ScanDynamicSub *mysub;
    mysub = new ScanDynamicSub();
    std::vector<std::any> anyArray(4);

    const int frameHeight = static_cast<int>(480);
    const int frameWidth = static_cast<int>(640);

    initilized = mysub->init("../xmls/camera.xml", "FrameCamera", "FrameCameraTopic");
    if(initilized){
        mysub->run(anyArray.data(), anyArray.size());
    }
    else{
        std::cout << "Error initilizing subscriber." << std::endl;
        return 0;
    }
           
    cv::namedWindow("Depth Image", cv::WINDOW_AUTOSIZE);
    cv::namedWindow("Infrared Image", cv::WINDOW_AUTOSIZE);

    cv::Mat ir_mat;
    cv::Mat depth_mat;

    while(1){
        if(mysub->m_listener.n_newdataflag >= 1){
            while (cv::waitKey(1) != 27 && getWindowProperty("Depth Image", cv::WND_PROP_AUTOSIZE) >= 0
                    && getWindowProperty("Infrared Image", cv::WND_PROP_AUTOSIZE) >= 0)
            {   
                //Convert the frame into depth mat                        
                depth_mat = cv::Mat(frameHeight, frameWidth, CV_16UC1, std::any_cast<std::uint16_t*>(anyArray[2]));     

                //Calculate the distance factor 
                double distance_scale = 255.0 / (double)(std::any_cast<std::uint16_t>(anyArray[3])); 

                //Convert from raw values to values that opencv can understand 
                depth_mat.convertTo(depth_mat, CV_8U, distance_scale);
                
                //Apply a rainbow color map to the depth mat to better visualize the depth data 
                applyColorMap(depth_mat, depth_mat, cv::COLORMAP_RAINBOW);
                                                    
                //Convert the frame into infrared mat
                ir_mat = cv::Mat(frameHeight, frameWidth, CV_16UC1, std::any_cast<std::uint16_t*>(anyArray[1]));

                //Display the depth and the infrared image
                imshow("Depth Image", depth_mat);
                imshow("Infrared Image", ir_mat);
                
                //Save the last depth and infrared image in the directory
                imwrite("../cameraImages/depth.jpg", depth_mat);
                imwrite("../cameraImages/ir.jpg", ir_mat);
                                
                mysub->m_listener.n_newdataflag = 0;  //Flag zuruecksetzten 
            }
        }
    }


    delete mysub;
    Log::Reset();
    return 0;
}
