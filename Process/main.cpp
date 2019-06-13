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
    processes.processes[0].serviceTime = 6;

    processes.processes[1].ID = "B";
    processes.processes[1].arrivalTime = 1;
    processes.processes[1].serviceTime = 2;

    processes.processes[2].ID = "C";
    processes.processes[2].arrivalTime = 2;
    processes.processes[2].serviceTime = 5;

    processes.processes[3].ID = "D";
    processes.processes[3].arrivalTime = 3;
    processes.processes[3].serviceTime = 9;

    processes.processes[4].ID = "E";
    processes.processes[4].arrivalTime = 4;
    processes.processes[4].serviceTime = 8;
    
    //取消下方注释以从键盘输入数据, 因类对象名称相同不能与上方初始化共存
    /*
    int numberOfProcesses;
    cout << "进程数: ";
    cin >> numberOfProcesses;
    Processes processes(numberOfProcesses);
    for (int i = 0; i < numberOfProcesses; i++) {
        cout << "按顺序输入进程ID, 到达时间和服务时间,格式'A 0 6': ";
        cin >> processes.processes[i].ID >> processes.processes[i].arrivalTime >> processes.processes[i].serviceTime;
    }
    */
    
    cout << "FCFS: " << endl;
    // 算法函数中包含输出函数, 下同
    processes.FCFS();

    cout << "SJF: " << endl;
    processes.SJF();

    cout << "HRRN: " << endl;
    processes.HRRN();

    cout << "RR: " << endl;
    int q;
    cout << "输入时间片长度(整数): ";
    cin >> q;
    cout << "q = " << q << ": " << endl;
    processes.RR(q);
    cout << endl;
    
    return 0;
}
