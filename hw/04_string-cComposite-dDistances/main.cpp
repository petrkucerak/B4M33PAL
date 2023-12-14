#include <iostream>
#include <string>
#include <vector>

using namespace std;

struct Node {
   Node *to[2];
   int occurrence;

   Node() : to{nullptr, nullptr}, occurrence(0) {}
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

void getPrams(Node *node, int depth, int &global_depth, int &leaf_count)
{
   // Hanlde leaf
   if (node->to[0] == nullptr && node->to[1] == nullptr) {
      if (node->occurrence > 0) {
         global_depth += depth;
         ++leaf_count;
      }
      return;
   }
   if (node->to[0] != nullptr)
      getPrams(node->to[0], depth + 1, global_depth, leaf_count);
   if (node->to[1] != nullptr)
      getPrams(node->to[1], depth + 1, global_depth, leaf_count);
}

int main()
{
   // Load data
   int s_length, t_length, length_min, length_max;
   cin >> s_length >> t_length >> length_min >> length_max;
   string s_pattern, t_pattern;
   cin >> s_pattern >> t_pattern;

   // Build trie
   Node *root = new Node;

   for (int id = 0; id < s_length; ++id) {
      Node *node = root;
      int depth = 0;
      for (int length = length_min; length <= length_max; ++length) {
         if (id + length > s_length)
            break;
         string tmp = s_pattern.substr(id, length);
         for (int i = depth; i < tmp.size(); ++i) {
            char curr = tmp[i];
            if (node->to[curr - 48] == nullptr)
               node->to[curr - 48] = new Node;
            node = node->to[curr - 48];
            ++depth;
         }
         ++node->occurrence;
      }
   }

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
            if (i + depth >= t_length)
               break;
            if (node->to[t_pattern[i + depth] - 48] == nullptr)
               break;
            node = node->to[t_pattern[i + depth] - 48];
         }
      }
      is_end = false;
      node = root;
   }

   int leaf_count = 0;
   int global_depth = 0;
   getPrams(root, 0, global_depth, leaf_count);

   // Remove data
   deleteTrie(root);

   // Output result
   cout << RDC << " " << leaf_count << " " << global_depth << endl;

   return 0;
}
