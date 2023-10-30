/**
 * @file ScanDynamicPub.cpp
 *
 */

#include "ScanDynamicPub.h"


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

    DomainParticipantQos pqos;
    pqos.name("Participant_pub");
    mp_participant = DomainParticipantFactory::get_instance()->create_participant(0, pqos);

    if (mp_participant == nullptr)
    {
        return false;
    }

    //REGISTER THE TYPE
    m_type.get()->auto_fill_type_information(false);
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
        writer_->write(m_ScanDyn.get());
        return true;
    }
    return false;
}
