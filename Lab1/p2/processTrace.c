/**
  * Author: Fateh Sandhu
  */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

typedef struct node {
    int count;
    unsigned int address;
    struct node *next;
} node;

void printLinkedList(node* head) {
    node* current = head;
    
    while(current->next != NULL) {
        printf("Address: %d\nCount: %d\n", current->address, current->count);
        current = current->next;
    }
}

int main(int argc, char** argv) {

   if (argc != 3) {
    printf("Usage: %s, input file, output file", argv[0]);
  }

  char *inputPath  = argv[1]; // create new string for input
  char *outputPath = argv[2]; // create new string for output

  FILE *input  = fopen(inputPath, "r"); // open file for input
  FILE *output = fopen(outputPath, "w"); // open file for output

  // NULL Pointer check
  if (input == NULL || output == NULL) {
      printf("Error: Invalid Input or Output Path\n");
      exit(1);
  }

  char str[128]; // create string for copying contents from file
  fgets(str, 128, input); // get contents of file line by line
  char addressString[128]; // string to store address
  strncpy(addressString, str+2, 128); // copy string from third index just to get address from string
  unsigned int address = atoi(addressString); // convert string to unsigned int
  
  // create head node to start linked list
  node* head = (node*)malloc(sizeof(node));
  head->address = address;
  head->count = 1;
  head->next = NULL;
  
  // use current node to iterate over list and set it to head to start
  node* current = (node*)malloc(sizeof(node));
  current = head;

  while (fgets(str, 128, input) != NULL) {
      char addressString[128];
      strncpy(addressString, str+2, 120);
      unsigned int address = atoi(addressString);
      
      if (address == current->address) {
          int count = current->count;
          int newCount = count+1; // add to count if address already exists
          current->count = newCount;
      } else if (address != current->address) {
          fprintf(output, "%4d %d\n", current->count, current->address);
          node* newNode = (node*)malloc(sizeof(node));
          newNode->address = address;
          newNode->count = 1;
          newNode->next = NULL;
          current->next = newNode;
          current = newNode;
      }
  }

  fprintf(output, "%4d %d\n", current->count, current->address); // print contents of last node outside loop

    return 0;
}