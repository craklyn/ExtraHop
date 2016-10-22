#include<stdlib.h>
#include<stdio.h>
#include <string.h>

// Struct for tree of characters.  Can't limit next chars to A-Z/a-z since hyphens are possible.
// Not worried about using memory carelessly, so just have it be a 128-ary tree.
const int numChars = 128;
struct node {
  int count;
  struct node *nextChar[numChars];
};

FILE *openFile(int, char*[]);
struct node *readFile(FILE *);
struct node *createNode();
void printResults();
void addWordToTree(struct node* root, char *buf);

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

  struct node* root = readFile(ptr_file);

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

  struct node* root = createNode();

  while (fgets(buf,1000, ptr_file)!=NULL) {
    // Find the position of the first colon in the string
    char *colon = strchr(buf, ':');

    // Terminate the string at the location of the first colon
    buf[colon-buf] = '\0';

    addWordToTree(root, buf);
  }

  return NULL;
}

void addWordToTree(struct node* root, char *buf) {
  struct node* curNode = root;
  int pos = 0;

  while(buf[pos] != '\0') {
    if(curNode->nextChar[buf[pos]] == NULL)
      curNode->nextChar[buf[pos]] = createNode();

    pos += 1;
  }
  
  curNode->count += 1;
}

struct node *createNode() {
  struct node* theNode = (struct node*) malloc(sizeof(struct node));

  // initialize values
  theNode->count = 0;
  for(int i = 0; i < numChars; i++) {
    theNode->nextChar[i] = NULL;
  }

  return theNode;
}

void printResults() {

}

