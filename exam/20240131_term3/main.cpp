#include <iostream>
#include <vector>

using namespace std;

int main(int argc, char const *argv[])
{
   int p_max, m_max, n;
   if (scanf("%d %d %d\n", &p_max, &m_max, &n) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }
   printf("Metadata: %d %d %d\n", p_max, m_max, n);
   vector<long> sequence(n);
   for (auto &x : sequence) {
      if (scanf("%ld", &x) != 1) {
         fprintf(stderr, "ERROR - can't load the data!\n");
         exit(EXIT_FAILURE);
      }
   }
   for (auto &x : sequence)
      cout << x << " ";
   cout << endl;
   return 0;
}
