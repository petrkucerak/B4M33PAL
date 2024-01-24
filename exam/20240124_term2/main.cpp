#include <iostream>
#include <queue>
#include <vector>

using namespace std;

struct Connection {
   int t;
   int value;
   // bool is_used;
};

struct QPU {
   vector<Connection> neighbour;
   int red_connection = 0;
};

struct CompareConnection {
   bool operator()(Connection const &c1, Connection const &c2)
   {
      return c1.value > c2.value;
   }
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
   vector<QPU> QPUs(unit_counts);
   for (int i = 0; i < connection_counts; ++i) {
      int q1, q2, m;
      if (scanf("%d %d %d\n", &q1, &q2, &m) != 3) {
         fprintf(stderr, "ERROR - can't load data correctly!\n");
         exit(EXIT_FAILURE);
      }
      q1--;
      q2--;
      QPUs[q1].neighbour.push_back({q2, m});
      QPUs[q2].neighbour.push_back({q1, m});
      // printf("%d %d %d\n", q1, q2, m);
   }

   // // print data
   // for (auto &QPU : QPUs) {
   //    for (auto &connection : QPU)
   //       printf("[%d,%d] ", connection.t, connection.value);
   //    cout << endl;
   // }

   vector<vector<Connection>> red_network(unit_counts);
   vector<bool> red_visited(unit_counts, false);
   priority_queue<Connection, vector<Connection>, CompareConnection>
       neightbours;

   int red_sum = 0;
   int red_added_units = 0;
   // create red network
   red_visited[0] = true;
   for (int i = 0; i < QPUs[0].neighbour.size(); ++i) {
      neightbours.push(QPUs[0].neighbour[i]);
   }
   while (!neightbours.empty() || red_added_units == unit_counts) {
      Connection tmp = neightbours.top();
      neightbours.pop();
      if (red_visited[tmp.t])
         continue;
      if (QPUs[tmp.t].red_connection == 3) // relevancy of this condition?
         continue;

      // add QPU to network
      red_visited[tmp.t] = true;
      for (int i = 0; i < QPUs[tmp.t].neighbour.size(); ++i) {
         neightbours.push(QPUs[tmp.t].neighbour[i]);
      }
      red_added_units += 1;
      red_sum += tmp.value;
   }

   cout << red_sum << endl;

   return 0;
}
