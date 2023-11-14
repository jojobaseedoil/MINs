#pragma once

#include <iostream>

template<typename T>
class MyStack{
public:
    /* Constructors and Destructor */
    MyStack(unsigned int size):
            mSize(size),
            mCursor(0)
    {
        mData = new T[mSize];
    }
    MyStack() = default;
    MyStack(const MyStack<T>&other){
        mData = nullptr;
        *this = other;
    }
    MyStack<T> &operator=(const MyStack<T>&other){
        if(this == &other)
            return *this;
        delete[] mData;
        mSize = other.mSize;
        mCursor = other.mCursor;
        mData = new T[mSize];
        for(int i=0; i<mSize; i++)
            mData[i] = other.mData[i];
        return *this;
    }
    ~MyStack(){
        delete[] mData;
    }
    void push(T item){
        if(mCursor >= mSize){
            std::cerr << "The maximum stack size has been exceeded.\n";
            exit(EXIT_FAILURE);
        }
        mData[ mCursor++ ] = item;
    }
    void pop(){ mCursor = ( mCursor==0 ? 0 : mCursor-1 ); }
    T top() const{ return ( mCursor==0 ? T() : mData[mCursor-1] ); }
    bool empty() const{ return mCursor==0; }
    void clear(){ mCursor = 0; }

private:
    T *mData;
    int mSize;
    int mCursor;
};