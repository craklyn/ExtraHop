#include<stdlib.h>
#include<stdio.h>
#include <string.h>

// Struct for doubly-linked list of strings
struct node {
  char *word;
  struct node *prev;
  struct node *next;
};

FILE *openFile(int, char*[]);
struct node *readFile(FILE *);
void printResults();

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

  struct node* headersList = (struct node*) malloc(sizeof(struct node)); 
  headersList = readFile(ptr_file);

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
  char buf[1000];  // A long string, with length chosen arbitrarily.

  while (fgets(buf,1000, ptr_file)!=NULL) {
    char *colon = strchr(buf, ':');
    printf("%ld - ", (colon-buf));

    printf("%s",buf);
  }

//  quicksortX();
  return NULL;
}


void printResults() {

}

/*
private static void quicksortX(char *a[], int lo, int hi, int d) {
  if (hi - lo <= 0) return;
  int i = lo-1, j = hi;
  int p = lo-1, q = hi;
  char v = a[hi].charAt(d);
  while (i < j) {
    while (a[++i].charAt(d) < v) if (i == hi) break;
    while (v < a[--j].charAt(d)) if (j == lo) break;
    if (i > j) break;
    exch(a, i, j);
    if (a[i].charAt(d) == v) exch(a, ++p, i);
    if (a[j].charAt(d) == v) exch(a, j, --q);
  }
  if (p == q) {
    if (v != '\0') quicksortX(a, lo, hi, d+1);
    return;
  }
  if (a[i].charAt(d) < v) i++;
  for (int k = lo; k <= p; k++) exch(a, k, j--);
  for (int k = hi; k >= q; k--) exch(a, k, i++);
  quicksortX(a, lo, j, d);
  if ((i == hi) && (a[i].charAt(d) == v)) i++;
  if (v != '\0') quicksortX(a, j+1, i-1, d+1);
  quicksortX(a, i, hi, d);
}
*/
