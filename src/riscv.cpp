//
// Created by marko on 20.4.22..
//

#include "../h/riscv.hpp"
#include "../h/tcb.hpp"
#include "../h/scb.hpp"
#include "../h/MemoryAllocator.hpp"

void Riscv::popSppSpie()
{
    __asm__ volatile("csrw sepc, ra");
    mc_sstatus(SSTATUS_SPP);
    __asm__ volatile("sret");
}

void Riscv::handleSupervisorTrap()
{

    uint64 scause = r_scause();

    if (scause == 0x0000000000000009UL){
        // interrupt: no; cause code: environment call from S-mode(9)
        uint64 op;
        __asm__ volatile("mv %0, a0" : "=r" (op));
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch(op){

            case 0x01: // void* mem_alloc (size_t size);

                size_t numBlocks; // u Blokovima
                __asm__ volatile("mv %0, a1" : "=r" (numBlocks));
                __mem_alloc(numBlocks);
                //MemoryAllocator::mem_alloc(numBlocks);
                break;

            case 0x02: //int mem_free (void*);

                void *pointer;
                __asm__ volatile("mv %0, a1" : "=r" (pointer));
                __mem_free(pointer);
                //MemoryAllocator::mem_free(pointer);
                break;

            case 0x90: //void switch_to_Umode();

                Riscv::mc_sstatus(SSTATUS_SPP);
                w_sepc(sepc);
                return;

        }//switch end

        w_sstatus(sstatus);
        w_sepc(sepc);
    }
    else if (scause == 0x0000000000000008UL)
    {// interrupt: no; cause code: environment call from U-mode(8)

        uint64 op;
        __asm__ volatile("mv %0, a0" : "=r" (op));
        uint64 volatile sepc = r_sepc() + 4;
        uint64 volatile sstatus = r_sstatus();

        switch(op) {

            case 0x01: // void* mem_alloc (size_t size);

                size_t numBlocks; // u Blokovima
                __asm__ volatile("mv %0, a1" : "=r" (numBlocks));
                __mem_alloc(numBlocks);
                //MemoryAllocator::mem_alloc(numBlocks);
                break;

            case 0x02: //int mem_free (void*);

                void *pointer;
                __asm__ volatile("mv %0, a1" : "=r" (pointer));
                __mem_free(pointer);
               //MemoryAllocator::mem_free(pointer);
                break;

            case 0x11: //int thread_create ( thread_t* handle, void(*start_routine)(void*), void* arg ,void* stack);

                TCB **handle;
                __asm__ volatile("mv %0, a1"  : "=r" (handle));

                void (*routine)(void *);
                __asm__ volatile("mv %0, a2"  : "=r" (routine));

                void *arg;
                __asm__ volatile("mv %0, a6"  : "=r" (arg));

                uint64 *stack;
                __asm__ volatile("mv %0, a7"  : "=r" (stack));

                *handle = TCB::createThread(routine, arg, stack);
                break;

            case 0x12: // int thread_exit ();

                TCB::running->setFinished(true);
                TCB::dispatch();
                break;

            case 0x13: // void thread_dispatch ();

                TCB::dispatch();
                break;

            case 0x21: //int sem_open (sem_t* handle,unsigned init);

                SCB **semhandle;
                __asm__ volatile("mv %0, a1"  : "=r" (semhandle));

                uint64 value;
                __asm__ volatile("mv %0, a2"  : "=r" (value));

                *semhandle = SCB::createSemaphore(value);
                break;

            case 0x22: //int sem_close (sem_t handle);

                SCB *sem;
                __asm__ volatile("mv %0, a1"  : "=r" (sem));
                SCB::close(sem);
                break;

            case 0x23: //int sem_wait (sem_t id);

                __asm__ volatile("mv %0, a1"  : "=r" (sem));
                SCB::wait(sem);
                break;

            case 0x24: //int sem_signal (sem_t id);

                __asm__ volatile("mv %0, a1"  : "=r" (sem));
                SCB::signal(sem);
                break;

            case 0x31: //int time_sleep (time_t);
                break;

            case 0x41: //char getc ();

                Riscv::w_stvec((uint64) &Riscv::consoleTrap);
                __getc();
                Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
                break;

            case 0x42: //void putc (char character);

                char character;
                __asm__ volatile("mv %0, a1"  : "=r" (character));
                Riscv::w_stvec((uint64) &Riscv::consoleTrap);
                __putc(character);
                Riscv::w_stvec((uint64) &Riscv::supervisorTrap);
                break;

            case 0x90: //void switch_to_Umode();

                Riscv::mc_sstatus(SSTATUS_SPP);
                w_sepc(sepc);
                return;

            case 0x91: //void switch_to_Smode();

                Riscv::ms_sstatus(SSTATUS_SPP);
                w_sepc(sepc);
               return;

        } //switch end

        w_sstatus(sstatus);
        w_sepc(sepc);

    }

    else if (scause == 0x8000000000000001UL) {
        // interrupt: yes; cause code: supervisor software interrupt (CLINT; machine timer interrupt)
        mc_sip(SIP_SSIP);
    }
    else if (scause == 0x8000000000000009UL) {
        // interrupt: yes; cause code: supervisor external interrupt (PLIC; could be keyboard)
        console_handler();
       // mc_sip(SIP_SEIP);
    }
    else {
        // unexpected trap cause
    }

}


void Riscv::handleConsole() {

    uint64 scause = r_scause();
    if(scause == 0x8000000000000001UL) { // timer

        mc_sip(SIP_SSIP);

    }


    if(scause == 0x8000000000000009UL) { //console


        console_handler();
        mc_sip(SIP_SEIP);


    }

}