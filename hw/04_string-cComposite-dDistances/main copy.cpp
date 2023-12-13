#include <iostream>
#include <vector>

#define CHILD root->to[pattern[pattern_start]]

struct Node {
   bool type;
   Node *to[2];
   int depth;
   int occurrence;

   Node() : type(0), to{NULL, NULL}, depth(0), occurrence(0) {}
};

using namespace std;

int global_depth = 0;
int leaf_count = 0;

void delete_trie(Node **root)
{
   if (*root == NULL)
      return;
   if ((*root)->to[0] != NULL) {
      delete_trie(&(*root)->to[0]);
      (*root)->to[0] = NULL;
   }
   if ((*root)->to[1] != NULL) {
      delete_trie(&(*root)->to[1]);
      (*root)->to[1] = NULL;
   }
   delete *root;
   *root = NULL;
}

void build_trie(Node *root, vector<bool> pattern, int pattern_start,
                const int max_depth, const int min_depth)
{
   if (root->depth < max_depth) {
      bool is_leaf = true;
      if (CHILD == NULL) {
         CHILD = new Node;
         CHILD->depth = root->depth + 1;
         CHILD->type = pattern[pattern_start];
         // TODO: CHILD->occurrence;
      }
   } else {
      ++leaf_count;
      global_depth += root->depth;
   }
}

int main(int argc, char const *argv[])
{
   // LOAD DATA
   int s_length, t_length, length_min, length_max;
   if (scanf("%d %d %d %d\n", &s_length, &t_length, &length_min, &length_max) !=
       4) {
      fprintf(stderr, "Can't load metadata!\n");
      exit(EXIT_FAILURE);
   }

   vector<bool> s_pattern;
   vector<bool> t_pattern;
   s_pattern.reserve(s_length);
   t_pattern.reserve(t_length);

   for (int i = 0; i < s_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load S string!\n");
         exit(EXIT_FAILURE);
      }
      s_pattern.push_back(n == '0' ? false : true);
   }
   scanf("\n");
   for (int i = 0; i < t_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load T string!\n");
         exit(EXIT_FAILURE);
      }
      t_pattern.push_back(n == '0' ? false : true);
   }

   for (auto pattern : s_pattern) {
      !pattern ? printf("0") : printf("1");
   }
   printf("\n");

   // COMPUTE TRIE
   Node *root = new Node;
   root->depth = 0;
   build_trie(root, s_pattern, 0, length_max, length_min);

   // REMOVE DATA
   delete_trie(&root);

   return 0;
}
