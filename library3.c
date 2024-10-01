#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "library3.h"
#define MAXNAME 50
#define MAXLISTS 3



//function to display a board 
void display(ListPtr listNames[MAXNAME], int numOfLists) {
    int i = 0;
    while (listNames[i] != '\0') {//loop to go through list names
        ListPtr curList = listNames[i];
        printf("%s:\n", curList->nameoflist);//printing the name of the current list 
        ListNodePtr current = curList->start;//create pointer to point to start of the listnames array 
        while (current) {
            printf("\t%s\n", current->taskname);//displaying the name of the task in the list
            current = current->next;//moves to next node in the list
        }
        i++;
    }
}
void menu(ListPtr *defaultBoardPtr){
 int choice;// user's choice variable
 ListPtr *loadBoardPtr = NULL;
    printf("%s", "\nMenu:\n1.Display Board\n2.Load board from a file\n3.Edit "
               "list\n4.Edit board\n5.Save board to a file\n6.Quit\n");
    printf("Please enter your choice (1-5):\n");//printing of user options
    
    if(scanf("%d", &choice)!= 1){//checking for valid input of user's choice 
    printf("Invalid option.Program Terminating");//default error message 
    exit(0);
    }

    else if (choice == 1) {
        display(defaultBoardPtr, defaultBoardPtr[0]->numOfLists);//call to display 
        menu(defaultBoardPtr);//call to menu
    }
    else if (choice == 2) {
        loadBoardPtr = loadboard();//assigns the pointer for loadboard
	if (loadBoardPtr == NULL){
	    menu (defaultBoardPtr);
	}	else{
        menu(loadBoardPtr);//call to menu 
    }
}
    else if (choice == 3) {
            display(defaultBoardPtr, defaultBoardPtr[0]->numOfLists);// call to display
            editlist(defaultBoardPtr, defaultBoardPtr[0]->numOfLists);//call to editlist 
        }
        
    else if (choice == 4) {
        display(defaultBoardPtr, defaultBoardPtr[0]->numOfLists);//call to display
        editboard(defaultBoardPtr, defaultBoardPtr[0]->numOfLists);//call to editboard 
    }
    else if (choice == 5) {
        saveboard(defaultBoardPtr);//call to saveboard for deault board
        menu(defaultBoardPtr);
        }

    else if (choice == 6) {
        exit(0);//terminates the program 
    } else {
        printf("\nInvalid option. Please try again.\n");//default error message 
        menu(defaultBoardPtr);//call to menu 
    }
}

ListPtr *loadboard() {
    char filename[MAXNAME];//variable to hold the name of the file
    printf("Please enter the name of the file:");//prompt user to enter the name of the file 
    scanf("%s", filename);

    FILE *file_pointer;
    file_pointer = fopen(filename, "r");

    if (file_pointer == NULL) {//check if file can be opened 
        printf("Error opening the file.\n");//error message 
        return NULL;
    }

    ListPtr *loadedBoardPtr = NULL; // create and initialise pointer for loaded board

    char line[MAXNAME];
    int listIndex = 0;

    while (fgets(line, MAXNAME, file_pointer) != NULL) {
        // Removing trailing newline character if present
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        char listName[MAXNAME];
        char data[MAXNAME];

        // Extract list name and data
        sscanf(line, "%s %s", listName, data);

        // Reallocate memory to accommodate for the lists in the file
        // CHANGED to "* (listIndex+1)" from MAXLISTS 
        loadedBoardPtr = (ListPtr *)realloc(loadedBoardPtr, sizeof(ListPtr) * (listIndex+1));
        if (loadedBoardPtr == NULL) {
            printf("Memory allocation failed.\n");
            fclose(file_pointer);
            return NULL;
        }

        createList(&loadedBoardPtr[listIndex], listName);//create a list 
        createNode(&(loadedBoardPtr[listIndex]->start), data);//create nodes for the list

        listIndex++;//increment the list index
    }

    fclose(file_pointer);// close the file 

    // Display the loaded board
    for (int j = 0; j < listIndex; j++) {
        printf("%s:\n", loadedBoardPtr[j]->nameoflist);//print list name 
        ListNodePtr current = loadedBoardPtr[j]->start;
        while (current) {
            printf("\t%s\n", current->taskname);//print each task 
            current = current->next;
        }
    }
    loadedBoardPtr[listIndex] = '\0';
    loadedBoardPtr[0]->numOfLists = listIndex;

    return loadedBoardPtr;// return pointer to the loaded board
}

