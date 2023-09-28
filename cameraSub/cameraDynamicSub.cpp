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

#include "../dynddssub/ScanDynamicSub.h"

#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#ifdef OPENCV2
#include <opencv2/contrib/contrib.hpp>
#endif

#include <iostream>
#include <chrono>
#include <thread>

using namespace eprosima::fastdds::dds;

using namespace cv;

int main(int argc, char** argv)
{
    std::cout << "Starting subscriber." << std::endl;

    bool initilized = false;

    ScanDynamicSub *mysub;
    mysub = new ScanDynamicSub();
    std::string mytopic = "FrameCameraTopic";
    std::vector<std::any> anyArray(4);
    uint16_t irData[640*480];
    uint16_t depthData[640*480];
    const int frameHeight = static_cast<int>(480);
    const int frameWidth = static_cast<int>(640);

    initilized = mysub->init("camera.xml", "FrameCamera", mytopic);
    if(initilized){
        mysub->run(anyArray.data(), anyArray.size(), mytopic);
    }

    while(1)
    {       
       
        if (mysub->m_listener.n_samples==1)// ->listener_.newFrameFlag_ == 1) //TODO: check if n_samples is the right variable unten nicht vergessen
        {
            cv::namedWindow("Depth Image", cv::WINDOW_AUTOSIZE);
            cv::namedWindow("Infrared Image", cv::WINDOW_AUTOSIZE);
            while (cv::waitKey(1) != 27 && getWindowProperty("Depth Image", cv::WND_PROP_AUTOSIZE) >= 0
                    && getWindowProperty("Infrared Image", cv::WND_PROP_AUTOSIZE) >= 0)
            {                 
                //Convert the frame into depth mat                        
                cv::Mat depth_mat;
                depth_mat = cv::Mat(frameHeight, frameWidth, CV_16UC1, std::any_cast<std::uint16_t*>(anyArray[2])); //&mysub->listener_.adiFrame_.depthFrame());
                
                //Calculate the distance factor 
                double distance_scale = 255.0 / std::any_cast<std::uint16_t>(anyArray[3]); //mysub->listener_.adiFrame_.cameraRange();
                
                //Convert from raw values to values that opencv can understand 
                depth_mat.convertTo(depth_mat, CV_8U, distance_scale);
                
                //Apply a rainbow color map to the depth mat to better visualize the depth data 
                applyColorMap(depth_mat, depth_mat, cv::COLORMAP_RAINBOW);
                                                 
                //Convert the frame into infrared mat
                cv::Mat ir_mat;
                ir_mat = cv::Mat(frameHeight, frameWidth, CV_16UC1, std::any_cast<std::uint16_t*>(anyArray[1]));//&mysub->listener_.adiFrame_.irFrame());
                                    
                //Display the depth and the infrared image
                imshow("Depth Image", depth_mat);
                imshow("Infrared Image", ir_mat);
                
                //Save the last depth and infrared image in the directory
                imwrite("depth.jpg", depth_mat);
                imwrite("ir.jpg", ir_mat);
                                
                mysub->m_listener.n_samples = 0;  //listener_.newFrameFlag_ = 0; //Flag zuruecksetzten //TODO: check if n_samples is the right variable wie oben
            }
        }
    }

    delete mysub;
    return 0;
}
