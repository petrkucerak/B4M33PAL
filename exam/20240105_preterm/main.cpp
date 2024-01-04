#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

struct Atom {
   unordered_set<int> target;
};

int main(int argc, char const *argv[])
{
   // Load the input
   int count_mulecules, count_atoms, count_bonds;
   if (scanf("%d %d %d\n", &count_mulecules, &count_atoms, &count_bonds) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }
   printf("M: %d, A: %d, B: %d\n", count_mulecules, count_atoms, count_bonds);

   vector<vector<Atom>> molecules(count_mulecules, vector<Atom>(count_atoms));
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

   return 0;
}