//function to create default board
ListPtr *defaultBoard() {
    ListPtr *listNames = (ListPtr *)malloc(sizeof(ListPtr) * 3);//dynamically allocate memory for an array of three ListPtr pointers 
    char *Names[] = {"Monday", "Tuesday", "Wednesday"};//array to hold the default names 
    char *tasks1[] = {"Gym", "Work", "Laundry"};//tasks for list 1
    char *tasks2[] = {"Hiking", "Dinner"};//tasks for list 2
    char *tasks3[] = {"Rest"};// tasks for list 3

    for (int i = 0; i < 3; i++) {
        createList(&listNames[i], Names[i]);// creates a list using names stored in the array 
        ListNodePtr curr = NULL;//initilise current pointer to NULL
        ListNodePtr prev = NULL; // initialise previous pointer to NULL
        //loop to iterate through the different tasks will iterate 3,2 or 1 time depending on current list
        for (int j = 0; j < (i == 0 ? 3 : (i == 1 ? 2 : 1)); j++) {
            //create new node with task name and store in curr
            createNode(&curr, (i == 0 ? tasks1[j] : (i == 1 ? tasks2[j] : tasks3[j])));
            //select from either task1,task2 or task3 array
            if (prev == NULL) {
                listNames[i]->start = curr; // If it's the first node, set it as the start
            } else {
                prev->next = curr; // Link the previous node to the current node
            }
            prev = curr; // Update the previous node
        }
    }
    listNames[3] = '\0';
    listNames[0]->numOfLists = 3;

    return listNames;//Returns the array of list pointers containing the default board setup.
}

//function that contains editlist options
ListPtr *editlist(ListPtr *listNames, int numOfLists){
    int choice;//user choice variable
    char listname[MAXNAME];//variable to hold name of list to edit

    printf("\nPlease enter the name of the list to edit (case sensitive):");//prompt user enter the name of the list
    scanf("%s", listname);

    printf("\nOptions:\n1.Edit an item\n2.Add a new item\n3.Delete an item\n4.Return to main menu\n");
    printf("Enter your option:");//printing of options 

    if(scanf("%d", &choice)!= 1){//checking for valid input of user's choice 
    printf("Invalid option.Program Terminating");//default error message 
    exit(0);
    }

    else if (choice == 1) {
        editItem(listNames, listname, numOfLists);//call to edit item
    }
    else if (choice == 2) {
        addItem(listNames, listname, numOfLists);//call to add item
    }
    else if (choice == 3) {
        deleteItem(listNames, listname, numOfLists);//call to delete item 
    }
    else if (choice == 4) {

        menu(listNames);// return to menu 

    }
    else{
    printf("Invalid option.Please try again\n");//default error message 
    editlist(listNames,numOfLists);
    }

      return listNames;// return pointer to the edited list
    
}

void editItem(ListPtr *listNames, char listname[], int numOfLists){
    char itemToEdit[MAXNAME];//variable to hold the name of item to edit
    char newItemName[MAXNAME];//variable to hold the new name of the item 

    int c;
    while ((c = getchar()) != '\n' && c != EOF);

    printf("Enter the name of the item to edit (case sensitive):");
    fgets(itemToEdit, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (itemToEdit[strlen(itemToEdit) - 1] == '\n') {
        itemToEdit[strlen(itemToEdit) - 1] = '\0';
    } 

    printf("Enter the new name for the item:");
    fgets(newItemName, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (newItemName[strlen(newItemName) - 1] == '\n') {
        newItemName[strlen(newItemName) - 1] = '\0';
    } 

    int listIndex = -1;//initialise listIndex to -1
    //loop to iterate all the names of the list and compare to the users input 
    for (int i = 0; i < numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, listname) == 0) {
            listIndex = i;//update listIndex 
            break;
        }
    }

    if (listIndex != -1) {
        //assign current to the start of the linked list 
        ListNodePtr current = listNames[listIndex]->start;
        int found = 0;
        //transverse though each node of linked list 
        while (current != NULL) {
            //comparing current task name to user input to edit
            if (strcmp(current->taskname, itemToEdit) == 0) {
                strcpy(current->taskname, newItemName); // Update item name
                found = 1;
                break;
            }
            current = current->next;//point to next task in the list 
        }

        if (found) {
         display(listNames,numOfLists);//display board
         editlist(listNames,numOfLists);// call to edit list
        } else {
            printf("Item '%s' found in the list",itemToEdit);//error message
             editlist(listNames,numOfLists);//call to editlist
        }
    } else {
        printf("List '%s' not found.\n", listname);//error message 
        editlist(listNames,numOfLists);//call to editlist 
    }
}

