#include <chrono>
#include <iostream>
#include <vector>

using namespace std;

#define GET_TIME chrono::duration_cast<chrono::microseconds>

struct Node {
   // bool type;
   Node *to[2];
   int depth;
   int occurrence;
   bool leaf;

   Node()
       : /*type(false), */ to{nullptr, nullptr}, depth(0), occurrence(0),
         leaf(false)
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

// Function to find a node in the trie
Node *findNode(Node *root, const vector<bool> &pattern, int pattern_start,
               int pattern_end)
{
   Node *tmp = root;
   for (int i = pattern_start; i < pattern_end; ++i) {
      if (tmp->to[pattern[i]] != nullptr)
         tmp = tmp->to[pattern[i]];
      else
         return nullptr;
   }
   return tmp;
}

// Function to add a pattern to the trie
void addPattern(Node *root, const vector<bool> &pattern, int pattern_start,
                int pattern_end, const int max_depth, const int min_depth,
                int &leaf_count, int &global_depth)
{
   Node *&child = root->to[pattern[pattern_start]];
   if (child == nullptr) {
      child = new Node;
      child->depth = root->depth + 1;
      // child->type = pattern[pattern_start];
   }
   if (pattern_end > pattern_start + 1)
      addPattern(child, pattern, pattern_start + 1, pattern_end, max_depth,
                 min_depth, leaf_count, global_depth);
   else {
      // final stage
      if (child->depth >= min_depth)
         child->occurrence += 1;
      // is leaf?
      if (!child->leaf && child->to[0] == nullptr && child->to[1] == nullptr) {
         child->leaf = true;
         ++leaf_count;
         global_depth += child->depth;
      }
   }
}

int main()
{
   // auto start = chrono::high_resolution_clock::now();

   // Load data
   int s_length, t_length, length_min, length_max;
   cin >> s_length >> t_length >> length_min >> length_max;

   vector<bool> s_pattern(s_length);
   vector<bool> t_pattern(t_length);

   for (int i = 0; i < s_length; ++i) {
      char n;
      cin >> n;
      s_pattern[i] = (n == '0' ? false : true);
   }

   for (int i = 0; i < t_length; ++i) {
      char n;
      cin >> n;
      t_pattern[i] = (n == '0' ? false : true);
   }

   // auto time_1 = chrono::high_resolution_clock::now();

   // Compute trie
   Node *root = new Node;
   root->depth = 0;
   int leaf_count = 0;
   int global_depth = 0;
   for (int pattern_length = length_max; pattern_length >= length_min;
        --pattern_length) {
      for (int pattern_start = 0; pattern_start < s_length + 1 - pattern_length;
           ++pattern_start) {
         addPattern(root, s_pattern, pattern_start,
                    pattern_start + pattern_length, length_max, length_min,
                    leaf_count, global_depth);
      }
   }

   // auto time_2 = chrono::high_resolution_clock::now();

   // Compute RDC
   long int RDC = 0;
   for (int id = 0; id <= t_pattern.size() - length_min; ++id) {
      for (int length = length_min;
           length <= length_max && id + length < t_pattern.size() + 1;
           ++length) {
         Node *tmp = findNode(root, t_pattern, id, id + length);
         if (tmp != nullptr) {
            RDC += (tmp->depth * tmp->occurrence);
         } else
            break;
      }
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
