#include "stdafx.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

#define LEN 1000
#define MAX (LEN*5)
// swap value in place
template <class T> void swap_value(T &a, T &b)
{
	T temp = a;
	a = b;
	b = temp;
}
// function template
template<class T> void print_function(T i)
{
	cout<<i<<"\t";
}

//class template
template<class T> class print_object
{
public:
	void operator() (T i)
	{
		if(i >= _i && i <= _j )
			cout<<i<<"\t";
	}
	// initialize  list
	print_object(T i = -100000, T j = 100000 ):_i(i), _j(j){}
	
private:
	T _i;
	T _j;
};

template <class T> void print_array(vector<T> &num, int count)
{
	cout<<"swap  num   is     "<<count<<endl;
	//for_each(num.begin(),  num.end(),  print_function<T>); 
	print_object<T>  print_o(-100);
	for_each(num.begin(), num.end(), print_o);
}
//bubble_sort
int bubble_sort(vector<int> &num)
{
	int count = 0;
	for(int i = 0; i < num.size() ; i++)
	{
		for(int j = 0; j < num.size() - i -1; j++)
		{
				if(num[j] > num[j+1])
				{
					swap_value(num[j] , num[j+1]);
					count ++;
				}
		}
	}
	cout<<"bubble sort "<<endl;
	print_array(num, count);
	return 0;
}
// select_sort
void select_sort(vector<int>&num)
{
	int count = 0;
	for(int i = 0; i < num.size(); i++)
	{
		int povit = i;
		for(int j = i+1; j < num.size(); j++)
		{
			if(num[j] < num[povit])
				povit = j;
		}
		if(povit != i)
		{
			swap_value(num[i] , num[povit]);
			count++;
		}
	}
	cout <<"select sort  "<<endl;
	print_array(num, count);
}
//insert_sort
void insert_sort(vector<int> &num)
{
	int count = 0;
	for(int i = 1;  i < num.size(); i++)
	{
		int temp = num[i];
		int k = i - 1;
		while(k>=0 && num[k] > temp)
		{
			num[k+1] = num[k];
			k--; 
			count ++ ;
		}
		num[k+1] = temp;
	}

	cout<<"insert sort " <<endl;
	print_array(num, count);
}

//quick_sort
//q_sort 的思想？？
/*为什么一定要j指针先动呢？首先这也不是绝对的，这取决于基准数的位置，
因为在最后两个指针相遇的时候，要交换基准数到相遇的位置。一般选取第一个数作为基准数，
那么就是在左边，所以最后相遇的数要和基准数交换，那么相遇的数一定要比基准数小。
所以j指针先移动才能先找到比基准数小的数*/
void quicksort(vector<int>&num, int left, int right, int &count)
{
	if(left > right)
		return;
	srand((unsigned)time(NULL));
	int t = rand()%(right - left + 1) + left;
	swap_value(num[t], num[left]);
	int povit = num[left];
	int i = left;
	int j = right;
	while(i < j)
	{
		while(j > i && num[j] >= povit)  // num[j] equal povit 也无所谓，
			j--;
		while(j> i && num[i] <= povit)
			i++;
		if(i != j)
		{
			swap_value(num[i], num[j]);
			count++;
		}
	}
	swap_value(num[left] , num[j]);
	count++;
	quicksort(num, left, j-1,count);
	quicksort(num, j+1, right, count);
}


void quick_sort(vector<int> &num)
{
	int count = 0;
	int size = num.size();
	quicksort(num, 0, size-1, count);
	cout<<"Quick_sort"<<endl;
	print_array(num, count);
}



int left_child(int i )
{
	return 2*i + 1;
}
int right_child(int i)
{
	return 2*i + 2;
}
int parent(int i)
{
	return (i-1)/2;
}



void heapify(vector<int>&num, int cur,  int limit, int &count)
{
	if(cur >= limit)
		return;
	int max_index = cur;
	int left_index = left_child(cur);
	int right_index = right_child(cur);
	if(left_index < limit && num[left_index] > num[max_index])
		max_index = left_index;
	if(right_index < limit && num[right_index] > num[max_index])
		max_index = right_index;
	if(max_index != cur)
	{
		count++;
		swap_value(num[cur], num[max_index]);
		heapify(num, max_index, limit, count);
	}
		
}

