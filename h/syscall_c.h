//
// Created by os on 8/2/22.
//

#ifndef PROJECT_BASE_V1_1_SYSCALL_C_H
#define PROJECT_BASE_V1_1_SYSCALL_C_H

#include "../lib/hw.h"
#include"../lib/console.h"
#include "../h/tcb.hpp"



class TCB;
typedef TCB* thread_t;

void* mem_alloc (size_t size);

int mem_free (void*);

int thread_create (thread_t* handle, void(*start_routine)(void*), void *arg);

int thread_exit ();

void thread_dispatch ();

class SCB;
typedef SCB* sem_t;

int sem_open (sem_t* handle, unsigned init);

int sem_close (sem_t handle);

int sem_wait (sem_t id);

int sem_signal (sem_t id);
/*
typedef unsigned long time_t;

int time_sleep (time_t);
 */
const int EOF = -1;

char getc ();

void putc (char);

void switch_to_Umode();

void switch_to_Smode();

#endif //PROJECT_BASE_V1_1_SYSCALL_C_H
