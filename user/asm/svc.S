.cpu cortex-m4
.syntax unified
.thumb 

.section .text 

#define nthread_create_svc (1)
#define initialize_threads_svc (2) 
#define nthread_kill_svc (3) 
#define nthread_run_svc (4)
#define start_svc (5)

.global nthread_create
nthread_create:
    svc nthread_create_svc
    bx lr 

.global initialize_threads
initialize_threads:
    svc initialize_threads_svc
    bx lr 

.global nthread_kill
nthread_kill:
    svc nthread_kill_svc
    bx lr 

.global nthread_run
nthread_run:
    svc nthread_run_svc
    bx lr 

.global start 
start:
    svc start_svc
    bx lr