void build_heap(vector<int> &num , int &count)
{
	int len = num.size() - 1;
	for(int i = (len - 1)/2; i >= 0; i--)
	//for(int i = 0; i < (len)/2; i++)
	{
		heapify(num, i , num.size(), count);
	}

}

void heap_sort(vector<int> &num)
{
	int count = 0;
	build_heap(num , count);
	for(int i = num.size() -1 ; i >= 0; i --)
	{
		swap_value(num[0] , num[i]);
		heapify(num, 0 , i , count);
	}
	cout<<"heap_sort"<<endl;
	print_array(num,count);
}


void shell_sort(vector<int>&num)
{
	int count = 0;
	for(int step = num.size()/2; step >0; step = step/2)
	{
		//直接插入排序
		for(int i = step; i < num.size(); i++)
		{
			int k = i - step ;
			int temp = num[i];
			while( k >= 0 && num[k] > temp)
			{
				num[k + step] = num[k];
				k -= step;
				count++;
			}
			num[k+step] = temp;
		}

	}


	cout<<"Shell_sort"<<endl;
	print_array(num, count);
}


void mergesort(vector<int>&num, int left, int right, vector<int>&result, int &count)
{
	if(left >= right)
		return;
	int mid = (left + right)/2;
	mergesort(num, left,  mid, result, count);
	mergesort(num , mid+1, right, result, count);

	int k = left;
	int i = left;
	int j = mid + 1;
	while( i <= mid && j <= right)
	{
		if(num[i] < num[j])
			result[k] = num[i++];
		else
			result[k] = num[j++];
		k++;
		count++;
	}
	while(i <= mid)
		result[k++] = num[i++];
	while(j <= right)
		result[k++] = num[j++];
    //使用完之后一定要copy回去
	for(int i = left; i <= right; i++)
		num[i] = result[i];
}


void merge_sort(vector<int>&num)
{
	int count = 0;
	int len = num.size();
	vector<int>result(len , 0);
	mergesort(num, 0, len-1, result, count);
	cout<<"Merge_sort"<<endl;
	print_array(num, count);
}


typedef struct ListNode{
	
	int value;
	ListNode *next;
	ListNode(int x):value(x), next(NULL){}
}ListNode;
void bucket_sort(vector<int> &num)
{
	ListNode *bucket[LEN];  //指针数组
	//vector<ListNode *> bucket(num.size(), NULL);
	memset(bucket, 0, num.size()*sizeof(ListNode*));
	for(int i = 0; i < num.size(); i++)
	{
		int val = num[i];
		int index = (val * num.size()) / MAX;
		ListNode *node = new ListNode(val);
		if(NULL == bucket[index])
		{
			bucket[index] = node;
		}
		else
		{
			ListNode *cur = bucket[index];
			if( cur->value > val)
			{
				bucket[index] = node;
				node->next = cur;
			}
			else
			{
				ListNode *pre = cur;
				while(cur && cur->value < val)
				{
					pre = cur;
					cur = cur->next;
				}
				node->next = pre->next;
				pre->next = node;
			}
		}
	}
	int j = 0;
	for(int i = 0; i < num.size(); i++)
	{
		if(NULL !=bucket[i])
		{
			ListNode *temp = bucket[i];
			while(temp)
			{
				num[j++] = temp->value;
				temp = temp->next;
			}
		}
	}
	cout<<"Bucket_sort "<<endl;
	print_array(num, 0);

}


void count_sort(vector<int> &num)
{
	int len = LEN*5;
	vector<int>v(len,0);
	for(int i = 0; i < num.size(); i++)
	{
		int index = num[i];
		v[index]++;
	}
	
	int i = 0;
	int j = 0;
	while(i < v.size())
	{
		while(v[i] > 0)
		{
			num[j++] = i;
			v[i]--;
		}
		i++;
	}



	cout<<"Counting_sort"<<endl;
	print_array(num, 0 );
}



void function_sort()
{
	vector<int> num(LEN, 0);
	srand((unsigned)time(NULL));
	for(int i = 0; i <LEN; i++)
		num[i] = rand()%MAX;
	//print_array(num, 0);
	cout<<endl;
	//bubble_sort(num);
	//select_sort(num);
	//insert_sort(num);
	//quick_sort(num);
	//merge_sort(num);
	//shell_sort(num);
	//bucket_sort(num);
	//heap_sort(num);
	//count_sort(num);

	cout<<endl;
}
