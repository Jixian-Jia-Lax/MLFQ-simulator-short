#include <iostream>
#include <vector>
#include <queue>

using namespace std;

// Process structure to represent a job
struct Process {
    int id;
    int startTime;
    int runTime;

    int remainingTime;
    int priorityLevel;
    int remainingAllotment;

    Process(int id, int startTime, int runTime, int priorityLevel, int remainingAllotment)
            : id(id), startTime(startTime), runTime(runTime),
              remainingTime(runTime), priorityLevel(priorityLevel),remainingAllotment(remainingAllotment){}
};

// MLFQ Scheduler class
class MLFQScheduler {
public:
    MLFQScheduler(int numLevels, vector<int> quantums, vector<int> allotments);

    // Add a process to the scheduler
    void addProcess(Process process);

    // Run the scheduler and display logs
    void run();

private:
    vector<int> quantums;
    vector<int> allotments;
    int numLevels;
    vector<queue<Process> > queues;
    int currentTime;

    // Helper function to check if all queues are empty
    bool allQueuesEmpty();

    // Helper function to move processes between queues
    void reduceProcessPriority(Process process);
};

MLFQScheduler::MLFQScheduler(int numLevels, vector<int> quantums, vector<int> allotments) : numLevels(numLevels), currentTime(0),quantums(quantums),allotments(allotments) {
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
    process.remainingAllotment = allotments[process.priorityLevel];
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
                    currentProcess.remainingTime -= quantums[currentProcess.priorityLevel];
                    currentProcess.remainingAllotment-= quantums[currentProcess.priorityLevel];

                    if (currentProcess.remainingTime <= 0) {
                        queues[i].pop();
                    } else if(currentProcess.remainingAllotment <= 0){
                        queues[i].pop();
                        reduceProcessPriority(currentProcess);
                    }
                    else{
                        queues[i].pop();
                        queues[i].push(currentProcess);
                    }

                    currentTime+= quantums[currentProcess.priorityLevel];
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

    vector<int> quantums;
    vector<int> allotments;

    quantums.resize(numLevels);
    allotments.resize((numLevels));
    for(int i =0; i< numLevels; ++i){
        cout << "Enter the quantum of queue " << i;
        cin >> quantums[i];
        cout << "Enter the allotment of queue " << i;
        cin >> allotments[i];
    }


    MLFQScheduler scheduler(numLevels, quantums, allotments);

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

        scheduler.addProcess(Process(i, startTime, runTime, 0,allotments[0]));
    }

    // Run the MLFQ scheduler and display logs
    scheduler.run();

    return 0;
}
