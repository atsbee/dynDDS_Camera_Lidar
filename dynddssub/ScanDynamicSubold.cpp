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
 * @file ScanDynamicSub.cpp
 *
 */

#include "ScanDynamicSub.h"
#include <fastrtps/attributes/ParticipantAttributes.h>
#include <fastrtps/attributes/SubscriberAttributes.h>
#include <fastdds/dds/subscriber/Subscriber.hpp>
#include <fastdds/dds/subscriber/SampleInfo.hpp>
#include <fastdds/dds/subscriber/qos/DataReaderQos.hpp>
#include <fastdds/dds/domain/DomainParticipantFactory.hpp>

#include <fastrtps/types/DynamicDataHelper.hpp>
#include <fastrtps/types/DynamicDataFactory.h>
#include <mutex>
#include <memory>

#include <fastrtps/xmlparser/XMLProfileManager.h>


using namespace eprosima::fastdds::dds;
using eprosima::fastrtps::types::ReturnCode_t;

template <typename T>
void HandleArrayCase(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    std::cout << "not supported"  << std::endl;
}

template <>
void HandleArrayCase<uint16_t>(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    // Specialized handling for uint16_t arrays
    uint16_t* copyArray = new uint16_t[data->get_item_count()];
    for (int i = 0; i < data->get_item_count(); i++) {
        copyArray[i] = data->get_uint16_value(i);
    }

    *anyPointers[id] = copyArray;  // how to move ownership to std::any?
    printf("uint16_t array\n");
}


template <>
void HandleArrayCase<int16_t>(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    if(anyPointers[id] && anyPointers[id]->has_value() && anyPointers[id]->type() == typeid(int16_t*)) {
        // Deallocate previous data (if any)
        delete[] std::any_cast<int16_t*>(*anyPointers[id]);
    }

    // Specialized handling for int16_t arrays
    int16_t* copyArray = new int16_t[data->get_item_count()];
    for (int i = 0; i < data->get_item_count(); i++) {
        copyArray[i] = data->get_int16_value(i);
    }
    *anyPointers[id] = copyArray;
    printf("int16_t array\n");
}

template <>
void HandleArrayCase<uint32_t>(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    if(anyPointers[id] && anyPointers[id]->has_value() && anyPointers[id]->type() == typeid(uint32_t*)) {
        // Deallocate previous data (if any)
        delete[] std::any_cast<uint32_t*>(*anyPointers[id]);
    }

    // Specialized handling for uint32_t arrays
    uint32_t* copyArray = new uint32_t[data->get_item_count()];
    for (int i = 0; i < data->get_item_count(); i++) {
        copyArray[i] = data->get_uint32_value(i);
    }
    *anyPointers[id] = copyArray;
    printf("uint32_t array\n");
}

template <>
void HandleArrayCase<int32_t>(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    if(anyPointers[id] && anyPointers[id]->has_value() && anyPointers[id]->type() == typeid(int32_t*)) {
        // Deallocate previous data (if any)
        delete[] std::any_cast<int32_t*>(*anyPointers[id]);
    }

    // Specialized handling for int32_t arrays
    int32_t* copyArray = new int32_t[data->get_item_count()];
    for (int i = 0; i < data->get_item_count(); i++) {
        copyArray[i] = data->get_int32_value(i);
    }
    *anyPointers[id] = copyArray;
    printf("int32_t array\n");
}

template <>
void HandleArrayCase<float>(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    if(anyPointers[id] && anyPointers[id]->has_value() && anyPointers[id]->type() == typeid(float*)) {
        // Deallocate previous data (if any)
        delete[] std::any_cast<float*>(*anyPointers[id]);
    }

    // Specialized handling for float arrays
    float* copyArray = new float[data->get_item_count()];
    for (int i = 0; i < data->get_item_count(); i++) {
        copyArray[i] = data->get_float32_value(i);
    }
    *anyPointers[id] = copyArray;
    printf("float array\n");
}

