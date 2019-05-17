//
//  Process.h
//  Process
//
//  Created by LTXX on 2019/5/14.
//  Copyright © 2019 LTXX. All rights reserved.
//

#ifndef Process_h
#define Process_h

#include <iostream>
#include <string>
#include <iomanip>
using namespace std;

class Process {
public:
    string ID;//进程ID
    int arrivalTime;//到达时间
    int serviceTime;//服务时间
    int finishTime;//完成时间
    int wholeTime;//周转时间
    double weightWholeTime;//带权周转时间
    
    bool executed = false;//判断是否已执行
    
    int WholeTime() { return wholeTime = finishTime - arrivalTime; }//计算周转时间
    double WeightWholeTime() { return weightWholeTime = double(wholeTime) / double(serviceTime); }//计算带权周转时间
    
    Process(): ID(""), arrivalTime(0), serviceTime(0), executed(false) {}
    
    Process(string i, int at, int st) {
        ID = i;
        arrivalTime = at;
        serviceTime = st;
        executed = false;
    }
    
    void PrintToConsole() {
        cout << "进程ID: " << ID << endl;
        cout << "到达时间: " << arrivalTime << endl;
        cout << "服务时间: " << serviceTime << endl;
        cout << "完成时间: " << finishTime << endl;
        cout << "周转时间: " << wholeTime << endl;
        cout << "带权周转时间: " << setiosflags(ios::fixed) << setprecision(2) << weightWholeTime << endl;
    }
};


#endif /* Process_h */
