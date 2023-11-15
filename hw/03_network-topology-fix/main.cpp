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

   return 0;
}
