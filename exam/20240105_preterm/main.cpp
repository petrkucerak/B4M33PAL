#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
   // Load the input
   int count_mulecules, count_atoms, count_bonds;
   if (scanf("%d %d %d\n", &count_mulecules, &count_atoms, &count_bonds) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }
   printf("M: %d, A: %d, B: %d\n", count_mulecules, count_atoms, count_bonds);
   return 0;
}
