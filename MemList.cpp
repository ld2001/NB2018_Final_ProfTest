// Copyright (c) 2018 Jason Graalum
//
// 
// MemList.cpp
//
// // Class implementation of MemList for New Beginnings Final Proficiency Exam
// 2018

#include "MemList.hpp"
#include "MemBlock.hpp"
#include <iostream>

using namespace std;
/////////////////////////////////////////////////////////////////////////////////
// Level 0 Methods
/////////////////////////////////////////////////////////////////////////////////
// Default constructor which creates empty lists for both the Free and Reserved
// lists
//
// Level 0
//
MemList::MemList()
{
    free_head = NULL;
    reserved_head = NULL;
}
// Display the Free List
//
// Level 0
//
void MemList::displayFree() 
{
    std::cout << "Free Memory" << std::endl;
    MemBlock * current = free_head;
    free_head->printBlockInfoHeader();
    int count = 0;
    while(current) {
        current->printBlockInfo(count);
        current = current->getNext();
        count++;
    }
    std::cout << std::endl;
}

// Display the Reserved List
//
// Level 0
//
void MemList::displayReserved() 
{
    std::cout << "Reserved Memory" << std::endl;
    MemBlock * current = reserved_head;
    reserved_head->printBlockInfoHeader();

    int count = 0;
    while(current) {
        current->printBlockInfo(count);
        current = current->getNext();
        count++;
    }
    std::cout << std::endl;

}
/////////////////////////////////////////////////////////////////////////////////
// Level 1 Methods
/////////////////////////////////////////////////////////////////////////////////
// Parameterized constructor which create two lists of MemBlock 
// The Free List will contain a single MemBlock with all the memory(start_addr and mem_size)
// The Reserved List will be empty
//
// MemList = new MemList(start_addr, mem_size);
//
// Level 1
//
MemList::MemList(unsigned int s_addr, unsigned int block_size)
{
   MemBlock * temp = new MemBlock(s_addr, block_size);
   temp->setAddr(s_addr);
   temp->setSize(block_size);
   free_head = temp;
   MemBlock * temp2 = new MemBlock(0, 0);
   temp2->setAddr(0);
   temp2->setSize(0);
   reserved_head = temp2;
   

}

// Find the first MemBlock in the Free list which greater than or equal to the amount requested(via 
// the function argument). Update the MemBlock in the Free List - change the start_addr and mem_size
// by the amount of memory being reserved.
// Add the newly reserved memory into a new MemBlock in the Reserved List
// If no MemBlock in the Free List is large enough return NULL, otherwise return a pointer to 
// the new MemBlock in the Reserved List.
//
// Level 1
//
MemBlock * MemList::reserveMemBlock(unsigned int block_size)
{
   MemBlock * current = free_head;
   
    if(freeSize() < block_size) 
      { cout << "Error, not enough free memory" << endl;
      return NULL;}
    else {
      while(current->getNext() != NULL) {
      if(current->getSize() >= block_size) {
         MemBlock * temp = new MemBlock (current->getAddr(), block_size);
         temp->setNext(reserved_head);
         reserved_head = temp;
         current->setAddr(current->getAddr() + block_size);
         current->setSize(current->getSize() - block_size);
         return temp;
       } else { current = current->getNext();}}
       if(current->getSize() >= block_size) {
         MemBlock * temp = new MemBlock (current->getAddr(), block_size);
         temp->setNext(reserved_head);
         reserved_head = temp;
         current->setAddr(current->getAddr() + block_size);
         current->setSize(current->getSize() - block_size);
         return temp;
         }
      return NULL;
    }
}




// Return the total size of all blocks in the Reserved List
//
// Level 1
//
unsigned int MemList::reservedSize()
{
    MemBlock * current = reserved_head;
    unsigned int x = 0;

    while(current->getNext() != NULL) {
       x += current->getSize();
      current = current->getNext();
    }
      x += current->getSize();

   return x;  

}

// Return the total size of all blocks in the Free List
//
// Level 1
unsigned int MemList::freeSize()
{
    MemBlock * current = free_head;
    unsigned int x = 0;

    while(current->getNext() != NULL) {
       x += current->getSize();
      current = current->getNext();
    }
      x += current->getSize();

   return x;
}

/////////////////////////////////////////////////////////////////////////////////
// Level 2 Methods
/////////////////////////////////////////////////////////////////////////////////
// Removes the MemBlock provided(via the pointer) from the Reserved List and adds it
// back into the Free List - it must be added back "in order of starting address".
//
// (Challenge) If the MemBlock to be freed does exist or it is a bad block(it overlaps 
// with a block in the Free list), return false.  Otherwise return true.
//
// Level 2
//
bool MemList::freeMemBlock(MemBlock * block_to_free)
{
    if(block_to_free == NULL) {return false;} else {
    
    MemBlock * current_free = free_head;
    MemBlock * current_reserved = reserved_head;

   while(current_reserved->getNext() != block_to_free)
   {
      current_reserved = current_reserved->getNext();
   }

   if(free_head == NULL) {
      free_head = block_to_free;
      current_reserved->setNext(block_to_free->getNext());
      block_to_free->setNext(NULL);
    } else if(block_to_free->getAddr() < free_head->getAddr()) { 
      current_reserved->setNext(block_to_free->getNext());
      block_to_free->setNext(free_head); 
      free_head = block_to_free;
    } else {
      while(current_free->getNext()->getAddr() < block_to_free->getAddr() && current_free->getNext() != NULL) {
         current_free = current_free->getNext();
      }
      
      current_reserved->setNext(block_to_free->getNext());
      block_to_free->setNext(current_free->getNext());
      current_free->setNext(block_to_free);
    }
    return true;
    }
}



// Return a pointer to the MemBlcok with the largest size from the Free List
//
// Level 2
//
MemBlock * MemList::maxFree() 
{
    MemBlock * current = free_head;
    MemBlock * max = free_head;

    while(current->getNext() != NULL) {
      if(max->getSize() < current->getSize()) {
         max = current;
      }
      current = current->getNext();
    }
    return max;
}

// Return a pointer to the MemBlcok with the smallest size from the Free List
//
// Level 2
//
MemBlock * MemList::minFree()
{
    MemBlock * current = free_head;
    MemBlock * min = free_head;

    while(current->getNext() != NULL) {
      if(min->getSize() > current->getSize()) {
         min = current;
      }
      current = current->getNext();
    }
    return min;
}

// Return the number of MemBlocks in the Free List
//
// Level 2
//
unsigned int MemList::freeBlockCount()
{
    MemBlock * current = free_head;
    unsigned int count = 0;

    while(current->getNext() != NULL) {
      if(current->getSize() != 0) {
         count++;
      }
      current = current->getNext();
    }
    return count;
 
}

/////////////////////////////////////////////////////////////////////////////////
// Level 3 Methods
/////////////////////////////////////////////////////////////////////////////////
// Iterate through the Free List - combine any blocks that are adjacent
// Return the number of MemBlocks removed
//
// Level 3
//
unsigned int MemList::defragFree()
{
    // To be implemented
    return 0;
}

// Return the start address of the smallest block that fits the size requested
// Ex:  MemList_Obj->minMax(100);  // Return the address of the smallest block 
//                                    that is greater than 100
// The return values are the same as in the "reserveMemBlock" member function.
//
// Level 3
//
MemBlock * MemList::reserveMinMemBlock(unsigned int)
{
    // To be implemented
    return NULL;
}




