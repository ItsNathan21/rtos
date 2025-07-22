.cpu cortex-m4
.syntax unified
.thumb 

.section .text 

#define nthread_create_svc (1)

.global nthread_create
nthread_create:
    svc nthread_create_svc
    bx lr 

