#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define PRINT_NODE(n, color) \
  printf("  \"%p\" [label=\"%d (%d)\",style=filled,fillcolor=%s];\n", n, n->val, n->weight, color);
#define PRINT_EMPTY_NODE(i, color) \
  printf("  \"%d\" [label=\"NULL\",style=filled,fillcolor=%s];\n", i, color);

typedef struct node node_t;
struct node {
  int val;
  int weight;
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

node_t* insert_node2(node_t* root, node_t *pt) {
  if (root == NULL) return pt;
  if (pt->val < root->val) {
    root->l = insert_node2(root->l, pt);
    root->weight++;
  } else if (pt->val > root->val) {
    root->r = insert_node2(root->r, pt); 
    root->weight++;
  } else {
    free(pt);
  }
  return root;
}

node_t* rotate_right(node_t* n) {
  node_t* p = n;
  if ( n->r ) {
    //n = n->r;
  }
  return n;
}

node_t* rotate_left(node_t* n) {
  node_t* p = n;
  if ( n->l ) {
    //n = n->l;
  }
  return n;
}

node_t* balance_tree(node_t* n) {
  if ( n->l && n->r ) {
    if ( n->l->weight > n->r->weight ) {
      n = rotate_right(n);
    } else if ( n->l->weight < n->r->weight ) {
      n = rotate_left(n);
    }
  } else if ( n->r == NULL && n->l ) {
    if ( n->l->weight > 1 ) {
      n = rotate_right(n);
    }
  } else if ( n->l == NULL && n->r ) {
    if ( n->r->weight > 1 ) {
      n = rotate_left(n);
    }
  }
  return n;
}

node_t* insert_node(node_t* root, node_t *pt) {
  root = insert_node2(root, pt);
  root = balance_tree(root);
  return root;
}

// gcc rbtree.c && ./a.out | dot -Tsvg > rb.svg
int main(void) {
  int vals[] = { 16, 4, 18, 22, 27, 31, 6, 26, 25 };
  //int vals[] = { 1, 2, 3 };
  //int vals[] = { 2, 1, 3 };
  node_t* head = NULL;
  for ( int i = 0; i < ARRAY_SIZE(vals); i++ )
    head = insert_node(head, new_node(vals[i]));
  print_digraph(head);
  return 0;
}
