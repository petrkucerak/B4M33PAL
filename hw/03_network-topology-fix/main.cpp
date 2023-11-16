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

int compare_vectr_by_num(vector<int> a, vector<int> b)
{
   for (int i = 0; i < a.size(); ++i) {
      if (a[i] > b[i])
         return 1;
      else if (a[i] < b[i])
         return -1;
   }
   return 0;
}

void split2groups(vector<Server> &G, vector<vector<int>> &groups,
                  vector<vector<int>> &a_neighbour_groups, int servers_num)
{
   int num = 0;
   int slow_index = 0;
   int fast_index = servers_num;
   int current_max;
   vector<bool> used(servers_num, false);
   // TODO: use for insted
   while (num < servers_num) {

      for (int i = 0; i < servers_num; ++i) {
         if (used[i])
            continue;
         current_max = i;
         break;
      }
      for (int i = 0; i < servers_num; ++i) {
         if (used[i])
            continue;
         if (compare_vectr_by_num(a_neighbour_groups[i],
                                  a_neighbour_groups[current_max]) == 1) {
            current_max = i;
         }
      }
      if (G[current_max].is_fast) {
         if (groups[fast_index].empty() ||
             compare_vectr_by_num(a_neighbour_groups[groups[fast_index][0]],
                                  a_neighbour_groups[current_max]) == 0) {
            groups[fast_index].push_back(current_max);
         } else {
            fast_index++;
            groups[fast_index].push_back(current_max);
         }
      } else {
         if (groups[slow_index].empty() ||
             compare_vectr_by_num(a_neighbour_groups[groups[slow_index][0]],
                                  a_neighbour_groups[current_max]) == 0) {
            groups[slow_index].push_back(current_max);
         } else {
            slow_index++;
            groups[slow_index].push_back(current_max);
         }
      }
      used[current_max] = true;
      num++;
   }
}

bool group_servers(vector<Server> &A, vector<Server> &B,
                   vector<vector<int>> &a_groups, vector<vector<int>> &b_groups,
                   int servers_num)
{
   vector<vector<int>> a_neighbour_groups(servers_num,
                                          vector<int>(servers_num, 0));
   vector<vector<int>> b_neighbour_groups(servers_num,
                                          vector<int>(servers_num, 0));
   for (int i = 0; i < servers_num; ++i) {
      for (auto a : A[i].target)
         a_neighbour_groups[i][A[a].target.size()]++;
      for (auto b : B[i].target)
         b_neighbour_groups[i][B[b].target.size()]++;
   }

   // Split into groups
   split2groups(A, a_groups, a_neighbour_groups, servers_num);
   split2groups(B, b_groups, b_neighbour_groups, servers_num);

   for (int i = 0; i < a_groups.size(); ++i) {
      if (a_groups[i].size() != b_groups[i].size())
         return false;
   }
   // sort A subnetworks
   for (vector<int> a_group : a_groups)
      sort(a_group.begin(), a_group.end());
   // sort B subnetworks
   for (vector<int> b_group : b_groups)
      sort(b_group.begin(), b_group.end());
   return true;
}

bool validate_mapping(vector<Server> &A, vector<Server> &B, vector<int> &A2B,
                      int servers_num)
{

   for (int a_source = 0; a_source < servers_num; ++a_source) {
      for (int a_targe : A[a_source].target) {
         if (B[A2B[a_source]].target.find(A2B[a_targe]) ==
             B[A2B[a_source]].target.end()) {
            return false;
         }
      }
   }
   return true;
}

bool permutation(vector<Server> &A, vector<Server> &B,
                 vector<vector<int>> &a_groups, vector<vector<int>> &b_groups,
                 int id, vector<int> &A2B, int servers_num)
{
   vector<int> loc_A_group = a_groups[id];
   vector<int> loc_B_group = b_groups[id];
   // TODO: change to for
   while (loc_A_group.empty() && id + 1 < a_groups.size()) {
      id++;
      loc_A_group = a_groups[id];
      loc_B_group = b_groups[id];
   }
   if (id + 1 >= a_groups.size()) {
      do {
         for (int i = 0; i < loc_A_group.size(); ++i) {
            A2B[loc_A_group[i]] = loc_B_group[i];
         }
         if (validate_mapping(A, B, A2B, servers_num))
            return true;
      } while (next_permutation(loc_A_group.begin(), loc_A_group.end()));
   } else {
      do {
         for (int i = 0; i < loc_A_group.size(); ++i) {
            A2B[loc_A_group[i]] = loc_B_group[i];
         }
         if (permutation(A, B, a_groups, b_groups, id + 1, A2B, servers_num))
            return true;
      } while (next_permutation(loc_A_group.begin(), loc_A_group.end()));
   }
   return false;
}

bool is_isomorfism(vector<Server> &A, vector<Server> &B, int servers_num)
{
   bool result = true;
   vector<vector<int>> a_groups(servers_num * 3);
   vector<vector<int>> b_groups(servers_num * 3);

   result &= group_servers(A, B, a_groups, b_groups, servers_num);
   if (!result)
      return false;
   vector<int> A2B(servers_num);
   result &= permutation(A, B, a_groups, b_groups, 0, A2B, servers_num);
   return result;
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
                  a_source < a_target ? (a = a_source, b = a_target)
                                      : (a = a_target, b = a_source);
                  b_source < b_target ? (c = b_source, d = b_target)
                                      : (c = b_target, d = b_source);
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
