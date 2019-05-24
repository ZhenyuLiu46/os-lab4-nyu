//
//  main.cpp
//  lab4-working
//
//  Created by Zhenyu Liu on 5/8/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <getopt.h>

#include "Scheduler.h"

using namespace std;



int main(int argc, char * argv[]) {
    //Scheduler* sched = new FIFO(); // work
    //Scheduler* sched = new SSTF(); // work
    //Scheduler* sched = new LOOK(); // work
    //Scheduler* sched = new CLOOK(); // work
    //Scheduler* sched = new FLOOK(); // work
    int c = 0;
    Scheduler* sched;
    while (( c = getopt(argc, argv, "s:")) != -1) {
        switch (c) {
            case 's':
                string schedType = optarg;
                
                switch (schedType[0]) {
                    case 'i':
                        sched = new FIFO();
                        break;
                        
                    case 'j':
                        sched = new SSTF();
                        break;
                        
                    case 's':
                        sched = new LOOK();
                        break;
                        
                    case 'c':
                        sched = new CLOOK();
                        break;
                        
                    case 'f':
                        sched = new FLOOK();
                        break;
                }
                
                break;
        }
    }
    
    int Idx = optind;
    string fileName = argv[Idx];
    
    ifstream inputFile;
    //inputFile.open("/Users/zhenyuliu/Desktop/Lab4_stuff/TestData/input9");
    inputFile.open(fileName);
    if(inputFile.is_open()){
        string line;
        int timeStep = 0;
        int track = 0;
        
        vector<IO_op*> opVec;
        
        int operationidx = 0;
        while (getline(inputFile, line))
        {
            istringstream ss(line);
            char firstLetter;
            ss >> firstLetter;
            if(firstLetter == '#'){
                continue;
            }
            ss.str(line);
            ss.clear();
            ss >> timeStep;
            ss >> track;
            
            IO_op *operation = new IO_op();
            operation->arrivalTime = timeStep;
            operation->track = track;
            operation->id = operationidx;
            operationidx++;
            opVec.push_back(operation);
            
        }
        //simulation
        int currentTime = 0;
        int totalTime = 0;
        int totalMove = 0;
        double avg_turnaround = 0;
        double  avg_waittime = 0;
        int max_waittime = 0;
        int currentOpidx = 0;
        
        bool currentActive = false;
        int currentHead = 0; // compare with track
        IO_op* currOperation;
        while(!sched->IOqueue.empty() || currentOpidx < opVec.size() || currentActive){
            
            //(1)add_to_IOqueue
            if(currentOpidx < opVec.size() && currentTime == opVec[currentOpidx]->arrivalTime){
                sched->IOqueue.push_back(opVec[currentOpidx]);
                if(sched->scheName == "FLOOK"){
                    
                    sched->addOnQueue.push_back(opVec[currentOpidx]);
                }
                currentOpidx++;
            }
            
            //current active
            if(currentActive == true){

                //(2)
                if(currentHead == currOperation->track){
                    currOperation->endTime = currentTime;
                    currentActive = false;
                }
                //(3)
                else if(currOperation->track > currentHead){
                    currentHead++;
                    totalMove++;
                } else if(currOperation->track < currentHead){
                    currentHead--;
                    totalMove++;
                }
            }
            
            //(4) fetch one from IOqueue
            if(currentActive == false && !sched->IOqueue.empty()){
                currOperation = sched->get_nextIO(currentHead);
                currOperation->startTime = currentTime;
                currentActive = true;
                continue;
            }
            
            //(5)
            currentTime++;
        } // end while loop here
        
        //to Print
        for(IO_op* operation : opVec){
            printf("%5d: %5d %5d %5d\n",operation->id, operation->arrivalTime, operation->startTime, operation->endTime);
            avg_turnaround += (operation->endTime - operation->arrivalTime);
            avg_waittime += (operation->startTime - operation->arrivalTime);
            max_waittime = max( max_waittime, operation->startTime - operation->arrivalTime );
        }
        avg_turnaround = avg_turnaround / currentOpidx;
        avg_waittime = avg_waittime / currentOpidx;
        totalTime = currentTime - 1;
        printf("SUM: %d %d %.2lf %.2lf %d\n",
               totalTime, totalMove, avg_turnaround, avg_waittime, max_waittime);
        
    } else {
        cout << "file not exsit";
    }
    return 0;
}
