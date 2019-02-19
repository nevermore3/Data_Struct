typedef struct Point
{
    int x;
    int y;
    int g;  //start -> current  cost
    int h;  //current -> destiation cost
    int cost; //overall cost
    Point(int _x, int _y):x(_x), y(_y), g(0), h(0), cost(0){}
}Point;

class Astar
{
public:
    Astar();
    bool search();
    ~Astar();
    void print_board();
    void setdestination(int x, int y);
    void setstart(int x, int y);
    void print_path();
private:
    Point& find(int x, int y);
    void load_data(const char* filename) ;
    list<Point>openlist;
    list<Point>closelist;
    vector<vector<int>>board;
    vector<vector<pair<int,int>>>parent;
    bool isvaild(int x, int y);
    int calch(int x, int y);
    vector<vector<int>>visit;
    int x_end;
    int y_end;
    int x_start;
    int y_start;
};

typedef struct Dir
{
    int x;
    int y;
    int g;
    const char* name;
}Dir;

Dir direction[] =
{
    { -1, -1, 14, "左上" },
    { -1,  0, 10, "正上" },
    { -1,  1, 14, "右上" },
    { 0,  -1, 10, "正左" },
    { 0,   1, 10, "正右" },
    { 1,  -1, 14, "左下" },
    { 1,   0, 10, "正下" },
    { 1,   1, 14, "右下" }
};

Astar::Astar()
{
    const char *filename = "board.txt";
    load_data(filename);
    print_board();
    int row = board.size();
    int col = board[0].size();
    visit.resize(row, vector<int>(col, 0));
    parent.resize(row, vector<pair<int, int>>(col, pair<int,int>(0,0)));
}

bool compare(Point &a, Point &b)
{
    return a.cost < b.cost;
}

bool Astar::isvaild(int x, int y)
{
    int row = board.size();
    int col = board[0].size();
    if (x < 0 || x >= row || y < 0 || y >= col || board[x][y] != 0)
        return false;
    return true;
}

int Astar::calch(int x, int y)
{
    return (abs(x - x_end) + abs(y - y_end)) * 10;
}

bool Astar::search()
{
    int row = board.size();
    int col = board[0].size();
    openlist.push_back(Point(x_start,y_start));
    visit[x_start][y_start] = 1;
    while (!openlist.empty())
    {
        openlist.sort(compare);
        Point curr = openlist.front();
        if (curr.x == x_end && curr.y == y_end)
        {
            cout << "Find path" << endl;
            return true;
        }
        openlist.pop_front();
        closelist.push_back(curr);
        visit[curr.x][curr.y] = 2;
        for (int i = 0; i < sizeof(direction) / sizeof(Dir); i++)
        {
            int _x = curr.x + direction[i].x;
            int _y = curr.y + direction[i].y;
            cout << direction[i].name << endl;
            if (isvaild(_x, _y))
            {
                if (visit[_x][_y] == 0)
                {
                    Point node(_x, _y);
                    node.g = curr.g + direction[i].g;
                    node.h = calch(_x, _y);
                    node.cost = node.g + node.h;
                    openlist.push_back(node);
                    visit[_x][_y] = 1;
                    parent[_x][_y] = pair<int, int>(curr.x, curr.y);
                }
                else if (visit[_x][_y] == 1)
                {
                    Point t = find(_x, _y);
                    if (t.g > curr.g + direction[i].g)
                    {
                        t.g = curr.g + direction[i].g;
                        t.cost = t.g + t.h;
                        parent[_x][_y] = pair<int, int>(curr.x, curr.y);

                    }
                }
            }
        }
    }
    cout << "Can't find path" << endl;
    return false;
}


Astar::~Astar()
{
}

void Astar::print_board()
{
    cout << endl<<"---------------------" << endl;
    for (int i = 0; i < board.size(); i++)
    {
        for (int j = 0; j < board[i].size(); j++)
        {
            cout << board[i][j] << "\t";
        }
        cout << endl;
    }
}

void Astar::setdestination(int x, int y)
{
    x_end = x;
    y_end = y;
}

void Astar::setstart(int x, int y)
{
    x_start = x;
    y_start = y;
}

void Astar::print_path()
{
    for (int i = 0; i < parent.size(); i++)
    {
        for (int j = 0; j < parent[i].size(); j++)
        {
            cout << "(" << parent[i][j].first << "," << parent[i][j].second << ")" << "\t";
        }
        cout << endl;
    }
    int x = x_end;
    int y = y_end;
    int tempx, tempy;
    while (!(x == x_start && y == y_start))
    {
        //cout << x << "\t" << y <<"\t";
        tempx = x;
        tempy = y;
        x = parent[tempx][tempy].first;
        y = parent[tempx][tempy].second;
        //cout << x << "\t" << y << endl;
        board[x][y] = 2;
    }
    print_board();
}

Point& Astar::find(int x, int y)
{
    for (auto iter = openlist.begin(); iter != openlist.end(); iter++)
    {
        if (iter->x == x && iter->y == y)
            return *iter;
    }
    return Point(-1, -1);
}

void Astar::load_data(const char * filename)
{
    fstream  fin(filename);
    if (!fin.is_open())
    {
        cout << "open file:" << filename << " fail!!!" << endl;
        return;
    }
    string  line;
    int num;
    vector<int>temp;
    stringstream ss;
    while (getline(fin, line))
    {
        cout << "line is :" << line << endl;
        ss.clear();
        ss.str(line);
        while (!ss.eof())
        {
            ss >> num;
            temp.push_back(num);
        }
        board.push_back(temp);
        temp.clear();
    }
}
