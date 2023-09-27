/**
 * @file ScanDynamicSub.h
 *
 */

#ifndef SCANDYNAMICSUB_H_
#define SCANDYNAMICSUB_H_

#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastdds/dds/domain/DomainParticipantListener.hpp>
#include <fastdds/dds/subscriber/DataReader.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastrtps/subscriber/SampleInfo.h>
#include <fastrtps/rtps/common/Types.h>

#include <fastrtps/types/TypeIdentifier.h>
#include <fastrtps/types/TypeObject.h>

#include <fastrtps/attributes/SubscriberAttributes.h>

#include <atomic>
#include <condition_variable>
#include <map>

#include <any>


class ScanDynamicSub
{
public:

    ScanDynamicSub();

    virtual ~ScanDynamicSub();

    //!Initialize the subscriber
    bool init(std::string xml_path, std::string dyntype_name, std::string topic_name);

    //!RUN the subscriber
    void run(std::any *anyArray, std::size_t anySize, std::string topic_name);

    //! Initialize all required entities for data transmission
    void initialize_entities(std::string topic_name);


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

        // void on_type_discovery(
        //         eprosima::fastdds::dds::DomainParticipant* participant,
        //         const eprosima::fastrtps::rtps::SampleIdentity& request_sample_id,
        //         const eprosima::fastrtps::string_255& topic,
        //         const eprosima::fastrtps::types::TypeIdentifier* identifier,
        //         const eprosima::fastrtps::types::TypeObject* object,
        //         eprosima::fastrtps::types::DynamicType_ptr dyn_type) override;

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
