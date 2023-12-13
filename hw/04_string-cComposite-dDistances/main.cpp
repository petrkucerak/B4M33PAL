#include <iostream>
#include <vector>

#define CHILD root->to[pattern[pattern_start]]

struct Node {
   bool type;
   Node *to[2];
   int depth;
   int occurrence;
   bool leaf;

   Node() : type(0), to{NULL, NULL}, depth(0), occurrence(0), leaf(false) {}
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

Node *find_node(Node *root, vector<bool> pattern, int pattern_start,
                int pattern_end)
{
   Node *tmp = root;
   for (int i = pattern_start; i < pattern_end; ++i) {
      if (tmp->to[pattern[i]] != NULL)
         tmp = tmp->to[pattern[i]];
      else
         return NULL;
   }
   return tmp;
}

void add_pattern(Node *root, vector<bool> pattern, int pattern_start,
                 int pattern_end, const int max_depth, const int min_depth)
{
   if (CHILD == NULL) {
      CHILD = new Node;
      CHILD->depth = root->depth + 1;
      CHILD->type = pattern[pattern_start];
   }
   if (pattern_end > pattern_start + 1)
      add_pattern(CHILD, pattern, pattern_start + 1, pattern_end, max_depth,
                  min_depth);
   else {
      // final stage
      if (CHILD->depth >= min_depth)
         CHILD->occurrence += 1;
      // is leaf?
      if (!CHILD->leaf && CHILD->to[0] == NULL && CHILD->to[1] == NULL) {
         CHILD->leaf = true;
         ++leaf_count;
         global_depth += CHILD->depth;
      }
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

   // COMPUTE TRIE
   Node *root = new Node;
   root->depth = 0;
   for (int pattern_length = length_max; pattern_length >= length_min;
        --pattern_length) {
      for (int pattern_start = 0; pattern_start < s_length + 1 - pattern_length;
           ++pattern_start) {
         add_pattern(root, s_pattern, pattern_start,
                     pattern_start + pattern_length, length_max, length_min);
      }
   }

   // Calcule RCD
   long int RDC = 0;
   for (int lenght = length_min; lenght <= length_max; ++lenght) {
      for (int id = 0; id <= t_length - lenght; ++id) {
         Node *tmp = find_node(root, t_pattern, id, id + lenght);
         if (tmp != NULL) {
            RDC += (tmp->depth * tmp->occurrence);
         }
      }
   }

   // REMOVE DATA
   delete_trie(&root);

   printf("%ld %d %d\n", RDC, leaf_count, global_depth);

   return 0;
}
