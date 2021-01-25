// Student id: hmukhta
// Student number: 251030469
#include <stdlib.h>
#include <stdio.h> 
#include "scheduling.h"

// helper function to see if the processes have been completed or not 
// returns 0 if the burst array is empty, 1 if not
int empty(int *queue, int count){
	for (int x=0; x<count;x++){
		if (queue[x] > 0)
			return 1;
	}
	return 0;   
}
/*
##  Round-Robin Scheduling Algorithm  ##

	Parameters:-

	queue: 1D int array containing the CPU burst time for every process in a ready queue
	np: number of processes (length of `queue` array)
	tq: time quantum for the RR algorithm


	Returns:-

	rr_result struct pointer, where
	rr_result -> order: 1D int array, containing the execution order of processes (i.e. `(rr_result -> order)[0]` is the process number of the first process)
		** hint: p1 is always the first item in order of selection in round-robin and FCFS
    rr_result -> order_n: int, representing the size of the `order` array, described above ^
	rr_result -> turnarounds: 1D int array, containing the turnaround time for each process (i.e. `(rr_result -> turnarounds)[0]` is the turnaround time for p1)
*/
rr_result *rr(int *queue, int np, int tq)
{
	rr_result *result = malloc(sizeof(rr_result));
	result->np = np;
	result->turnarounds = malloc(sizeof(int) * np);
	int total_times = 0;


	// first, figure out how many times each process needs to be visited before it will complete
	for (int x=0;x<np-1;x++){
		if ((queue[x] % tq) > 0)
			total_times += (queue[x] / tq) + 1;
		else 
			total_times += (queue[x] / tq);
	}

	// second, allocate the memory for the order of processes
	result->order = malloc(sizeof(int) * total_times);
	result->order_n = total_times;
    
	// third, figure out the order of processes
	// initialize a variable to be able to store the process number 
	// in the order array
	int loop_processes = 0;
	int ind = 0;
	// until all the processes are complete (using helper method)

	int *rem_queue = malloc(sizeof(int) * np);

	for(int i = 0; i < np; i++){
		rem_queue[i] = queue[i];
	}

	while (empty(queue, np) != 0){
		// go through all of the processes
		for (int i=0;i<np;i++){
			// if the process isn't complete
			if (rem_queue[i] > 0){
				// spend time on the process
				rem_queue[i] -= tq;
				// CREATING THE ORDER
				// add the process to the list
				(result->order)[ind] = i;
				//printf("Process index: %d, Process #: %d\n", ind, i+1);
				// add to the index
				ind++;
			}
		}
		loop_processes++;
	}
	
	// fourth, lets calculate the elapsed time
	// go through all of the processes
	for (int x=0;x<np;x++){
		int times_arrived = 0;
		int time_elapsed = 0;
		int done = 0;
		int start = 0;
		// for every process, go through the order of processes
		for (int y=0;y<total_times;y++){
			//printf("Current process is : %d\n", result->order[y] );
			// if the process is arrived at
			if (result->order[y] == x){
				// add to count
				times_arrived++;
				start = 1;
			}
			// if the process has started
			if (start == 1 && queue[result->order[y]] != 0){
				// see if other processes are less than the time quantum
				if (queue[result->order[y]] < tq){
					time_elapsed += tq + queue[result->order[y]];
				}
				else
					time_elapsed += tq - queue[result->order[y]];
			}
			//printf("Process #%d Time: %d\n", x, time_elapsed);
		}
		int time;
		// calculate the total time 
		if (times_arrived == 1){
		 	time = tq + queue[x-1];
		 	//printf("Time: %d\n", time);
		}
		else {
			time = time_elapsed;
			//printf("Process #%d Time: %d\n", x, time);
		}
		// add the time to the turnaround time
		(result->turnarounds)[x-1] = time;
	}

	return result;
}
