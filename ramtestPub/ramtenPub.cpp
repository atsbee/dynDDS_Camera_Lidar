#include "../dynddspub/ScanDynamicPub.h"
#include <fastrtps/log/Log.h>

using eprosima::fastdds::dds::Log;

float getNextValue() {
    static float currentValue = 0.0;
    currentValue += 0.5;
    if (currentValue > 15.0) {
        currentValue = 0.0;
    }
    return currentValue;
}

int main()
{ 
    bool initialized = false;
    std::cout << "Starting " << std::endl;

    ScanDynamicPub *mypub;
    mypub = new ScanDynamicPub();
    initialized = mypub->init("../xmls/ramtest_ten.xml", "Ten", "ten_topic");
    printf("init success\n");
    while(initialized){
            for (int i = 0; i < 10; i++) {
            float value = getNextValue();
            printf("value: %f\n", value);
            mypub->putData_float32_value(value, i);
            printf("put data success\n");
            mypub->publish();
            printf("publish success\n");
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }
    }


    delete mypub;
    Log::Reset();
    return 0;
}
