#include <iostream>
#include <vector>

#define MAP(x, y) ((x) + (servers_num * (y)))

using namespace std;

struct Point {
   int x;
   int y;
};

struct Result {
   int a, b; // a, b (a < b) are the labels of two servers in network A
   int c, d; // c, d (c < d) are the labels of two servers in network B
};

void print_map(vector<int> map, int servers_num)
{
   cout << endl;
   for (int y = 0; y < servers_num; ++y) {
      for (int x = 0; x < servers_num; ++x)
         cout << map[MAP(x, y)] << " ";
      cout << endl;
   }
}

int main(int argc, char const *argv[])
{
   // LOAD THE DATA
   int servers_num, planned_connection_num, fast_servers_num;
   if (scanf("%d %d %d\n", &servers_num, &planned_connection_num,
             &fast_servers_num) != 3) {
      fprintf(stderr, "Can't load the metadata\n");
      exit(EXIT_FAILURE);
   }

   const int map_size = servers_num * servers_num;

   vector<bool> A_map_fast(servers_num, false);
   vector<int> A_map(map_size, 0);
   // Load fast servers for A network
   for (int i = 0; i < fast_servers_num; ++i) {
      int tmp;
      if (scanf("%d", &tmp) != 1) {
         fprintf(stderr, "Can't load list of fast servers for A network\n");
         exit(EXIT_FAILURE);
      }
      A_map_fast[tmp] = true;
   }
   // Load fast connection for A network
   for (int i = 0; i < planned_connection_num - 1; ++i) {
      int a, b;
      if (scanf("%d %d\n", &a, &b) != 2) {
         fprintf(stderr, "Can't load connection for A network\n");
         exit(EXIT_FAILURE);
      }
      A_map[MAP(b, a)] = 1;
      A_map[MAP(a, b)] = 1;
   }

   vector<bool> B_map_fast(servers_num, false);
   vector<int> B_map(map_size, 0);
   vector<Point> different;
   // Load fast servers for B network
   for (int i = 0; i < fast_servers_num; ++i) {
      int tmp;
      if (scanf("%d", &tmp) != 1) {
         fprintf(stderr, "Can't load list of fast servers for B network\n");
         exit(EXIT_FAILURE);
      }
      B_map_fast[tmp] = true;
   }
   // Load fast connection for B network
   for (int i = 0; i < planned_connection_num + 1; ++i) {
      int a, b;
      if (scanf("%d %d\n", &a, &b) != 2) {
         fprintf(stderr, "Can't load connection for B network\n");
         exit(EXIT_FAILURE);
      }
      B_map[MAP(b, a)] = 1;
      B_map[MAP(a, b)] = 1;
      if (!A_map[MAP(b, a)])
         different.push_back({b, a});
   }

   // print_network(A);
   // print_network(B);

   print_map(A_map, servers_num);
   print_map(B_map, servers_num);

   vector<Result> results;

   // HERUISTIC
   // 1. pair only server with same num of connection
   // 2. pair only fast server to fast server

   // START CALCULATION
   // 1. Add and remove connection to every possible connection
   // 2. Sort by connection count
   // 3. verify isomorfism

   // Print differents
   // for (auto &d : different)
   //    cout << "[" << d.x << "," << d.y << "] ";
   // cout << endl;

   for (auto &point : different) {
      // remove connection
      B_map[MAP(point.x, point.y)] = 0;
      B_map[MAP(point.y, point.x)] = 0;
      for (int y = 0; y < servers_num - 1; ++y) {
         for (int x = y + 1; x < servers_num; ++x) {
            if (A_map[MAP(x, y)]) // connection exists in A network
               continue;
            A_map[MAP(x, y)] = 1;
            // check isomorfism
         }
      }
      // add connection back
      B_map[MAP(point.x, point.y)] = 1;
      B_map[MAP(point.y, point.x)] = 1;
   }

   // PRINT RESULTS
   for (auto result : results)
      printf("%d %d %d %d\n", result.a, result.b, result.c, result.d);
   return 0;
}
