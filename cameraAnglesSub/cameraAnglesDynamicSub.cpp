/**
 * @file FrameCameraAngleSub.cpp
 *
 */

#include "FrameCameraPubSubTypes.h"

#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/topic/TypeSupport.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/DataReaderListener.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>

#include <iostream>
#include <chrono>
#include <thread>

using namespace eprosima::fastdds::dds;


class FrameCameraAngleSub
{
public:

    DomainParticipant* participant_;

    Subscriber* subscriber_;

    DataReader* reader_;

    Topic* topic_;

    TypeSupport type_;

    class SubListener : public DataReaderListener
    {
    public:

        SubListener()
            : newFrameFlag_(0)
        {
        }

        ~SubListener() override
        {
        }

        void on_subscription_matched(
                DataReader*,
                const SubscriptionMatchedStatus& info) override
        {
            if (info.current_count_change == 1)
            {
                std::cout << "Subscriber matched." << std::endl;
            }
            else if (info.current_count_change == -1)
            {
                std::cout << "Subscriber unmatched." << std::endl;
            }
            else
            {
                std::cout << info.current_count_change
                        << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
            }
        }

        void on_data_available(
                DataReader* reader) override
        {
            SampleInfo info;
            if (reader->take_next_sample(&adiFrame_, &info) == ReturnCode_t::RETCODE_OK)
            {
                if (info.valid_data)
                {
                    std::cout << "Data Frame of Camera received." << std::endl;
                    
                    newFrameFlag_ = 1; //Flag wird gesetzt wenn neue Frames verfuegbar sind 
                }
            }
        }

        FrameCamera adiFrame_;
        
        std::atomic_int newFrameFlag_;

    } listener_;

    FrameCameraAngleSub()
        : participant_(nullptr)
        , subscriber_(nullptr)
        , topic_(nullptr)
        , reader_(nullptr)
        , type_(new FrameCameraPubSubType())
    {
    }

    virtual ~FrameCameraAngleSub()
    {
        if (reader_ != nullptr)
        {
            subscriber_->delete_datareader(reader_);
        }
        if (topic_ != nullptr)
        {
            participant_->delete_topic(topic_);
        }
        if (subscriber_ != nullptr)
        {
            participant_->delete_subscriber(subscriber_);
        }
        DomainParticipantFactory::get_instance()->delete_participant(participant_);
    }
    
    struct DepthAngle
    {
        double angleData_HFOV[640];
        double angleData_VFOV[640];
        uint16_t depthData_HFOV[640];
        uint16_t depthData_VFOV[480];
        uint16_t depthData_VFOV_67[67];//For testing onlz 67 deth frame data are sent
    }; 
    
    DepthAngle centerFrame_;

