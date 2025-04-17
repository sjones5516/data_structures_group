#include "PriorityQueue.h"

PriorityQueue::PriorityQueue() {
  
}

int PriorityQueue::parent(int i) {
  return (i - 1) / 2;
}

int PriorityQueue::leftChild(int i) {
  return 2 * i + 1;
}

int PriorityQueue::rightChild(int i) {
  return 2 * i + 2;
}

void PriorityQueue::heapifyUp(int i) {
  while (i > 0) {
    int p = parent(i);

    if (heap[p].priority <= heap[i].priority) {
      break;
    } else {
      std::swap(heap[p], heap[i]);
      i = p;
    }
  }
}

void PriorityQueue::heapifyDown(int i) {
  int n = heap.size();
  int least = i;

  while (true) {
    int l = leftChild(i);
    int r = rightChild(i);

    if (l < n && heap[l].priority < heap[least].priority) {
      least = l;
    }

    if (r < n && heap[r].priority < heap[least].priority) {
      least = r;
    }

    if (least == i) {
      break;
    }

    std::swap(heap[i], heap[least]);
    i = least;
  }
}

void PriorityQueue::add_with_priority(int value, int priority) {
  Node n;
  n.value = value;
  n.priority = priority;
  heap.push_back(n);
  heapifyUp(heap.size() - 1);
}

int PriorityQueue::extract_min() {
  int min = heap[0].value;
  heap[0] = heap.back();
  heap.pop_back();

  if (!heap.empty()) {
    heapifyDown(0);
  }

  return min;
}

bool PriorityQueue::empty() {
  return heap.empty();
}