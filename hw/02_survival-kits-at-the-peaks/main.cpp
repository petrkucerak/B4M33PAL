#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <stdlib.h>
#include <vector>

using namespace std;

// #define PRINT_LOADED_DATA
// #define PRINT_LOWLINK

struct Area {
   set<int> in;
   set<int> out;
   set<int> points;
   bool cointains_CP;
};

void find_scc(int i, stack<int> &stack, vector<int> &lowlink,
              vector<int> &indexes, vector<bool> &in_stack,
              vector<vector<int>> &data, vector<Area> &areas, int &index,
              int &cp_no)
{
   indexes[i] = lowlink[i] = ++index;
   stack.push(i);
   in_stack[i] = true;
   for (long unsigned int n = 0; n < data[i].size(); ++n) {
      int target = data[i][n];
      if (indexes[target] == 0) { // not yet visited
         find_scc(target, stack, lowlink, indexes, in_stack, data, areas, index,
                  cp_no);
         lowlink[i] = min(lowlink[i], lowlink[target]);
      } else if (in_stack[target]) {
         lowlink[i] = min(lowlink[i], indexes[target]);
      }
   }

   if (lowlink[i] == indexes[i]) { // point is head of SCC (area)
      Area new_area = {.cointains_CP = false};
      int j;
      while (stack.top() != i) {
         j = stack.top();
         in_stack[j] = false;
         stack.pop();

         new_area.points.insert(j);
         if (j == cp_no)
            new_area.cointains_CP = true;
      }
      j = stack.top();
      in_stack[j] = false;
      stack.pop();

      new_area.points.insert(j);
      if (j == cp_no)
         new_area.cointains_CP = true;

      areas.push_back(new_area);
   }

#ifdef PRINT_LOWLINK
   for (long unsigned int l = 0; l < lowlink.size(); ++l)
      printf("%3d ", lowlink.at(l));
   printf("\n");
#endif // end of PRINT_LOWLINK
}

void topological_sort(int i, vector<bool> &visited,
                      vector<int> &topological_order, vector<Area> &areas)
{
   visited[i] = true;
   for (auto &target : areas[i].out)
      if (!visited[target])
         topological_sort(target, visited, topological_order, areas);
   topological_order.push_back(i);
}

void through_parents(int i, vector<bool> &visited_parents,
                     vector<vector<int>> &parents, set<int> &tmp_areas,
                     vector<int> &topological_order)
{
   if (i != -1 && !visited_parents[i]) {
      for (auto &j : parents[i])
         through_parents(j, visited_parents, parents, tmp_areas,
                         topological_order);
      tmp_areas.insert(topological_order[i]);
      visited_parents[i] = true;
   }
}

