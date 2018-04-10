typedef struct Node
{
	int value;
	Node* next;
	Node(int _value):value(_value),next(NULL){}
}Node;

class List
{
public:
	List():Head(NULL){}
	int create_list(int *array, int size);	
	void print_list();
	void sort_list_bubble_value();
	void reverse_list();
	void sort_list_select();
	Node* merge_sort(Node *node);
	Node* get_head();
	void set_head(Node *node);
	~List();
private:
	Node* merge_(Node*, Node*);
	Node *Head;
	int insert_node(int value);
	int insert_node_reverse(int value);

};

int List::create_list(int *array, int size)
{
	if(NULL == array || size <= 0)
	{
		cout<<"array is NULL or size le zero "<<endl;
		return -1;
	}
	for(int i = 0; i < size; i++)
	{
		if(-1 == insert_node(array[i]))
		{
			cout<<"insert_node error !!!!"<<endl;
			return -1;
		}
	}
	cout<<"create_list  success !!!"<<endl;
	return 0;
}

int List::insert_node_reverse(int value)
{
	Node *temp = new Node(value);
	if(NULL == temp)
	{
		cout<<"malloc  fail !!!"<<endl;
		return -1;
	}
	if(Head == NULL)
	{
		Head = temp;
	}
	else
	{
		temp->next = Head;
		Head = temp;
	}
	return 0;
}

int List::insert_node(int value)
{
	Node *temp = new Node(value);
	if(NULL == temp)
	{
		cout<<"malloc  fail !!!"<<endl;
		return -1;
	}

	if(Head == NULL)
	{
		Head = temp;
	}
	else
	{
		Node *curr = Head;
		while(curr->next)
		{
			curr = curr->next;
		}
		curr->next = temp;
	}
	return 0;
}

void List::print_list()
{
	if(!Head)
	{
		cout<<"List not exist"<<endl;
		return;
	}
	else
	{
		Node *curr = Head;
		cout<<"The List is :   "<<endl;
		while(curr)
		{
			if(curr->next )
				cout<<curr->value<<"-->";
			else
				cout<<curr->value;
			curr = curr->next;
		}
		cout<<endl;
	}
}

void List::reverse_list()
{
	if(NULL == Head || NULL == Head->next)
		return;
	else
	{
		Node *pre = Head;
		Node *curr = Head->next;
		Head->next = NULL;
		while(curr)
		{
			Node *temp = curr->next;
			curr ->next = pre;
			pre = curr;
			curr = temp;
		}
		Head = pre;
	}
	print_list();
}

void swap(Node &node1, Node &node2)
{
	int temp = node1.value;
	node1.value = node2.value;
	node2.value = temp;
}


void List::sort_list_bubble_value()
{
	if(Head == NULL || Head->next == NULL)
	{
		cout<<"list 's size is equal one or zero "<<endl;
		return;
	}
	else
	{
		Node* index = Head;
		while (index != NULL)
		{
			Node *curr = index;
			while(curr != NULL && curr->next != NULL)
			{
				if(curr->value < curr->next->value)
				{
					swap(*curr, *(curr->next));
				}
				curr = curr->next;
			}
			//因为list只能是向右访问的，导致最右值只能冒向右侧，所以将头结点和尾结点互换，使得最值冒向左侧。
			swap(*index, *curr);
			index = index->next;
		}
	}
	print_list();
}


void List::sort_list_select()
{
	if(Head == NULL || Head->next == NULL)
	{
		cout<<"list 's size is equal one or zero "<<endl;
		return;
	}
	else
	{
		Node *selectd = Head;
		Node *index = Head;
		while(index)
		{
			Node *curr = index->next;
			int temp = index->value;
			bool flag = false;
			while(curr != NULL)
			{
				if(curr->value < temp)
				{
					selectd = curr;
					temp = curr->value;
					flag = true;
				}
				curr = curr->next;
			}
			if(flag)
			{
				swap(*index, *selectd);
			}
			index = index->next;
		}
		print_list();
	}
}

Node *List::get_head()
{
	return Head;
}


Node* List::merge_sort(Node *node)
{
	if(node == NULL || node->next == NULL)
	{
		cout<<"list 's size is equal one or zero "<<endl;
		return node;
	}
	else
	{
		Node *fast = node;
		Node *slow = node;
		// find the middle node of the list
		while(fast->next && fast->next->next)
		{
			fast = fast->next->next;
			slow = slow->next;
		}
		
		Node *temp = slow->next;
		//把前面从中间结点切断
		slow->next = NULL;
		Node *left = merge_sort(node);
		Node *right = merge_sort(temp);
		Node *result = merge_(left, right);
		return result;
	}
}
Node* List::merge_(Node *left, Node* right)
{
	Node *result = new Node(0);
	Node *curr = result;
	while(left && right)
	{
		if(left->value < right->value)
		{
			result->next = left;
			result = left;
			left = left->next;
		}
		else
		{
			result->next = right;
			result = right;
			right = right->next;
		}
	}
	if(left)
	{
		result->next = left;
	}
	if(right)
	{
		result->next = right;
	}
	return curr->next;
}
void List::set_head(Node *node)
{
	Head = node;
}
List::~List()
{
	if(Head)
	{
		Node *curr = Head;
		Node *pre = Head;
		while(curr)
		{
			pre = curr;
			curr = curr->next;
			free(pre);
		}
	}
}
