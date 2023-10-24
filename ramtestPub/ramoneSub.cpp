
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
    std::string mytopic = "one_topic";
	std::vector<std::any> anyArray(1);

    // Scan data in polar coordinates
    float cscan[10]={0};
    bool initilized1 = false;
    initilized1 = mysub->init("ramtest_one.xml", "One", mytopic);

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
            std::cout << "sizeof: " << sizeof(anyArray[0]) << std::endl;
            mysub->m_listener.n_samples = 0;
        }
    }

    delete mysub;
    Log::Reset();
    return 0;
}
