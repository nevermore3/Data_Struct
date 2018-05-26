#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <fstream>
#include <stdlib.h>
using namespace std;

#define DELIM " \t"

#define INT_MAX 32767

class Dijikstra
{
public:
	Dijikstra(const char* file_name_) :file_name(file_name_) {};
	~Dijikstra();
	void bulid_graph();
	void shortest_road(int index);
	void get_path(int end);

private:
	//use smart point
	//unique_ptr<char*>file_name;
	const char* file_name;
	int row;
	int column;
	vector<vector<int>>graph;
	int get_rows();
	int get_column();
	vector<int>path;
};



void function()
{
	const char* file_name = "test.txt";
	Dijikstra dia(file_name);
	dia.bulid_graph();

	int start = 3;
	dia.shortest_road(start);
	int end = 6;
	dia.get_path(end);
	return;
}



Dijikstra::~Dijikstra(){}

int Dijikstra::get_rows()
{
	int rows = 0;
	//unique_ptr<FILE, decltype(&fclose)>f(fopen(file_name, "r"), fclose);
	fstream  fp;
	string line;
	fp.open(file_name, fstream::in);
	while (getline(fp, line))
	{
		cout << line << endl;
		rows++;
	}
	cout << "the rows is :" << rows << endl;
	fp.close();
	this->row = rows;
	return 1;

}

int Dijikstra::get_column()
{
	int column = 0;
	fstream fp(file_name, fstream::in);
	string line;
	getline(fp, line);

	char *line_ = const_cast<char*>(line.c_str());
	char *temp = NULL;
	char *buff = NULL;
	temp = strtok_s(line_, DELIM, &buff);
	while (temp)
	{
		temp = strtok_s(NULL, DELIM, &buff);
		column++;
	}
	cout << "column is :" << column << endl;

	fp.close();
	this->column = column;
	return 1;
}

void Dijikstra::bulid_graph()
{
	get_rows();
	get_column();
	int rows = this->row;
	int col = this->column;
	vector<vector<int> >gh(rows);
	for (int i = 0; i < rows; i++)
	{
		gh[i].resize(col);
	}
	fstream in(file_name, fstream::in);
	string line;

	int i = 0;
	int j = 0;
	while (getline(in, line))
	{
		char *line_ = const_cast<char*>(line.c_str());
		char *buff = NULL;
		char *temp = strtok_s(line_, DELIM, &buff);
		j = 0;
		while (temp)
		{
			gh[i][j++] = (0 == strcmp(temp, "#")) ? INT_MAX : atoi(temp);
			temp = strtok_s(NULL, DELIM, &buff);
		}
		i++;
	}
	in.close();
	this->graph.swap(gh);
	for (int i = 0; i < graph.size(); i++)
	{
		for (int j = 0; j < graph[i].size(); j++)
		{
			cout << graph[i][j] << "\t";
		}
		cout << endl;
	}
}


void Dijikstra::shortest_road(int start)
{
	cout << "start index is :" << start << endl;
	int size = this->row;
	vector<int>Flag(size, 0);
	vector<int>distance(size, 0);
	vector<int>prev(size, 0);

	
	int min_index = start;
	for (int i = 0; i < size; i++)
	{
		distance[i] = this->graph[start][i];
		if (distance[i] != INT_MAX)
		{
			prev[i] = start;
		}
	}
	Flag[start] = 1;

	for (int i = 0; i < size - 1; i++)
	{
		int MIN = INT_MAX;
		// find min_index
		for (int j = 0; j < size; j++)
		{
			if (Flag[j] == 0 && distance[j] < MIN)
			{
				MIN = distance[j];
				min_index = j;
			}
		}
		//add S
		Flag[min_index] = 1;
		//update distance
		for (int k = 0; k < size; k++)
		{
			int temp = (this->graph[min_index][k] == INT_MAX) ? INT_MAX : (distance[min_index] + this->graph[min_index][k]);
			if (Flag[k] == 0 && distance[k] > temp)
			{
				distance[k] = temp;
				prev[k] = min_index;
			}
		}


	}
	prev.swap(this->path)
	cout << endl << "-----------" << endl;
	for (int i = 0; i < size; i++)
	{
		cout << distance[i] << "\t";
	}
	cout << endl;
}


void Dijikstra::get_path(int end)
{
	int index = end;
	while (index != this->path[index])
	{
		cout << index << "<--";
		index = this->path[index];
	}
	cout << index << endl;
}
