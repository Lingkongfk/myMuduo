#pragma once


//作为基类，可以迅速的让自己写的类的拷贝和赋值delete
//派生类对象可以正常构造和析构，但是无法进行拷贝和赋值操作
class noncopyable
{
public:
    noncopyable(const noncopyable&) = delete;
    noncopyable& operator=(const noncopyable&) = delete;
protected:
    noncopyable() = default; 
    ~noncopyable() = default;
};