void addItem(ListPtr *listNames, char listname[], int numOfLists) {
    char newItemName[MAXNAME]; // variable to hold new name of the item

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    // clearing the input buffer

    printf("Enter the name of the new item:");
    fgets(newItemName, MAXNAME, stdin); // prompt user to enter new name 

    // Removing trailing newline character if present
    if (newItemName[strlen(newItemName) - 1] == '\n') {
        newItemName[strlen(newItemName) - 1] = '\0';
    }

    // initialise listIndex to -1 
    int listIndex = -1;
    // loop to iterate all the names of the list and compare to the users input 
    for (int i = 0; i < numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, listname) == 0) {
            listIndex = i; // update listIndex
            break;
        }
    }

    if (listIndex != -1) {
        // Check if the item already exists in the list
        ListNodePtr current = listNames[listIndex]->start;
        while (current != NULL) {
            if (strcmp(current->taskname, newItemName) == 0) {
                printf("Item '%s' already exists in list '%s'.\n", newItemName, listname);
                editlist(listNames, numOfLists); // call to editlist
                return;
            }
            current = current->next;
        }

        // dynamically allocate memory for the new node
        ListNodePtr newNode = (ListNodePtr)malloc(sizeof(ListNode));
        // check that memory was allocated properly
        if (newNode == NULL) {
            printf("Memory allocation failed.\n");
            return;
        }
        // assign newItemName to new node
        strcpy(newNode->taskname, newItemName);
        newNode->next = listNames[listIndex]->start; // new node points to start
        listNames[listIndex]->start = newNode; // set new node as the start node

        // Display the edited list and allow further editing
        display(listNames, numOfLists);
        editlist(listNames, numOfLists);
    } else {
        printf("List '%s' not found.\n", listname);
        editlist(listNames, numOfLists); // call to editlist 
    }
}


void deleteItem(ListPtr *listNames, char listname[], int numOfLists) {
    char itemToDelete[MAXNAME];//variable to hold the item to delete

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    //clear input buffer


    printf("Enter the name of the item to delete (case sensitive):");//prompt user to enter the name of the item to delete
    fgets(itemToDelete, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (itemToDelete[strlen(itemToDelete) - 1] == '\n') {
        itemToDelete[strlen(itemToDelete) - 1] = '\0';
    }

//initialise listIndex to -1
    int listIndex = -1;
    for (int i = 0; i < numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, listname) == 0) {
            listIndex = i;//update listIndex
            break;
        }
    }


    if (listIndex != -1) {
        ListNodePtr current = listNames[listIndex]->start;//assign current to the start of the linked list
        ListNodePtr prev = NULL;// Pointer to keep track of the previous node
 //traverse through each node of linked list 
        while (current != NULL) {
            if (strcmp(current->taskname, itemToDelete) == 0) {
                // Found the item to delete
                if (prev == NULL) {
         // If the item to delete is the first node
         listNames[listIndex]->start = current->next; // Update the start node to skip over the first node
      } else {
         // If the item to delete is not the first node
         prev->next = current->next;
      }
   free(current); // Free memory for the node to delete
   display(listNames, numOfLists); // Display the edited list and allow further editing
   editlist(listNames, numOfLists); // Call editlist
   }
            prev = current;// assign current to previous node
            current = current->next;
        }

        // Item not found in the list
        printf("Item '%s' not found in list '%s'.\n", itemToDelete, listname);//error message
        editlist(listNames, numOfLists);// call to editlist
    } 
   else {
        printf("List '%s' not found.\n", listname);//error message 
        editlist(listNames, numOfLists);//call to editlist
    }
}

