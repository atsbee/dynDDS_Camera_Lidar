
/**
 * @file visuDynamicSub.cpp
 *
 */

#include <iostream>
#include <cmath>
#include "../dynddssub/ScanDynamicSub.h"

using eprosima::fastdds::dds::Log;


int main()
{
    //DDS Subscriber1
    ScanDynamicSub *mysub;
    mysub = new ScanDynamicSub();
    std::string mytopic = "ten_topic";
	std::vector<std::any> anyArray(10);

    // Scan data in polar coordinates
    bool initilized1 = false;
    initilized1 = mysub->init("ramtest_ten.xml", "Ten", mytopic);

    if (initilized1)
    {
		printf("init success\n");
		mysub->run(anyArray.data(), anyArray.size(), mytopic);
		printf("run success\n");
    }

    
    while (1)
    {
        if(mysub->m_listener.n_samples == 0){    
        }
        else{
            printf("new data \n");
            mysub->m_listener.n_samples = 0;
        }
    }

    delete mysub;
    Log::Reset();
    return 0;
}
