#include <algorithm>
#include <iostream>
#include <unordered_set>
#include <vector>

using namespace std;

struct Atom {
   unordered_set<int> target;
   bool is_fast = false;
};

int compare_vectr_by_num(vector<int> a, vector<int> b)
{
   long unsigned int i = 0;
   while (i < a.size()) {
      if (a[i] > b[i])
         return 1;
      else if (a[i] < b[i])
         return -1;
      ++i;
   }
   return 0;
}

void split2groups(vector<Atom> &G, vector<vector<int>> &groups,
                  vector<vector<int>> &a_neighbour_groups, int servers_num)

{
   // const int COMPARE_LESS = -1; // unused
   const int COMPARE_EQUAL = 0;
   const int COMPARE_GREATER = 1;
   int normal_id = 0;
   int fast_id = servers_num;
   std::vector<bool> used(servers_num, false);
   for (int serverIndex = 0; serverIndex < servers_num; ++serverIndex) {
      auto it = std::find(used.begin(), used.end(), false);
      if (it == used.end()) {
         break; // All servers are used
      }
      int current_max = std::distance(used.begin(), it);
      for (int i = 0; i < servers_num; ++i) {
         if (!used[i] &&
             compare_vectr_by_num(a_neighbour_groups[i],
                                  a_neighbour_groups[current_max]) ==
                 COMPARE_GREATER) {
            current_max = i;
         }
      }
      int &current_id = G[current_max].is_fast ? fast_id : normal_id;
      auto &current_group = groups[current_id];

      if (current_group.empty() ||
          compare_vectr_by_num(a_neighbour_groups[current_group[0]],
                               a_neighbour_groups[current_max]) ==
              COMPARE_EQUAL) {
         current_group.push_back(current_max);
      } else {
         current_id++;
         groups[current_id].push_back(current_max);
      }
      used[current_max] = true;
   }
}

bool group_atoms(vector<Atom> &A, vector<Atom> &B,
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

   for (long unsigned int i = 0; i < a_groups.size(); ++i) {
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

bool validate_mapping(vector<Atom> &A, vector<Atom> &B, vector<int> &A2B,
                      int servers_num)
{

   for (int a_source = 0; a_source < servers_num; ++a_source) {
      auto &a_source_targets = A[a_source].target;
      auto &b_source_targets = B[A2B[a_source]].target;
      for (int a_target : a_source_targets) {
         if (b_source_targets.find(A2B[a_target]) == b_source_targets.end()) {
            return false;
         }
      }
   }

   return true;
}

bool permutation(vector<Atom> &A, vector<Atom> &B,
                 vector<vector<int>> &a_groups, vector<vector<int>> &b_groups,
                 int id, vector<int> &A2B, int servers_num)
{
   vector<int> loc_A_group = a_groups[id];
   vector<int> loc_B_group = b_groups[id];
   while (loc_A_group.empty() &&
          (long unsigned int)(id + 1) < a_groups.size()) {
      id++;
      loc_A_group = a_groups[id];
      loc_B_group = b_groups[id];
   }
   if ((long unsigned int)(id + 1) >= a_groups.size()) {
      do {
         for (int i = 0; (long unsigned int)i < loc_A_group.size(); ++i) {
            A2B[loc_A_group[i]] = loc_B_group[i];
         }
         if (validate_mapping(A, B, A2B, servers_num))
            return true;
      } while (next_permutation(loc_A_group.begin(), loc_A_group.end()));
   } else {
      do {
         for (long unsigned int i = 0; i < loc_A_group.size(); ++i) {
            A2B[loc_A_group[i]] = loc_B_group[i];
         }
         if (permutation(A, B, a_groups, b_groups, id + 1, A2B, servers_num))
            return true;
      } while (next_permutation(loc_A_group.begin(), loc_A_group.end()));
   }
   return false;
}

bool is_isomorfism(vector<Atom> &A, vector<Atom> &B, int count_atoms)
{
   bool result = true;
   vector<vector<int>> a_groups(count_atoms * 3);
   vector<vector<int>> b_groups(count_atoms * 3);

   result &= group_atoms(A, B, a_groups, b_groups, count_atoms);
   if (!result)
      return false;
   vector<int> A2B(count_atoms);
   result &= permutation(A, B, a_groups, b_groups, 0, A2B, count_atoms);

   return result;
}

int main(int argc, char const *argv[])
{
   // Load the metadata
   int count_mulecules, count_atoms, count_bonds;
   if (scanf("%d %d %d\n", &count_mulecules, &count_atoms, &count_bonds) != 3) {
      fprintf(stderr, "ERROR - can't load the metadata!\n");
      exit(EXIT_FAILURE);
   }
   // printf("M: %d, A: %d, B: %d\n", count_mulecules, count_atoms,
   // count_bonds);

   // Load the data
   vector<vector<Atom>> molecules(count_mulecules, vector<Atom>(count_atoms));
   for (auto &molecule : molecules) {
      for (int i = 0; i < count_bonds; ++i) {
         int a, b;
         if (scanf("%d %d\n", &a, &b) != 2) {
            fprintf(stderr, "ERROR - can't load the data!\n");
            exit(EXIT_FAILURE);
         }
         a -= 1;
         b -= 1;
         molecule[a].target.insert(b);
         molecule[b].target.insert(a);
      }
   }

   // Map ordered molecules
   vector<bool> is_ordered(count_mulecules, false);
   vector<int> group_counts;

   for (int i = 0; i < count_mulecules; ++i) {
      // If source molecule is orderd, skip it
      if (is_ordered[i])
         continue;

      // Create new class
      is_ordered[i] = true;
      group_counts.push_back(1);

      for (int j = i + 1; j < count_mulecules; ++j) {
         // If molecule is orderd, skip it
         if (is_ordered[j])
            continue;

         // Valid isomorfism
         if (is_isomorfism(molecules[i], molecules[j], count_atoms)) {
            is_ordered[j] = true;
            // incereas last index
            group_counts.back() += 1;
         }
      }
   }

   sort(group_counts.begin(), group_counts.end(),
        [](const int &a, const int &b) { return a > b ? false : true; });

   // Print results
   for (int i = 0; i < group_counts.size(); ++i) {
      if (i == 0)
         cout << group_counts[i];
      else
         cout << " " << group_counts[i];
   }
   cout << endl;

   return 0;
}
