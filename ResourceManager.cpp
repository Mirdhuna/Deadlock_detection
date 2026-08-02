/*#include "ResourceManager.h"

ResourceManager::ResourceManager(int p,int r)
{
    num_processes=p;
    num_resources=r;

    Available.resize(r);
    Max.resize(p,vector<int>(r));
    Allocation.resize(p,vector<int>(r));
    Need.resize(p,vector<int>(r));
}

void ResourceManager::setAvailable(vector<int>a){ Available=a; }
void ResourceManager::setMax(vector<vector<int>>m){ Max=m; }
void ResourceManager::setAllocation(vector<vector<int>>a){ Allocation=a; }

void ResourceManager::calculateNeed()
{
    for(int i=0;i<num_processes;i++)
        for(int j=0;j<num_resources;j++)
            Need[i][j]=Max[i][j]-Allocation[i][j];
}

vector<vector<int>> ResourceManager::getNeed(){ return Need; }
vector<int> ResourceManager::getAvailable(){ return Available; }

bool ResourceManager::isSafeState(QString &sequence)
{
    vector<int> Work = Available;
    vector<bool> Finish(num_processes,false);

    sequence="";

    for(int count=0;count<num_processes;count++)
    {
        bool found=false;

        for(int i=0;i<num_processes;i++)
        {
            if(!Finish[i])
            {
                bool possible=true;

                for(int j=0;j<num_resources;j++)
                {
                    if(Need[i][j] > Work[j])
                    {
                        possible=false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<num_resources;j++)
                        Work[j]+=Allocation[i][j];

                    sequence+="P"+QString::number(i)+" ";

                    Finish[i]=true;
                    found=true;
                }
            }
        }

        if(!found)
            return false;
    }

    return true;
}
*/

#include "ResourceManager.h"

ResourceManager::ResourceManager(int p,int r)
{
    num_processes=p;
    num_resources=r;

    Available.resize(r);
    Max.resize(p,vector<int>(r));
    Allocation.resize(p,vector<int>(r));
    Need.resize(p,vector<int>(r));
}

void ResourceManager::setAvailable(vector<int>a){ Available=a; }
void ResourceManager::setMax(vector<vector<int>>m){ Max=m; }
void ResourceManager::setAllocation(vector<vector<int>>a){ Allocation=a; }

void ResourceManager::calculateNeed()
{
    for(int i=0;i<num_processes;i++)
        for(int j=0;j<num_resources;j++)
            Need[i][j]=Max[i][j]-Allocation[i][j];
}

vector<vector<int>> ResourceManager::getNeed(){ return Need; }
vector<int> ResourceManager::getAvailable(){ return Available; }

bool ResourceManager::isSafeState(QString &sequence)
{
    vector<int> Work = Available;
    vector<bool> Finish(num_processes,false);

    sequence="";

    for(int count=0;count<num_processes;count++)
    {
        bool found=false;

        for(int i=0;i<num_processes;i++)
        {
            if(!Finish[i])
            {
                bool possible=true;

                for(int j=0;j<num_resources;j++)
                {
                    if(Need[i][j] > Work[j])
                    {
                        possible=false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<num_resources;j++)
                        Work[j]+=Allocation[i][j];

                    sequence+="P"+QString::number(i)+" ";
                    Finish[i]=true;
                    found=true;
                }
            }
        }

        if(!found)
            return false;
    }

    return true;
}

bool ResourceManager::requestResources(int pid, vector<int> request)
{
    for(int i=0;i<num_resources;i++)
    {
        if(request[i] > Need[pid][i])
            return false;

        if(request[i] > Available[i])
            return false;
    }

    for(int i=0;i<num_resources;i++)
    {
        Available[i] -= request[i];
        Allocation[pid][i] += request[i];
        Need[pid][i] -= request[i];
    }

    QString seq;

    if(isSafeState(seq))
        return true;

    for(int i=0;i<num_resources;i++)
    {
        Available[i] += request[i];
        Allocation[pid][i] -= request[i];
        Need[pid][i] += request[i];
    }

    return false;
}

QString ResourceManager::detectDeadlock()
{
    vector<int> Work = Available;
    vector<bool> Finish(num_processes,false);

    bool progress=true;

    while(progress)
    {
        progress=false;

        for(int i=0;i<num_processes;i++)
        {
            if(!Finish[i])
            {
                bool possible=true;

                for(int j=0;j<num_resources;j++)
                {
                    if(Need[i][j] > Work[j])
                    {
                        possible=false;
                        break;
                    }
                }

                if(possible)
                {
                    for(int j=0;j<num_resources;j++)
                        Work[j]+=Allocation[i][j];

                    Finish[i]=true;
                    progress=true;
                }
            }
        }
    }

    QString result="Deadlocked Processes: ";
    bool found=false;

    for(int i=0;i<num_processes;i++)
    {
        if(!Finish[i])
        {
            result+="P"+QString::number(i)+" ";
            found=true;
        }
    }

    if(!found)
        result="No Deadlock Detected";

    return result;
}
