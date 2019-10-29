#ifndef _CHANGE_LIST_H_
#define _CHANGE_LIST_H_

typedef struct node
{
    int changedElementNumber;
    struct node* next;
} node;

typedef struct Lists
{
    node* head;
    node* tail;
} List;

void push(List** list, int newElement);
int pop(List** list);
void print_list(List** list);

#endif 