//function that contain editboard options
ListPtr *editboard(ListPtr *listNames, int numOfLists){
    int choice;//variable to hold users choice
    char listname[MAXNAME];// variable to hold list name

    printf("\nOptions:\n1.Edit the name of a list\n2.Add a new list\n3.Delete a list\n4.Return to main menu\n");
    printf("Enter your option:");

    if(scanf("%d", &choice)!= 1){//checking for valid input of user's choice 
    printf("Invalid option.Program Terminating");//default error message 
    exit(0);
    }

    else if (choice == 1){
        editListName(listNames,numOfLists);

    }

    else if (choice == 2){
       addNewList(listNames,&numOfLists);
    }

    else if (choice == 3) {
    deleteList(listNames,&numOfLists) ;
    }

    else if (choice == 4){
        menu(listNames);

    }

    else{
    printf("Invalid option.Please try again\n");//default error message 
    editboard(listNames,numOfLists);
    }
    return listNames;
}


void editListName(ListPtr *listNames, int numOfLists) {
    char oldName[MAXNAME];//variable to hold old name of the list
    char newName[MAXNAME];// variable to hold new name of the list

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    //clearing input buffer 

    printf("Enter the name of the list to edit (case sensitive):");//prompt user to enter the name of the list they want to edit 
    fgets(oldName, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (oldName[strlen(oldName) - 1] == '\n') {
        oldName[strlen(oldName) - 1] = '\0';
    } 

    printf("Enter the new name for the list:");//prompt user to enter the new name of the list 
    fgets(newName, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (newName[strlen(newName) - 1] == '\n') {
        newName[strlen(newName) - 1] = '\0';
    } 

//initialise listIndex to -1 
    int listIndex = -1;
    //loop to iterate all the names of the list and compare to the users input 
    for (int i = 0; i < numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, oldName) == 0) {
            listIndex = i;//update listIndex
            break;
        }
    }

 for (int i = 0; i < numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, newName) == 0) {
            printf("List '%s' already exists. Program Terminating\n", newName);
            exit(0);
            return;
        }
    }

    //if the list was found     
    if (listIndex != -1) {
        strcpy(listNames[listIndex]->nameoflist, newName); // Update list name
        display(listNames, numOfLists);//display board
        editboard(listNames, numOfLists);//call editboard 
    } else {
       printf("List '%s' not found.\n", oldName);
        editboard(listNames, numOfLists);
    }
}

void addNewList(ListPtr *listNames, int *numOfLists) {
    char newListName[MAXNAME];//variable to hold the new list name

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    //clearing the input buffer

    printf("Enter the name of the new list:");//prompt user to enter the name of the new list
    fgets(newListName, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (newListName[strlen(newListName) - 1] == '\n') {
        newListName[strlen(newListName) - 1] = '\0';
    }

//loop through all the lists and compare to users input
    for (int i = 0; i < *numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, newListName) == 0) {
            printf("List '%s' already exists. Program Terminating\n", newListName);
            exit(0);
            return;
        }
    }

    // Dynamically allocate memory for the new list
    ListPtr newList = (ListPtr)malloc(sizeof(List));
    if (newList == NULL) {
        printf("Memory allocation failed.\n");//error message
        return;
    }

    // Copy the name of the new list
    strcpy(newList->nameoflist, newListName);
    newList->start = NULL; // Initialize the start pointer of the new list to NULL
    newList->next = NULL; // Initialize the next pointer of the new list to NULL

    // Resize the listNames array to accommodate the new list
    listNames[0]->numOfLists += 1;
    *numOfLists += 1;
    //reallocating memory for the 
    listNames = (ListPtr *)realloc(listNames, sizeof(ListPtr) * (*numOfLists));
    if (listNames == NULL) {
        printf("Memory allocation failed.\n");
        free(newList); // Free memory for the new list if realloc fails
        return;
    }

    // Add the new list to the listNames array
    listNames[*numOfLists - 1] = newList;
    listNames[*numOfLists] = '\0';

    // Display the updated board and allow further editing
    display(listNames, *numOfLists);
    editboard(listNames, *numOfLists);
}

