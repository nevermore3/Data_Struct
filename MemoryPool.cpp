#include <iostream>
#include <vector>
using  namespace std;

#define POOL_SIZE 16
#define __ALIGN  8

int RoundUp(int size) {
    return ((size + (__ALIGN - 1)) & ~(__ALIGN - 1));
}

class Mem {
public:
    void *address_;
    int size_;
    Mem *next_;

    Mem(void *address, int size) {
        next_ = nullptr;
        size_ = size;
        address_ = address;
    }
};

class MemPool {
public:
    MemPool();

    Mem *GetMem(int size);

    void FreeMem(Mem *address);
private:
    vector<Mem*> pool_;
    Mem *CutMem(int index, int i);

    void Alloc();

    int ConvertToIndex(int size);
};

int MemPool::ConvertToIndex(int size) {
    return (RoundUp(size) / 8) - 1;
}


Mem *MemPool::CutMem(int index, int i) {
    Mem *cur = pool_[i];
    pool_[i] = pool_[i]->next_;

    void *address = cur->address_;
    int size = cur->size_;
    while (index != i) {

        if (index >= i / 2) {
            int newIndex = (size - (index + 1) * __ALIGN) / __ALIGN - 1;
            void *newAddress = (void *)((char*)address + (index + 1) * __ALIGN);
            Mem *link = new Mem(newAddress, (size - (index + 1) * __ALIGN));
            link->next_=pool_[newIndex];
            pool_[newIndex] = link;
            break;

        } else {
            size = size / 2;
            void *newAddress = (void *) ((char*)address + size);

            Mem *link = new Mem(newAddress, size);
            i = i / 2;
            link->next_ = pool_[i];
            pool_[i] = link;
        }

    }
    Mem *mem = new Mem(address, (index + 1) * __ALIGN);
    return mem;

}

void MemPool::Alloc() {
    void *temp = (void *) malloc(POOL_SIZE * __ALIGN);
    if (temp == nullptr) {
        cout<<"Alloc Fail"<<endl;
    }
    Mem *mem = new Mem(temp, POOL_SIZE *__ALIGN);
    pool_[POOL_SIZE - 1] = mem;

}

Mem *MemPool::GetMem(int size) {
    if (size > POOL_SIZE * __ALIGN) {
        cout <<" too large" <<endl;
        return nullptr;
    }
    Mem *result = nullptr;

    // 取整为8的倍数
    int index = ConvertToIndex(size);

    if (pool_[index] == nullptr) {
        // 向下遍历 找到不为空的
        int i = index;
        while (i != POOL_SIZE ) {
            if (pool_[i] != nullptr) {
                break;
            }
            i++;
        }
        if (i == POOL_SIZE) {
            // 重新申请内存并放入pool_中
            Alloc();
            // 从头继续
            result = GetMem(size);
        } else {
            //切分
            result = CutMem(index, i);
        }
    } else {
        Mem *cur = pool_[index];
        pool_[index] = cur->next_;
        result = cur;
        cur->next_ = nullptr;
    }
    return result;
}

MemPool::MemPool(){
    pool_.resize(POOL_SIZE);
}

void MemPool::FreeMem(Mem *address) {
    int index = address->size_ / 8 - 1;
    address->next_ = pool_[index];
    pool_[index] = address;
}

int main() {

    MemPool memPool;
    Mem *a = memPool.GetMem(10);
    Mem *b = memPool.GetMem(40);
    Mem *c = memPool.GetMem(6);
    Mem *d = memPool.GetMem(12);
    Mem *e = memPool.GetMem(8);

    memPool.FreeMem(b);
    memPool.FreeMem(e);

    return 0;
}
