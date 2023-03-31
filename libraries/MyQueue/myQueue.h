#pragma once
#ifndef TETRIS_MY_QUEUE_H
#define TETRIS_MY_QUEUE_H

#include <array>
#include <iostream>

using namespace std;

class Queue {
private:
    int size, front_, rear, maxSize;
    int* queue;

    void inc(int &n);
public:
    explicit Queue(int maxSize = 1024);
    ~Queue();
    bool isEmpty() const;
    bool isFull() const;
    int front();
    void pop();
    void push(int n);
    int getAndPopFront();
    int getSize() const;
    int getMaxSize() const;
    int* getQueue();

    friend ostream& operator<<(ostream& out, Queue& queue);
};

#endif // TETRIS_MY_QUEUE_H