void deleteList(ListPtr *listNames, int *numOfLists) {
    char listToDelete[MAXNAME];//variable to hold name of the list to delete

    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    //clearing input buffer

    printf("Enter the name of the list to delete (case sensitive):");//prompt user to enter the name of the list to delete
    fgets(listToDelete, MAXNAME, stdin);

    // Removing trailing newline character if present
    if (listToDelete[strlen(listToDelete) - 1] == '\n') {
        listToDelete[strlen(listToDelete) - 1] = '\0';
    }

    //initialise listIndex to -1 
    int listIndex = -1;
    //loop to iterate all the names of the list and compare to the users input 
    for (int i = 0; i < *numOfLists; i++) {
        if (strcmp(listNames[i]->nameoflist, listToDelete) == 0) {
            listIndex = i;//update listIndex
            break;
        }
    }

    if (listIndex != -1) {
        ListNodePtr current = listNames[listIndex]->start;// assign current to the start of the node for the linked list
        while (current != NULL) {
            ListNodePtr temp = current;//assign current node to temporary variable
            current = current->next;//curent points to next node
            free(temp);// free memory for node held in temp
        }

        // Free memory for the list itself
        free(listNames[listIndex]);

        // Shift the remaining lists in the array 
        for (int i = listIndex; i < *numOfLists - 1; i++) {
            listNames[i] = listNames[i + 1];
        }

        (*numOfLists)--; // Decrease the number of lists
        listNames[*numOfLists] = '\0';

        display(listNames, *numOfLists);
        editboard(listNames, *numOfLists); 
    } else {
        printf("List '%s' not found.\n", listToDelete);
        editboard(listNames, *numOfLists); 
    }
}

void saveboard(ListPtr *listNames) {
    FILE *file_pointer;
    // Open the file in write mode
    file_pointer = fopen("new_file.txt", "w");

    // Check if listNames array is NULL
    if (listNames == NULL){
        printf("Board does not exist, create board before loading to file.");
        menu(listNames);//call to menu
    }


    // Check if the file opened successfully
    if (file_pointer == NULL) {
        printf("Error creating the file.\n");
        return;
    }

    
    
    int i = 0;
    // Iterate through the list of lists
    while (listNames[i] != '\0') {
        ListPtr curList = listNames[i];
        fprintf(file_pointer, "%s", curList->nameoflist); // Write list name to the file


        // Iterate through the tasks in the list
        ListNodePtr current = curList->start;
        while (current != NULL) {
            fprintf(file_pointer, "\t%s", current->taskname); // Write task to the file
            current = current->next;
        }
        fprintf(file_pointer, "\n"); // End of list
        i++;
    }

    // Close the file
    fclose(file_pointer);   


    printf("Data written to the file successfully.\n");
    menu(listNames);//call to menu
}





/*                 ___                                ___                       
      ___        /\__\          ___                 /\  \                      
     /\  \      /::|  |        /\  \               /::\  \                     
     \:\  \    /:|:|  |        \:\  \             /:/\:\  \                    
     /::\__\  /:/|:|  |__      /::\__\           /::\~\:\  \                   
  __/:/\/__/ /:/ |:| /\__\  __/:/\/__/          /:/\:\ \:\__\                  
 /\/:/  /    \/__|:|/:/  / /\/:/  /             \/__\:\/:/  /                  
 \::/__/         |:/:/  /  \::/__/                   \::/  /                   
  \:\__\         |::/  /    \:\__\                   /:/  /                    
   \/__/         /:/  /      \/__/                  /:/  /                     
                 \/__/                              \/__/                      
      ___           ___           ___           ___                    ___     
     /\__\         /\  \         /\__\         /\  \                  /\  \    
    /:/  /        /::\  \       /::|  |       /::\  \                /::\  \   
   /:/__/        /:/\:\  \     /:|:|  |      /:/\:\  \              /:/\:\  \  
  /::\__\____   /::\~\:\  \   /:/|:|  |__   /::\~\:\  \            /:/  \:\  \ 
 /:/\:::::\__\ /:/\:\ \:\__\ /:/ |:| /\__\ /:/\:\ \:\__\          /:/__/ \:\__\
 \/_|:|~~|~    \:\~\:\ \/__/ \/__|:|/:/  / \:\~\:\ \/__/          \:\  \  \/__/
    |:|  |      \:\ \:\__\       |:/:/  /   \:\ \:\__\             \:\  \      
    |:|  |       \:\ \/__/       |::/  /     \:\ \/__/              \:\  \     
    |:|  |        \:\__\         /:/  /       \:\__\                 \:\__\    
     \|__|         \/__/         \/__/         \/__/                  \/__/    
*/