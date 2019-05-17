//
//  main.cpp
//  Process
//
//  Created by LTXX on 2019/5/14.
//  Copyright © 2019 LTXX. All rights reserved.
//

#include <iostream>
#include "Process.h"
#include "Processes.h"

int main() {
    // 声明名为 processes 的 进程数组类型的对象
    Processes processes(5);
    
    // 初始化 processes 对象的 processes 数组
    processes.processes[0].ID = "A";
    processes.processes[0].arrivalTime = 0;
    processes.processes[0].serviceTime = 5;
    
    processes.processes[1].ID = "B";
    processes.processes[1].arrivalTime = 1;
    processes.processes[1].serviceTime = 7;
    
    processes.processes[2].ID = "C";
    processes.processes[2].arrivalTime = 3;
    processes.processes[2].serviceTime = 3;
    
    processes.processes[3].ID = "D";
    processes.processes[3].arrivalTime = 4;
    processes.processes[3].serviceTime = 8;
    
    processes.processes[4].ID = "E";
    processes.processes[4].arrivalTime = 6;
    processes.processes[4].serviceTime = 2;
    
    cout << "FCFS: " << endl;
    // FCFS()函数中包含输出函数, 下同
    processes.FCFS();
    cout << "SJF: " << endl;
    processes.SJF();
    
    return 0;
}
