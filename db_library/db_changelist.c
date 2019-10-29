#include <stdio.h>
#include <stdlib.h>
 
#include "./db_changelist.h"

void push(List** list, int newElement) 
{ 
    struct node* new_node = (struct node*)malloc(sizeof(struct node)); 
    new_node->changedElementNumber = newElement; 
    new_node->next=NULL;

    new_node->next = (*list)->head;  
    (*list)->head = new_node; 
    
    while(new_node->next!=NULL){
        new_node=new_node->next;
    }
    (*list)->tail=new_node;
            
}

int pop(List** list)
{
    if((*list) == NULL){
        printf("No solution available, exiting program.\n");
        exit(-1);
    }
    else if((*list)->head==(*list)->tail)
    {
        int pop_element=(*list)->tail->changedElementNumber;
        free((*list)->head);
        (*list)->head=(*list)->tail=NULL;
        return pop_element;
    }
    else
    {
        struct node* new_tail=NULL;
        int pop_element=(*list)->tail->changedElementNumber;
        for (new_tail = (*list)->head; new_tail->next != (*list)->tail; new_tail = new_tail->next);
        new_tail->next = NULL;
        free((*list)->tail);
        (*list)->tail = new_tail;
        return pop_element;
    }
}
     
void print_list(List** list)
{
    node* list_element = (*list)->head; 
    while(list_element != NULL) {
        printf("%d ", list_element ->changedElementNumber);
        list_element = list_element->next;
    }
    printf("\n");
}


