#include <stdlib.h>
#include <stdio.h>
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
void printResults(struct node*, int, char *headers[]);
void addWordToTree(struct node* root, char *word);
int getWordcountFromTree(struct node* root, char *word);
void freeMemory(struct node*);

int main(int argc, char* argv[]) {
  // Headers-to-track are chosen at compilation-time
  int numHeaders = 3;
  char *headers[numHeaders];
  headers[0] = "Connection";
  headers[1] = "Accept";
  headers[2] = "Content-Length";

  FILE *ptr_file = openFile(argc, argv);
  if(!ptr_file)
    return 1;

  struct node* root = readFile(ptr_file);

  printResults(root, numHeaders, headers);

  fclose(ptr_file);
  freeMemory(root);
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
    printf("New line read.  ");

    // Find the position of the first colon in the string
    char *colon = strchr(buf, ':');

    // Terminate the string at the location of the first colon
    buf[colon-buf] = '\0';

    printf("Adding word to tree: %s\n", buf);

    addWordToTree(root, buf);
  }

  return root;
}

void addWordToTree(struct node* root, char *word) {
  struct node* curNode = root;
  int pos = 0;

  while(word[pos] != '\0') {
    if(curNode->nextChar[word[pos]] == NULL)
      curNode->nextChar[word[pos]] = createNode();

    curNode = curNode->nextChar[word[pos]];
    pos += 1;
  }
  
  curNode->count += 1;
}

int getWordcountFromTree(struct node* root, char *word) {
  struct node* curNode = root;
  int pos = 0;

  while(word[pos] != '\0') {
    if(curNode->nextChar[word[pos]] == NULL)
      return 0;

    curNode = curNode->nextChar[word[pos]];
    pos += 1;
  }

  return curNode->count;
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


void printResults(struct node* root, int nHeaders, char *headers[]) {
  for(int i = 0; i < nHeaders; i++) {
    printf("%s was seen %d times.\n", headers[i], getWordcountFromTree(root, headers[i]));
  }
}


void freeMemory(struct node* theNode) {
  for(int i = 0; i < numChars; i++) {
    if(theNode->nextChar[i] != NULL) {
      freeMemory(theNode->nextChar[i]);
    }
  }
  free(theNode); 
}
