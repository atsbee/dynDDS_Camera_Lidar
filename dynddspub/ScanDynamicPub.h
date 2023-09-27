// Copyright 2016 Proyectos y Sistemas de Mantenimiento SL (eProsima).
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

/**
 * @file ScanDynamicPub.h
 *
 */

#ifndef SCANDYNAMICPUB_H_
#define SCANDYNAMICPUB_H_

#include <fastdds/dds/publisher/DataWriterListener.hpp>
#include <fastdds/dds/domain/DomainParticipant.hpp>

class ScanDynamicPub
{
public:

    ScanDynamicPub();

    virtual ~ScanDynamicPub();

    //!Initialize
    bool init(std::string xml_path, std::string dyntype_name, std::string topic_name);

    //!Publish a sample
    bool publish();

    //put data into the dynamic data
    void putData_uint16_array(uint16_t *array, int size, int member_id);

    //put data into the dynamic data
    void putData_uint16_value(uint16_t value, int member_id);

    //put data into the dynamic data
    void putData_uint32_value(uint32_t value, int member_id);


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