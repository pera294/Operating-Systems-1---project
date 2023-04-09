//
// Created by os on 8/2/22.
//

#include "../h/syscall_c.h"



void* mem_alloc (size_t size){

    size_t numBlocks;
    numBlocks= size/MEM_BLOCK_SIZE;
    if(size%MEM_BLOCK_SIZE>0) numBlocks++;
    numBlocks=numBlocks*64;

    __asm__ volatile("mv a1, %0" : : "r" (numBlocks));
    __asm__ volatile("mv a0, %0" : : "r" (0x01));

    __asm__ volatile("ecall");

    void* ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;

}

int mem_free (void* pointer){

    __asm__ volatile("mv a1, %0" : : "r" (pointer));
    __asm__ volatile("mv a0, %0" : : "r" (0x02));

    __asm__ volatile("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}



int thread_create (thread_t* handle, void(*start_routine)(void*), void *arg){

   void* stack;
    stack = mem_alloc(DEFAULT_STACK_SIZE);
   //stack=MemoryAllocator::mem_alloc(DEFAULT_STACK_SIZE);
    if(stack == nullptr) return -1;

    __asm__ volatile("mv a7, %0" : : "r" (stack));
    __asm__ volatile("mv a6, %0" : : "r" (arg));
    __asm__ volatile("mv a2, %0" : : "r" (start_routine));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (0x11));

    __asm__ volatile("ecall");

    if(*handle == nullptr) return -1;

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;

}

int thread_exit (){

    asm volatile("mv a0, %0" : : "r" (0x12));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

void thread_dispatch (){

    asm volatile("mv a0, %0" : : "r" (0x13));


    __asm__ volatile ("ecall");

    return;
}

int sem_open (sem_t *handle, unsigned init){

    __asm__ volatile("mv a2, %0" : : "r" (init));
    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (0x21));

    __asm__ volatile ("ecall");

    if(*handle == nullptr) return -1;

    return 0;


}

int sem_close (sem_t handle){

    __asm__ volatile("mv a1, %0" : : "r" (handle));
    __asm__ volatile("mv a0, %0" : : "r" (0x22));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_wait (sem_t id){

    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (0x23));

    __asm__ volatile ("ecall");

    int ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}

int sem_signal (sem_t id){
    __asm__ volatile("mv a1, %0" : : "r" (id));
    __asm__ volatile("mv a0, %0" : : "r" (0x24));

    __asm__ volatile ("ecall");

    return 0;
}


char getc (){

    __asm__ volatile("mv a0, %0" : : "r" (0x41));

    __asm__ volatile ("ecall");

    char ret;
    __asm__ volatile("mv %0, a0" : "=r" (ret));
    return ret;
}



void putc (char character){

    __asm__ volatile("mv a1, %0" : : "r" (character));
    __asm__ volatile("mv a0, %0" : : "r" (0x42));

    __asm__ volatile ("ecall");

    return;
}

void switch_to_Umode(){

    __asm__ volatile("mv a0, %0" : : "r" (0x90));
    __asm__ volatile ("ecall");

    return;

}

void switch_to_Smode(){

    asm volatile("mv a0, %0" : : "r" (0x91));
    __asm__ volatile ("ecall");

    return;
}
