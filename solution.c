#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
 * The node structure makes an n-ary tree with each node having a 'count'
 * variable.  This tree is used to keep track of which headers have been
 * observed and how many times each word has been observed.  Space is
 * wasted by using 128 pointers per node, corresponding to the char
 * values 0-127.  In principle, only a-z, A-Z, and '-' are used in the
 * testing set, so the memory usage could be reduced to 26+26+1=53
 * (or 26+1 if case sensitivity were dropped).  However, this would
 * require a character->mapping index that would slightly slow the program.
 */
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
    // Find the position of the first colon in the string
    char *colon = strchr(buf, ':');

    // Terminate the string at the location of the first colon
    buf[colon-buf] = '\0';

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
    printf("'%s' seen %d times.\n", headers[i], getWordcountFromTree(root, headers[i]));
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
