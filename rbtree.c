#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define PRINT_NODE(n, color) \
  printf("  \"%p\" [label=\"%d\",style=filled,fillcolor=%s];\n", n, n->val, color);
#define PRINT_EMPTY_NODE(i, color) \
  printf("  \"%d\" [label=\"NULL\",style=filled,fillcolor=%s];\n", i, color);

typedef struct node node_t;
struct node {
  int val;
  node_t* l;
  node_t* r;
};

node_t* new_node(int i) {
  node_t* n = calloc(1, sizeof(node_t));
  n->val = i;
  return n;
}

int empty_node_id = 0;
void print_edge(node_t* a, node_t* b, char* color) {
  if ( !a ) return;
  if ( !b ) {
    int id = empty_node_id++;
    PRINT_EMPTY_NODE(id, color);
    printf("  \"%p\" -> \"%d\";\n", a, id);
  } else {
    PRINT_NODE(b, color);
    printf("  \"%p\" -> \"%p\" [penwidth=4];\n", a, b);
    print_edge(b, b->l, "red");
    print_edge(b, b->r, "grey");
  }
}



void print_digraph(node_t* head) {
  printf("digraph {\n");
  PRINT_NODE(head, "white");  
  print_edge(head, head->l, "red");
  print_edge(head, head->r, "grey");
  printf("}\n");
}

node_t* insert_node(node_t* root, node_t *pt) {
  if (root == NULL) return pt;
  if (pt->val < root->val) {
    root->l = insert_node(root->l, pt);
  } else if (pt->val > root->val) {
    root->r = insert_node(root->r, pt); 
  } else {
    free(pt);
  }
  return root;
}

// gcc rbtree.c && ./a.out | dot -Tsvg > rb.svg
int main(void) {
  int vals[] = { 16, 4, 18, 22, 27, 31, 6, 26, 25 };
  node_t* head = new_node(15);
  for ( int i = 0; i < ARRAY_SIZE(vals); i++ )
    head = insert_node(head, new_node(vals[i]));
  print_digraph(head);
  return 0;
}
