#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <vector>

using namespace std;

// Define a structure to represent an area in the graph
struct Area {
   set<int> out;    // Outgoing edges from this area
   set<int> in;     // Incoming edges to this area
   set<int> points; // Points within this area
   bool has_CP;     // Flag indicating if this area contains a CP (Checkpoint)
};

// #define TEST_PRINT_SORT

int cp_number; // The checkpoint number

// Depth-First Search (DFS) function to traverse the graph
void dfs(int i, stack<int> &s, vector<vector<int>> &data, vector<int> &visited,
         bool print)
{
   visited[i] = true;
   if (print)
      cout << i << ", ";
   long unsigned int j = 0;
   while (j < data[i].size()) {
      int target = data[i][j];
      if (!visited[target]) {
         dfs(target, s, data, visited, print);
         // After visiting, increment j
         ++j;
      } else {
         // Skip already visited nodes
         ++j;
      }
   }
   if (!print)
      s.push(i);
}

// Depth-First Search (DFS) function to traverse the graph
void dfs_component(int i, vector<vector<int>> &data, vector<int> &visited,
                   Area &comp, int area_number, vector<int> &area_points)
{
   visited[i] = true;
   comp.points.insert(i);
   area_points[i] = area_number;
   if (i == cp_number)
      comp.has_CP = true;
   long unsigned int j = 0;
   while (j < data[i].size()) {
      int target = data[i][j];
      if (!visited[target]) {
         dfs_component(target, data, visited, comp, area_number, area_points);
         // After visiting, increment j
         ++j;
      } else {
         // Skip already visited nodes
         ++j;
      }
   }
}

// Find strongly connected components (SCCs) in the graph
vector<Area> find_scc(vector<vector<int>> &data,
                      vector<vector<int>> &inverse_data,
                      vector<int> &area_points)
{
   // First DFS to push nodes to the stack
   stack<int> s;
   int size = data.size();
   vector<int> visited(size, false);

   for (int i = 0; i < size; ++i) {
      if (!visited[i]) {
         dfs(i, s, data, visited, false);
      }
   }
   // Second DFS on the inverse data
   for (long unsigned int i = 0; i < visited.size(); ++i) {
      visited[i] = false;
   }

   vector<Area> areas;
   int area_number = 0;
   while (!s.empty()) {
      int i = s.top();
      s.pop();
      if (!visited[i]) {
         Area new_area = {.has_CP = false};
         dfs_component(i, inverse_data, visited, new_area, area_number,
                       area_points);
         areas.push_back(new_area);
         area_number++;
      }
   }
   return areas;
}

// Perform topological sorting on the graph
void topological_sort(vector<int> &order, vector<int> &visited,
                      vector<Area> &areas, int i)
{
   visited[i] = true;
   for (long unsigned int j = 0; j < areas[i].out.size(); ++j) {
      int target = *next(areas[i].out.begin(), j);
      if (!visited[target]) {
         topological_sort(order, visited, areas, target);
      }
   }
   order.push_back(i);
}

// Traverse parents of an area in the topological order
void traverse_parents(vector<vector<int>> &parents, int parent,
                      set<int> &result, vector<int> &topological_mask,
                      vector<bool> &visited)
{
   if (parent != -1 && !visited[parent]) {
      for (long unsigned int i = 0; i < parents[parent].size(); ++i) {
         int new_parent = parents[parent][i];
         traverse_parents(parents, new_parent, result, topological_mask,
                          visited);
      }
      result.insert(topological_mask[parent]);
      visited[parent] = true;
   }
}

