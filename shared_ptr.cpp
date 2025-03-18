#include<iostream>

//指针管理计数器
template<typename T>
class sharecount
{
    T *ptr;     //指向管理的的对象
    int count;  //计数器，当前引用次数

    sharecount(const sharecount&) = delete;                 //禁止拷贝
    sharecount& operator= (const sharecount&) = delete;     //禁止赋值

public:
    sharecount(T *p) : ptr(p),count(1){}            //构造函数
    ~sharecount(){delete ptr;}

    // 增加引用计数
    void increas(){count++;}
    // 减少引用计数
    void decreas(){
        if(--count==0)
        {
            delete this;
        }
    }
    //得到指向对象的原始指针
    T* get() const{
        return ptr;
    }
};

//对象管理器
template<typename T>
class share_ptr
{
private:
    T* ptr;                     //指向管理对象
    sharecount<T>* count_ptr;   //指向管理对象的指针
public:
    share_ptr(T *p = nullptr) :ptr(p)   
    //构造函数
    {
        if(p)
        {
            count_ptr = new sharecount<T>(p);
        }
        else{
            p=nullptr;
        }
    }

    //拷贝构造
    share_ptr(const share_ptr& other): ptr(other.ptr),count_ptr(other.count_ptr)
    {
        if(count_ptr)
        {
            count_ptr->increas();
        }
    }
    //移动构造函数，使用之后原来的为空，现在的为原来的
    share_ptr(share_ptr && other): ptr(other.ptr),count_ptr(other.count_ptr)
    {
        other.ptr=nullptr;
        other.count_ptr=nullptr;
    }
    //析构函数
    ~share_ptr()
    {
        if(count_ptr)
        {
            count_ptr->decreas();
        }
    }

    //提供智能指针访问对象地址功能
    T* operator->()const
    {
        return ptr;
    }
    //解引用
    T& operator*() const
    {
        return *ptr;
    }


    //reset 释放当前资源并管理一个新的资源
    void reset(T* p = nullptr)
    {
        if(p != ptr)
        {
            if(count_ptr)
            {
                count_ptr->decreas();
            }
        }
        ptr = p;
        if(p)
        {
            count_ptr = new sharecount<T>(p);
        }
        else{
            count_ptr = nullptr;
        }
    }

    //获取原始指针
    T* get()const{
        return ptr;
    }

};



using namespace std;
int main()
{
    share_ptr<int> ptr1(new int(10));
    share_ptr<int> ptr2 = ptr1;
    
    std::cout <<"ptr1 :"<< *ptr1 <<std::endl; 
    std::cout <<"ptr2 :"<< *ptr2 <<std::endl; 
    
    ptr1.reset();
   // std::cout <<"ptr :"<< *ptr1 <<std::endl; 
    std::cout <<"ptr2 :"<< *ptr2 <<std::endl; 
    //将ptr2转存给ptr3，ptr2没了 
    share_ptr<int> ptr3 =std::move(ptr2);
    std::cout <<"ptr3 :"<< * ptr3.get() <<std::endl; 
}