/**
 * RB-Tree C implementation.
 * gcc rbtree.c && ./a.out > rb.dot && cat rb.dot | dot -Tpng > rb.png
 */

#include <stdio.h>
#include <stdlib.h>

#define ARRAY_SIZE(x) (sizeof(x)/sizeof(x[0]))
#define PRINT_NODE(n, color) \
  printf("  \"%d\" [label=\"%d\",xlabel=\"w:%d\",style=filled,fillcolor=%s];\n", n->uid, n->val, n->weight, color);
#define PRINT_EMPTY_NODE(i, color) \
  printf("   \"%d\" [label=\"\",shape=circle,style=filled,width=0.2,height=0.2,fillcolor=black,fontcolor=white];\n", i);
#define __LOG(args...) fprintf(stderr, ##args)
#define LOG(fmt, args...) __LOG("%s:%d " fmt "\n", __func__, __LINE__, ##args)

typedef struct node node_t;
struct node {
  int val;
  int weight;
  int uid;
  node_t* l;
  node_t* r;
};

node_t* balance_tree(node_t* n);

int node_uid = 1;
node_t* new_node(int i) {
  node_t* n = calloc(1, sizeof(node_t));
  n->val = i;
  n->weight = 1;
  n->uid = node_uid++;
  LOG("val=%d, uid=%d", n->val, n->uid);
  return n;
}

void print_edge(node_t* a, node_t* b, char* color) {
  if ( a == NULL ) return;
  if ( b == NULL ) {
    int id = node_uid++;
    PRINT_EMPTY_NODE(id, color);
    printf("   \"%d\" -> \"%d\";\n", a->uid, id);
  } else {
    PRINT_NODE(b, color);
    printf("  \"%d\" -> \"%d\" [penwidth=4];\n", a->uid, b->uid);
    print_edge(b, b->l, "red");
    print_edge(b, b->r, "grey");
  }
}

void print_digraph(node_t* head) {
  printf(
    "digraph {\n"
    "  edge [\n"
    "    arrowhead=\"none\"\n"
    "  ];\n"
  
  );
  PRINT_NODE(head, "white");  
  print_edge(head, head->l, "red");
  print_edge(head, head->r, "grey");
  printf("}\n");
}

node_t* insert_node2(node_t* root, node_t *pt) {
  LOG("root: %p, pt: %p", root, pt);
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

int set_weight(node_t* n) {
  if ( n == NULL ) return 0;
  int weight = 1;
  if ( n->l ) weight += set_weight(n->l);
  if ( n->r ) weight += set_weight(n->r);
  n->weight = weight;
  return weight;
}

node_t* rotate_right(node_t* a) {
  LOG("rotate right");
  if ( !a->r ) return a;

  node_t* b  = a->l;
  node_t* r = b->r;
  a->l = NULL;
  b->r = a;

  if (r) {
    insert_node2(b, r);
  }

  set_weight(b);

  return b;
}

node_t* rotate_left(node_t* a) {
  LOG("rotate left");
  if ( !a->r ) return a;

  node_t* b  = a->r;
  node_t* l = b->l;
  a->r = NULL;
  b->l = a;

  if (l) {
    insert_node2(b, l);
  }

  set_weight(b);

  return b;
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
  //int vals[] = { 2, 1, 3, 5, 4, 6, 9, 7, 8 };
  node_t* head = NULL;
  int size = ARRAY_SIZE(vals);
  for ( int i = 0; i < size; i++ )
    head = insert_node(head, new_node(vals[i]));
  print_digraph(head);
  LOG("Done with graph with %d nodes", size);
  return 0;
}