int main()
{
   // Load metadata
   int points_count, tracks_count;
   if (scanf("%d %d %d\n", &points_count, &tracks_count, &cp_number) != 3) {
      fprintf(stderr, "ERROR: Can not load correctly the meta data!\n");
      exit(EXIT_FAILURE);
   }

   vector<vector<int>> inverse_data(points_count);
   vector<vector<int>> data(points_count);
   // Load data
   int source, target;
   for (int i = 0; i < tracks_count; ++i) {
      scanf("%d %d", &source, &target);
      data[source].push_back(target);
      inverse_data[target].push_back(source); // symetricity
   }

   // Find Strongly Connected Components (SCCs) in the graph
   // Inspiration: https://www.geeksforgeeks.org/strongly-connected-components/
   vector<int> area_points(points_count, -1);
   vector<Area> areas = find_scc(data, inverse_data, area_points);

   // Transform data to areas (Directed Acyclic Graph - DAG)
   int area_number = 0;
   for (long unsigned int i = 0; i < areas.size(); ++i) {
      for (long unsigned int j = 0; j < areas[i].points.size(); ++j) {
         int point = *next(areas[i].points.begin(), j);
         for (long unsigned int k = 0; k < data[point].size(); ++k) {
            int target = data[point][k];
            if (areas[i].points.count(target) == 0) {
               areas[i].out.insert(area_points[target]);
               areas[area_points[target]].in.insert(area_number);
            }
         }
      }
      area_number++;
   }

   // Perform topological sorting
   // inspiration https://www.geeksforgeeks.org/topological-sorting/
   vector<int> topological_mask;
   vector<int> visited(areas.size(), false);
   for (long unsigned int i = 0; i < areas.size(); ++i) {
      if (!visited[i]) {
         topological_sort(topological_mask, visited, areas, i);
      }
   }

   // Swap the order
   reverse(topological_mask.begin(), topological_mask.end());
   vector<int> reversed_order(topological_mask.size());
   for (long unsigned int i = 0; i < topological_mask.size(); ++i) {
      reversed_order[topological_mask[i]] = i;
   }

   // Get the CP index
   int CP_area_number = 0;
   for (long unsigned int i = 0; i < topological_mask.size(); ++i) {
      if (areas[topological_mask[i]].has_CP) {
         CP_area_number = i;
         break;
      }
   }

   set<int> result;
   vector<int> dist(areas.size(), 0);
   vector<vector<int>> parents_target(areas.size(), vector<int>(1, -1));
   dist[0] = 0;
   // Find the trip to CP
   for (int i = 1; i <= CP_area_number; ++i) {
      int max_dist = -1;
      for (long unsigned int j = 0; j < areas[topological_mask[i]].in.size();
           ++j) {
         int edge_in = *next(areas[topological_mask[i]].in.begin(), j);
         int position = reversed_order[edge_in];
         if (dist[position] == max_dist) {
            parents_target[i].push_back(position);
         }
         if (dist[position] > max_dist) {
            max_dist = dist[position];
            parents_target[i].clear();
            parents_target[i].push_back(position);
         }
      }
      max_dist += 1;
      dist[i] = max_dist;
   }

   // Traverse parents
   int max_trip = 0;
   vector<bool> visited_target(areas.size(), false);
   result.insert(topological_mask[CP_area_number]);
   max_trip += dist[CP_area_number];
   for (long unsigned int i = 0; i < parents_target[CP_area_number].size();
        ++i) {
      traverse_parents(parents_target, parents_target[CP_area_number][i],
                       result, topological_mask, visited_target);
   }

   // Find the trip from CP
   vector<vector<int>> parents_source(areas.size());
   dist[CP_area_number] = 0;
   for (int i = areas.size() - 2; i >= CP_area_number; --i) {
      int max_dist = -1;
      vector<int> parent(1, -1);
      for (long unsigned int j = 0; j < areas[topological_mask[i]].out.size();
           ++j) {
         int edge_source = *next(areas[topological_mask[i]].out.begin(), j);
         int position = reversed_order[edge_source];
         if (dist[position] == max_dist) {
            parent.push_back(position);
         }
         if (dist[position] > max_dist) {
            max_dist = dist[position];
            parent.clear();
            parent.push_back(position);
         }
      }
      max_dist += 1;
      dist[i] = max_dist;
      parents_source[i] = parent;
   }
   // Traverse parents from CP
   max_trip += dist[CP_area_number] + 1;
   vector<bool> visited_from(areas.size(), false);
   for (long unsigned int i = 0; i < parents_source[CP_area_number].size();
        ++i) {
      traverse_parents(parents_source, parents_source[CP_area_number][i],
                       result, topological_mask, visited_from);
   }

   int kit_count = 0;
   for (auto &area : result) {
      kit_count += areas[area].points.size();
   }
   printf("%d %d\n", max_trip, kit_count);
   return 0;
}
