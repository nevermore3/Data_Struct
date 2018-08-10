typedef  enum
{
	FILETYPE,
	DIRTYPE
}TYPE;


typedef struct Node
{
	TYPE type;
	string name;
	struct Node *firstchild;     //point to child
	struct Node *nextsibling; //point to sibling
	struct Node *pre;
	Node(TYPE _type,  string _name)
	{
		type = _type;
		name = _name;
		firstchild = NULL;
		nextsibling = NULL;
		pre = NULL;
	}
}Node;

Node*  is_dir_exist(const char*dirname)
{
	string target = dirname;
	queue<Node*>q;
	q.push(root);
	while(!q.empty())
	{
		Node *temp = q.front();
		q.pop();
		if(temp->name == target)
		{
			return temp;
		}
		Node *child = temp->firstchild;
		if(child != NULL)
		{
			q.push(child);
			Node *sibling = child->nextsibling;
			while(sibling != NULL)
			{
				q.push(sibling);
				sibling = sibling->nextsibling;
			}
		}
	}
	return NULL;
}


int CreateDir(const char * ParentDirName, const char * DirName)
{
	Node *pdir =  is_dir_exist(ParentDirName);

	if(NULL == pdir || NULL !=  is_dir_exist(DirName) )
	{
		return -1;
	}
	else
	{
		Node *insert_node = new Node(DIRTYPE, DirName);
		Node  *fchild = pdir->firstchild;
		if(fchild  == NULL)
		{
			pdir->firstchild = insert_node;
			insert_node->pre = pdir;
		}
		else
		{
			Node *slow = fchild;
			Node *fast = fchild->nextsibling;
			while(fast != NULL)
			{
				fast = fast->nextsibling;
				slow = slow->nextsibling;
			}
			slow->nextsibling = insert_node;
			insert_node->pre = slow;
		}
		return 0;
	}
}

void DeleteDir(const char * DirName)
{
	Node *t = is_dir_exist(DirName);
	Node *pre = t->pre;
	if(pre->nextsibling == t)
	{
		pre->nextsibling =t->nextsibling;
	}
	else
	{
		pre->firstchild = t->nextsibling;
	}
    if(t->nextsibling != NULL)
			t->nextsibling->pre = pre;
	t->pre = NULL;
    return;
}

bool is_contain(const char * SrcDirName, const char * DestDirName)
{
	string target = DestDirName;
	Node *p = is_dir_exist(SrcDirName);
	queue<Node*>q;
	q.push(p);
	while(!q.empty())
	{
		Node *temp = q.front();
		q.pop();
		if(temp->name == target)
		{
			return true;
		}
		Node *child = temp->firstchild;
		if(child != NULL)
		{
			q.push(child);
			Node *sibling = child->nextsibling;
			while(sibling != NULL)
			{
				q.push(sibling);
				sibling = sibling->nextsibling;
			}
		}
	}
	return false;
}

int MoveDir(const char * SrcDirName, const char * DestDirName)
{
	if( strcmp( SrcDirName, DestDirName) == 0 || NULL ==  is_dir_exist(SrcDirName) || NULL == is_dir_exist(DestDirName) || is_contain(SrcDirName,DestDirName ) ||  is_contain(DestDirName,SrcDirName ))
	{
		return -1;
	}
	else
	{
		Node *src= is_dir_exist(SrcDirName);
		if(src->pre->nextsibling == src)
		{
			src->pre->nextsibling = src->nextsibling;
		}
		else
		{
			src->pre->firstchild =src->nextsibling;
		}
		if(src->nextsibling != NULL)
				src->nextsibling->pre = src->pre;

		src->nextsibling = NULL;
		src->pre = NULL;
		Node *des =  is_dir_exist(DestDirName);
		Node *fchild = des->firstchild;
		if(fchild  == NULL)
		{
			des->firstchild = src;
			src->pre = des;
		}
		else
		{
			Node *slow = fchild;
			Node *fast = fchild->nextsibling;
			while(fast != NULL)
			{
				fast = fast->nextsibling;
				slow = slow->nextsibling;
			}
			slow->nextsibling = src;
			src->pre = slow;
		}
		return 0;
	}
}

int CreateFile(const char * DirName, const char * FileName)
{
	if(NULL == is_dir_exist(DirName) || NULL != is_dir_exist(FileName) )
	{
		return -1;
	}
	else
	{
		Node *pdir =  is_dir_exist(DirName);
		Node *insert_node = new Node(FILETYPE, FileName);
		Node  *fchild = pdir->firstchild;
		if(fchild  == NULL)
		{
			pdir->firstchild = insert_node;
			insert_node->pre = pdir;
		}
		else
		{
			Node *slow = fchild;
			Node *fast = fchild->nextsibling;
			while(fast != NULL)
			{
				fast = fast->nextsibling;
				slow = slow->nextsibling;
			}
			slow->nextsibling = insert_node;
			insert_node->pre = slow;
		}

		return 0;
	}
}

void DeleteFile(const char * FileName)
{
	Node *t = is_dir_exist(FileName);
	Node *pre = t->pre;
	if(pre->nextsibling == t)
	{
		pre->nextsibling = t->nextsibling;
	}
	else
	{
		pre->firstchild = t->nextsibling;
	}
	if(t->nextsibling != NULL)
		t->nextsibling->pre = pre;
	t->pre = NULL;
    return;
}

unsigned int GetFileNum(const char * DirName)
{
	int Count = 0;
	Node *now = is_dir_exist(DirName);
	if(NULL == now)
	{
		return 0;
	}
	queue<Node*>q;
	q.push(now);
	while(!q.empty())
	{
		Node *temp = q.front();
		q.pop();
		if(temp->type == FILETYPE)
		{
			Count++;
		}
		Node *child = temp->firstchild;
		if(child != NULL)
		{
			q.push(child);
			Node *sibling = child->nextsibling;
			while(sibling != NULL)
			{
				q.push(sibling);
				sibling = sibling->nextsibling;
			}
		}
	}
    return Count;
}

void Clear(void)
{
	root->nextsibling = NULL;
	root->firstchild = NULL;
    return;
}
