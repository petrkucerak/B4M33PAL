#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

struct Atom {
   unordered_set<int> target;
   // bool is_fast = false;
};

bool is_isomorfism(vector<Atom> &A, vector<Atom> &B, int count_atoms)
{
   bool result = true;

   return result;
}

int main(int argc, char const *argv[])
{
   // Load the metadata
   int count_molecules, count_atoms, count_bonds;
   if (scanf("%d %d %d\n", &count_molecules, &count_atoms, &count_bonds) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }
   // printf("M: %d, A: %d, B: %d\n", count_molecules, count_atoms,
   // count_bonds);

   // Load the data
   vector<vector<Atom>> molecules(count_molecules, vector<Atom>(count_atoms));
   for (auto &molecule : molecules) {
      for (int i = 0; i < count_bonds; ++i) {
         int a, b;
         if (scanf("%d %d\n", &a, &b) != 2) {
            fprintf(stderr, "ERROR - can't load the data!\n");
            exit(EXIT_FAILURE);
         }
         a -= 1;
         b -= 1;
         molecule[a].target.insert(b);
         molecule[b].target.insert(a);
      }
   }

   // Calcule token for each combination by targets count
   vector<long unsigned int> molecule_tokens(count_molecules, 0);
   for (int i = 0; i < count_molecules; ++i) {
      for (auto &atom : molecules[i]) {
         int size = atom.target.size();
         // 1 - 1
         // 2 - 10000
         // 3 - 100000000
         if (size == 1)
            molecule_tokens[i] += 1;
         else if (size == 2)
            molecule_tokens[i] += 10000;
         else
            molecule_tokens[i] += 100000000;
      }
   }

   // Map ordered molecules
   vector<bool> is_ordered(count_molecules, false);
   vector<int> group_counts;

   for (int i = 0; i < count_molecules; ++i) {
      // If source molecule is orderd, skip it
      if (is_ordered[i])
         continue;

      // Create new class
      is_ordered[i] = true;
      group_counts.push_back(1);

      for (int j = i + 1; j < count_molecules; ++j) {
         // If molecule is orderd, skip it
         if (is_ordered[j])
            continue;

         // Valid isomorfism
         if (molecule_tokens[i] == molecule_tokens[j] &&
             is_isomorfism(molecules[i], molecules[j], count_atoms)) {
            is_ordered[j] = true;
            // incereas last index
            group_counts.back() += 1;
         }
      }
   }

   sort(group_counts.begin(), group_counts.end(),
        [](const int &a, const int &b) { return a > b ? false : true; });

   // Print results
   for (long unsigned int i = 0; i < group_counts.size(); ++i) {
      if (i == 0)
         cout << group_counts[i];
      else
         cout << " " << group_counts[i];
   }
   cout << endl;

   return 0;
}
