//
// Created by os on 8/8/22.
//

#ifndef PROJECT_BASE_V1_1_SEM_H
#define PROJECT_BASE_V1_1_SEM_H

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"

class SCB {

public:

    //~SCB(){}

    uint64 getValue(){ return this->value;}
    void setValue(uint64 value){ this->value = value;}

    static SCB *createSemaphore (uint64 value);
    static void signal(SCB* ptr);
    static void wait(SCB* ptr);
    static void close(SCB*ptr);

    void* operator new(size_t n);
    void operator delete(void* ptr);


private:

     uint64 value;
     List<TCB> blockedQueue;

    SCB(uint64 val){
        this->value=val;
    }

    static void signal();
    static void wait();

    friend class Riscv;
};


#endif //PROJECT_BASE_V1_1_SEM_H
