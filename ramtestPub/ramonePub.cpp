#include "../dynddspub/ScanDynamicPub.h"
#include <fastrtps/log/Log.h>

using eprosima::fastdds::dds::Log;

void fillArray(float array[10]) {
    static float currentValue = 0.0;
    static int startIndex = 0;

    // Fill array with 0
    for (int i = 0; i < 10; i++) {
        array[i] = 0.0;
    }

    // Set the 5 adjacent values
    for (int i = startIndex; i < startIndex + 5; i++) {
        int index = i % 10;
        array[index] = currentValue;
        currentValue += 0.5;
        if (currentValue > 15.0) {
            currentValue = 0.0;
        }
    }

    // Update the starting index for the next call
    startIndex = (startIndex + 1) % 10;
}

int main()
{ 
    //dummy data
    float distance_arr[10] = {0.0};

    bool initialized = false;
    std::cout << "Starting " << std::endl;

    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    initialized = mypub->init("ramtest_one.xml", "One", "one_topic");
printf("init success\n");
    while(initialized){
        fillArray(distance_arr);
        printf("fill array success\n");
        mypub->putData_float32_array(distance_arr, 10, 0);
        printf("put data success\n");
        mypub->publish();
        printf("publish success\n");
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    delete mypub;
    Log::Reset();
    return 0;
}
