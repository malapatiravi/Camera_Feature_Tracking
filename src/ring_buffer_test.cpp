#include "ring_buffer.h"
#include <iostream>

int main()
{
    RingBuffer<int, 2> ring_buffer;
    ring_buffer.Push(1);
    //std::cout<<*ring_buffer.GetEnd(0)<<std::endl;
    ring_buffer.Push(2);
    std::cout<<*ring_buffer.GetEnd(0)<<std::endl;
    std::cout<<*ring_buffer.GetEnd(1)<<std::endl;
    ring_buffer.Push(3);
    std::cout<<*ring_buffer.GetEnd(0)<<std::endl;
    std::cout<<*ring_buffer.GetEnd(1)<<std::endl;
    ring_buffer.Push(4);
    ring_buffer.Push(5);
    std::cout<<*ring_buffer.GetEnd(0)<<std::endl;
    std::cout<<*ring_buffer.GetEnd(1)<<std::endl;
    ring_buffer.Push(6);

}