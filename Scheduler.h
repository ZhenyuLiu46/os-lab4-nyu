//
//  Scheduler.h
//  lab4-working
//
//  Created by Zhenyu Liu on 5/8/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//

#ifndef Scheduler_h
#define Scheduler_h


#include <iostream>
#include <vector>
#include <list>

using namespace std;
struct IO_op{
    int id;
    int arrivalTime;
    int startTime;
    int endTime;
    int track;
    IO_op(){
        id = 0;
        arrivalTime = 0;
        startTime = 0;
        endTime = 0;
        track = 0;
    }
};


class Scheduler{
public:
    vector<IO_op*> IOqueue;
    string scheName;
    vector<IO_op*> activeQueue;
    vector<IO_op*> addOnQueue;
    virtual IO_op* get_nextIO(int currentHead) = 0;
    Scheduler(){};
};

class FIFO : public Scheduler {
public:
    FIFO() : Scheduler(){};
    IO_op* get_nextIO(int currentHead);
};


class SSTF : public Scheduler {
public:
    SSTF() : Scheduler(){};
    IO_op* get_nextIO(int currentHead);
};


class LOOK : public Scheduler {
public:
    bool directionBackwards = false; //false moving forward
    LOOK() : Scheduler(){};
    IO_op* get_nextIO(int currentHead);
};

class CLOOK : public Scheduler {
public:
    
    CLOOK() : Scheduler(){};
    IO_op* get_nextIO(int currentHead);
};

class FLOOK : public Scheduler {
public:
    bool directionBackwards = false; //false moving forward
    FLOOK() : Scheduler(){scheName = "FLOOK";};
    IO_op* get_nextIO(int currentHead);
};




#endif /* Scheduler_h */
