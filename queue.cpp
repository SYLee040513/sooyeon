// queue.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>

int main()
{
    
}

#include "queue.h"
#include "qtype.h"
#include <mutex>
#include <cstring>

Queue* init() {
    Queue* q = new Queue();
    q->head = q->tail = nullptr;
    return q;
}

void release(Queue* queue) {
    std::lock_guard<std::mutex> guard(queue->lock);
    Node* cur = queue->head;
    while (cur) {
        Node* next = cur->next;
        delete[] cur->item.value;
        delete cur;
        cur = next;
    }
    delete queue;
}

Node* nalloc(Item item) {
    Node* node = new Node();
    node->item.key = item.key;
    node->item.value_size = item.value_size;
    node->item.value = new uint8_t[item.value_size];
    std::memcpy(node->item.value, item.value, item.value_size);
    node->next = nullptr;
    return node;
}

void nfree(Node* node) {
    delete[] node->item.value;
    delete node;
}

Node* nclone(Node* node) {
    return nalloc(node->item);
}

Reply enqueue(Queue* queue, Item item) {
    std::lock_guard<std::mutex> guard(queue->lock);
    Node* new_node = nalloc(item);

    if (!queue->head || item.key < queue->head->item.key) {
        new_node->next = queue->head;
        queue->head = new_node;
        if (!queue->tail) queue->tail = new_node;
    }
    else {
        Node* cur = queue->head;
        while (cur->next && cur->next->item.key <= item.key) {
            cur = cur->next;
        }
        new_node->next = cur->next;
        cur->next = new_node;
        if (!new_node->next) queue->tail = new_node;
    }

    return { true, new_node->item };
}

Reply dequeue(Queue* queue) {
    std::lock_guard<std::mutex> guard(queue->lock);
    if (!queue->head) return { false, {} };

    Node* node = queue->head;
    queue->head = node->next;
    if (!queue->head) queue->tail = nullptr;

    Item item = node->item;
    node->item.value = nullptr;  
    delete node;
    return { true, item };
}

Queue* range(Queue* queue, Key start, Key end) {
    std::lock_guard<std::mutex> guard(queue->lock);
    Queue* result = init();
    Node* cur = queue->head;

    while (cur) {
        if (cur->item.key >= start && cur->item.key <= end) {
            enqueue(result, cur->item); 
        }
        cur = cur->next;
    }

    return result;
}
