# A3
### By Lucas J. Antonsen

#### Task 1 - rrsim.c

rrsim.c simulates round robin scheduling through a while loop where each pass through the loop represents a single tick in the simulation and ends when both the event list and ready 
queue are empty. At the start of the loop, the event list is assessed for any tasks slated for arrival and these tasks are added to the ready queue. The ready queue is then used to 
determine the course of action of the simulation. If the ready queue is empty, the CPU idles. Otherwise, the head task of the ready queue is checked for completion and is removed from 
the ready queue if so. If the ready queue is now empty and the event list still has elements then the CPU idles. Otherwise, if the head task is not complete, it is either dispatched 
or run depending on the dispatch progress of the task. If the task runs, it is moved from the front of the ready queue to the end once its quantum expires as long as the task is not 
complete. This condition ensures that tasks that have completed are freed in the following tick and a new task is dispatched. 


#### Task 2 - Analyzing output from the simulator
graph_gen.sh and graph.py provide statistics and graphing utilities for the rrsim.c program.

graph_gen.sh provides average turnaround and waiting times for rrsim.c using input from 20 different seeds in simgen.c. Each quantum and dispatch combination is run using the 20 
different seeds and then averaged. The averages are saved to an array resulting in 24 elements. Every consecutive group of 6 elements corresponds to the average output from quantums 
50, 100, 250, 500 with 0, 5, 10, 15, 20, 25 dispatch times, ie. the first 6 elements correspond to quantum:50 dispatch:0, 5, 10, 15, 20, 25. The average list is then passed to graph.py 
for modeling.

graph.py uses the average list provided by graph_gen.py to model the rrsim.c statistics.