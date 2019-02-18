const int MAXLEVEL = 16;
int getlevel();
typedef struct Node
{
    int value;
    int level;
    Node *next[MAXLEVEL];
    Node(int _value, int _level)
    {
        value = _value;
        level = _level;
        for (int i = 0; i < MAXLEVEL; i++)
        {
            next[i] = NULL;
        }
    }
}Node;


class skiplist
{
public:
    skiplist();
    bool find(int _value);
    void insert(int value);
    void printlist();
    void deletevalue(int _value);
    int sizes();
private:
    int levelcount;
    int size;
    Node *head;
};

int main()
{
    int a[] = { 10,2,1,5,7,8 };
    int size = sizeof(a) / sizeof(int);
    skiplist list;
    for (int i = 0; i < size; i++)
    {
        //cout << getlevel() << endl;
        list.insert(a[i]);
    }
    list.printlist();
    list.find(7);
    list.find(3);
    list.insert(3);
    list.find(3);
    list.printlist();
    list.deletevalue(3);
    list.deletevalue(10);
    list.printlist();
    return 0;
}

skiplist::skiplist()
{
    size = 0;
    levelcount = 1;
    head = new Node(-1, MAXLEVEL);
}

bool skiplist::find(int values)
{
    Node *temp = head;
    for (int i = levelcount - 1; i >= 0; i--)
    {
        while (temp->next[i] != NULL && temp->next[i]->value < values)
        {
            temp = temp->next[i];
        }
    }
    if (temp->next[0] != NULL && temp->next[0]->value == values)
    {
        cout << "Find value in SkipList" << endl;
        return true;
    }
    else
    {
        cout << "Values not in SkipList" << endl;
        return false;
    }
}



int getlevel()
{
    //srand((unsigned int)time(NULL));
    int num = rand() % MAXLEVEL;
    cout << "GetLevel  is " << num << endl;
    return num;
}
void skiplist::insert(int values)
{
    int newlevel = getlevel();
    Node *newNode = new Node(values, newlevel);
    Node **update = (Node**)malloc(sizeof(Node*) *newlevel);
    //Node *update1 = new Node[newlevel];
    Node *temp = head;
    for (int i = newlevel - 1; i >= 0; i--)
    {
        while (temp->next[i] != NULL && temp->next[i]->value < values)
        {
            temp = temp->next[i];
        }
        //update1[i] = temp;
        update[i] = temp;
    }
    levelcount = newlevel > levelcount ? newlevel : levelcount;
    for (int i = 0; i < newlevel; i++)
    {
        newNode->next[i] = update[i]->next[i];
        update[i]->next[i] = newNode;
    }
    size++;
    cout << "insert Success" << endl;
}

void skiplist::printlist()
{
    Node *temp = head;
    while (temp->next[0] != NULL)
    {
        int v= temp->next[0]->value;
        cout << v << "--->";
        temp = temp->next[0];
    }
    cout << endl;
}

void skiplist::deletevalue(int values)
{
    Node **update = (Node**)malloc(sizeof(Node*) * levelcount);
    memset(update, 0, sizeof(Node*)*levelcount);
    Node *temp = head;
    for (int i = levelcount - 1; i >= 0; i--)
    {
        while (temp->next[i] != NULL && temp->next[i]->value < values)
        {
            temp = temp->next[i];
        }
        update[i] = temp;
    }
    for (int i = 0; i < levelcount; i++)
    {
        if (update[i]->next[i] != NULL && update[i]->next[i]->value == values)
        {
            update[i]->next[i] = update[i]->next[i]->next[i];
        }
    }
    size--;
}

int skiplist::sizes()
{
    return size;
}
