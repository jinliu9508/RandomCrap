#include "myscheduler.h"
#include <iostream>
#include <stdlib.h>     /* srand, rand */

int main(int argc, char* argv[])
{

	Policy policy = STRFwoP;			// replace policy: FCFS, STRFwoP, STRFwP, PBS
	unsigned int CPU_num = 1;		// number of CPU
	int thread_num = 10;	// number of thread to be scheduled

	MyScheduler scheduler(policy, CPU_num);

	int randomRemainingTime = 0;
	int randomPriority = 0;
	// create thread
	for (int i = 0; i < thread_num; i++) {
		randomPriority = rand() % thread_num;		// random int from 0 to thread_num -1
		randomRemainingTime = rand() % thread_num + 1;	// remaining time from 1 to thread_num

		scheduler.CreateThread(i, randomRemainingTime, randomPriority, i);
		cout << "\t*** Thread #" << i << " has been created. \n" << "\tArrival: " << i << "\tRemaining: " << randomRemainingTime << "\tPriority: " << randomPriority << endl << endl;
	}

	// print the scheduler policy type
	switch (policy)
	{
	case FCFS:
		std::cout << "FCFS ";
		break;
	case STRFwoP:
		std::cout << "STRFwoP ";
		break;
	case STRFwP:
		std::cout << "STRFwP ";
		break;
	case PBS:
		std::cout << "PBS " << std::endl;
		break;
	default:
		cout << "Invalid policy!";
		throw 0;
		break;
	}
	std::cout << "running result: " << std::endl;
	scheduler.Go();

	return 0;
}
