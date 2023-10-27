/**
 * @file ScanDynamicPub.h
 *
 */

#ifndef SCANDYNAMICPUB_H_
#define SCANDYNAMICPUB_H_

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastrtps/types/DynamicDataFactory.h>
#include <fastrtps/xmlparser/XMLProfileManager.h>

#include <thread>

class ScanDynamicPub
{
public:

    ScanDynamicPub();

    virtual ~ScanDynamicPub();

    //!Initialize
    bool init(std::string xml_path, std::string dyntype_name, std::string topic_name);

    //!Publish a sample
    bool publish();

    //put data in arrays
    void putData_uint16_array(uint16_t *array, int size, int member_id);

    void putData_uint32_array(uint32_t *array, int size, int member_id);

    void putData_int16_array(int16_t *array, int size, int member_id);

    void putData_int32_array(int32_t *array, int size, int member_id);

    void putData_int64_array(int64_t *array, int size, int member_id);

    void putData_float32_array(float *array, int size, int member_id);

    void putData_float64_array(double *array, int size, int member_id);


    //put data into values
    void putData_uint16_value(uint16_t value, int member_id);

    void putData_uint32_value(uint32_t value, int member_id);

    void putData_int16_value(int16_t value, int member_id);

    void putData_int32_value(int32_t value, int member_id);

    void putData_int64_value(int64_t value, int member_id);

    void putData_float32_value(float value, int member_id);

    void putData_float64_value(double value, int member_id);

    void putData_char8_value(char value, int member_id);

    void putData_char16_value(wchar_t value, int member_id);

    void putData_string_value(std::string value, int member_id);

    void putData_wstring_value(std::wstring value, int member_id);

    void putData_bool_value(bool value, int member_id);


private:

    eprosima::fastrtps::types::DynamicData_ptr m_ScanDyn;

    eprosima::fastdds::dds::DomainParticipant* mp_participant;

    eprosima::fastdds::dds::Publisher* mp_publisher;

    eprosima::fastdds::dds::Topic* topic_;

    eprosima::fastdds::dds::DataWriter* writer_;


    class PubListener : public eprosima::fastdds::dds::DataWriterListener
    {
    public:

        PubListener() : n_matched(0), firstConnected(false)
        {
        }

        ~PubListener() override
        {
        }

        void on_publication_matched(
                eprosima::fastdds::dds::DataWriter* writer,
                const eprosima::fastdds::dds::PublicationMatchedStatus& info) override;
        int n_matched;

        bool firstConnected;

    } m_listener;

};



#endif /* SCANDYNAMICPUB_H_ */
