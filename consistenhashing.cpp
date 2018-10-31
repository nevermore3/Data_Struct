string hashvalue(string name)
{
    string hashvalue = function(name);
    return hashvalue;
}


typedef struct RealNode
{
    string hashcode;
    string name;
    int  servicenum;
    int  numofvirtualhash;
    RealNode(string _name, int _numofvirtualhash)
    {
        name = _name;
        numofvirtualhash = _numofvirtualhash + 1;     //include itself
        servicenum = 0;
    }

}RealNode;


typedef struct VirtualNode
{
    string  realnodename;
    string  virtualnodename;
    string  hashcode;
    VirtualNode(string _realname, string _virtualname)
    {
        realnodename = _realname;
        virtualnodename = _virtualname;
    }

}VirtualNode;


class  ConsistenHash
{
public:
    ConsistenHash();
    ~ConsistenHash();
    void addnode(RealNode *node);
    void delnode(RealNode *node);
    string  getservice(string request);
private:
    // first is node's hashcode
    map<string, RealNode*>MapRealNodes;
    map<string, VirtualNode*>MapVirtualNodes;
};



void ConsistenHash::addnode(RealNode *node)
{
    string name = node->name;
    node->hashcode = hashvalue(name);
    // add realnode
    MapRealNodes.insert(pair<string, RealNode*>(node->hashcode, node));

    VirtualNode *real = new VirtualNode(name, name);
    MapVirtualNodes.insert(pair<string, VirtualNode*>(node->hashcode, real));

    //add virtualnode
    int num = node->numofvirtualhash;
    for(int i = 0; i < num; i++)
    {
        char buf[10] = {0};
        sprintf(buf, ":%04d", i);
        string virtualname = name + buf;
        VirtualNode *virtualnode = new VirtualNode(name, virtualname);
        virtualnode->hashcode = hashvalue(virtualname);
        MapVirtualNodes.insert(pair<string, VirtualNode*>(virtualnode->hashcode, virtualname));
    }
}

void ConsistenHash::delnode(RealNode *node)
{
    string name = node->name;
    auto iter = MapRealNodes.find(node->hashcode);
    if(iter != MapRealNodes.end())
    {
        //del realnode
        MapRealNodes.erase(iter);
        delete node;

        //del virtual node
        for(auto virIter = MapVirtualNodes.begin(), virIter != MapVirtualNodes.end())                 //important
        {
            if(virIter->second->realnodename == name)
            {
                VirtualNode *virnode = virIter->second;
                MapVirtualNodes.erase(virIter);
                virIter++;
                delete virnode;
            }
            else
                virIter++;                
        }
    }
}

string ConsistenHash::getservice(string request)
{
    string requesthashcode = hashvalue(request);
    string servicename;
    for(auto iter = MapVirtualNodes.begin(), iter != MapVirtualNodes.end(); iter++)
    {
        if(iter->first >= requesthashcode)        //first server
        {
            servicename = iter->second->realnodename;
            for(auto realIter = MapRealNodes.begin(); realIter != MapRealNodes.end(); realIter++)
            {
                if(realIter->second->name == iter->second->realnodename)
                {
                    realIter->second->servicenum++;
                    break;
                }
            }
            break;
        }
    }
    return servicename;
}
