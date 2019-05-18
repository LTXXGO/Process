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
    int size;// 进程数量
    Process * processes;// 进程数组
    double averageWholeTime;// 平均周转时间
    double averageWeightWholeTime;// 平均带权周转时间
    
    int now = 0;// 当前时刻
    int numberOfExecutedProcess = 0;// 已执行过的进程数量，用于判断是否全部执行完毕
    Process * executableProcess;// 存放当前可执行的进程的数组
    int numberOfExecutableProcesses;// 当前的可执行进程数
    
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
    // 高响应比优先 / HRRN 算法
    void HRRN();
    // 时间片轮转算法
    void RR(int q);
    
    // 计算平均时间
    void calculateAverage();
    // 测试输出函数
    void PrintToConsole();
        // 输出平均时间
    void averageOutput();
        // 输出每一时刻计算机状态
    void ExecutionStatus();
};

// MARK: - 先来先服务 / FCFS 算法
void Processes::FCFS() {
    // 循环模拟执行过程并计算
    for (int i = 0; i < size; i = i + 1) {
        // 计算完成时间
        if(i == 0) {
            processes[0].finishTime = processes[0].arrivalTime + processes[0].serviceTime;
        } else {
            processes[i].finishTime = processes[i - 1].finishTime + processes[i].serviceTime;
        }
        // 计算周转时间和带权周转时间
        processes[i].WholeTime();
        processes[i].WeightWholeTime();
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
}

// MARK: - 短作业优先 / SJF 算法
void Processes::SJF() {
    int minServiceTime;// 最短服务时间
    Process minServiceTimeProcess;// 用于暂时存储最短服务时间进程的变量
    // 执行第一个进程
    processes[0].finishTime = processes[0].arrivalTime + processes[0].serviceTime;
    processes[0].WholeTime();
    processes[0].WeightWholeTime();
    processes[0].executed = true;
    numberOfExecutedProcess = 1;
    // 更新当前时间
    now = processes[0].finishTime;
    // 判断是否传入函数的所有进程都已执行过
    while (numberOfExecutedProcess < size) {
        // 更新可执行程序数量为0
        numberOfExecutableProcesses = 0;
        // 判断其他进程是否已到达
        for (int i = 0; i < size; i = i + 1) {
            if(processes[i].arrivalTime <= now && !processes[i].executed) {
                executableProcess[numberOfExecutableProcesses] = processes[i];
                numberOfExecutableProcesses += 1;
            }
        }
        // 在已到达的可执行进程中找出服务时间最短的进程
        minServiceTimeProcess = executableProcess[0];
        minServiceTime = minServiceTimeProcess.serviceTime;
        for (int i = 1; i < numberOfExecutableProcesses; i = i + 1) {
            if(executableProcess[i].serviceTime < minServiceTime) {
                minServiceTimeProcess = executableProcess[i];
                minServiceTime = executableProcess[i].serviceTime;
            }
        }
        // 执行已到达的可执行进程中服务时间最短的进程
        for (int i = 0; i < size; i = i + 1) {
            if(minServiceTimeProcess.ID == processes[i].ID) {
                processes[i].finishTime = now + minServiceTimeProcess.serviceTime;
                processes[i].WholeTime();
                processes[i].WeightWholeTime();
                processes[i].executed = true;
                numberOfExecutedProcess += 1;
                // 执行完之后更新当前时间
                now = processes[i].finishTime;
                break;
            }
        }
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
}

// MARK: - 高响应比优先 / HRRN 算法
void Processes::HRRN() {
    double highestPriority;//最高优先权
    Process highestPriorityProcess;// 用于暂时存储优先权 / 响应比最高的进程的变量
    
    // 执行第一个到达的进程
    processes[0].finishTime = processes[0].serviceTime;
    processes[0].WholeTime();
    processes[0].WeightWholeTime();
    processes[0].executed = true;
    numberOfExecutedProcess = 1;
    // 更新当前时间
    now = processes[0].finishTime;
    // 判断是否传入函数的所有进程都已执行过
    while(numberOfExecutedProcess < size) {
        // 更新可执行程序数量为0
        numberOfExecutableProcesses = 0;
        // 判断其他进程是否到达
        for(int i = 1; i < size; i += 1) {
            if(processes[i].arrivalTime <= now && !processes[i].executed) {
                // 若进程到达时间在现在以前并且还未执行过, 则当前可执行进程数增加1
                executableProcess[numberOfExecutableProcesses] = processes[i];
                numberOfExecutableProcesses += 1;
                // 计算响应比(优先权)
                processes[i].priority = (now - processes[i].arrivalTime + processes[i].serviceTime) / processes[i].serviceTime;
            }
        }
        // 找出当前已到达进程中拥有最高响应比 / 优先权的进程
        highestPriorityProcess = executableProcess[0];
        highestPriority = highestPriorityProcess.priority;
        for (int i = 1; i < numberOfExecutableProcesses; i += 1) {
            if(executableProcess[i].priority > highestPriority) {
                highestPriority = executableProcess[i].priority;
                highestPriorityProcess = executableProcess[i];
            }
        }
        // 执行已到达的可执行进程中优先权 / 响应比最高的进程
        for (int i = 0; i < size; i = i + 1) {
            if(highestPriorityProcess.ID == processes[i].ID) {
                processes[i].finishTime = now + highestPriorityProcess.serviceTime;
                processes[i].WholeTime();
                processes[i].WeightWholeTime();
                processes[i].executed = true;
                numberOfExecutedProcess += 1;
                // 执行完之后更新当前时间
                now = processes[i].finishTime;
                break;
            }
        }
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
}

// MARK: - 时间片轮转 / RR 算法
void Processes::RR(int q) {
    // 执行第一个进程
    // 判断在当前时间片进程是否可以执行完毕
    if(processes[0].serviceTime <= q) {
        processes[0].finishTime = processes[0].serviceTime;
        processes[0].WholeTime();
        processes[0].WeightWholeTime();
        processes[0].executed = true;
        numberOfExecutedProcess += 1;
        now = processes[0].finishTime;
    } else {
        now = q;
        // 判断此时已到达程序有哪些
        for(int i = 1; i < size; i += 1) {
            if(processes[i].arrivalTime <= now) {
                // 若进程到达时间在现在以前并且还未执行完成, 则当前可执行进程数增加1
                executableProcess[numberOfExecutableProcesses] = processes[i];
                numberOfExecutableProcesses += 1;
            }
        }
        // 把当前时间片未执行完成的进程加入可执行进程数组的末尾
        executableProcess[numberOfExecutableProcesses] = processes[0];
        // 更新当前时间片未执行完成的进程的服务时间为剩余服务时间
        executableProcess[numberOfExecutableProcesses].serviceTime = processes[0].serviceTime - q;
        // 可执行c进程数加1
        numberOfExecutableProcesses += 1;
    }
    // 判断是否传入函数的所有进程都已执行过
    while (numberOfExecutedProcess < size) {
        for (int i = 0; i < numberOfExecutableProcesses; i += 1) {
            // 从now开始执行可执行进程数组中的第一个进程
            // 如果进程剩余服务时间小于或等于时间片长度, 即在本时间片内可执行完毕
            if(executableProcess[i].serviceTime <= q) {
                // 执行完成processes数组中相对应的进程
                for (int j = 0; j < size; j += 1) {
                    if(executableProcess[j].ID == processes[j].ID) {
                        processes[j].finishTime = now + executableProcess[j].serviceTime;
                        processes[j].WholeTime();
                        processes[j].WeightWholeTime();
                        processes[j].executed = true;
                        numberOfExecutedProcess += 1;
                        // 执行完之后更新当前时间
                        now = processes[i].finishTime;
                        break;
                    }
                }
            } else {
                now = now + q;
                // 判断在当前时间片内已到达程序有哪些
                for(int j = 1; j < size; j += 1) {
                    if(processes[j].arrivalTime <= now && processes[j].arrivalTime > now - q) {
                        // 若进程到达时间在当前时间片内, 则当前可执行进程数增加1
                        executableProcess[numberOfExecutableProcesses] = processes[j];
                        numberOfExecutableProcesses += 1;
                    }
                }
                // 把当前时间片未执行完成的进程加入可执行进程数组的末尾
                executableProcess[numberOfExecutableProcesses] = executableProcess[i];
                // 更新当前时间片未执行完成的进程的服务时间为剩余服务时间
                executableProcess[numberOfExecutableProcesses].serviceTime = executableProcess[i].serviceTime - q;
                numberOfExecutableProcesses += 1;
            }
        }
    }
}

// MARK: - 计算平均周转时间和带权平均周转时间
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

// MARK: - 在控制台输出平均周转时间和平均带权周转时间
void Processes::averageOutput()  {
    cout << "平均周转时间: " << setiosflags(ios::fixed) << setprecision(2) << averageWholeTime << endl;
    cout << "平均带权周转时间: " << setiosflags(ios::fixed) << setprecision(2) << averageWeightWholeTime << endl;
    cout << endl;
}

// MARK: - 在控制台输出每一时刻的状态
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

// MARK: - 输出到控制台
void Processes::PrintToConsole() {
    // 输出每个进程的各项属性
    for(int i = 0; i < size; i = i + 1) {
        processes[i].PrintToConsole();
        cout << endl;
    }
    // 输出平均值
    averageOutput();
    // 输出状态
    ExecutionStatus();
}

#endif /* Processes_h */
