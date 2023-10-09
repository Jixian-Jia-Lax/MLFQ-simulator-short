#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Process structure to represent a job
struct Process {
    int id;
    int startTime;
    int runTime;
    int ioFrequency;
    int remainingTime;
    int priorityLevel;
    int remainingAllotment;

    Process(int id, int startTime, int runTime, int ioFrequency, int priorityLevel)
            : id(id), startTime(startTime), runTime(runTime), ioFrequency(ioFrequency),
              remainingTime(runTime), priorityLevel(priorityLevel),remainingAllotment(5){}
};

// MLFQ Scheduler class
class MLFQScheduler {
public:
    MLFQScheduler(int numLevels);

    // Add a process to the scheduler
    void addProcess(Process process);

    // Run the scheduler and display logs
    void run();

private:
    int numLevels;
    vector<queue<Process> > queues;
    int currentTime;

    // Helper function to check if all queues are empty
    bool allQueuesEmpty();

    // Helper function to move processes between queues
    void reduceProcessPriority(Process process);
};

MLFQScheduler::MLFQScheduler(int numLevels) : numLevels(numLevels), currentTime(0) {
    queues.resize(numLevels);
}

void MLFQScheduler::addProcess(Process process) {
    queues[process.priorityLevel].push(process);
}

bool MLFQScheduler::allQueuesEmpty() {
    for (const auto& queue : queues) {
        if (!queue.empty()) {
            return false;
        }
    }
    return true;
}

void MLFQScheduler::reduceProcessPriority(Process process) {
    process.priorityLevel = min(process.priorityLevel + 1, numLevels - 1);
    process.remainingAllotment = 5;
    queues[process.priorityLevel].push(process);
}

void MLFQScheduler::run() {
    cout << "Simulation Log:" << endl;

    while (!allQueuesEmpty()) {
        for (int i = 0; i < numLevels; ++i) {
            if (!queues[i].empty()) {
                Process& currentProcess = queues[i].front();
                if (currentProcess.startTime <= currentTime) {
                    cout << "Time " << currentTime << ": Running Process " << currentProcess.id
                         << " (Priority " << currentProcess.priorityLevel << ")" << endl;
                    currentProcess.remainingTime--;
                    currentProcess.remainingAllotment--;

                    if (currentProcess.remainingTime == 0) {
                        queues[i].pop();
                    } else if(currentProcess.remainingAllotment == 0){
                        queues[i].pop();
                        reduceProcessPriority(currentProcess);
                    }
                    else{
                        queues[i].pop();
                        queues[i].push(currentProcess);
                    }

                    currentTime++;
                } else {
                    queues[i].pop();
                    queues[i].push(currentProcess);
                }
            }
        }
    }
}

int main() {
    int numLevels;
    cout << "Enter the number of levels: ";
    cin >> numLevels;



    MLFQScheduler scheduler(numLevels);

    int numJobs;
    cout << "Enter the number of jobs: ";
    cin >> numJobs;

    for (int i = 0; i < numJobs; ++i) {
        int startTime, runTime, ioFrequency;
        cout << "Job " << i + 1 << " details:" << endl;
        cout << "Start Time: ";
        cin >> startTime;
        cout << "Run Time: ";
        cin >> runTime;
        cout << "IO Frequency: ";
        cin >> ioFrequency;
        scheduler.addProcess(Process(i, startTime, runTime, ioFrequency, 0));
    }

    // Run the MLFQ scheduler and display logs
    scheduler.run();

    return 0;
}