int main(int argc, char const *argv[])
{
   // Load metadata
   int points_count, tracks_count, cp_no;
   if (scanf("%d %d %d\n", &points_count, &tracks_count, &cp_no) != 3) {
      fprintf(stderr, "ERROR: Can not load correctly the meta data!\n");
      exit(EXIT_FAILURE);
   }

   printf("Points count: %d | Tracks count: %d | CP no: %d\n", points_count,
          tracks_count, cp_no);

   vector<vector<int>> data(points_count);
   for (auto i = 0; i < tracks_count; ++i) {
      int node, target;
      if (scanf("%d %d\n", &node, &target) != 2) {
         fprintf(stderr, "ERROR: Can not load correctly the graph data!\n");
         exit(EXIT_FAILURE);
      };
      data[node].push_back(target); // TODO: make it effective
   }

#ifdef PRINT_LOADED_DATA
   for (auto i = 0; i < points_count; ++i) {
      printf("%4d | ", i);
      for (auto j = 0; j < data[i].size(); ++j) {
         printf("%d ", data[i][j]);
      }
      printf("\n");
   }
#endif // end of PRINT_LOADED_DATA

   // Tarjan's Algorithm to get CSS
   vector<Area> areas;
   stack<int> stack;
   vector<bool> in_stack(points_count, false);
   vector<int> lowlink(points_count);
   vector<int> indexes(points_count, 0);
   int index = 0;
   for (auto i = 0; i < points_count; ++i) {
      if (indexes[i] == 0) // is not visited
      {
         find_scc(i, stack, lowlink, indexes, in_stack, data, areas, index,
                  cp_no);
         // std::cout << "Start from node: " << i << std::endl;
      }
   }

   // Create areas mask
   vector<int> areas_mask(points_count, -1);
   for (auto i = 0; i < areas.size(); ++i) {
      for (auto &point : areas[i].points) {
         areas_mask[point] = i;
      }
   }

   // Trasnform data to areas (DAG)
   int comp_id = 0;
   for (auto &area : areas) {
      for (auto &point : area.points) {
         for (auto &to : data[point]) {
            if (area.points.count(to) == 0) { // edge is not added
               area.out.insert(areas_mask[to]);
               areas[areas_mask[to]].in.insert(areas_mask[comp_id]);
            }
         }
      }
      ++comp_id;
   }

   const int areas_count = areas.size();

   // Make DAG topological ordered
   vector<int> topological_order;
   vector<bool> visited(areas_count, false);

   // Topological sorting
   // inspiration https://www.geeksforgeeks.org/topological-sorting/
   for (int i = 0; i < areas_count; ++i) {
      if (!visited[i])
         topological_sort(i, visited, topological_order, areas);
   }

   reverse(topological_order.begin(), topological_order.end());

   vector<int> reverse_order(topological_order.size());
   for (int i = 0; i < topological_order.size(); ++i) {
      reverse_order[topological_order[i]] = i;
   }

   // find CP
   int CP_area_no;
   for (int i = 0; i < topological_order.size(); ++i)
      if (areas[topological_order[i]].cointains_CP) {
         CP_area_no = i;
         break;
      }

   set<int> tmp_areas;
   vector<vector<int>> parents(areas_count, vector<int>(1, -1));
   vector<int> distan(areas_count, 0); // distance
   distan[0] = 0;

   // find trip to CP
   int max;
   for (int i = 1; i <= CP_area_no; ++i) {
      max = -1;
      for (auto &track_in : areas[topological_order[i]].in) {
         int pos = reverse_order[track_in];
         if (distan[pos] == max)
            parents[i].push_back(pos);
         if (distan[pos] > max) {
            max = distan[pos];
            parents[i].clear();
            parents[i].push_back(pos);
         }
      }
      max += 1;
      distan[i] = max;
   }

   int max_track = 0;
   vector<bool> visited_parents(areas_count, false);
   tmp_areas.insert(topological_order[CP_area_no]);
   max_track += distan[CP_area_no];
   for (auto &parent : parents[CP_area_no])
      through_parents(parent, visited_parents, parents, tmp_areas,
                      topological_order);

   // find trip from CP
   distan[CP_area_no] = 0;
   vector<vector<int>> parents_r(areas.size());
   for (int i = areas_count - 2; i >= CP_area_no; --i) {
      max = -1;
      vector<int> par(1, -1);
      for (auto &track_from : areas[topological_order[i]].out) {
         int pos = reverse_order[track_from];
         if (distan[pos] == max)
            par.push_back(pos);
         if (distan[pos] > max) {
            max = distan[pos];
            par.clear();
            par.push_back(pos);
         }
      }
      max += 1;
      distan[i] = max;
      parents_r[i] = par;
   }
   max_track += distan[CP_area_no] + 1;
   for (int i = 0; i < visited_parents.size(); ++i)
      visited_parents[i] = false;
   for (auto &parent : parents_r[CP_area_no])
      through_parents(parent, visited_parents, parents_r, tmp_areas,
                      topological_order);

   int kit_count = 0;
   for (int i = 0; i < tmp_areas.size(); ++i)
      kit_count += areas[i].points.size();
   printf("%d %d\n", max_track, kit_count);

   return 0;
}
