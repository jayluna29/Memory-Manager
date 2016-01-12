//
//  MemoryManager.cpp
//  MemoryManager
//
//  Created by Justin Luna on 12/6/15.
//  Copyright © 2015 Justin Luna. All rights reserved.
//

#include <string>
#include <iostream>
#include "MemoryManager.h"


using namespace std;

MemoryManager:: MemoryManager(char* byte, int requestSize)
{
    //A constructor that takes a pointer to a chunk of bytes to be managed by the memory manager and an integer representing the size of that heap space in bytes
    sizeOfBlock = requestSize;
    b = byte;
    
    p_head = (Node*)(b + sizeof(Node));
    p_head->sizeInBytes = sizeOfBlock - (2 * sizeof(Node));
    p_head->freeSpace = true;
    
    p_head->p_left = 0;
    p_head->p_right = (Node*)(b+(sizeOfBlock-sizeof(Node)));
    
    
    p_tail = p_head->p_right;
    p_tail->p_right = 0;
    p_tail->p_left = p_head;
    p_tail->freeSpace = false;
    
    
}
MemoryManager::~MemoryManager()
{
    // we need to delete this off the heap
    delete [] b;
    b = 0;
}
void* MemoryManager:: allocate(int requestSize)
{
    Node* retVal = 0;
    int largest = 0;
    Node* hold_largest = 0;
    
    // We need to make current head so that we can later
    // control and get rid of
    p_current = p_head;
    
    
    while(p_current -> p_right != 0)
    {
        // while that is done we are going to search for the free space
        if(p_current ->sizeInBytes > largest && p_current -> freeSpace)
        {
            largest = p_current ->sizeInBytes;
            hold_largest = p_current;
        }
        p_current = p_current -> p_right;
    }
    //we are holding largest here space
    p_current = hold_largest;
    
    
    if(requestSize <= largest - sizeof(Node))
    {
        //will set a new node to divide the memory and set part to be used memory
        Node* nodeSplit;
        
        nodeSplit = (Node*)(((char*)p_current) + requestSize + sizeof(Node));
        nodeSplit -> freeSpace = true;
        nodeSplit ->p_left = p_current;
        nodeSplit -> p_right = p_current -> p_right;
        nodeSplit -> sizeInBytes = p_current-> sizeInBytes - requestSize - sizeof(Node);
        
    
        p_current ->p_right -> p_left = nodeSplit;
        p_current -> p_right = nodeSplit;
        p_current -> freeSpace = false;
        p_current -> sizeInBytes = requestSize;
        
        // adding one to that of current size
        retVal = p_current + 1;
    }
    //not returning a pointer of nothing
    return (void*) retVal;
    
}
void MemoryManager:: dump()
{
    Node* p_node = p_head;
    int counter = 0;
    
    //while there are more nodes they will each be printed out
    while(p_node != 0)
    {
        if(p_node != p_tail)
        {
            if(p_node ->freeSpace)
            {
                
                // here we are just making the print statements that are going to cout
                cout << "     Block " << counter + 1 << ":" << p_node -> sizeInBytes << " bytes of free space" << endl;
            }
            // same for the else statement
            else
            {
                cout << "     Block " << counter + 1 << ":" << p_node -> sizeInBytes << " bytes of used space" << endl;
            }
        }
        counter++;
        
        p_node = p_node -> p_right;
    }
}
void MemoryManager:: deallocate(void* pointerToRemove)
{
    //will reset the current to the head.
    p_current = p_head;
    while(p_current -> p_right != 0)
    {
        //will check if the current node is the node that is desired to remove
        if(p_current + 1 != pointerToRemove)
        {
            p_current = p_current -> p_right;
        }
        else
        {
            //if the spot is the head spot
            if(p_current == p_head)
            {
                //if the heads p_next is free
                if(p_current -> p_right -> freeSpace)
                {
                    p_head -> sizeInBytes = p_head -> sizeInBytes + p_head -> p_right -> sizeInBytes + sizeof(Node);
                    p_head -> p_right = p_current -> p_right -> p_right;
                    p_head -> p_right -> p_left = p_head;
                    p_current -> freeSpace = true;
                    break;
                }
                
                else
                {
                    p_head -> freeSpace = true;
                    break;
                }
            }
            else if(p_current == p_tail -> p_left)
            {
                //if the tails p_prev is free
                if(p_current -> p_left -> freeSpace)
                {
                    p_current -> p_left -> sizeInBytes = p_current -> sizeInBytes + p_current -> p_left -> sizeInBytes + sizeof(Node);
                    p_tail -> p_left -> p_left -> p_right = p_tail;
                    p_tail -> p_left = p_current -> p_left;

                    break;
                }
                else
                {
                    //if the tails p_prev is not free
                    p_tail ->freeSpace = true;
                    break;
                }
            }
            //UUU
            //this is the case where the desired node to be removed is in between two other used chunks of memory
            else if(!p_current -> p_right -> freeSpace && !p_current ->p_left ->freeSpace)
            {
                p_current ->freeSpace = true;
                break;
            }
            //FUF
            //this is the case where the desired node to be removed is in between two other free chunks of memory
            else if(p_current -> p_right -> freeSpace && p_current ->p_left ->freeSpace)
            {
                p_current -> p_left -> p_right = p_current -> p_right -> p_right;
                p_current -> p_right -> p_right -> p_left = p_current -> p_left;
                p_current -> p_left -> sizeInBytes = p_current -> sizeInBytes + p_current -> p_left -> sizeInBytes + p_current -> p_right -> sizeInBytes + (2*sizeof(Node));
                break;
            }
            //FUU
            //this is the case where the desired node to be removed is in between a free node on the left and a used node on the right.
            else if(!p_current -> p_right -> freeSpace && p_current ->p_left ->freeSpace)
            {
                
                // section is confusing had to draw it out like
                //linked list 2
                p_current -> p_left -> p_right = p_current -> p_right;
                p_current  ->  p_right -> p_left = p_current -> p_left;
                p_current -> p_left -> sizeInBytes = p_current -> sizeInBytes + p_current -> p_left -> sizeInBytes + sizeof(Node);
                p_current = p_current -> p_left;
                break;
            }
            else
            {
                p_current ->freeSpace =  true;
                p_current -> sizeInBytes = p_current -> sizeInBytes + p_current -> p_right -> sizeInBytes + sizeof(Node);
                p_current -> p_right->p_right -> p_left = p_current;
                p_current -> p_right = p_current -> p_right->p_right;
                break;
            }
        }
    }

}
