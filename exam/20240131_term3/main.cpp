#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{

   // LOAD THE DATA
   int p_max, m_max, n;
   if (scanf("%d %d %d\n", &p_max, &m_max, &n) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }

   vector<long> sequence(n);
   for (auto &x : sequence) {
      if (scanf("%ld", &x) != 1) {
         fprintf(stderr, "ERROR - can't load the data!\n");
         exit(EXIT_FAILURE);
      }
   }
   return 0;
}
