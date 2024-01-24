#include <iostream>
#include <queue>
#include <vector>

#define MOD 65536

using namespace std;

struct Connection {
   int t;
   int value;
   bool is_used;
};

struct QPU {
   vector<Connection> neighbour;
   int red_connection = 0;
};

struct CompareConnection {
   bool operator()(Connection *const &c1, Connection *const &c2)
   {
      return c1->value > c2->value;
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
      QPUs[q1].neighbour.push_back({q2, m, false});
      QPUs[q2].neighbour.push_back({q1, m, false});
      // printf("%d %d %d\n", q1, q2, m);
   }

   vector<vector<Connection>> red_network(unit_counts);
   vector<bool> red_visited(unit_counts, false);
   priority_queue<Connection *, vector<Connection *>, CompareConnection>
       neightbours;

   int red_sum = 0;
   int red_added_units = 0;
   // create red network
   red_visited[0] = true;
   for (int i = 0; i < QPUs[0].neighbour.size(); ++i) {
      neightbours.push(&QPUs[0].neighbour[i]);
   }
   while (!neightbours.empty() || red_added_units == unit_counts) {
      Connection *tmp = neightbours.top();
      neightbours.pop();
      if (red_visited[tmp->t])
         continue;
      if (QPUs[tmp->t].red_connection == 3) // relevancy of this condition?
         continue;

      // add QPU to network
      red_visited[tmp->t] = true;
      for (int i = 0; i < QPUs[tmp->t].neighbour.size(); ++i) {
         neightbours.push(&QPUs[tmp->t].neighbour[i]);
      }
      red_added_units += 1;
      red_sum += tmp->value;
      tmp->is_used = true;
   }

   vector<vector<Connection>> yellow_network(unit_counts);
   vector<bool> yellow_visited(unit_counts, false);

   int yellow_sum = 0;
   int yellow_added_units = 0;
   // create red network
   yellow_visited[0] = true;
   for (int i = 0; i < QPUs[0].neighbour.size(); ++i) {
      // if is used in red network, skip it
      if (QPUs[0].neighbour[i].is_used)
         continue;
      neightbours.push(&QPUs[0].neighbour[i]);
   }
   while (!neightbours.empty() || yellow_added_units == unit_counts) {
      Connection *tmp = neightbours.top();
      neightbours.pop();
      if (yellow_visited[tmp->t])
         continue;

      // add QPU to network
      yellow_visited[tmp->t] = true;
      for (int i = 0; i < QPUs[tmp->t].neighbour.size(); ++i) {
         neightbours.push(&QPUs[tmp->t].neighbour[i]);
      }
      yellow_added_units += 1;
      yellow_sum += tmp->value;
      tmp->is_used = true;
   }

   // // print data
   // for (int i = 0; i < QPUs.size(); ++i) {
   //    for (int j = 0; j < QPUs[i].neighbour.size(); ++j) {
   //       printf("[%d,%d,%d] ", QPUs[i].neighbour[j].t,
   //              QPUs[i].neighbour[j].value, QPUs[i].neighbour[j].is_used);
   //    }
   //    printf("\n");
   // }

   cout << red_sum % MOD << " " << yellow_sum % MOD << endl;

   return 0;
}
