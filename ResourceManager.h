#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <vector>
#include <QString>

using namespace std;

class ResourceManager
{
private:
    int num_processes;
    int num_resources;

    vector<int> Available;
    vector<vector<int>> Max;
    vector<vector<int>> Allocation;
    vector<vector<int>> Need;

public:
    ResourceManager(int p,int r);

    void setAvailable(vector<int> a);
    void setMax(vector<vector<int>> m);
    void setAllocation(vector<vector<int>> a);

    void calculateNeed();

    bool isSafeState(QString &sequence);

    bool requestResources(int pid, vector<int> request);

    QString detectDeadlock();

    vector<vector<int>> getNeed();
    vector<int> getAvailable();
};

#endif
