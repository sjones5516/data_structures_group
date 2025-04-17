#ifndef PRIORITYQUEUE_H
#define PRIORITYQUEUE_H

#include <vector>

class PriorityQueue {
    private:
        struct Node {
            int priority;
            int value;
        };

        std::vector<Node> heap;
        int parent(int);
        int leftChild(int);
        int rightChild(int);

        void heapifyUp(int);
        void heapifyDown(int);

    public:
        PriorityQueue();
        void add_with_priority(int, int);
        int extract_min();
        bool empty();
};

#endif