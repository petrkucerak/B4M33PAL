#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

struct Server {
   unordered_set<int> target;
   bool is_fast = false;
};

void print_network(vector<Server> servers)
{
   cout << endl;
   for (auto &server : servers) {
      cout << server.is_fast << " | ";
      for (auto &target : server.target)
         cout << target << " ";
      cout << endl;
   }
}

bool is_isomorfism(vector<Server> A, vector<Server> B, int servers_num)
{
   return true;
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
   vector<Server> A(servers_num);
   // Load fast servers for A network
   for (int i = 0; i < fast_servers_num; ++i) {
      int tmp;
      if (scanf("%d", &tmp) != 1) {
         fprintf(stderr, "Can't load list of fast servers for A network\n");
         exit(EXIT_FAILURE);
      }
      A[tmp].is_fast = true;
   }
   // Load connection for A network
   for (int i = 0; i < planned_connection_num - 1; ++i) {
      int a, b;
      if (scanf("%d %d\n", &a, &b) != 2) {
         fprintf(stderr, "Can't load connection for A network\n");
         exit(EXIT_FAILURE);
      }
      A[a].target.insert(b);
      A[b].target.insert(a);
   }
   vector<Server> B(servers_num);
   // Load fast servers for B network
   for (int i = 0; i < fast_servers_num; ++i) {
      int tmp;
      if (scanf("%d", &tmp) != 1) {
         fprintf(stderr, "Can't load list of fast servers for A network\n");
         exit(EXIT_FAILURE);
      }
      B[tmp].is_fast = true;
   }
   // Load connection for B network
   for (int i = 0; i < planned_connection_num + 1; ++i) {
      int a, b;
      if (scanf("%d %d\n", &a, &b) != 2) {
         fprintf(stderr, "Can't load connection for A network\n");
         exit(EXIT_FAILURE);
      }
      B[a].target.insert(b);
      B[b].target.insert(a);
   }

   // print_network(A);
   // print_network(B);

   // Process data by upper matrix triangel withou diagonal
   for (int a_source = 0; a_source < servers_num; ++a_source) {
      for (int a_target = a_source + 1; a_target < servers_num; ++a_target) {
         // Handle if edge is allready in the network
         if (A[a_source].target.end() != A[a_source].target.find(a_target))
            continue;

         // Try B network combination in upper matrix triangle without diagonal
         for (int b_source = 0; b_source < servers_num; ++b_source) {
            for (int b_target = b_source + 1; b_target < servers_num;
                 ++b_target) {
               if (b_source > b_target)
                  continue;
               if (B[b_source].target.end() ==
                   B[b_source].target.find(b_target))
                  continue;
               // Add one connection to A network
               A[a_source].target.insert(a_target);
               A[a_target].target.insert(a_source);
               // Remove one connection from B network
               B[b_source].target.erase(b_target);
               B[b_target].target.erase(b_source);
               // Check isomorifsm
               if (is_isomorfism(A, B, servers_num)) {
                  int a, b, c, d;
                  b_source < b_target                ? c = b_source,
                                        d = b_target : c = b_target,
                                        d = b_source;
                  a_source < a_target                ? a = a_source,
                                        b = a_target : a = a_target,
                                        b = a_source;
                  printf("%d %d %d %d\n", a, b, c, d);
               }
               // Remove added connection
               A[a_source].target.erase(a_target);
               A[a_target].target.erase(a_source);
               // Add removed connection
               B[b_source].target.insert(b_target);
               B[b_target].target.insert(b_source);
            }
         }
      }
   }

   return 0;
}
