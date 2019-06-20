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
private:
    int now;// 当前时刻
    int numberOfExecutedProcess;// 已执行过的进程数量，用于判断是否全部执行完毕
    Process * executableProcesses;// 存放当前可执行的进程的数组
    int numberOfExecutableProcesses;// 当前的可执行进程数
    // 重置进程的已执行属性
    void restore() {
        for (int i = 0; i < size; i++) {
            processes[i].executed = false;
        }
    }
public:
    int size;// 进程数量
    Process * processes;// 进程数组
    double averageWholeTime;// 平均周转时间
    double averageWeightWholeTime;// 平均带权周转时间
    int totalServiceTime();// 返回总服务时间
    
    // 构造函数
    Processes() {
        size = 0;
        now = 0;
        numberOfExecutedProcess = 0;
        numberOfExecutableProcesses = 0;
        processes = new Process[size];
        executableProcesses = new Process[size];
    }
    Processes(int n) {
        size = n;
        now = 0;
        numberOfExecutedProcess = 0;
        numberOfExecutableProcesses = 0;
        processes = new Process[size];
        executableProcesses = new Process[size];
    }
    
    // 调度算法
    void FCFS();// 先来先服务 / FCFS 算法
    void SJF();// 短作业优先 / SJF 算法
    void HRRN();// 高响应比优先 / HRRN 算法
    void RR(int q);// 时间片轮转算法
    
    //辅助函数
    void execution(Process p); // 执行进程并计算相关属性
    void otherProcessesArrival(int t1, int t2); // 判断是否有其他进程在时间(t1, t2]内到达
    void calculateAverage();// 计算平均时间
    void PrintToConsole();// 将测试内容打印到控制台
    void averageOutput();// 输出平均周转时间和平均带权周转时间
    void ExecutionStatus();// 输出每一时刻计算机状态
};

