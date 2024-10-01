#ifndef LIBRARY3_H_
#define LIBRARY3_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAXLISTS 3
#define MAXNAME 50

//structure called Node
struct Node {
    char listName[MAXNAME];//variable to hold the list name
    char taskname[MAXNAME];// variable to hold task name
    struct Node *next;//pointer to another Node struct creating linked list
};

//definition of structure list
typedef struct list {
    char nameoflist[MAXNAME];//variable to hold the name of a list
    struct list *next;// pointer to another list struct
    struct Node *start;//pointer to the beginning of the linked list of Node structures
    int numOfLists;
} List;

typedef struct Node ListNode;//synonym for struct Node
typedef ListNode *ListNodePtr;//creation of pointer for ListNode
typedef List *ListPtr;//creation of a pointer to List



void display(ListPtr listNames[MAXNAME], int numOfLists);
void saveboard(ListPtr *listNames);
ListPtr *loadboard();
ListPtr *defaultBoard();
void menu(ListPtr *defaultBoardPtr);
ListPtr *editlist(ListPtr *listNames, int numOfLists);
void editItem(ListPtr *listNames, char listname[], int numOfLists);
void addItem(ListPtr *listNames, char listname[], int numOfLists);
void deleteItem(ListPtr *listNames, char listname[], int numOfLists);
ListPtr *editboard(ListPtr *listNames, int numOfLists);
void editListName(ListPtr *listNames, int numOfLists);
void deleteList(ListPtr *listNames, int *numOfLists);
void addNewList(ListPtr *listNames, int *numOfLists);

//function to create a node
void createNode(ListNodePtr *listPtr, char task[MAXNAME]) {
    *listPtr = (ListNodePtr)malloc(sizeof(ListNode));//dynamically allocate memory for the newnode
    strcpy((*listPtr)->taskname, task);//copies the task into the pointer for next node
    (*listPtr)->next = NULL;//assigns next node to NULL
}

// function to create a new list
void createList(ListPtr *Ptr, char listName[MAXNAME]) {
    *Ptr = (ListPtr)malloc(sizeof(List));//dynamically allocate memory for the new list
    strcpy((*Ptr)->nameoflist, listName);
    (*Ptr)->next = NULL;//points next list to NULL
}

//function to reset an array
void resetArray(char arr[], int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = '\0'; // Set each element to the null character
    }
}

#endif