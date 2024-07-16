#ifndef N_ARY_TREE_H
#define N_ARY_TREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum {node, leaf} ttype;
typedef struct tree tree;

struct tree {
  tree *left, *right;
  int id;
  ttype type;
};

#define __t(t) \
  ({ if (t) \
      printf("(%p)\nid (%d)\ntype (%d)\nleft (%p)\nright (%p)\n", \
              t, t->id, t->type, t->left, t->right); \
    else printf("%p\n", t); })
#define __tid(t) \
  ({ if (t) printf("id (%d)\n", t->id); \
    else printf("%p\n", t); })
#define __ts(t) \
  ({ printf("("); t_preorder(t); printf("\b)\n"); })

/* tree *t_dfs(tree *t); */
tree *t_fork(int id, tree *left, tree *right);
tree *t_fork_order(int order[], int n);
tree *__t_fork_order(int order[], int start, int end);

tree *t_insert(tree *t, int id);
tree *t_find(tree *t, int id);
tree *t_find_max(tree *t);
tree *t_find_min(tree *t);
tree *t_del(tree *t, int id);

tree *t_balance(tree *t);
tree *t_rotate_right(tree *t);
tree *t_rotate_right_1(tree *t);
tree *t_rotate_right_2(tree *t);
tree *t_rotate_left(tree *t);
tree *t_rotate_left_1(tree *t);
tree *t_rotate_left_2(tree *t);

void t_preorder(tree *t);
void t_clean(tree *t);
size_t t_stack(tree *t);
size_t t_size(tree *t);

void t_print(tree *t);
void t_print_stack(tree *t, char *pretab, size_t stack);
void t_print_tab(char *pretab, const char *currtab, size_t stack);
void t_stack_tab(char *tab, char *pretab, const char *currtab, size_t stack);

size_t t_stack_n = 0;

tree *t_fork(int id, tree *left, tree *right) {
  tree *t = malloc(sizeof (tree));
  ttype type = leaf;
  
  t->left = left;
  t->right = right;
  
  if (t->left != null || t->right != null)
    type = node;
  
  t->type = type;
  t->id = id;

  return t;
}

tree *__t_fork_order(int order[], int start, int end) {
  if (start > end) return null;
  
  int i, id = order[start];
  for (i = start+1; i <= end; i++)
    if (order[i] > id) break;

  return t_fork(
    id,
    __t_fork_order(order, start+1, i-1),
    __t_fork_order(order, i, end)
  );
}

tree *t_fork_order(int order[], int n) {
  tree *t = __t_fork_order(order, 0, n-1);
  t = t_balance(t);
  // ugly hack (t_stack_n)
  t_stack_n = t_stack(t);
  return t;
}

tree *t_insert(tree *t, int id) {
  if (t == null)
    return t_fork(id, null, null);
  if (id < t->id) {
    t->left = t_insert(t->left, id);
    t = t_balance(t);
  }
  else if (id > t->id) {
    t->right = t_insert(t->right, id);
    t = t_balance(t);
  }
  return t; 
}

tree *t_find(tree *t, int id) {
  if (t == null)
    return null;
  if (id < t->id)
    return t_find(t->left, id);    
  else if (id > t->id)
    return t_find(t->right, id);
  else
    return t;
}

tree *t_find_max(tree *t) {
  if (t->right)
    return t_find_max(t->right);
  else if (t->left)
    return t_find_max(t->left);
  else
    return t;
}

tree *t_find_min(tree *t) {
  if (t->left)
    return t_find_min(t->left);
  else if (t->right)
    return t_find_min(t->right);
  else
    return t;
}

tree *t_del(tree *t, int id) {
  if (t == null) return null;
  tree *t_cell;
  
  if (id < t->id) {
    t->left = t_del(t->left, id);
    t = t_balance(t);
  }
  else if (id > t->id) {
    t->right = t_del(t->right, id);
    t = t_balance(t);
  }
  else if (t->left && t->right) {/* 2 child */
    tree *limit = t_find_max(t->left); /* find maximum limit of left */
    t->id = limit->id;
    t->left = t_del(t->left, limit->id);
    t = t_balance(t);
  }
  else {/* 0 or 1 child */
    t_cell = t;
    if (t->left == null)
      t = t->right;
    else if (t->right == null)
      t = t->left;
    free(t_cell);
  }

  return t;
}