// MARK: - 先来先服务 / FCFS 算法
void Processes::FCFS() {
    now = 0;
    // 依次执行到达进程
    for (int i = 0; i < size; i += 1) {
        execution(processes[i]);
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
    restore();
}

// MARK: - 短作业优先 / SJF 算法
void Processes::SJF() {
    Process minServiceTimeProcess;// 用于暂时存储最短服务时间进程的变量
    now = 0;
    numberOfExecutableProcesses = 0;
    numberOfExecutedProcess = 0;
    // 执行第一个进程
    execution(processes[0]);
    // 判断是否传入函数的所有进程都已执行过
    while (numberOfExecutedProcess < size) {
        // 更新可执行程序数量为0
        numberOfExecutableProcesses = 0;
        // 判断其他进程是否已到达
        otherProcessesArrival(0, now);
        // 在已到达的可执行进程中找出服务时间最短的进程
        minServiceTimeProcess = executableProcesses[0];
        for (int i = 1; i < numberOfExecutableProcesses; i += 1) {
            if(executableProcesses[i].serviceTime < minServiceTimeProcess.serviceTime) {
                minServiceTimeProcess = executableProcesses[i];
            }
        }
        // 执行已到达的可执行进程中服务时间最短的进程
        execution(minServiceTimeProcess);
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
    restore();
}

// MARK: - 高响应比优先 / HRRN 算法
void Processes::HRRN() {
    Process highestPriorityProcess;// 用于暂时存储优先权 / 响应比最高的进程的变量
    now = 0;
    numberOfExecutableProcesses = 0;
    numberOfExecutedProcess = 0;
    // 执行第一个到达的进程
    execution(processes[0]);
    // 判断是否传入函数的所有进程都已执行过
    while (numberOfExecutedProcess < size) {
        numberOfExecutableProcesses = 0;
        otherProcessesArrival(0, now);
        // 找出当前已到达进程中拥有最高响应比 / 优先权的进程
        highestPriorityProcess = executableProcesses[0];
        for (int i = 1; i < numberOfExecutableProcesses; i += 1) {
            // 计算响应比(优先权)
            processes[i].priority = (now - processes[i].arrivalTime + processes[i].serviceTime) / processes[i].serviceTime;
            if (executableProcesses[i].priority > highestPriorityProcess.priority) {
                highestPriorityProcess = executableProcesses[i];
            }
        }
        // 执行已到达的可执行进程中优先权 / 响应比最高的进程
        execution(highestPriorityProcess);
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
    restore();
}

// MARK: - 时间片轮转 / RR 算法
void Processes::RR(int q) {
    executableProcesses = new Process[totalServiceTime()];
    now = 0;
    numberOfExecutableProcesses = 0;
    numberOfExecutedProcess = 0;
    // 将第一个进程加入可执行进程数组
    executableProcesses[numberOfExecutableProcesses] = processes[0];
    numberOfExecutableProcesses += 1;
    // 判断是否传入函数的所有进程都已执行过
    for (int i = 0; numberOfExecutedProcess < size; i += 1) {
        // 从now开始执行可执行进程数组中的当前进程
        // 如果进程剩余服务时间小于或等于时间片长度, 即在本时间片内可执行完毕
        if (executableProcesses[i].serviceTime <= q) {
            // 如果最后一个进程的到达时间在当前时间之后
            if (now < processes[size - 1].arrivalTime) {
                // 判断在当前时间片内已到达程序有哪些
                otherProcessesArrival(now, now + executableProcesses[i].serviceTime);
            }
            // 执行完成processes数组中相对应的进程
            execution(executableProcesses[i]);
        } else {
            // 如果最后一个进程的到达时间在当前时间之后
            if (now < processes[size - 1].arrivalTime) {
                // 判断在当前时间片内已到达程序有哪些
                otherProcessesArrival(now, now + q);
            }
            // 更新当前时间
            now = now + q;
            // 把当前时间片未执行完成的进程加入可执行进程数组的末尾
            executableProcesses[numberOfExecutableProcesses] = executableProcesses[i];
            // 更新当前时间片未执行完成的进程的服务时间为剩余服务时间
            executableProcesses[numberOfExecutableProcesses].serviceTime = executableProcesses[i].serviceTime - q;
                numberOfExecutableProcesses += 1;
        }
    }
    // 计算平均值
    calculateAverage();
    // 输出到控制台
    PrintToConsole();
    restore();
}

// MARK: - 辅助函数

// 执行完成processes数组中与进程p相对应的进程
void Processes::execution(Process p) {
    for (int i = 0; i < size; i += 1) {
        if (p.ID == processes[i].ID) {
            processes[i].finishTime = now + p.serviceTime;
            processes[i].WholeTime();
            processes[i].WeightWholeTime();
            processes[i].executed = true;
            // 更新已执行完成进程数
            numberOfExecutedProcess += 1;
            // 执行完之后更新当前时间
            now = processes[i].finishTime;
            break;
        }
    }
}

// 判断是否有进程在时间(t1, t2]内到达
void Processes::otherProcessesArrival(int t1, int t2) {
    for(int i = 0; i < size; i += 1) {
        if(processes[i].arrivalTime > t1 && processes[i].arrivalTime <= t2 && !processes[i].executed) {
            // 若进程到达时间在当前时间片内, 则当前可执行进程数增加1
            executableProcesses[numberOfExecutableProcesses] = processes[i];
            numberOfExecutableProcesses += 1;
        }
    }
}

// 返回总服务时间
int Processes::totalServiceTime() {
    int total = 0;
    for (int i = 0; i < size; i += 1) {
        total += processes[i].serviceTime;
    }
    return total;
}

// 计算平均周转时间和带权平均周转时间
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

// 在控制台输出平均周转时间和平均带权周转时间
void Processes::averageOutput()  {
    cout << "平均周转时间: " << setiosflags(ios::fixed) << setprecision(2) << averageWholeTime << endl;
    cout << "平均带权周转时间: " << setiosflags(ios::fixed) << setprecision(2) << averageWeightWholeTime << endl;
    cout << endl;
}

// 在控制台输出每一时刻的状态
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
    cout << "-----------------------" << endl;
}

// 输出到控制台
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