    //!Initialize the subscriber
    bool init()
    {
        DomainParticipantQos participantQos;
        participantQos.name("Participant_subscriber");
        participant_ = DomainParticipantFactory::get_instance()->create_participant(0, participantQos);

        if (participant_ == nullptr)
        {
            return false;
        }

        // Register the Type
        type_.register_type(participant_);

        // Create the subscriptions Topic
        topic_ = participant_->create_topic("FrameCameraTopic", "FrameCamera", TOPIC_QOS_DEFAULT);

        if (topic_ == nullptr)
        {
            return false;
        }

        // Create the Subscriber
        subscriber_ = participant_->create_subscriber(SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (subscriber_ == nullptr)
        {
            return false;
        }

        // Create the DataReader
        reader_ = subscriber_->create_datareader(topic_, DATAREADER_QOS_DEFAULT, &listener_);

        if (reader_ == nullptr)
        {
            return false;
        }

        return true;
    }

};

int main(
        int argc,
        char** argv)
{
    std::cout << "Starting subscriber." << std::endl;

    FrameCameraAngleSub* mysub = new FrameCameraAngleSub();
    
    mysub->init();
           
    const int frameHeight = static_cast<int>(480);
    const int frameWidth = static_cast<int>(640);
    const int pixelStart_HFOV = static_cast<int>(frameWidth * ((frameHeight/2)-1));//Pixelstart of center in the horizontal plane frame is 640x239
    const int pixelStart_VFOV = static_cast<int>((frameWidth/2)-1);//Pixelstart of center in the vertical plane frame is 319 = (640/2)-1
    const double angleOffset_HFOV = (87.0/static_cast<double>(frameWidth-1));//angle for each pixel in the horizontal plane
    const double angleOffset_VFOV = (67.0)/static_cast<double>(frameHeight-1);//angle for each pixel in the vertical plane
            
    
    while(1)
    {       
        if (mysub->listener_.newFrameFlag_ == 1) 
        {                    
            //!--------------- Calculating the angle for the central frame on the horizontal plane (HFOV: 87°) ----------------- 
            
            //Assign edge values
            mysub->centerFrame_.angleData_HFOV[0] = 0;
            mysub->centerFrame_.depthData_HFOV[0] = mysub->listener_.adiFrame_.depthFrame()[pixelStart_HFOV];
            for (unsigned int i = 1; i < frameWidth; ++i){
                mysub->centerFrame_.angleData_HFOV[i] = mysub->centerFrame_.angleData_HFOV[i-1] + angleOffset_HFOV;
                mysub->centerFrame_.depthData_HFOV[i] = mysub->listener_.adiFrame_.depthFrame()[pixelStart_HFOV + i];
                std::cout << "Angle information in HFOV: " << mysub->centerFrame_.angleData_HFOV[i] << " with index: " << i << " RECEIVED" << std::endl;
            }
            
            
            //!--------------- Calculating the angle for the central frame on the vertical plane (VFOV: 67°) -------------------
            
            //Assign edge values
            mysub->centerFrame_.angleData_VFOV[0] = 0;
            mysub->centerFrame_.depthData_VFOV[0] = mysub->listener_.adiFrame_.depthFrame()[pixelStart_VFOV];
            for (unsigned int i = 1; i < frameHeight; ++i){
                mysub->centerFrame_.angleData_VFOV[i] = mysub->centerFrame_.angleData_VFOV[i-1] + angleOffset_VFOV;
                mysub->centerFrame_.depthData_VFOV[i] = mysub->listener_.adiFrame_.depthFrame()[(frameWidth * i) + (pixelStart_VFOV)];
                std::cout << "Angle information in VFOV: " << mysub->centerFrame_.angleData_VFOV[i] << " with index: " << i << " RECEIVED" << std::endl;
            }
            
            //!--------------- Calculating the angle for the central frame on the vertical plane and send 67 data ---------------
                        
            //Assign edge values
            mysub->centerFrame_.angleData_VFOV[0] = 0;
            mysub->centerFrame_.depthData_VFOV[0] = mysub->listener_.adiFrame_.depthFrame()[pixelStart_VFOV];
            
            unsigned int j = 0;
            mysub->centerFrame_.depthData_VFOV_67[j] = mysub->centerFrame_.depthData_VFOV[0];
            unsigned int count = 1;//The of the pixel start must also be taken into account when calculating the mean value
            
            for (unsigned int i = 1; i < frameHeight; ++i){
                mysub->centerFrame_.angleData_VFOV[i] = mysub->centerFrame_.angleData_VFOV[i-1] + angleOffset_VFOV;
                mysub->centerFrame_.depthData_VFOV[i] = mysub->listener_.adiFrame_.depthFrame()[(frameWidth * i) + (pixelStart_VFOV)];
                if (static_cast<int>(mysub->centerFrame_.angleData_VFOV[i]) == j)
                {
                    //Calculate the sum of the depth frame data with the same pre-decimal number of the angle
                    mysub->centerFrame_.depthData_VFOV_67[j] = mysub->centerFrame_.depthData_VFOV_67[j] + mysub->centerFrame_.depthData_VFOV[i];
                    ++count;
                } else {
                    //calcule the mean value
                    std::cout << "Sum value: " << mysub->centerFrame_.depthData_VFOV_67[j] << " with count: " << count << std::endl;
                    mysub->centerFrame_.depthData_VFOV_67[j] = static_cast<int>(static_cast<double>(mysub->centerFrame_.depthData_VFOV_67[j]) / static_cast<double>(count));
                    std::cout << "Mean value depth frame: " << mysub->centerFrame_.depthData_VFOV_67[j] << " with index: " << j << " RECEIVED" << std::endl;
                    ++j;
                    count = 1;//reset the value count of the elements
                    mysub->centerFrame_.depthData_VFOV_67[j] = mysub->centerFrame_.depthData_VFOV[i];//Save the next depth frame in the nextrow of the array
                    std::cout << "Center frame: " << mysub->centerFrame_.depthData_VFOV_67[j] << " with count: " << count << " RECEIVED" << std::endl << std::endl;
                }
            }
            
                            
            mysub->listener_.newFrameFlag_ == 0; //Flag zuruecksetzten
        }
    }

    delete mysub;
    return 0;
}
