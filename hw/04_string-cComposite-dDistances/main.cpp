#include <iostream>
#include <string>

#define TYPE_0 0
#define TYPE_1 1
#define TYPE_ROOT 2

using namespace std;

struct Node {
   uint8_t type;
   Node *add[2];
   int depth;

   Node() : type(0), add{NULL, NULL}, depth(0) {}
};

void build_trie(Node *root, string pattern, string path, const int max_depth)
{
   if (root->depth <= max_depth) {
      string path_0 = path;
      path_0.append(sizeof(char), '0');
      if (pattern.find(path_0) != string::npos) {
         root->add[0] = new Node;
         root->add[0]->type = TYPE_0;
         root->add[0]->depth = root->depth + 1;
         build_trie(root->add[0], pattern, path_0, max_depth);
      }

      string path_1 = path;
      path_1.append(sizeof(char), '1');
      if (pattern.find(path_1) != string::npos) {
         root->add[1] = new Node;
         root->add[1]->type = TYPE_1;
         root->add[1]->depth = root->depth + 1;
         build_trie(root->add[1], pattern, path_1, max_depth);
      }
   } else {
      // Only for testing
      cout << path << endl;
   }
}

void delete_trie(Node **root)
{
   if (*root == NULL)
      return;
   if ((*root)->add[0] != NULL) {
      delete_trie(&(*root)->add[0]);
      (*root)->add[0] = NULL;
   }
   if ((*root)->add[1] != NULL) {
      delete_trie(&(*root)->add[1]);
      (*root)->add[1] = NULL;
   }
   delete *root;
   *root = NULL;
}

int main(int argc, char const *argv[])
{
   int s_length, t_length, length_min, length_max;
   if (scanf("%d %d %d %d\n", &s_length, &t_length, &length_min, &length_max) !=
       4) {
      fprintf(stderr, "Can't load metadata!\n");
      exit(EXIT_FAILURE);
   }
   int *S = new int[s_length];
   int *T = new int[t_length];
   string pattern;

   for (int i = 0; i < s_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load S string!\n");
         exit(EXIT_FAILURE);
      }
      pattern.append(sizeof(char), n);
      S[i] = int(n) - 48;
   }
   scanf("\n");
   for (int i = 0; i < t_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load T string!\n");
         exit(EXIT_FAILURE);
      }
      T[i] = int(n) - 48;
   }

   // Print S array
   for (int i = 0; i < s_length; ++i)
      printf("%d", S[i]);
   printf("\n");
   // Print T array
   for (int i = 0; i < t_length; ++i)
      printf("%d", T[i]);
   printf("\n");

   Node *root = new Node;
   root->depth = 0;
   root->type = TYPE_ROOT;
   if (pattern.find("0") != string::npos) {
      root->add[0] = new Node;
      root->add[0]->type = TYPE_0;
      root->add[0]->depth = root->depth + 1;
      build_trie(root->add[0], pattern, "0", length_max);
   }

   if (pattern.find("1") != string::npos) {
      root->add[1] = new Node;
      root->add[1]->type = TYPE_1;
      root->add[1]->depth = root->depth + 1;
      build_trie(root->add[1], pattern, "1", length_max);
   }

   delete_trie(&root);

   delete[] S;
   delete[] T;

   return 0;
}
