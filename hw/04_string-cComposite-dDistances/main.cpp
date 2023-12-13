#include <chrono>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

#define GET_TIME chrono::duration_cast<chrono::microseconds>

struct Node {
   Node *to[2];
   int occurrence;

   Node()
       : /*type(false), */ to{nullptr, nullptr},
         /*depth(0),*/ occurrence(0) /*, leaf(false) */
   {
   }
};

// Function to delete the trie
void deleteTrie(Node *&root)
{
   if (root == nullptr) {
      return;
   }
   if (root->to[0] != nullptr) {
      deleteTrie(root->to[0]);
      root->to[0] = nullptr;
   }
   if (root->to[1] != nullptr) {
      deleteTrie(root->to[1]);
      root->to[1] = nullptr;
   }
   delete root;
   root = nullptr;
}

int main()
{
   // auto start = chrono::high_resolution_clock::now();

   // Load data
   int s_length, t_length, length_min, length_max;
   cin >> s_length >> t_length >> length_min >> length_max;
   string s_pattern, t_pattern;
   cin >> s_pattern >> t_pattern;

   // auto time_1 = chrono::high_resolution_clock::now();

   // Compute trie
   Node *root = new Node;
   int leaf_count = 0;
   int global_depth = 0;
   for (int id = 0; id < s_length; ++id) {
      Node *node = root;
      int depth = 0;
      for (int length = length_min; length <= length_max; ++length) {
         if (id + length > s_length)
            break;
         string tmp = s_pattern.substr(id, length);
         for (int i = depth; i < tmp.size(); ++i) {
            char curr = tmp[i];
            if (node->to[curr - 48] == nullptr) {
               node->to[curr - 48] = new Node;
               node = node->to[curr - 48];
            }
            ++depth;
         }
      }
      ++node->occurrence;
   }

   // auto time_2 = chrono::high_resolution_clock::now();

   // Compute RDC
   int RDC = 0;
   Node *node = root;
   bool is_end = false;
   for (int i = 0; i <= t_length - length_min; ++i) {
      for (int j = 0; j < length_min; ++j) {
         if (node->to[t_pattern[i + j] - 48] == nullptr) {
            is_end = true;
            break;
         }
         node = node->to[t_pattern[i + j] - 48];
      }
      if (!is_end) {
         for (int depth = length_min; depth <= length_max; ++depth) {
            if (depth + i > t_length)
               continue;
            if (node->occurrence > 0)
               RDC += depth * node->occurrence;
            if (depth == length_max)
               continue;
            if (node->to[t_pattern[i + depth] - 48] == nullptr)
               break;
            node = node->to[t_pattern[i + depth] - 48];
         }
      }
      is_end = false;
      node = root;
   }

   // auto time_3 = chrono::high_resolution_clock::now();

   // Remove data
   deleteTrie(root);

   // auto time_4 = chrono::high_resolution_clock::now();

   // Output result
   cout << RDC << " " << leaf_count << " " << global_depth << endl;

   // auto time_load_data = GET_TIME(time_1 - start);
   // auto time_trie = GET_TIME(time_2 - time_1);
   // auto time_RDC = GET_TIME(time_3 - time_2);
   // auto time_free = GET_TIME(time_4 - time_3);

   // cout << endl << "Time consumebd by function" << endl;
   // cout << time_load_data.count() << endl;
   // cout << time_trie.count() << endl;
   // cout << time_RDC.count() << endl;
   // cout << time_free.count() << endl;

   return 0;
}
