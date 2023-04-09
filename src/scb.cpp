//
// Created by os on 8/8/22.
//

#include "../h/scb.hpp"


SCB *SCB::createSemaphore(uint64 value) {
    return new SCB(value);
}

void SCB::signal(SCB *ptr) {

    if(ptr->blockedQueue.peekFirst()==nullptr) {
        ptr->value++;
    }
    else{
        TCB* thread=ptr->blockedQueue.removeFirst();
       TCB::unblock(thread);
    }
}

void SCB:: wait(SCB* ptr){

    if(ptr->value>0) {
        ptr->value--;
    }
    else{
        ptr->blockedQueue.addLast(TCB::running);
        TCB::block(TCB::running);
    }
}

void SCB :: close(SCB*ptr){

    TCB* thread;
    thread=ptr->blockedQueue.removeFirst();

    while(thread!=0){
        TCB::unblock(thread);
        thread=ptr->blockedQueue.removeFirst();
    }
}

void* SCB::operator new(size_t n) {
    return __mem_alloc(n);
    //return MemoryAllocator::mem_alloc(n);
}

void SCB::operator delete(void *ptr) {
    __mem_free(ptr);
    //MemoryAllocator::mem_free(ptr);
}



