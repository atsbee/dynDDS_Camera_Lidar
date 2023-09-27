#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <iostream>
#include <cmath>
#include <thread>

#include "ScanDynamicPub.h"
#include "../include/sl_lidar.h" 
#include "../include/sl_lidar_driver.h"
#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif
using namespace sl;
using eprosima::fastdds::dds::Log;

void thrpub(ScanDynamicPub* mypub, uint16_t distance[362]){
    uint16_t temp = 0;
    for(int i = 0; i<360/2; i++){               //reihenfolde umkehren 
        temp = distance[i];                     //bei messung in uhrzeigersinn
        distance[i] = distance[360-i-1];        //    
        distance[360-i-1] = temp;               //
    }
    distance[360] = 1;
    distance[361] = 1;
    mypub->publish(distance);
}

int main(int argc, char** argv)
{
    // meine entfernungen im array
    uint16_t distance_arr[362];
    uint16_t zwischen = 0, temp = 0 ;
    int winkel = 0;
    int sameangle_cnt = 1;
    bool initialized = false;

   	std::cout << "Starting scanner publisher." << std::endl;

	//ScanDynamicPub* mypub = new ScanDynamicPub();
    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    initialized = mypub->init("pub1.xml", "SensorData", "Scanner1Topic");

    ///  Create a communication channel instance
    IChannel* _channel;
    _channel = (*createSerialPortChannel("/dev/ttyUSB0", 115200));

    ///  Create a LIDAR driver instance
    ILidarDriver * drv = *createLidarDriver();
    sl_result res = drv->connect(_channel);
            
    if(SL_IS_OK(res)){
        sl_lidar_response_device_info_t deviceInfo;
        res = drv->getDeviceInfo(deviceInfo);
        if(SL_IS_OK(res)){
            printf("Model: %d, Firmware Version: %d.%d, Hardware Version: %d\n",
            deviceInfo.model,
            deviceInfo.firmware_version >> 8, deviceInfo.firmware_version & 0xffu,
            deviceInfo.hardware_version);
        }else{
            fprintf(stderr, "Failed to get device information from LIDAR %08x\r\n", res);
        }
    }else{
        fprintf(stderr, "Failed to connect to LIDAR %08x\r\n", res);
    }
   
    drv->setMotorSpeed();
    // TODO
    drv->startScan(0,1);

    if(initialized)
	{
        while (1) {
            sl_lidar_response_measurement_node_hq_t nodes[8192];
            size_t   count = _countof(nodes);

            res = drv->grabScanDataHq(nodes, count);

            if (SL_IS_OK(res)) {
                drv->ascendScanData(nodes, count);
                for (int pos = 0; pos < (int)count ; ++pos) {
                    if ((winkel == (int)std::round((nodes[pos].angle_z_q14 * 90.f) / (1 << 14))) && nodes[pos].quality != 0){//nullen nicht für zwischen ergebnis beachten
                        sameangle_cnt++;
                        //zwischen += nodes[pos].dist_mm_q2/4.0f;
                        //durch 4 teilen um auf mm zu kommen(equivalent zu /(1<<2) ); min um overflow zu verhindern welcher nie auftreten sollte da der sensor nur ca 6000mm weit sieht; round um auf ganze zahl zu kommen; static cast um overflow zu verhindern
                        zwischen += static_cast<uint16_t>(std::round(std::min(static_cast<uint32_t>(nodes[pos].dist_mm_q2/4.0f), static_cast<uint32_t>(UINT16_MAX))));
                    }
                    else{
                        sameangle_cnt = 1;
                        //zwischen = nodes[pos].dist_mm_q2/4.0f;
                        zwischen = static_cast<uint16_t>(std::round(std::min(static_cast<uint32_t>(nodes[pos].dist_mm_q2/4.0f), static_cast<uint32_t>(UINT16_MAX))));
                    }
                    winkel = (int)std::round((nodes[pos].angle_z_q14 * 90.f) / (1 << 14));//wenn neu dann aktualisiert sonst gleich
                    
                    distance_arr[winkel] = static_cast<uint16_t>(std::round(zwischen/(sameangle_cnt*1.f))); //nur wenn winkel sich ändert wichtig

                    if(nodes[pos].flag & SL_LIDAR_RESP_HQ_FLAG_SYNCBIT){
                        std::thread t1(thrpub, mypub, distance_arr);
                        t1.join();                
                    }
                    
                }  
            }
        }
    }

    drv->stop();
	usleep(2000000);

    drv->setMotorSpeed(DEFAULT_MOTOR_SPEED);

    delete mypub;
    Log::Reset();
    /// Delete Lidar Driver and channel Instance
    delete drv;
    delete _channel;
}
