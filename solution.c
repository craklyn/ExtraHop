#include<stdlib.h>
#include<stdio.h>
#include <string.h>

// Struct for doubly-linked list of strings
struct node {
  int count;
  char next['z'-'A'+1];
};

FILE *openFile(int, char*[]);
struct node *readFile(FILE *);
void printResults();
void addWordToTree(char*);

int main(int argc, char* argv[]) {
  // Headers-to-track are chosen at compilation-time
  const int numHeaders = 3;
  const char *headers[numHeaders];
  headers[0] = "Connection";
  headers[1] = "Accept";
  headers[2] = "Content-Length";

  FILE *ptr_file = openFile(argc, argv);
  if(!ptr_file)
    return 1;

  struct node* root = (struct node*) malloc(sizeof(struct node));

  fclose(ptr_file);
  return 0;
}

FILE *openFile(int argc, char* argv[]) {
  const char *fileInput = argv[1];

  if(argc != 2) {
    printf("Please supply one argument for input filename.\n");
    printf("E.g.: ./solution inputFile.txt\n");
    return NULL;
  }

  FILE *ptr_file = fopen(fileInput, "r");
  if (!ptr_file) {
    printf("Error opening input file.\n");
    return NULL;
  }

  return ptr_file;
}


struct node *readFile(FILE *ptr_file) {
  char buf[1000]; // Artibrary length; assumed longer than any header line

  while (fgets(buf,1000, ptr_file)!=NULL) {
    // Find the position of the first colon in the string
    char *colon = strchr(buf, ':');

    // Terminate the string at the location of the first colon
    buf[colon-buf] = '\0';

    addWordToTree(buf);
  }

  return NULL;
}

void addWordToTree(char *buf) {

}

void printResults() {

}

