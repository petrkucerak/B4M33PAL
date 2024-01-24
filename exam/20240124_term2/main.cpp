#include <iostream>
#include <vector>

using namespace std;

struct Connection {
   int t;
   int value;
   bool is_used;
};

int main(int argc, char const *argv[])
{
   // load the metadata
   int unit_counts, connection_counts;
   int D; // cycle limit
   if (scanf("%d %d %d\n", &unit_counts, &connection_counts, &D) != 3) {
      fprintf(stderr, "ERROR - can't load metadata correctly!\n");
      exit(EXIT_FAILURE);
   }
   printf("Metadata are: %d %d %d\n", unit_counts, connection_counts, D);

   // load the data
   vector<vector<Connection>> QPUs(unit_counts);
   for (int i = 0; i < connection_counts; ++i) {
      int q1, q2, m;
      if (scanf("%d %d %d\n", &q1, &q2, &m) != 3) {
         fprintf(stderr, "ERROR - can't load data correctly!\n");
         exit(EXIT_FAILURE);
      }
      q1--;
      q2--;
      QPUs[q1].push_back({q2, m, false});
      QPUs[q2].push_back({q1, m, false});
      // printf("%d %d %d\n", q1, q2, m);
   }

   for (auto &QPU : QPUs) {
      for (auto &connection : QPU)
         printf("[%d,%d] ", connection.t, connection.value);
      cout << endl;
   }

   return 0;
}
