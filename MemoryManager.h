//
//  MemoryManager.h
//  MemoryManager
//
//  Created by Justin Luna on 12/6/15.
//  Copyright © 2015 Justin Luna. All rights reserved.
//

#pragma once

class MemoryManager
{
public:
    MemoryManager(char* byte, int requestSize);
    ~MemoryManager();
    void deallocate (void* pointerToRemove);
    void dump();
    void* allocate (int requestSize);
    
private:
    // create the node with pointers
    struct Node
    {
        Node* p_right;
        Node* p_left;
        int sizeInBytes;
        bool freeSpace;
    };
    char* b;
    Node* p_head;
    Node* p_tail;
    int sizeOfBlock;
    Node*p_current;
};
