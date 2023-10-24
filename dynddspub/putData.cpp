
#include "ScanDynamicPub.h"


//array functions
void ScanDynamicPub::putData_uint16_array(uint16_t *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_uint16_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

void ScanDynamicPub::putData_uint32_array(uint32_t *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_uint32_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

void ScanDynamicPub::putData_int32_array(int32_t *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_int32_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

void ScanDynamicPub::putData_int64_array(int64_t *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_int64_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

void ScanDynamicPub::putData_float32_array(float *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_float32_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

void ScanDynamicPub::putData_float64_array(double *distance, int size, int member_id)
{
    eprosima::fastrtps::types::DynamicData* array = m_ScanDyn->loan_value(member_id);
    for(unsigned int i=0; i<size; i++){
        array->set_float64_value(distance[i], array->get_array_index({0, i}));
    }
    m_ScanDyn->return_loaned_value(array); 
}

//value functions
void ScanDynamicPub::putData_uint16_value(uint16_t value, int member_id)
{
    m_ScanDyn->set_uint16_value(value, member_id);
}

void ScanDynamicPub::putData_uint32_value(uint32_t value, int member_id)
{
    m_ScanDyn->set_uint32_value(value, member_id);
}

void ScanDynamicPub::putData_int32_value(int32_t value, int member_id)
{
    m_ScanDyn->set_int32_value(value, member_id);
}

void ScanDynamicPub::putData_int64_value(int64_t value, int member_id)
{
    m_ScanDyn->set_int64_value(value, member_id);
}

void ScanDynamicPub::putData_float32_value(float value, int member_id)
{
    m_ScanDyn->set_float32_value(value, member_id);
}

void ScanDynamicPub::putData_float64_value(double value, int member_id)
{
    m_ScanDyn->set_float64_value(value, member_id);
}

void ScanDynamicPub::putData_char8_value(char value, int member_id)
{
    m_ScanDyn->set_char8_value(value, member_id);
}

void ScanDynamicPub::putData_char16_value(wchar_t value, int member_id)
{
    m_ScanDyn->set_char16_value(value, member_id);
}

void ScanDynamicPub::putData_string_value(std::string value, int member_id)
{
    m_ScanDyn->set_string_value(value, member_id);
}

void ScanDynamicPub::putData_wstring_value(std::wstring value, int member_id)
{
    m_ScanDyn->set_wstring_value(value, member_id);
}

void ScanDynamicPub::putData_bool_value(bool value, int member_id)
{
    m_ScanDyn->set_bool_value(value, member_id);
}

