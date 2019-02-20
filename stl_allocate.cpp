//use  union ， 不浪费空间
typedef union obj {
	union obj *free_list_link;
	char client_data[1];  //client sees this
}obj;

class MyAlloc
{
public:
	MyAlloc();
	~MyAlloc();
	void *allocate(size_t n);
	void deallocate(void *p, size_t n);
private:
	char *chunk_alloc(size_t n, int & nobjs);
	void *refill(size_t n);
	vector<obj*>free_list;
	void *start_free;   //内存池开始
	void *end_free;    //内存池结束
	size_t heap_size;
};



const size_t MAXSIZE = 128; //小型区块的上限
const size_t ALIGN = 8;  //小型区块的上调边界
const size_t LISTNUM = 16; //free-list的个数
size_t FreeList_index(size_t n)
{
	return (n + ALIGN - 1) / ALIGN - 1;
}

size_t ROUND_UP(size_t n)
{
	return (n + ALIGN - 1) & ~(ALIGN - 1);
}

MyAlloc::MyAlloc()
{
	free_list.resize(LISTNUM, nullptr);
	start_free = nullptr;
	end_free = nullptr;
	heap_size = 0;
}


MyAlloc::~MyAlloc()
{
}

void * MyAlloc::allocate(size_t n)
{
	if (n > MAXSIZE)
	{
		cout << "require size:" << n << " is large than 128 byte, not deal" << endl;
		return nullptr;
	}
	//二级配置器
	obj *my_free_list = free_list[FreeList_index(n)];
	obj *result = my_free_list;
	if (result == nullptr)
	{
		void *r = refill(ROUND_UP(n));
		return r;
	}
	my_free_list = result->free_list_link;
	return result;
}

void MyAlloc::deallocate(void * p, size_t n)
{
	if (n > MAXSIZE)
	{
		cout << "free space large than 128 bytes, not deal it" << endl;
		return;
	}
	obj *my_free_list = free_list[FreeList_index(n)];
	((obj*)p)->free_list_link = my_free_list;
	my_free_list = (obj*)p;
}

char * MyAlloc::chunk_alloc(size_t n, int & nobjs)
{
	size_t total = n * nobjs;
	size_t left = (char*)end_free - (char*)start_free;
	if (left >= total)  //剩余空间满足要求
	{
		char *result = (char*)start_free;
		start_free = (void*)((char*)start_free + total);
		return result;
	}
	else if (left >= n) //满足一个或者以上区块的大小
	{
		nobjs = left / n;
		char* result = (char*)start_free;
		start_free = (void*)((char*)start_free + nobjs * n);
		return result;
	} //一个区块大小也不满足，则使用malloc重新申请内存
	else
	{
		size_t newtotal = 2 * total;
		if (left > 0)  //将剩余的链入free_list中先
		{
			obj* my_free_list = free_list[FreeList_index(left)];
			((obj*)start_free)->free_list_link = my_free_list;
			my_free_list = (obj*)start_free;
		}
		start_free = malloc(newtotal);
		if (start_free == nullptr)
		{
			cout << "malloc  fail, deal it" << endl;
			//deal it
			//从现有的free_list上搜刮比 size大小n还要大的，是否有空闲
			for (size_t i = n; i < MAXSIZE; i += ALIGN)
			{
				obj* my_free_list = free_list[FreeList_index(i)];
				if (my_free_list != nullptr)
				{
					obj *temp = my_free_list;
					my_free_list = my_free_list->free_list_link;
					start_free = (void*)temp;
					end_free = (void*)((char*)start_free + i);
					return chunk_alloc(n, nobjs);  //主要为了更新nobjs
				}
			}
			//没有一丁点空间了，gg思密达
			end_free = nullptr;
			cout << "gg 思密达" << endl;
			//抛出异常
			assert(0);
		}
		heap_size = newtotal;
		end_free = (void*)((char*)start_free + newtotal);
		return chunk_alloc(n, nobjs);
	}
}

void * MyAlloc::refill(size_t n)
{
	int nobjs = 20; //预计每次取得20个新节点(区块)，每个区块的size是n
	char *chunk = chunk_alloc(n, nobjs);
	if (nobjs == 1) // 如果内存池里只有一个节点，则直接返回给用户
	{
		cout << "only 1 nobjs" << endl;
		return (void*)chunk;
	}
	free_list[FreeList_index(n)] = (obj*)(chunk + n);
	obj* my_free_list = free_list[FreeList_index(n)];
	//申请好的多余的内存块链接到free_list中
	obj* next = my_free_list;
	obj* current = nullptr;
	for (int i = 1; ; i++)
	{
		current = next;
		next = (obj*)(next + n);
		if (i == nobjs - 1)
		{
			current->free_list_link = nullptr;
			break;
		}
		else
		{
			current->free_list_link = next;
		}
	}
	return  (void*)chunk;
}
