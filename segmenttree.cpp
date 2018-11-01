class SegTree
{
public:
	SegTree(vector<int> _data, int(*a)(int i, int j));
	void build(int index, int left, int right);
	void update(int i, int value);
	int query(int left, int right);
private:
	vector<int>data;
	vector<int>tree;
	int(*function)(int i, int j);
	int getleft(int i);
	int getright(int j);
	int _query(int index, int left, int right, int i, int j);
	void _update(int i, int value, int root, int left, int right);
};


int SegTree::getleft(int i)
{
	return 2 * i + 1;
}
int SegTree::getright(int i)
{
	return 2 * i + 2;
}

int max1(int a, int b)
{
	return a > b ? a : b;
}

int sum(int a, int b)
{
	return a + b;
}

int min1(int a, int b)
{
	return a > b ? b : a;
}


SegTree::SegTree(vector<int> _data, int(*a)(int i, int j))
{
	data.swap(_data);
	int len = data.size();
	tree.resize(len * 4, 0);
	function = a;
	build(0, 0, len - 1);
}

void SegTree::build(int root, int left, int right)
{
	if (left == right)
	{
		tree[root] = data[right];
		return;
	}
	int mid = left + (right - left) /2;
	int leftchild = getleft(root);
	int rightchild = getright(root);
	build(leftchild, left, mid);
	build(rightchild, mid + 1, right);
	tree[root] = function(tree[leftchild], tree[rightchild]);
}

int SegTree::_query(int root, int left, int right, int query_left, int query_right)
{
	if (left == query_left && right == query_right) {
		return tree[root];
	}

	int mid = left + ((right - left) >> 1);
	int leftchild = getleft(root);
	int rightchild = getright(root);
	// 在左边
	if (query_right <= mid) {
		return _query(leftchild, left, mid, query_left, query_right);
	} else if (query_left >= mid + 1) {
		return _query(rightchild, mid + 1, right, query_left, query_right);
	} else {
		int l = _query(leftchild, left, mid, query_left, mid);
		int r = _query(rightchild, mid + 1, right, mid + 1, query_right);
		return function(l, r);
	}
}

int SegTree::query(int i, int j)
{
	return _query(0, 0, data.size() - 1, i, j);
}

void SegTree::_update(int i, int value, int root,int left, int right)
{
	if (left == right &&left == i)
	{
		tree[root] = value;
		return;
	}
	int mid = left + ((right - left) >> 1);
	int leftchild = getleft(root);
	int rightchild = getright(root);
	if (mid >= i)
	{
		_update(i, value, leftchild, left, mid);
	}
	else
	{
		_update(i, value, rightchild, mid + 1, right);
	}
	tree[root] = function(tree[leftchild], tree[rightchild]);
}

void SegTree::update(int i, int value)
{
	_update(i, value, 0, 0, data.size() - 1);
	return;
}

void function()
{
	int a[] = { 2,4,1,5,8,3,4,1,2,5 };
	int len = sizeof(a) / sizeof(int);
	vector<int>data(a, a + len);
	SegTree obj(data, sum);
	cout<<obj.query(2, 6)<<endl;
	obj.update(2, 10);
	cout << obj.query(2, 6) << endl;
}
