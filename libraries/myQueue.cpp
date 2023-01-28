#include "../headers/myQueue.h"

Queue::Queue(int maxSize) {
    size = 0;
    front_ = 0;
    rear = 0;
    this->maxSize = maxSize;
    queue = new int[this->maxSize];
}
Queue::~Queue() {
    delete[] queue;
}
void Queue::inc(int &n) {
    n = (n + maxSize + 1) % maxSize;
}
bool Queue::isEmpty() const {
    return size == 0;
}
bool Queue::isFull() const {
    return size == maxSize;
}
int Queue::front() {
    if (!isEmpty()) {
        return queue[front_];
    }
    else {
        cerr << "Queue is empty" << endl;
    }
}
void Queue::pop() {
    if (!isEmpty()) {
        inc(front_);
        --size;
    }
    else {
        cerr << "Queue is empty" << endl;
    }
}
void Queue::push(int n) {
    if (!isFull()) {
        queue[rear] = n;
        inc(rear);
        ++size;
    }
    else {
        cerr << "Queue is full" << endl;
    }
}
int Queue::getAndPopFront() {
    if (!isEmpty()) {
        int f = front();
        inc(front_);
        --size;
        return f;
    }
    else {
        cerr << "Queue is empty" << endl;
    }
}
int Queue::getSize() const {
    return size;
}
int Queue::getMaxSize() const {
    return maxSize;
}
int* Queue::getQueue() {
    int* arr = new int[size];
    int pos = front_;
    for (int i = 0; i < size; ++i) {
        arr[i] = queue[pos];
        inc(pos);
    }
    return arr;
}

void Queue::printQueue() {
    int* arr = getQueue();
    for (int i = 0; i < size; ++i) {
        cout << arr[i] << " ";
    }
    cout << endl;
}