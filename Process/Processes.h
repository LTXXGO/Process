//
//  Processes.h
//  Process
//
//  Created by LTXX on 2019/5/14.
//  Copyright © 2019 LTXX. All rights reserved.
//

#ifndef Processes_h
#define Processes_h

#include <iostream>
#include <string>
#include <iomanip>

#include "Process.h"

class Processes {
public:
    // 进程数量
    int size;
    // 进程数组
    Process * processes;
    // 平均周转时间
    double averageWholeTime;
    // 平均带权周转时间
    double averageWeightWholeTime;
    
    // 构造函数
    Processes() {
        size = 0;
        processes = new Process[size];
    }
    Processes(int n) {
        size = n;
        processes = new Process[size];
    }
    
    // 先来先服务 / FCFS 算法
    void FCFS();
    // 短作业优先 / SJF 算法
    void SJF();
    
    // 计算平均时间
    void calculateAverage();
    
    // 测试输出函数
        // 输出平均时间
    void averageOutput();
        // 输出当前时刻计算机状态
    void ExecutionStatus();
};

// MARK: - 先来先服务 / FCFS 算法
void Processes::FCFS() {
    for (int i = 0; i < size; i = i + 1) {
        //计算完成时间
        if(i == 0) {
            processes[0].finishTime = processes[0].arrivalTime + processes[0].serviceTime;
        } else {
            processes[i].finishTime = processes[i - 1].finishTime + processes[i].serviceTime;
        }
        //计算周转时间和带权周转时间
        processes[i].WholeTime();
        processes[i].WeightWholeTime();
    }
    //计算平均值
    calculateAverage();
    
    //测试输出
    for (int i = 0; i < size; i = i + 1) {
        processes[i].PrintToConsole();
        cout << endl;
    }
    averageOutput();
    //输出状态
    ExecutionStatus();
}

// MARK: - 短作业优先 / SJF 算法
void Processes::SJF() {
    int numberOfExecutedProcess = 0;//已执行过的进程数量，用于判断是否全部执行完毕
    int now = 0;//当前时刻
    Process executableProcess[size];//存放当前可执行的进程的数组
    int numberOfExecutableProcesses = 0;//当前的可执行进程数
    Process minServiceTimeProcess;//用于暂时存储最短服务时间进程的变量
    //执行第一个进程
    processes[0].finishTime = processes[0].arrivalTime + processes[0].serviceTime;
    processes[0].WholeTime();
    processes[0].WeightWholeTime();
    processes[0].executed = true;
    numberOfExecutedProcess += 1;
    //更新当前时间
    now = processes[0].finishTime;
    //判断是否传入函数的所有进程都已执行过
    while (numberOfExecutedProcess != size) {
        //更新可执行程序数组为空数组
        numberOfExecutableProcesses = 0;
        //判断其他进程是否已到达
        for (int i = 0; i < size; i = i + 1) {
            if(processes[i].arrivalTime <= now && !processes[i].executed) {
                executableProcess[numberOfExecutableProcesses] = processes[i];
                numberOfExecutableProcesses += 1;
            }
        }
        //在已到达的可执行进程中找出服务时间最短的进程
        minServiceTimeProcess = executableProcess[0];
        int minServiceTime = minServiceTimeProcess.serviceTime;
        for (int i = 1; i < numberOfExecutableProcesses; i = i + 1) {
            if(executableProcess[i].serviceTime < minServiceTime) {
                minServiceTimeProcess = executableProcess[i];
                minServiceTime = executableProcess[i].serviceTime;
            }
        }
        //执行已到达的可执行进程中服务时间最短的进程
        for (int i = 0; i < size; i = i + 1) {
            if(minServiceTimeProcess.ID == processes[i].ID) {
                processes[i].finishTime = now + minServiceTimeProcess.serviceTime;
                processes[i].WholeTime();
                processes[i].WeightWholeTime();
                processes[i].executed = true;
                numberOfExecutedProcess += 1;
                //执行完之后更新当前时间
                now = processes[i].finishTime;
                break;
            }
        }
    }
    //计算平均值
    calculateAverage();
    
    //测试输出
    for(int i = 0; i < size; i = i + 1) {
        processes[i].PrintToConsole();
        cout << endl;
    }
    averageOutput();
    //输出状态
    ExecutionStatus();
}

void Processes::calculateAverage() {
    double totalWholeTime = 0;
    double totalWeightWholeTime = 0;
    for (int i = 0; i < size; i = i + 1) {
        totalWholeTime = totalWholeTime + processes[i].wholeTime;
        totalWeightWholeTime = totalWeightWholeTime + processes[i].weightWholeTime;
    }
    averageWholeTime = double(totalWholeTime) / double(size);
    averageWeightWholeTime = double(totalWeightWholeTime) / double(size);
}

void Processes::averageOutput()  {
    cout << "平均周转时间: " << setiosflags(ios::fixed) << setprecision(2) << averageWholeTime << endl;
    cout << "平均带权周转时间: " << setiosflags(ios::fixed) << setprecision(2) << averageWeightWholeTime << endl;
    cout << endl;
}

void Processes::ExecutionStatus()  {
    int totalTime = 0;
    for (int i = 0; i < size; i = i + 1) {
        totalTime = totalTime + processes[i].serviceTime;
    }
    cout << "时刻" << 0 << ", 进程" << processes[0].ID << "到达并开始执行" << endl;
    for (int t = 1; t <= totalTime; t = t + 1) {
        for (int i = 0; i < size; i = i + 1) {
            if(t == processes[i].arrivalTime) {
                cout << "时刻" << t << ", 进程" << processes[i].ID << "到达" << endl;
            }
            if(t == processes[i].finishTime) {
                cout << "时刻" << t << ", 进程" << processes[i].ID << "完成" << endl;
            }
            if(t == processes[i].finishTime - processes[i].serviceTime) {
                cout << "时刻" << t << ", 进程" << processes[i].ID << "开始执行" << endl;
            }
        }
    }
}

#endif /* Processes_h */
