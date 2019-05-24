iosched: Scheduler.cpp main.cpp 
	g++-8.1 -g -o iosched Scheduler.cpp main.cpp 
#module load gcc-8.1
clean:
	rm -f iosched *~
