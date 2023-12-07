#include <chrono>
#include <iostream>
#include <string>

#define TYPE_0 0
#define TYPE_1 1
#define TYPE_ROOT 2

#define MEASURE_TIME

#define CHILD_0 root->add[0]
#define CHILD_1 root->add[1]

using namespace std;

struct Node {
   uint8_t type;
   Node *add[2];
   int depth;
   int occurrence;

   Node() : type(0), add{NULL, NULL}, depth(0), occurrence(0) {}
};

int global_depth = 0;
int leaf_count = 0;

int count_occurrences(const string &str, const string &sub)
{
   int count = 0;
   for (int pos = 0; (pos = str.find(sub, pos)) != string::npos; ++pos, ++count)
      ;
   return count;
}

void build_trie(Node *root, string s_pattern, string path, const int max_depth,
                const int min_depth)
{
   if (root->depth < max_depth) {
      bool is_leaf = true;
      string path_0 = path;
      path_0.append(sizeof(char), '0');
      if (s_pattern.find(path_0) != string::npos) {
         CHILD_0 = new Node;
         CHILD_0->type = TYPE_0;
         CHILD_0->depth = root->depth + 1;

         // calcule occurrence of s_pattern
         if (path_0.size() >= min_depth)
            CHILD_0->occurrence = count_occurrences(s_pattern, path_0);

         build_trie(CHILD_0, s_pattern, path_0, max_depth, min_depth);
         is_leaf = false;
      }

      string path_1 = path;
      path_1.append(sizeof(char), '1');
      if (s_pattern.find(path_1) != string::npos) {
         CHILD_1 = new Node;
         CHILD_1->type = TYPE_1;
         CHILD_1->depth = root->depth + 1;

         // calcule occurrence of s_pattern
         if (path_1.size() >= min_depth)
            CHILD_1->occurrence = count_occurrences(s_pattern, path_1);

         build_trie(CHILD_1, s_pattern, path_1, max_depth, min_depth);
         is_leaf = false;
      }
      if (is_leaf) {
         // cout << path << endl;
         if (path.size() >= min_depth) {
            ++leaf_count;
            global_depth += root->depth;
         }
      }
   } else {
      // Only for testing
      // cout << path << endl;
      ++leaf_count;
      global_depth += root->depth;
   }
}

Node *find(Node *root, const string key)
{
   Node *tmp = root;
   for (int i = 0; i < key.size(); ++i) {
      if (tmp->add[int(key[i] - 48)] != NULL)
         tmp = tmp->add[int(key[i] - 48)];
      else
         return NULL;
   }
   return tmp;
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

#ifdef MEASURE_TIME
   auto start = chrono::high_resolution_clock::now();
#endif

   int s_length, t_length, length_min, length_max;
   if (scanf("%d %d %d %d\n", &s_length, &t_length, &length_min, &length_max) !=
       4) {
      fprintf(stderr, "Can't load metadata!\n");
      exit(EXIT_FAILURE);
   }
   string s_pattern;
   string t_pattern;

   s_pattern.reserve(s_length);
   for (int i = 0; i < s_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load S string!\n");
         exit(EXIT_FAILURE);
      }
      s_pattern.push_back(n);
   }
   scanf("\n");

   t_pattern.reserve(t_length);
   for (int i = 0; i < t_length; ++i) {
      char n;
      if (scanf("%c", &n) != 1) {
         fprintf(stderr, "Can't load T string!\n");
         exit(EXIT_FAILURE);
      }
      t_pattern.push_back(n);
   }

#ifdef MEASURE_TIME
   auto load_data = chrono::high_resolution_clock::now();
#endif

   Node *root = new Node;
   root->depth = 0;
   root->type = TYPE_ROOT;
   if (s_pattern.find("0") != string::npos) {
      root->add[0] = new Node;
      root->add[0]->type = TYPE_0;
      root->add[0]->depth = root->depth + 1;

      // calcule occurrence of s_pattern
      if (length_min <= 1)
         CHILD_0->occurrence = count_occurrences(s_pattern, "0");

      build_trie(root->add[0], s_pattern, "0", length_max, length_min);
   }

   if (s_pattern.find("1") != string::npos) {
      root->add[1] = new Node;
      root->add[1]->type = TYPE_1;
      root->add[1]->depth = root->depth + 1;

      // calcule occurrence of s_pattern
      if (length_min <= 1)
         CHILD_1->occurrence = count_occurrences(s_pattern, "1");

      build_trie(root->add[1], s_pattern, "1", length_max, length_min);
   }

#ifdef MEASURE_TIME
   auto created_trie = chrono::high_resolution_clock::now();
#endif

   // Calcule RCD
   long int RDC = 0;
   for (int lenght = length_min; lenght <= length_max; ++lenght) {
      for (int id = 0; id <= t_length - lenght; ++id) {
         Node *tmp = find(root, t_pattern.substr(id, lenght));
         if (tmp != NULL) {
            // cout << t_pattern.substr(id, lenght) << endl;
            RDC += (tmp->depth * tmp->occurrence);
         }
      }
   }

   // cout << endl;
   // cout << "RDC: " << RDC << endl;
   // cout << "Leaf count: " << leaf_count << endl;
   // cout << "Global depth: " << global_depth << endl;

   printf("%ld %d %d\n", RDC, leaf_count, global_depth);

#ifdef MEASURE_TIME
   auto print_results = chrono::high_resolution_clock::now();
#endif

   delete_trie(&root);

#ifdef MEASURE_TIME
   auto end = chrono::high_resolution_clock::now();

   auto loading_data_time =
       chrono::duration_cast<chrono::microseconds>(load_data - start);
   auto creating_trie_time =
       chrono::duration_cast<chrono::microseconds>(created_trie - load_data);
   auto get_RCD = chrono::duration_cast<chrono::microseconds>(print_results -
                                                              created_trie);
   auto free_memory =
       chrono::duration_cast<chrono::microseconds>(end - print_results);
   cout << "Time taken by function: " << endl;
   cout << loading_data_time.count() << endl;
   cout << creating_trie_time.count() << endl;
   cout << get_RCD.count() << endl;
   cout << free_memory.count() << endl;
#endif
   return 0;
}
