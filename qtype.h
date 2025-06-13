// ConsoleApplication4.cpp : 이 파일에는 'main' 함수가 포함됩니다. 거기서 프로그램 실행이 시작되고 종료됩니다.
//

#include <iostream>


#include <mutex>
#include <cstring>
int main() {

}

#pragma once

#include <cstdint>

typedef uint32_t Key;
typedef uint8_t* Value;

typedef struct {
    Key key;
    Value value;
    int value_size; // 메모리 복사에 사용됨
} Item;

typedef struct node_t {
    Item item;
    struct node_t* next;
} Node;

#ifdef __cplusplus
#include <mutex>
typedef struct Queue {
    Node* head, * tail;
    std::mutex lock; // C++ 환경에서만 사용
} Queue;
#else
typedef struct {
    Node* head, * tail;
} Queue;
#endif

typedef struct {
    bool success;
    Item item;
} Reply;