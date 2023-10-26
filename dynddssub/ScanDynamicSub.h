
/**
 * @file ScanDynamicSub.h
 *
 */

#ifndef SCANDYNAMICSUB_H_
#define SCANDYNAMICSUB_H_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastrtps/types/TypeIdentifier.h>
#include <fastrtps/types/TypeObject.h>
#include <fastrtps/types/DynamicDataHelper.hpp>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastrtps/rtps/common/Types.h>
#include <fastrtps/xmlparser/XMLProfileManager.h>

#include <atomic>
#include <condition_variable>
#include <map>
#include <any>
#include <mutex>
#include <memory>

class ScanDynamicSub
{
public:
    ScanDynamicSub();

    virtual ~ScanDynamicSub();

    // initialize the subscriber
    bool init(std::string xml_path, std::string dyntype_name, std::string topic_name);

    // run the subscriber
    void run(std::any *anyArray, std::size_t anySize, std::string topic_name);

private:
    eprosima::fastdds::dds::DomainParticipant* mp_participant;

    eprosima::fastdds::dds::Subscriber* mp_subscriber;

    std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastdds::dds::Topic*> topics_;

    std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastrtps::types::DynamicType_ptr> readers_;

    std::map<eprosima::fastdds::dds::DataReader*, eprosima::fastrtps::types::DynamicData_ptr> datas_;

    eprosima::fastrtps::SubscriberAttributes att_;

    eprosima::fastdds::dds::DataReaderQos qos_;

public:
    class SubListener  :  public eprosima::fastdds::dds::DomainParticipantListener
    {
    public:
        SubListener(ScanDynamicSub* sub)
            : n_matched(0)
            , n_samples(0)
            , subscriber_(sub)
        {
        }

        ~SubListener() override
        {
        }

        void on_data_available(eprosima::fastdds::dds::DataReader* reader) override;

        void on_subscription_matched(
                eprosima::fastdds::dds::DataReader* reader,
                const eprosima::fastdds::dds::SubscriptionMatchedStatus& info) override;

        int n_matched;

        uint32_t n_samples;

        std::mutex types_mx_;

        std::condition_variable types_cv_;

        eprosima::fastrtps::types::DynamicType_ptr received_type_;

        std::atomic<bool> reception_flag_{false};

        ScanDynamicSub* subscriber_;

        //my additions
        std::vector<std::any*> anyPointers;

    } m_listener;

};

#endif /* SCANDYNAMICSUB_H_ */
