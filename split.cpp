vector<string> split(const string &s)
{
    const char* delims = " \t";
    vector<string>tokens;
    int len = s.size();
    char *tok;
    char *line = (char*)malloc(len + 1);
    memset(line, 0, len + 1);
    strcpy_s(line, len + 1, s.c_str());
    char *ptr = NULL;
    tok = strtok_s(line, delims, &ptr);
    while (tok != NULL)
    {
        tokens.push_back(tok);
        tok = strtok_s(NULL, delims, &ptr);
    }
    return tokens;
}

int read_file(const char*filename, vector<string>&lines)
{
    fstream input;
    string line;
    input.open(filename, ios::in);
    if (!input.is_open())
    {
        cout << "can't open file " << endl;
        return -1;
    }
    else
    {
        int i = 1;
        while (getline(input, line))
        {
            lines.push_back(line);
            i++;
        }
        return 1;
    }
}