template <>
void HandleArrayCase<double>(eprosima::fastrtps::types::DynamicData* data, uint32_t id, std::vector<std::any*>& anyPointers) {
    if(anyPointers[id] && anyPointers[id]->has_value() && anyPointers[id]->type() == typeid(double*)) {
        // Deallocate previous data (if any)
        delete[] std::any_cast<double*>(*anyPointers[id]);
    }

    // Specialized handling for double arrays
    double* copyArray = new double[data->get_item_count()];
    for (int i = 0; i < data->get_item_count(); i++) {
        copyArray[i] = data->get_float64_value(i);
    }
    *anyPointers[id] = copyArray;
    printf("double array\n");
}



ScanDynamicSub::ScanDynamicSub()
    : mp_participant(nullptr)
    , mp_subscriber(nullptr)
    , m_listener(this)
{
}

bool ScanDynamicSub::init(std::string xml_path, std::string dyntype_name, std::string topic_name)
{
    if (eprosima::fastrtps::xmlparser::XMLP_ret::XML_OK !=
            eprosima::fastrtps::xmlparser::XMLProfileManager::loadXMLFile(xml_path))
    {
        std::cout << "Cannot open XML file " << xml_path << " Please, run the publisher from the folder that contatins this XML file." << std::endl;
        return false;
    }

    eprosima::fastrtps::types::DynamicType_ptr dyn_type =
            eprosima::fastrtps::xmlparser::XMLProfileManager::getDynamicTypeByName(dyntype_name)->build();
    TypeSupport m_type(new eprosima::fastrtps::types::DynamicPubSubType(dyn_type));

    DomainParticipantQos pqos;
    pqos.name("Participant_sub");
    StatusMask par_mask = StatusMask::subscription_matched() << StatusMask::data_available();

    mp_participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos, &m_listener, par_mask);

    if (mp_participant == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    m_type.get()->auto_fill_type_information(false);
    // m_type.get()->auto_fill_type_object(true);
    m_type.get()->auto_fill_type_object(false); //TODO: check if this is needed

    m_type.register_type(mp_participant);
    //TODO: vieleicht doppelt register type in initialize entities und hier noch recieved type setzen
    m_listener.received_type_ = dyn_type; 






    if (mp_subscriber == nullptr)
    {
        mp_subscriber = mp_participant->create_subscriber(
            SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (mp_subscriber == nullptr)
        {
            return false;
        }
    }

    //CREATE THE TOPIC
    Topic* topic = mp_participant->create_topic(
        topic_name,
        m_type->getName(),
        TOPIC_QOS_DEFAULT);

    if (topic == nullptr)
    {
        return false;
    }

    StatusMask sub_mask = StatusMask::subscription_matched() << StatusMask::data_available();
    DataReader* reader = mp_subscriber->create_datareader(
        topic,
        qos_,
        &m_listener,
        sub_mask);

    topics_[reader] = topic;
    readers_[reader] = dyn_type;
    eprosima::fastrtps::types::DynamicData_ptr data(
        eprosima::fastrtps::types::DynamicDataFactory::get_instance()->create_data(dyn_type));
    datas_[reader] = data;

















    // //Do not enable entities on creation
    // DomainParticipantFactoryQos factory_qos;
    // factory_qos.entity_factory().autoenable_created_entities = false;
    // DomainParticipantFactory::get_instance()->set_qos(factory_qos);

    // DomainParticipantQos pqos;
    // pqos.name("Participant_sub");
    // StatusMask par_mask = StatusMask::subscription_matched() << StatusMask::data_available();
    // mp_participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos, &m_listener, par_mask);

    // if (mp_participant == nullptr)
    // {
    //     return false;
    // }
    // if (mp_participant->enable() != ReturnCode_t::RETCODE_OK)
    // {
    //     DomainParticipantFactory::get_instance()->delete_participant(mp_participant);
    //     return false;
    // }

    // CREATE THE COMMON READER ATTRIBUTES
    qos_ = DATAREADER_QOS_DEFAULT;
    qos_.reliability().kind = RELIABLE_RELIABILITY_QOS;

    return true;
}

ScanDynamicSub::~ScanDynamicSub()
{
    for (const auto& it : topics_)
    {
        mp_subscriber->delete_datareader(it.first);
        mp_participant->delete_topic(it.second);
    }
    if (mp_subscriber != nullptr)
    {
        mp_participant->delete_subscriber(mp_subscriber);
    }

    DomainParticipantFactory::get_instance()->delete_participant(mp_participant);
    topics_.clear();
    readers_.clear();
    datas_.clear();
}

void ScanDynamicSub::SubListener::on_subscription_matched(
        DataReader*,
        const SubscriptionMatchedStatus& info)
{
    if (info.current_count_change == 1)
    {
        n_matched = info.total_count;
        std::cout << "Subscriber matched" << std::endl;
    }
    else if (info.current_count_change == -1)
    {
        n_matched = info.total_count;
        std::cout << "Subscriber unmatched" << std::endl;
    }
    else
    {
        std::cout << info.current_count_change
                  << " is not a valid value for SubscriptionMatchedStatus current count change" << std::endl;
    }
}

void ScanDynamicSub::SubListener::on_data_available(DataReader* reader)
{
    auto dit = subscriber_->datas_.find(reader);

    if (dit != subscriber_->datas_.end())
    {
        eprosima::fastrtps::types::DynamicData_ptr data = dit->second;
        SampleInfo info;
        if (reader->take_next_sample(data.get(), &info) == ReturnCode_t::RETCODE_OK)
        {
            if (info.instance_state == ALIVE_INSTANCE_STATE)
            {
                eprosima::fastrtps::types::DynamicType_ptr type = subscriber_->readers_[reader];
                this->n_samples++; //used as indicator of new data received
                std::cout << "Received data of type " << type->get_name() << std::endl;
                std::map<eprosima::fastrtps::types::MemberId, eprosima::fastrtps::types::DynamicTypeMember*> members;
	            type->get_all_members(members);


                for (auto pair : members)
                {
                    auto member = pair.second;
                    auto memberDesc = member->get_descriptor();
                    //bool ret = true;
                    switch (memberDesc->get_type()->get_kind())
                    {
                    case eprosima::fastrtps::types::TK_UINT16:
                        *this->anyPointers[memberDesc->get_id()] = data->get_uint16_value(memberDesc->get_id());
                        printf("uint16_t\n");
                        break;
                    case eprosima::fastrtps::types::TK_INT16:
                        *this->anyPointers[memberDesc->get_id()] = data->get_int16_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_UINT32:
                        *this->anyPointers[memberDesc->get_id()] = data->get_uint32_value(memberDesc->get_id());
                        printf("uint32_t\n");
                        break;
                    case eprosima::fastrtps::types::TK_INT32:
                        *this->anyPointers[memberDesc->get_id()] = data->get_int32_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_FLOAT32:
                        *this->anyPointers[memberDesc->get_id()] = data->get_float32_value(memberDesc->get_id());
                        break; 
                    case eprosima::fastrtps::types::TK_FLOAT64:
                        *this->anyPointers[memberDesc->get_id()] = data->get_float64_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_BOOLEAN:
                        *this->anyPointers[memberDesc->get_id()] = data->get_bool_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_CHAR8:
                        *this->anyPointers[memberDesc->get_id()] = data->get_char8_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_CHAR16:
                        *this->anyPointers[memberDesc->get_id()] = data->get_char16_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_STRING8:
                        *this->anyPointers[memberDesc->get_id()] = data->get_string_value(memberDesc->get_id());
                        break;
                    case eprosima::fastrtps::types::TK_STRING16:
                        *this->anyPointers[memberDesc->get_id()] = data->get_wstring_value(memberDesc->get_id());
                        break;  
                    case eprosima::fastrtps::types::TK_ARRAY:
                        eprosima::fastrtps::types::DynamicData* sequence;
                        data->get_complex_value(&sequence, memberDesc->get_id());
                        

                        switch (memberDesc->get_type()->get_descriptor()->get_element_type()->get_kind()) {    
                            case eprosima::fastrtps::types::TK_UINT16:
                                printf("hello1\n");
                                if(anyPointers[memberDesc->get_id()] && anyPointers[memberDesc->get_id()]->has_value() && anyPointers[memberDesc->get_id()]->type() == typeid(uint16_t*)) {
                                    // Deallocate previous data (if any)
                                    printf("hello2\n");
                                    delete[] std::any_cast<uint16_t*>(*anyPointers[memberDesc->get_id()]);
                                }
                                HandleArrayCase<uint16_t>(sequence, memberDesc->get_id(), anyPointers);
                                break;

                            case eprosima::fastrtps::types::TK_INT16:
                                HandleArrayCase<int16_t>(sequence, memberDesc->get_id(), anyPointers);
                                break;

                            case eprosima::fastrtps::types::TK_UINT32:
                                HandleArrayCase<uint32_t>(sequence, memberDesc->get_id(), anyPointers);
                                break;

                            case eprosima::fastrtps::types::TK_INT32:
                                HandleArrayCase<int32_t>(sequence, memberDesc->get_id(), anyPointers);
                                break;

                            case eprosima::fastrtps::types::TK_FLOAT32:
                                HandleArrayCase<float>(sequence, memberDesc->get_id(), anyPointers);
                                break;

                            case eprosima::fastrtps::types::TK_FLOAT64:
                                HandleArrayCase<double>(sequence, memberDesc->get_id(), anyPointers);
                                break;
                                
                            default:
                                // Unsupported array content type
                                std::cerr << "Unsupported array content type" << std::endl;
                                break;
                        }
                        break;
                        
                    default:
                        // Unsupported type
                        std::cerr << "Unsupported type" << std::endl;
                        break;    

                    }//end switch over type kind

                }//end for loop over members
                
            }//end if alive instance state
        }//end if take next sample
    }
}


// void ScanDynamicSub::SubListener::on_type_discovery(
//         DomainParticipant*,
//         const eprosima::fastrtps::rtps::SampleIdentity&,
//         const eprosima::fastrtps::string_255& topic_name,
//         const eprosima::fastrtps::types::TypeIdentifier*,
//         const eprosima::fastrtps::types::TypeObject*,
//         eprosima::fastrtps::types::DynamicType_ptr dyn_type)
// {
//     std::cout << "Discovered type: " << dyn_type->get_name() << " from topic " << topic_name << std::endl;
//     // received_type_ = dyn_type;
//     reception_flag_.store(true);
//     types_cv_.notify_one();
// }

void ScanDynamicSub::initialize_entities(std::string topic_name)
{
    auto type = m_listener.received_type_;
    std::cout << "Initializing DDS entities for type: " << type->get_name() << std::endl;
    TypeSupport m_type(new eprosima::fastrtps::types::DynamicPubSubType(type));
    m_type.register_type(mp_participant);

    if (mp_subscriber == nullptr)
    {
        mp_subscriber = mp_participant->create_subscriber(
            SUBSCRIBER_QOS_DEFAULT, nullptr);

        if (mp_subscriber == nullptr)
        {
            return;
        }
    }

    //CREATE THE TOPIC
    Topic* topic = mp_participant->create_topic(
        topic_name,
        m_type->getName(),
        TOPIC_QOS_DEFAULT);

    if (topic == nullptr)
    {
        return;
    }

    StatusMask sub_mask = StatusMask::subscription_matched() << StatusMask::data_available();
    DataReader* reader = mp_subscriber->create_datareader(
        topic,
        qos_,
        &m_listener,
        sub_mask);

    topics_[reader] = topic;
    readers_[reader] = type;
    eprosima::fastrtps::types::DynamicData_ptr data(
        eprosima::fastrtps::types::DynamicDataFactory::get_instance()->create_data(type));
    datas_[reader] = data;
}


void ScanDynamicSub::run(std::any *anyArray, size_t anyArraySize, std::string topic_name)
{
    std::cout << "Subscriber running. Please press enter to stop the Subscriber" << std::endl;
    // std::unique_lock<std::mutex> lock(m_listener.types_mx_); 
    // m_listener.types_cv_.wait(lock, [&]()
    //         {
    //             return m_listener.reception_flag_.exchange(false);
    //         });
    //initialize_entities(topic_name);
    printf("Initialized entities\n");

    for(int i=0; i<anyArraySize; i++){
        m_listener.anyPointers.push_back(&anyArray[i]);
    }

    m_listener.received_type_ = nullptr;
    
    // std::cin.ignore();
}

