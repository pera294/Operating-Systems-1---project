//
// Created by marko on 20.4.22..
//

#include "../h/tcb.hpp"
#include "../h/riscv.hpp"

TCB *TCB::running = nullptr;

uint64 TCB::timeSliceCounter = 0;

TCB *TCB::createThread(Body body,void *arg,uint64 *stack)
{
    //printString(" thread created \n");
    return new TCB((void (*)(void*))body, arg,stack,TIME_SLICE);
}

void TCB::yield()
{
     //asm volatile("mv a0, %0" : : "r" (0x13));
    //__asm__ volatile ("ecall");
    TCB::dispatch();
}

void TCB::dispatch()
{
    TCB *old = running;
    if (!old->isFinished() && !old->isBlocked()) { Scheduler::put(old); /*printString("dodaj u scheduler");*/ }
    running = Scheduler::get();
    TCB::contextSwitch(&old->context, &running->context);

}

void TCB::threadWrapper()
{
    Riscv::popSppSpie();

    running->body(running->arg);
    running->setFinished(true);
    //TCB::yield();
    //TCB::dispatch();
    thread_dispatch();
}

void TCB::unblock(TCB* ptr){
   // printString("unblock");
    ptr->setBlocked(false);
    Scheduler::put(ptr);

}

void TCB:: block(TCB* ptr){
    //printString("block");
    ptr->setBlocked(true);
    TCB::dispatch();

}

void* TCB::operator new(size_t n){
    return __mem_alloc(n);
    //return MemoryAllocator::mem_alloc(n);
}

void TCB::operator delete(void *ptr){
    __mem_free(ptr);
    //MemoryAllocator::mem_free(ptr);
}
