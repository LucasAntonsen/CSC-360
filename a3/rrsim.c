//By Lucas J. Antonsen, V00923982

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "linkedlist.h"

#define MAX_BUFFER_LEN 80

taskval_t *event_list = NULL;

void print_task(taskval_t *t, void *arg) {
    printf("task %03d: %5d %3.2f %3.2f\n",
        t->id,
        t->arrival_time,
        t->cpu_request,
        t->cpu_used
    );  
}

void increment_count(taskval_t *t, void *arg) {
    int *ip;
    ip = (int *)arg;
    (*ip)++;
}

//simulates round robin scheduling
//qlen: quantum length dlen: dispatch length
void run_simulation(int qlen, int dlen) {
    taskval_t *ready_q = NULL;
    taskval_t *temp;        //for moving tasks from event_list to ready_q
    taskval_t *temp2;       //for manipulating tasks in ready_q
    int tick = 0;
    int dispatch = 0;       //dispatch ticks used
    int quantum_used = 0;

    temp = event_list;
    while((event_list != NULL) || (ready_q != NULL)){

        //add newly arrived tasks to ready_q
        while(temp != NULL && temp->arrival_time == tick){
           event_list = remove_front(event_list);
           ready_q = add_end(ready_q, temp);
           temp = event_list;
        }

        //check if no tasks currently in ready_q
        if(ready_q == NULL){
            printf("[%05d] IDLE\n", tick);
        }else{

            //check if head task on ready_q has finished
            if(ready_q->cpu_used >= ready_q->cpu_request){
                ready_q->cpu_used = ready_q->cpu_request;
                ready_q->finish_time = tick;
                printf("[%05d] id=%05d EXIT w=%.2f ta=%d.00\n", tick, ready_q->id, (ready_q->finish_time - ready_q->cpu_used - ready_q->arrival_time),
                        (ready_q->finish_time - ready_q->arrival_time));
                temp2 = ready_q;
                ready_q = remove_front(ready_q);
                free(temp2);
                dispatch = 0;   //reset dispatch to begin dispatching new task
                quantum_used = 0;
            }

            //check if ready_q is empty and event_list is not after possible removal
            if((ready_q == NULL) && (event_list != NULL)){
            	printf("[%05d] IDLE\n", tick);

            //check if task is available and if it needs dispatching
        	}else if(dispatch != dlen && ready_q != NULL){

                printf("[%05d] DISPATCHING\n", tick);
                dispatch++;

            //otherwise the task should run
            }else if(ready_q != NULL){

                printf("[%05d] id=%05d req=%.2f used=%.2f\n", tick, ready_q->id, ready_q->cpu_request, ready_q->cpu_used);
                ready_q->cpu_used++;
                quantum_used++;

                //switch task out if quantum is used up and task is still not finished.
                //ensures that completed tasks are freed rather than put back on the ready_q
                if((quantum_used == qlen) && (ready_q->cpu_used < ready_q->cpu_request)){
                    temp2 = ready_q;
                    ready_q = remove_front(ready_q);
                    ready_q = add_end(ready_q, temp2);
                    dispatch = 0;
                    quantum_used = 0;
                }
            }
        }
        tick++;
   }
}


int main(int argc, char *argv[]) {
    char   input_line[MAX_BUFFER_LEN];
    int    i;
    int    task_num;
    int    task_arrival;
    float  task_cpu;
    int    quantum_length = -1;
    int    dispatch_length = -1;

    taskval_t *temp_task;

    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "--quantum") == 0 && i+1 < argc) {
            quantum_length = atoi(argv[i+1]);
        }
        else if (strcmp(argv[i], "--dispatch") == 0 && i+1 < argc) {
            dispatch_length = atoi(argv[i+1]);
        }
    }

    if (quantum_length == -1 || dispatch_length == -1) {
        fprintf(stderr, 
            "usage: %s --quantum <num> --dispatch <num>\n",
            argv[0]);
        exit(1);
    }


    while(fgets(input_line, MAX_BUFFER_LEN, stdin)) {
        sscanf(input_line, "%d %d %f", &task_num, &task_arrival,
            &task_cpu);
        temp_task = new_task();
        temp_task->id = task_num;
        temp_task->arrival_time = task_arrival;
        temp_task->cpu_request = task_cpu;
        temp_task->cpu_used = 0.0;
        event_list = add_end(event_list, temp_task);
    }

//#ifdef DEBUG
//    int num_events;
//    apply(event_list, increment_count, &num_events);
//    printf("DEBUG: # of events read into list -- %d\n", num_events);
//    printf("DEBUG: value of quantum length -- %d\n", quantum_length);
//    printf("DEBUG: value of dispatch length -- %d\n", dispatch_length);
//#endif

    run_simulation(quantum_length, dispatch_length);

    return (0);
}
