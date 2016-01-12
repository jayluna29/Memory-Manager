//
//  main.cpp
//  MemoryManager
//
//  Created by Justin Luna on 12/6/15.
//  Copyright © 2015 Justin Luna. All rights reserved.
//

#include <iostream>
#include "MemoryManager.h"

using namespace std;

//structure used later in a dynamic memory allocation
struct Point
{
    double x;
    double y;
    double z;
};

int main()
{
    //size of the managed heap
    const int heapSize = 10000;
    
    //create 10000 bytes of heap space (memory is actually a char pointer)
    char memory[heapSize];
    
    //let the memory manager know which chunk of memory it is managing
    MemoryManager m(memory,heapSize);
    
    cout<<"Initial look at memory"<<endl;
    //show memory
    m.dump();
    
    cout<<"Allocating 1000 ints to p_int (size required "<<(sizeof(int) * 1000)<<" bytes)"<<endl;
    //allocate room for 1000 ints. Allocate returns a void* that must be cast to the
    //correct type. The program needs to know the type in order to do pointer
    //arithmetic (array notation)
    int* p_int = (int*)(m.allocate(sizeof(int) * 1000));
    
    //if there was room on the heap the pointer will not be 0
    if(p_int != 0)
    {
        //use the memory somehow
        for(int i = 0;i < 1000;i++)
        {
            //store all 0's in the block of memory
            p_int[i] = 0;
        }
    }
    
    //show memory
    m.dump();
    
    cout<<"Allocating 100 doubles to p_double (size required "<<(sizeof(double) * 100)<<" bytes)"<<endl;
    //allocate room for 100 doubles
    double* p_double = (double*)(m.allocate(sizeof(double) * 100));
    
    //if there was room on the heap the pointer will not be 0
    if(p_double != 0)
    {
        for(int i = 0;i < 100;i++)
        {
            //use the memory by storing all 0.0's
            p_double[i] = 0.0;
        }
    }
    
    //show memory
    m.dump();
    
    cout<<"Allocating 10 Point structs to p_point (size required "<<(sizeof(Point) * 10)<<" bytes)"<<endl;
    //allocate room for 10 Points
    Point* p_point = (Point*)(m.allocate(sizeof(Point) * 10));
    
    //to be used to copy over
    Point p = {2.0, 4.0, 2.4};
    
    //if there was room on the heap the pointer will not be 0
    if(p_point != 0)
    {
        for(int i = 0;i < 10;i++)
        {
            //use the memory by copying p multiple times
            p_point[i] = p;
        }
    }
    
    //show memory
    m.dump();
    
    cout<<"Allocating 1000 more ints to p_int2 (size required "<<(sizeof(int) * 1000)<<" bytes)"<<endl;
    //allocate room for 1000 ints
    int* p_int2 = (int*)(m.allocate(sizeof(int) * 1000));
    
    //if there was room on the heap the pointer will not be 0
    if(p_int2 != 0)
    {
        //use the memory
        for(int i = 0;i < 1000;i++)
        {
            p_int2[i] = 0;
        }
    }
    
    //show memory
    m.dump();
    
    cout<<"Allocating 1000 more ints to p_int3 (size required "<<(sizeof(int) * 1000)<<" bytes- shouldn't work!)"<<endl;
    //shouldn't work
    int* p_int3 = (int*)(m.allocate(sizeof(int) * 1000));
    
    //if there was room on the heap the pointer will not be 0
    if(p_int3 != 0)
    {
        //use the memory
        for(int i = 0;i < 1000;i++)
        {
            p_int3[i] = 0;
        }
    }
    //show memory
    m.dump();
    
    //now start giving some of the memory back
    cout<<"Deallocating p_int"<<endl;
    m.deallocate(p_int);
    m.dump();
    cout<<"Deallocating p_double"<<endl;
    m.deallocate(p_double);
    m.dump();
    cout<<"Deallocating p_int2"<<endl;
    m.deallocate(p_int2);
    m.dump();
    cout<<"Deallocating p_point"<<endl;
    m.deallocate(p_point);
    m.dump();
    cout<<"Deallocating p_int3- shouldn't change memory"<<endl;
    m.deallocate(p_int3);
    m.dump();
    
    cout<<"Allocating 1000 more ints to p_int3 (should work this time)"<<endl;
    //should work
    p_int3 = (int*)(m.allocate(sizeof(int) * 1000));
    
    //if there was room on the heap the pointer will not be 0
    if(p_int3 != 0)
    {
        //use the memory
        for(int i = 0;i < 1000;i++)
        {
            p_int3[i] = 0;
        }
    }
    //show memory
    m.dump();
    
    cout<<"Deallocating p_int3"<<endl;
    //get rid of the memory
    m.deallocate(p_int3);
    
    //show memory
    m.dump();
    
    
    return 0;
}
