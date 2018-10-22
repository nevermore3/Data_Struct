#include <iostream>
#include <vector>
using namespace std;


class unionfind
{
public:
    unionfind(int _n);
    ~unionfind();
    bool find(int i, int j);
    void union(int i, int j);
    int getcount();
private:
    vector<int>parent;
    vector<int>size;
    int getroot(int i);
    int n;
    int count;   
};

unionfind::unionfind(int _n)
{
    this.n = _n;
    this.count = count;
    parent.resize(n, 0);
    size.resize(n, 1);
    for(int i = 0; i < n; i++)
    {
        parent[i] = i;
    }
}

int unionfind::getcount()
{
    return count;
}
int unionfind::getroot(int i)
{
    if(i == parent[i])
    {
        return i;
    }
    else
    {
        parent[i] = getroot(parent[i]);
        return parent[i];
    }
}

bool unionfind::find(int i, int j)
{
    int roota = getroot(i);
    int rootb = getroot(j);
    if(roota == rootb)
        return true;
    else
        return false;
}

void unionfind::union(int i, int j)
{
    int roota = getroot(i);
    int rootb = getroot(j);
    if(roota == rootb)
        return;
    else
    {
        if(size[roota] < size[rootb])
        {
            parent[roota] = rootb;
            size[rootb] += size[roota];
        }
        else
        {
            parent[rootb] = roota;
            size[roota] += size[rootb];
        }
        count--;
    }
}
