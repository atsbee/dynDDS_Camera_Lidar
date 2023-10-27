
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
	std::vector<std::any> anyArray(1);

    // Scan data in polar coordinates
    bool initilized1 = false;
    initilized1 = mysub->init("../xmls/ramtest_one.xml", "One", "one_topic");

    if (initilized1)
    {
		printf("init success\n");
		mysub->run(anyArray.data(), anyArray.size());
		printf("run success\n");
    }

    
    while (1)
    {
        if(mysub->m_listener.n_newdataflag == 0){    
        }
        else{
            std::cout << "sizeof: " << sizeof(anyArray[0]) << std::endl;
            mysub->m_listener.n_newdataflag = 0;
        }
    }

    delete mysub;
    Log::Reset();
    return 0;
}