tree *t_balance(tree *t) {
  int hl = t_stack(t->left);
  int hr = t_stack(t->right);

  if (hl - hr > 1) {
    t = t_rotate_right(t);
  }
  else if (hr - hl > 1) {
    t = t_rotate_left(t);
  }

  if (t->left) {
    t->left = t_balance(t->left);
  }
  if (t->right) {
    t->right = t_balance(t->right);
  }
  return t;
}

tree *t_rotate_right(tree *t) {
  tree *tl = t->left;
  size_t nl = t_stack(tl->left);
  size_t nr = t_stack(tl->right);

  if (nl > nr)
    return t_rotate_right_1(t);
  else
    return t_rotate_right_2(t);
}

tree *t_rotate_left(tree *t) {
  tree *tr = t->right;
  size_t nl = t_stack(tr->left);
  size_t nr = t_stack(tr->right);

  if (nr > nl)
    return t_rotate_left_1(t);
  else
    return t_rotate_left_2(t);
}

tree *t_rotate_right_1(tree *t) {
  tree *p;
  p = t->left;
  t->left = p->right;
  p->right = t;
  t = p;
  return t;
}

tree *t_rotate_left_1(tree *t) {
  tree *p;
  p = t->right;
  t->right = p->left;
  p->left = t;
  t = p;
  return t;
}

tree *t_rotate_right_2(tree *t) {
  t->left = t_rotate_left_1(t->left);
  t = t_rotate_right_1(t);
  return t;
}

tree *t_rotate_left_2(tree *t) {
  t->right = t_rotate_right_1(t->right);
  t = t_rotate_left_1(t);
  return t;
}

void t_clean(tree *t) {
  if (t == null) return;
  t_clean(t->left);
  t_clean(t->right);
  free(t);
}

size_t t_stack(tree *t) {
  if(t == null) return 0;
  
  int nl = 0, nr = 0;
  if (t->left)
    nl = t_stack(t->left);
  if (t->right)
    nr = t_stack(t->right);
  
  return 1 + max(nl, nr);
}

size_t t_size(tree *t) {
  if (t == null) return 0;
  return 1
    + t_size(t->left)
    + t_size(t->right);
}

void t_preorder(tree *t) {
  if (t == null)
    return;

  printf("%d ", t->id);
  
  t_preorder(t->left);
  t_preorder(t->right);
}

#define tab_max_size 100
const char *tabs[] = {"├ ", "└ ", "│ ", " ", "  "};

void t_print(tree *t) {
  t_print_stack(t, null, 0);
}

void t_print_stack(tree *t, char *pretab, size_t stack) {
  if (t == null && !stack) /* root is null */
    return;
  else if (t == null) {/* node is null */
    printf("\n");
    return;
  }

  // print id
  printf("%d\n", t->id);
  
  if (t->left == null && t->right == null) /* no child */
    return;

  // #ugly hack (t_stack_n)
  bool skip_stack = t->left == null && (2 <= (t_stack_n-1-stack));
  char tab[tab_max_size];
 
  // print tab of right
  t_print_tab(pretab, tabs[0], stack);
  
  // stack next-stack pre-tab of right
  t_stack_tab(tab, pretab, t->left == null ? tabs[1] : tabs[2], stack);
  
  // print right
  t_print_stack(t->right, tab, stack+1);

  if (skip_stack)
    return;
  
  // print tab of left
  // #duplicate (t->left == null)
  t_print_tab(pretab, tabs[1], stack);
  
  // stack next-stack pre-tab of left
  t_stack_tab(tab, pretab, tabs[4], stack);
  
  // print left
  t_print_stack(t->left, tab, stack+1);
}

void t_print_tab(char *pretab, const char *currtab,
                 size_t stack) {
  if (pretab != null) {/* pre-tab of node */
    printf("%s", pretab);
  }
  else {/* space instead */
    for1(i, 0, stack*2) printf("%s", tabs[3]);
  }
  printf("%s", currtab); /* curr-tab of left */
}

void t_stack_tab(char *tab, char *pretab, const char *currtab,
                 size_t stack) {
  memset(tab, '\0', tab_max_size);
  
  if (pretab != null) {/* prev-stack */
    char *pos = pretab + strlen(pretab) - strlen(tabs[1]);
    
    if (0 == strcmp(pos, tabs[1])) {
      memset(pos, '\0', strlen(tabs[1]));
      strcat(pretab, tabs[4]);
    }
    
    strcat(tab, pretab);
  }
  else {/* space instead */
    for1(i, 0, stack*2) strcat(tab, tabs[3]);
  }

  strcat(tab, currtab);
}

#endif
