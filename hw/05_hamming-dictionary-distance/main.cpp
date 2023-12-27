#include <iostream>
#include <queue>
#include <string>
#include <vector>

using namespace std;

struct Node {
   int hd;
   int distance;
};

struct CompareDistance {
   bool operator()(Node const &n1, Node const &n2)
   {
      return n1.distance < n2.distance;
   }
};

int hammingDistance(string &a, string &b, int b_start)
{
   int count = 0;
   for (int i = 0; i < a.size(); ++i) {
      if (a[i] != b[i + b_start])
         ++count;
   }
   return count;
}

int main(int argc, char const *argv[])
{
   // Load the input
   int dictionary_size, minimum_dictionary_word_size;
   cin >> dictionary_size >> minimum_dictionary_word_size;
   vector<string> dictionary(dictionary_size);
   for (int i = 0; i < dictionary_size; ++i) {
      cin >> dictionary[i];
   }
   string text;
   cin >> text;

   // Precalcule the HD for all combinations
   vector<vector<Node>> precalculation;
   for (auto &word : dictionary) {
      vector<Node> word_hd;
      for (int i = 0; i < text.size() - word.size() + 2; ++i) {
         word_hd.push_back({hammingDistance(word, text, i), INT32_MAX});
      }
      precalculation.push_back(word_hd);
   }

   // for (auto &word : precalculation) {
   //    for (auto &combination : word)
   //       cout << combination.hd << " ";
   //    cout << endl;
   // }

   priority_queue<Node, vector<Node>, CompareDistance> queue;
   // Add starting nodes

   return 0;
}
