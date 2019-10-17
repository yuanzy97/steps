#include "header/meter/continuous_buffer.h"
#include "header/steps_namespace.h"
#include "header/basic/utility.h"
#include <istream>
#include <iostream>
#include <algorithm>
/****
current time:T
step: t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T-5t  T-6t  T-7t
if a new data is appended, it becomes
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T-5t  T-6t  T+t
then
if a new data is appended, it becomes
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T-5t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T-4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T-3t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T-2t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T-t   T+6t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T     T+7t  T+6t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T+8   T+7t  T+6t  T+5t  T+4t  T+3t  T+2t  T+t
storage index: 0     1     2     3     4     5     6     7
data:          T+8   T+7t  T+6t  T+5t  T+4t  T+3t  T+2t  T+9t
****/


CONTINUOUS_BUFFER::CONTINUOUS_BUFFER()
{
    set_buffer_size(1);
    index_of_buffer_head = 0;
}

CONTINUOUS_BUFFER::CONTINUOUS_BUFFER(const CONTINUOUS_BUFFER& buffer)
{
    clear();
    copy_from_constant_buffer(buffer);
}

CONTINUOUS_BUFFER& CONTINUOUS_BUFFER::operator=(const CONTINUOUS_BUFFER& buffer)
{
    clear();
    if(this==&buffer)
        return *this;
    copy_from_constant_buffer(buffer);
    return *this;
}


void CONTINUOUS_BUFFER::copy_from_constant_buffer(const CONTINUOUS_BUFFER& buffer)
{
    index_of_buffer_head = 0;
    set_buffer_size(buffer.get_buffer_size());

    size_t from_buffer_size = buffer.get_buffer_size();
    size_t N = min(buffer_size, from_buffer_size);

    for(size_t i=0; i!=N; ++i)
    {
        time_buffer[i] = buffer.get_buffer_time_at_delay_index(i);
        value_buffer[i] = buffer.get_buffer_value_at_delay_index(i);
    }
}

CONTINUOUS_BUFFER::~CONTINUOUS_BUFFER()
{
    //time_buffer.clear();
    //value_buffer.clear();
}

void CONTINUOUS_BUFFER::clear()
{
    index_of_buffer_head = 0;
    for(size_t i=0; i!=buffer_size; ++i)
    {
        time_buffer[i] = 0.0;
        value_buffer[i] = 0.0;
    }
}

bool CONTINUOUS_BUFFER::is_valid() const
{
    return true;
}

void CONTINUOUS_BUFFER::check()
{
    ;
}

void CONTINUOUS_BUFFER::set_buffer_size(size_t buffer_size)
{
    if(buffer_size==0)
        buffer_size = 1;
    this->buffer_size = buffer_size;
    time_buffer.resize(buffer_size, 0.0);
    value_buffer.resize(buffer_size, 0.0);
}

size_t CONTINUOUS_BUFFER::get_buffer_size() const
{
    return buffer_size;
}

void CONTINUOUS_BUFFER::initialize_buffer(double initial_time, double value)
{
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    double delt = toolkit.get_dynamic_simulation_time_step_in_s();

    index_of_buffer_head = 0;
    for(size_t i=0; i!=buffer_size; ++i)
    {
        time_buffer[index_of_buffer_head+i] = initial_time-i*delt;
        value_buffer[index_of_buffer_head+i] = value;
    }
}

void CONTINUOUS_BUFFER::append_data(double time, double value)
{
    if(buffer_size==1)
    {
        time_buffer[0] = time;
        value_buffer[0] = value;
        return;
    }
    else
    {
        double time_at_head = get_buffer_time_at_head();
        if(fabs(time-time_at_head)<FLOAT_EPSILON)
            value_buffer[index_of_buffer_head] = value;
        else
        {
            if(time<time_at_head)
                return;
            else
            {
                if(index_of_buffer_head>0)
                    --index_of_buffer_head;
                else
                    index_of_buffer_head = buffer_size-1;

                time_buffer[index_of_buffer_head] = time;
                value_buffer[index_of_buffer_head] = value;
            }
        }
    }
}

size_t CONTINUOUS_BUFFER::get_index_of_buffer_head() const
{
    return index_of_buffer_head;
}

size_t CONTINUOUS_BUFFER::get_storage_index_of_delay_index(size_t index) const
{
    index = index_of_buffer_head+index;
    while(index>=buffer_size)
        index -= buffer_size;
    return index;
}

double CONTINUOUS_BUFFER::get_buffer_time_at_head() const
{
    return time_buffer[index_of_buffer_head];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_head() const
{
    return value_buffer[index_of_buffer_head];
}


double CONTINUOUS_BUFFER::get_buffer_time_at_delay_index(size_t index) const
{
    index = get_storage_index_of_delay_index(index);
    return time_buffer[index];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_delay_index(size_t index) const
{
    index = get_storage_index_of_delay_index(index);
    return value_buffer[index];
}

double CONTINUOUS_BUFFER::get_buffer_value_at_time(double time) const
{
    size_t index = get_delay_index_of_time(time);
    if(index==INDEX_NOT_EXIST)
        return 0.0;
    else
        return get_buffer_value_at_delay_index(index);
}

size_t CONTINUOUS_BUFFER::get_delay_index_of_time(double time) const
{
    size_t index_of_buffer_tail;
    if(index_of_buffer_head==0)
        index_of_buffer_tail = buffer_size-1;
    else
        index_of_buffer_tail = index_of_buffer_head-1;

    double time_at_head = get_buffer_time_at_head();
    double time_at_tail = time_buffer[index_of_buffer_tail];

    if(fabs(time_at_head-time)<FLOAT_EPSILON)
        return 0;
    if(fabs(time_at_tail-time)<FLOAT_EPSILON)
        return buffer_size-1;

    if(time<time_at_tail or time>time_at_head)
        return INDEX_NOT_EXIST;

    double time_error = fabs(time-get_buffer_time_at_head());
    size_t index = 0;
    for(size_t i=1; i!=buffer_size; ++i)
    {
        double delayed_time = get_buffer_time_at_delay_index(i);
        if(time_error>fabs(time-delayed_time))
        {
            index = i;
            time_error = fabs(time-delayed_time);
        }
        else
            break;
    }
    return index;
}
void CONTINUOUS_BUFFER::show_buffer() const
{
    ostringstream osstream;
    osstream<<"Buffer conttens:"<<endl;
    osstream<<"TIME,VALUE"<<endl;
    for(size_t i=0; i!=buffer_size; ++i)
    {
        osstream<<get_buffer_time_at_delay_index(i)<<","<<get_buffer_value_at_delay_index(i)<<endl;
    }
    STEPS& toolkit = get_toolkit(__PRETTY_FUNCTION__);
    toolkit.show_information_with_leading_time_stamp(osstream);
}
