#include "../dynddspub/ScanDynamicPub.h"
#include <fastrtps/log/Log.h>

using eprosima::fastdds::dds::Log;

void fillArray(float array[150]) {

    // Fill array with 0
    for (int i = 0; i < 150; i++) {
        array[i] = 99.0;
    }

}

int main()
{ 
    //dummy data
    float distance_arr[150] = {0.0};

    bool initialized = false;
    std::cout << "Starting " << std::endl;

    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    initialized = mypub->init("../xmls/ramtest_one.xml", "One", "one_topic");
printf("init success\n");
    while(initialized){
        fillArray(distance_arr);
        printf("fill array success\n");
        mypub->putData_float32_array(distance_arr, 150, 0);
        printf("put data success\n");
        mypub->publish();
        printf("publish success\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    delete mypub;
    Log::Reset();
    return 0;
}
