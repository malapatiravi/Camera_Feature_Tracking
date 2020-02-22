
/*
* ring_buffer.h
*/

#ifndef RING_BUFFER_H
#define RING_BUFFER_H

#include <iostream>
#include <cstdlib>

template <typename T, size_t size_rb>
class RingBuffer
{
public:
    RingBuffer();
    ~RingBuffer();
    void Clear();
    bool Push(const T &data_rb);
    T PopFront();
    size_t UnReaddataSize();
    void incReadIndex();
    void incWriteIndex();
    void incBufferSize();
    typedef T value_type;

    T buffers_rb[size_rb];
    size_t used_buffers_size;
    size_t next_write_index;
    size_t next_read_index;

    size_t size();
    
};

template <typename T, size_t size_rb>
RingBuffer<T, size_rb>::RingBuffer()
{
    next_write_index = -1;
    used_buffers_size = 0;
    next_read_index = -1;
}

template <typename T, size_t size_rb>
RingBuffer<T, size_rb>::~RingBuffer()
{
}
template <typename T, size_t size_rb>
void RingBuffer<T, size_rb>::Clear()
{
    next_write_index = -1;
    used_buffers_size = 0;
    next_read_index = -1;
}
template <typename T, size_t size_rb>
void RingBuffer<T, size_rb>::incReadIndex()
{
    next_read_index++;
    if (next_read_index >= size_rb)
    {
        next_read_index = 0;
    }
}

template <typename T, size_t size_rb>
void RingBuffer<T, size_rb>::incBufferSize()
{

    if (used_buffers_size < size_rb)
    {
        used_buffers_size++;
    }
}

template <typename T, size_t size_rb>
bool RingBuffer<T, size_rb>::Push(const T &data_rb)
{
    bool is_success = true;
    next_write_index++;
    if (next_write_index >= size_rb)
    {
        next_write_index = 0;
    }

    buffers_rb[next_write_index] = data_rb;
    //std::cout<<"Pushed "<<data_rb<<std::endl;
    incBufferSize();
    //std::cout<<"After Pushing data buffer size is "<<used_buffers_size<<" read index is "<<next_read_index<<" write index is "<<next_write_index<<std::endl;
    return is_success;
}

template <typename T, size_t size_rb>
T RingBuffer<T, size_rb>::PopFront()
{
    T ret_data;

    if (next_write_index == -1 || used_buffers_size <= 0)
    {
        std::cout << "No data to read" << next_read_index << "  " << next_write_index << std::endl;
    }
    else if (used_buffers_size > 0)
    {
        if (next_read_index != next_write_index || used_buffers_size > 0)
        {
            next_read_index++;
            if (next_read_index >= size_rb)
            {
                next_read_index = 0;
            }
            ret_data = buffers_rb[next_read_index];
            used_buffers_size--;

            //std::cout<<"After reading data buffer size is "<<used_buffers_size<<" read index is "<<next_read_index<<" write index is "<<next_write_index<<std::endl;
        }
        // else
        // {
        //     std::cout<<"Read index is equal to write index"<<std::endl;
        // }
    }
    return ret_data;
}

template <typename T, size_t size_rb>
size_t RingBuffer<T, size_rb>::UnReaddataSize()
{
    return used_buffers_size;
}


template <typename T, size_t size_rb>
size_t RingBuffer<T, size_rb>::size()
{
    return used_buffers_size;
}
#endif