//
//  Scheduler.cpp
//  lab4-working
//
//  Created by Zhenyu Liu on 5/8/19.
//  Copyright © 2019 Zhenyu Liu. All rights reserved.
//


#include <stdio.h>
#include <iostream>
#include <fstream>
#include <climits>
#include "Scheduler.h"


using namespace std;

IO_op* FIFO::get_nextIO(int currentHead){
    if(IOqueue.size() == 0) return nullptr;
    IO_op* op = IOqueue[0];
    IOqueue.erase(IOqueue.begin());
    
    return op;
}

IO_op* SSTF::get_nextIO(int currentHead){
    if(IOqueue.size() == 0) return nullptr;
    IO_op* op = nullptr;
    int curDistance = INT_MAX;
    int curIdx = 0;
    for(int i = 0; i < IOqueue.size(); i++ ){
        if(abs(IOqueue[i]->track - currentHead )< curDistance){
            curDistance = abs(IOqueue[i]->track - currentHead);
            curIdx = i;
        }
    }
    op = IOqueue[curIdx];
    IOqueue.erase(IOqueue.begin()+curIdx);
    return op;
}

IO_op* LOOK::get_nextIO(int currentHead){
    if(IOqueue.size() == 0) return nullptr;
    IO_op* op = nullptr;
    int curDistance = INT_MAX;
    int curIdx = 0;
    bool changed = false;
    for(int i = 0; i < IOqueue.size(); i++ ){
        if(!directionBackwards && IOqueue[i]->track - currentHead >= 0 && IOqueue[i]->track - currentHead < curDistance){
            curDistance = IOqueue[i]->track - currentHead;
            curIdx = i;
            changed = true;
        }
        if(directionBackwards && IOqueue[i]->track - currentHead <= 0 && abs(IOqueue[i]->track - currentHead) < curDistance){
            curDistance = abs(IOqueue[i]->track - currentHead);
            curIdx = i;
            changed = true;
        }
    }
    if (changed == false){
        directionBackwards = !directionBackwards;
        
        for(int i = 0; i < IOqueue.size(); i++ ){
            if(!directionBackwards && IOqueue[i]->track - currentHead >= 0 && IOqueue[i]->track - currentHead < curDistance){
                curDistance = IOqueue[i]->track - currentHead;
                curIdx = i;
            }
            if(directionBackwards && IOqueue[i]->track - currentHead <= 0 && abs(IOqueue[i]->track - currentHead) < curDistance){
                curDistance = abs(IOqueue[i]->track - currentHead);
                curIdx = i;
            }
        }
    }
    op = IOqueue[curIdx];
    IOqueue.erase(IOqueue.begin()+curIdx);
    return op;
}


IO_op* CLOOK::get_nextIO(int currentHead){
    if(IOqueue.size() == 0) return nullptr;
    IO_op* op = nullptr;
    int curDistance = INT_MAX;
    int curIdx = 0;
    bool changed = false;
    for(int i = 0; i < IOqueue.size(); i++ ){
        if( IOqueue[i]->track - currentHead >= 0 && IOqueue[i]->track - currentHead < curDistance){
            curDistance = IOqueue[i]->track - currentHead;
            curIdx = i;
            changed = true;
        }
    }
    if (changed == false){
        for(int i = 0; i < IOqueue.size(); i++ ){
            if( IOqueue[i]->track - currentHead <= 0 && abs(IOqueue[i]->track - 0) < curDistance){
                curDistance = abs(IOqueue[i]->track - 0);
                curIdx = i;
            }
        }
    }
    
    op = IOqueue[curIdx];
    IOqueue.erase(IOqueue.begin()+curIdx);
    return op;
}


IO_op* FLOOK::get_nextIO(int currentHead){
    if(IOqueue.size() == 0) return nullptr;
    if( activeQueue.size() == 0 && addOnQueue.size() == 0 ) return nullptr;
    if(activeQueue.size() == 0){
        vector<IO_op*> temp = activeQueue;
        activeQueue = addOnQueue;
        addOnQueue = temp;
    }
    
    IO_op* op = nullptr;
    int curDistance = INT_MAX;
    int curIdx = 0;
    bool changed = false;
    for(int i = 0; i < activeQueue.size(); i++ ){
        if(!directionBackwards && activeQueue[i]->track - currentHead >= 0 && activeQueue[i]->track - currentHead < curDistance){
            curDistance = activeQueue[i]->track - currentHead;
            curIdx = i;
            changed = true;
        }
        if(directionBackwards && activeQueue[i]->track - currentHead <= 0 && abs(activeQueue[i]->track - currentHead) < curDistance){
            curDistance = abs(activeQueue[i]->track - currentHead);
            curIdx = i;
            changed = true;
        }
    }
    if (changed == false){
        directionBackwards = !directionBackwards;
        
        for(int i = 0; i < activeQueue.size(); i++ ){
            if(!directionBackwards && activeQueue[i]->track - currentHead >= 0 && activeQueue[i]->track - currentHead < curDistance){
                curDistance = activeQueue[i]->track - currentHead;
                curIdx = i;
            }
            if(directionBackwards && activeQueue[i]->track - currentHead <= 0 && abs(activeQueue[i]->track - currentHead) < curDistance){
                curDistance = abs(activeQueue[i]->track - currentHead);
                curIdx = i;
            }
        }
    }
    op = activeQueue[curIdx];
    activeQueue.erase(activeQueue.begin()+curIdx);
    IOqueue.erase(IOqueue.begin()); // erase one here, does not matter, only use ioqueue.size
    return op;
}
