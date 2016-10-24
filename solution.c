/*
 * This problem asks me to read a text file as input and count the number of
 * occurrences of given HTTP headers.  Headers are found at the start of each 
 * line, prior to a colon in that line.  An additional objective is to devise 
 * an algorithm for CPU computational efficiency; doing so at the expense of 
 * memory is acceptable.
 *
 * To solve this problem, I will make use of an n-ary tree data structure with
 * an integer being stored at each branch and leaf.  The position of each
 * node's branch will correspond with the subsequent character.  To illustrate:
 *
 * Initially, there is a root node.  The initial node has pointers to future
 * branches, but no memory is allocated for the branches yet.
 * 
 *      (root)
 *    /  |  |  \ 
 *   (A)(B)(..)(Z)    <-The parenthesis means there is a pointer that can
 *                     point to a node, but is currently pointing to NULL.
 * 
 * Suppose, as our header string, we received the simple "A".  The first node,
 * corresponding to the character "A", would be initialized in memory and a 
 * value of 1 would be assigned to the character.
 * 
 *      (root)
 *    / |  |  \
 *   A (B)(..)(Z)  <- Words beginning in B through Z have not been seen yet.
 *   V
 *   1 <- "A" observed once.
 *
 * Next, suppose the next two header strings were "ABBA".  That would fill
 * the tree much deeper, and it would look like the following:  
 *
 *           root
 *         / |  |  \
 *        A (B)(..)(Z)
 *        V
 *     /--1--\  <- "A" observed once.
 *    / |  |  \
 *  (A) B (..)(C) <- "AB" observed zero times; node value is zero.
 *    / | \
 *   (A)B (..) <- "ABB" observed zero times; node value is zero. 
 *     / \
 *    A (..)
 *    V
 *    2 <- "ABBA" observed twice.
 *
 * This procedure is repeated until the entire input file is read.  Completing
 * this operation requires each input string to be read once before its
 * correct positioning in the tree can be found and the node, there,
 * incremented. Consequently, this operation is completed in O(N) time, where
 * N corresponds to the number of lines of the input file.
 *
 * Finally, we must find the value at the leaf node of the header files being
 * tracked.  To do this, we simply travel down the tree, as we did before,
 * but we measure the stored value at the leaf rather than incrementing.  This
 * operation is completed in O(K) time, where K is the number of headers that
 * are being tracked.  Assuming that the input sample is much larger than the
 * number of tracked headers (N >> K), the input sample is the dominant strain
 * on computation.  
 * 
 * One shortcoming of using trees in this way is that it is memory-
 * inefficient.  Each node has one leaf pointer for each possible child,
 * even though typically most of those pointers do not point to any 
 * memory in use.  The usage of memory could be improved by using a dynamic
 * array that could grow in size as the leafs are filled.  But I do not think
 * it is possible to use dynamic arrays without reducing CPU performance.
 */

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/* 
 * The node structure is used to create an n-ary tree with a 'count' variable
 * at each node.  This tree is used to keep track of which headers' words are
 * observed and how many times for each observation.  Space is
 * wasted by using 128 pointers per node, corresponding to the char
 * values 0-127.  In principle, only a-z, A-Z, and '-' are used in the
 * testing set, so the memory usage could be reduced to 26+26+1=53
 * (or 26+1 if case sensitivity were dropped).  However, this would
 * require a mapping of each character to a position in an array, which 
 * would slightly slow the program.
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
  int numHeaders = 4;
  char *headers[numHeaders];
  headers[0] = "Connection";
  headers[1] = "Accept";
  headers[2] = "Content-Length";
  headers[3] = "CableModems-testCase";

  // Open input file
  FILE *ptr_file = openFile(argc, argv);
  if(!ptr_file)
    return 1;

  // Read input file, build tree of headers
  struct node* root = readFile(ptr_file);

  // Print the number of occurences of the headers in the tree.
  printResults(root, numHeaders, headers);

  // Close file
  fclose(ptr_file);

  // Free memory from tree
  freeMemory(root);

  // Success
  return 0;
}

/*
 * openFile opens the file supplied at run-time and returns the file's pointer.
 */
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

/* 
 * The readFile function Reads the input text file, line by line, and builds
 * up the tree of word-count.
 */
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

/*
 * The addWordToTree function will find the location on the tree corresponding
 * to the passed word and incriment the count by 1.  If the tree doesn't
 * have branches corresponding to the letters of the word, the tree's missing
 * branches are first built by this function.
 */
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

/* 
 * The printResults function prints the number of occurences of watched
 * headers to stdout.
 */ 
void printResults(struct node* root, int nHeaders, char *headers[]) {
  for(int i = 0; i < nHeaders; i++) {
    printf("'%s' seen %d times.\n", headers[i], getWordcountFromTree(root, headers[i]));
  }
}

/*
 * The getWordcountFromTree function finds the tree's leaf corresponding to
 * the word that was passed and returns the count at that node.  If the tree
 * isn't built out to that leaf, it is due to that word not being observed in
 * a header file, so the count (i.e. 0) is returned.
 */
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

/* 
 * The createNode function allocates memory and initializes a node struct.
 */ 
struct node *createNode() {
  struct node* theNode = (struct node*) malloc(sizeof(struct node));

  // initialize values
  theNode->count = 0;
  for(int i = 0; i < numChars; i++) {
    theNode->nextChar[i] = NULL;
  }

  return theNode;
}

/*
 * The freeMemory function recursively, by depth first, frees memory of the
 * tree.
 */
void freeMemory(struct node* theNode) {
  for(int i = 0; i < numChars; i++) {
    if(theNode->nextChar[i] != NULL) {
      freeMemory(theNode->nextChar[i]);
    }
  }
  free(theNode); 
}
