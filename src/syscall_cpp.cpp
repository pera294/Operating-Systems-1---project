//
// Created by os on 8/13/22.
//
//
// Created by os on 8/3/22.
//



#include "../h/syscall_cpp.hpp"
void* operator new (size_t n){
    return  mem_alloc(n);
}
void operator delete (void* p){
    mem_free(p);
}


void Thread::runWrapper(void* thrd){
    Thread* t =(Thread*)thrd;
    t->run();

}

Thread::Thread(void (*body)(void *), void *arg) {
    myHandle=nullptr;

}
Thread::Thread() {
    myHandle=nullptr;
}

int Thread::start() {
    if(myHandle==nullptr){
        return thread_create(&myHandle,runWrapper,this);
    }

    else return -1;
}

void Thread::dispatch() {
    thread_dispatch();

}

Thread::~Thread() {
    myHandle->setFinished(true);
    myHandle=nullptr;
}

Semaphore::Semaphore(unsigned int init) {
    sem_open(&myHandle,init);
}

int Semaphore::signal() {
    return sem_signal(myHandle);

}

int Semaphore::wait() {
    return sem_wait(myHandle);

}

Semaphore::~Semaphore() {
    sem_close(myHandle);
    myHandle=nullptr;

}

 char Console:: getc (){
    return ::getc();
}

 void Console:: putc (char character){
    ::putc(character);
}