#include <iostream>
#include <vector>

using namespace std;

struct Server {
   bool is_fast = false;
   vector<int> to;
};

void print_network(vector<Server> S)
{
   cout << endl;
   for (auto &e : S) {
      cout << e.is_fast << " | ";
      for (auto &c : e.to)
         cout << c << " ";
      cout << endl;
   }
}

int main(int argc, char const *argv[])
{
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
   // Load fast connection for A network
   for (int i = 0; i < planned_connection_num - 1; ++i) {
      int a, b;
      if (scanf("%d %d\n", &a, &b) != 2) {
         fprintf(stderr, "Can't load connection for A network\n");
         exit(EXIT_FAILURE);
      }
      A[a].to.push_back(b);
      A[b].to.push_back(a);
   }

   vector<Server> B(servers_num);
   // Load fast servers for B network
   for (int i = 0; i < fast_servers_num; ++i) {
      int tmp;
      if (scanf("%d", &tmp) != 1) {
         fprintf(stderr, "Can't load list of fast servers for B network\n");
         exit(EXIT_FAILURE);
      }
      B[tmp].is_fast = true;
   }
   // Load fast connection for B network
   for (int i = 0; i < planned_connection_num + 1; ++i) {
      int a, b;
      if (scanf("%d %d\n", &a, &b) != 2) {
         fprintf(stderr, "Can't load connection for B network\n");
         exit(EXIT_FAILURE);
      }
      B[a].to.push_back(b);
      B[b].to.push_back(a);
   }

   print_network(A);
   print_network(B);

   return 0;
}
