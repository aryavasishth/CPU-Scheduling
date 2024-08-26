#include <iostream>
#include <vector>
#include <algorithm>
#include <deque>

struct Process {
    int id;              // Process ID
    int arrivalTime;     // Arrival time of the process
    int burstTime;       // Burst time required by the process
    int priority;        // Priority of the process (for Priority Scheduling)
    int waitingTime;     // Waiting time for the process
    int turnaroundTime;  // Turnaround time for the process
    int completionTime;  // Completion time of the process
    int remainingTime;   // Remaining time for SRTF and RR
};

// Function to calculate metrics like Turnaround Time, Waiting Time, CPU Utilization, and Throughput
void calculateMetrics(std::vector<Process>& processes) {
    int totalTurnaroundTime = 0, totalWaitingTime = 0;
    int totalBurstTime = 0, totalIdleTime = 0;
    int n = processes.size();
    
    for (auto& process : processes) {
        process.turnaroundTime = process.completionTime - process.arrivalTime;
        process.waitingTime = process.turnaroundTime - process.burstTime;
        totalTurnaroundTime += process.turnaroundTime;
        totalWaitingTime += process.waitingTime;
        totalBurstTime += process.burstTime;
    }

    // Calculate total time span (from first arrival to last completion)
    int totalTimeSpan = processes.back().completionTime - processes.front().arrivalTime;

    // Calculate CPU Utilization and Throughput
    float cpuUtilization = ((float)totalBurstTime / totalTimeSpan) * 100;
    float throughput = (float)n / totalTimeSpan;

    std::cout << "\nProcess\tAT\tBT\tCT\tTAT\tWT\n";
    for (const auto& process : processes) {
        std::cout << "P" << process.id << "\t" << process.arrivalTime << "\t" << process.burstTime << "\t" 
                  << process.completionTime << "\t" << process.turnaroundTime << "\t" << process.waitingTime << "\n";
    }

    std::cout << "\nAverage Turnaround Time: " << (float)totalTurnaroundTime / n << "\n";
    std::cout << "Average Waiting Time: " << (float)totalWaitingTime / n << "\n";
    std::cout << "CPU Utilization: " << cpuUtilization << "%\n";
    std::cout << "Throughput: " << throughput << " processes/unit time\n";
}
void fcfs(std::vector<Process>& processes) {
    int time = 0;
    for (auto& process : processes) {
        if (time < process.arrivalTime)
            time = process.arrivalTime;
        
        time += process.burstTime;
        process.completionTime = time;
    }
}

// Shortest Job First (SJF) Scheduling Algorithm
void sjf(std::vector<Process>& processes) {
    int time = 0, completed = 0;
    while (completed != processes.size()) {
        int idx = -1;
        int minBurst = INT16_MAX;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrivalTime <= time && processes[i].completionTime == 0 && processes[i].burstTime < minBurst) {
                minBurst = processes[i].burstTime;
                idx = i;
            }
        }

        if (idx != -1) {
            time += processes[idx].burstTime;
            processes[idx].completionTime = time;
            completed++;
        } else {
            time++;
        }
    }
}

// Shortest Remaining Time First (SRTF) Scheduling Algorithm
void srtf(std::vector<Process>& processes) {
    int time = 0, completed = 0;
    while (completed != processes.size()) {
        int idx = -1;
        int minRemaining = INT16_MAX;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrivalTime <= time && processes[i].remainingTime > 0 && processes[i].remainingTime < minRemaining) {
                minRemaining = processes[i].remainingTime;
                idx = i;
            }
        }

        if (idx != -1) {
            time++;
            processes[idx].remainingTime--;
            if (processes[idx].remainingTime == 0) {
                processes[idx].completionTime = time;
                completed++;
            }
        } else {
            time++;
        }
    }
}

// Priority Scheduling Algorithm
void priorityScheduling(std::vector<Process>& processes) {
    int time = 0, completed = 0;
    while (completed != processes.size()) {
        int idx = -1;
        int highestPriority = INT16_MAX;

        for (int i = 0; i < processes.size(); i++) {
            if (processes[i].arrivalTime <= time && processes[i].completionTime == 0 && processes[i].priority < highestPriority) {
                highestPriority = processes[i].priority;
                idx = i;
            }
        }

        if (idx != -1) {
            time += processes[idx].burstTime;
            processes[idx].completionTime = time;
            completed++;
        } else {
            time++;
        }
    }
}

// Round Robin Scheduling Algorithm with deque
void roundRobin(std::vector<Process>& processes, int quantum) {
    std::deque<int> readyQueue;
    int time = 0, completed = 0;
    
    for (int i = 0; i < processes.size(); i++) {
        readyQueue.push_back(i);
    }

    while (!readyQueue.empty()) {
        int idx = readyQueue.front();
        readyQueue.pop_front();

        if (processes[idx].remainingTime > quantum) {
            time += quantum;
            processes[idx].remainingTime -= quantum;

            for (int i = 0; i < processes.size(); i++) {
                if (i != idx && processes[i].arrivalTime <= time && processes[i].completionTime == 0 && std::find(readyQueue.begin(), readyQueue.end(), i) == readyQueue.end()) {
                    readyQueue.push_back(i);
                }
            }

            readyQueue.push_back(idx);
        } else {
            time += processes[idx].remainingTime;
            processes[idx].remainingTime = 0;
            processes[idx].completionTime = time;
            completed++;
        }
    }
}

int main() {
    int n, choice, quantum;
    std::cout << "Enter number of processes: ";
    std::cin >> n;

    std::vector<Process> processes(n);
    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        std::cout << "Enter arrival time, burst time, and priority for process " << i + 1 << ": ";
        std::cin >> processes[i].arrivalTime >> processes[i].burstTime >> processes[i].priority;
        processes[i].remainingTime = processes[i].burstTime;
    }

    std::cout << "Choose scheduling algorithm:\n1. FCFS\n2. SJF\n3. SRTF\n4. Priority Scheduling\n5. Round Robin\n";
    std::cin >> choice;

    switch (choice) {
        case 1:
            fcfs(processes);
            break;
        case 2:
            sjf(processes);
            break;
        case 3:
            srtf(processes);
            break;
        case 4:
            priorityScheduling(processes);
            break;
        case 5:
            std::cout << "Enter time quantum: ";
            std::cin >> quantum;
            roundRobin(processes, quantum);
            break;
        default:
            std::cout << "Invalid choice!";
            return 0;
    }

    calculateMetrics(processes);
    return 0;
}
