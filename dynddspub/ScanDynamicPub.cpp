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
 * @file ScanDynamicPub.cpp
 *
 */

#include "ScanDynamicPub.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/PublisherAttributes.h>
#include <fastdds/dds/publisher/Publisher.hpp>
#include <fastdds/dds/publisher/qos/PublisherQos.hpp>
#include <fastdds/dds/publisher/DataWriter.hpp>
#include <fastdds/dds/publisher/qos/DataWriterQos.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/types/DynamicDataFactory.h>

#include <fastrtps/xmlparser/XMLProfileManager.h>

#include <thread>

using namespace eprosima::fastdds::dds;

ScanDynamicPub::ScanDynamicPub()
    : mp_participant(nullptr)
    , mp_publisher(nullptr)
{
}

bool ScanDynamicPub::init(std::string xml_path, std::string dyntype_name, std::string topic_name)
{
    if (eprosima::fastrtps::xmlparser::XMLP_ret::XML_OK !=
            eprosima::fastrtps::xmlparser::XMLProfileManager::loadXMLFile(xml_path))
    {
        std::cout << "Cannot open XML file " << xml_path << ". Please, run the publisher from the folder that contatins this XML file." << std::endl;
        return false;
    }

    eprosima::fastrtps::types::DynamicType_ptr dyn_type =
            eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(dyntype_name)->build();
    TypeSupport m_type(new eprosima::fastrtps::types::DynamicPubSubType(dyn_type));
    m_ScanDyn = eprosima::fastrtps::types::DynamicDataFactory::get_instance()->create_data(dyn_type);

    // m_ScanDyn->set_uint16_value(362, 0); //set scan size

    // eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(1);
    // //array->clear_array_data(0);
    // m_ScanDyn->return_loaned_value(array); //set scan array

    // m_ScanDyn->set_uint16_value(7, 2); //set scan id to 7

    DomainParticipantQos pqos;
    pqos.name("Participant_pub");
    mp_participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    if (mp_participant == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    m_type.get()->auto_fill_type_information(false);
    // m_type.get()->auto_fill_type_object(true);
    m_type.get()->auto_fill_type_object(false); //TODO: check if this is needed


    m_type.register_type(mp_participant);

    //CREATE THE PUBLISHER
    mp_publisher = mp_participant->create_publisher(PUBLISHER_QOS_DEFAULT, nullptr);

    if (mp_publisher == nullptr)
    {
        return false;
    }

    topic_ = mp_participant->create_topic(topic_name, dyntype_name, TOPIC_QOS_DEFAULT);

    if (topic_ == nullptr)
    {
        return false;
    }

    // CREATE THE WRITER
    writer_ = mp_publisher->create_datawriter(topic_, DATAWRITER_QOS_DEFAULT, &m_listener);

    if (writer_ == nullptr)
    {
        return false;
    }

    return true;

}

ScanDynamicPub::~ScanDynamicPub()
{
    if (writer_ != nullptr)
    {
        mp_publisher->delete_datawriter(writer_);
    }
    if (mp_publisher != nullptr)
    {
        mp_participant->delete_publisher(mp_publisher);
    }
    if (topic_ != nullptr)
    {
        mp_participant->delete_topic(topic_);
    }
    DomainParticipantFactory::get_instance()->delete_participant(mp_participant);
}

void ScanDynamicPub::PubListener::on_publication_matched(
        eprosima::fastdds::dds::DataWriter*,
        const eprosima::fastdds::dds::PublicationMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        n_matched = info.total_count;
        firstConnected = true;
        std::cout << "Publisher matched" << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        n_matched = info.total_count;
        std::cout << "Publisher unmatched" << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for PublicationMatchedStatus current count change" << std::endl;
    }
}


bool ScanDynamicPub::publish()
{
    if (m_listener.firstConnected || m_listener.n_matched > 0) 
    {
        // m_ScanDyn->set_uint16_value(362, 0);

        // eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(1);
        // for(unsigned int i=0; i<362; i++){
        //     array->set_uint16_value(distance[i], array->get_array_index({0, i}));
        // }
        // m_ScanDyn->return_loaned_value(array); 

        //m_ScanDyn->set_uint16_value(7, 2);

        writer_->write(m_ScanDyn.get());
        return true;
    }
    return false;
}

void ScanDynamicPub::putData_uint16_array(uint16_t *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_uint16_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

void ScanDynamicPub::putData_uint16_value(uint16_t value, int member_id)
{
    m_ScanDyn->set_uint16_value(value, member_id);
}

void ScanDynamicPub::putData_uint32_value(uint32_t value, int member_id)
{
    m_ScanDyn->set_uint32_value(value, member_id);
}