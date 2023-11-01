#include <algorithm>
#include <iostream>
#include <set>
#include <stack>
#include <stdio.h>
#include <vector>

using namespace std;

struct Area {
   set<int> out;
   set<int> in;
   set<int> points;
   bool has_CP;
};

int cp_no;

void through_parents(vector<vector<int>> &parents, int parent, set<int> &res,
                     vector<int> &topological_mask, vector<bool> &visited)
{
   if (parent != -1 && !visited[parent]) {
      for (auto &newparent : parents[parent]) {
         through_parents(parents, newparent, res, topological_mask, visited);
      }
      res.insert(topological_mask[parent]);
      visited[parent] = true;
   }
}

void topological_sort(vector<int> &order, vector<int> &visited,
                      vector<Area> &areas, int i)
{
   visited[i] = true;
   for (auto &target : areas[i].out) {
      if (!visited[target]) {
         topological_sort(order, visited, areas, target);
      }
   }
   order.push_back(i);
}

void dfs_component(int i, vector<vector<int>> &data, vector<int> &visited,
                   Area &comp, int area_no, vector<int> &area_points)
{
   visited[i] = true;
   comp.points.insert(i);
   area_points[i] = area_no;
   if (i == cp_no)
      comp.has_CP = true;
   for (auto &target : data[i]) {
      if (!visited[target])
         dfs_component(target, data, visited, comp, area_no, area_points);
   }
}

void dfs(int i, stack<int> &s, vector<vector<int>> &data, vector<int> &visited,
         bool print)
{
   visited[i] = true;
   if (print)
      cout << i << ", ";
   for (auto &target : data[i])
      if (!visited[target])
         dfs(target, s, data, visited, print);
   if (!print)
      s.push(i);
}

vector<Area> find_scc(vector<vector<int>> &data,
                      vector<vector<int>> &inverse_data,
                      vector<int> &area_points)
{
   // first DFS, push to stack
   stack<int> s;
   int size = data.size();
   vector<int> visited(size, false);

   for (int i = 0; i < size; ++i) {
      if (!visited[i]) {
         dfs(i, s, data, visited, false);
      }
   }
   // second DFS on inverse_data
   for (int i = 0; i < visited.size(); ++i) {
      visited[i] = false;
   }

   vector<Area> areas;
   int area_no = 0;
   while (!s.empty()) {
      int i = s.top();
      s.pop();
      if (!visited[i]) {
         Area new_area = {.has_CP = false};
         dfs_component(i, inverse_data, visited, new_area, area_no,
                       area_points);
         areas.push_back(new_area);
         area_no++;
      }
   }
   return areas;
}

int main()
{
   // Load metadata
   int points_count, tracks_count;
   if (scanf("%d %d %d\n", &points_count, &tracks_count, &cp_no) != 3) {
      fprintf(stderr, "ERROR: Can not load correctly the meta data!\n");
      exit(EXIT_FAILURE);
   }

   vector<vector<int>> data(points_count);
   vector<vector<int>> inverse_data(points_count);
   // Load data
   int source, target;
   for (int i = 0; i < tracks_count; ++i) {
      scanf("%d %d", &source, &target);
      data[source].push_back(target);
      inverse_data[target].push_back(source);
   }

   // get CSS
   vector<int> area_points(points_count, -1);
   vector<Area> areas = find_scc(data, inverse_data, area_points);

   // Trasnform data to areas (DAG)
   int area_no = 0;
   for (auto &comp : areas) {
      for (auto &i : comp.points) {
         for (auto &target : data[i]) {
            if (comp.points.count(target) == 0) {
               comp.out.insert(area_points[target]);
               areas[area_points[target]].in.insert(area_no);
            }
         }
      }
      area_no++;
   }

   // Topological sorting
   // inspiration https://www.geeksforgeeks.org/topological-sorting/
   vector<int> topological_mask;
   vector<int> visited(areas.size(), false);
   for (int i = 0; i < areas.size(); ++i) {
      if (!visited[i]) {
         topological_sort(topological_mask, visited, areas, i);
      }
   }

   // swap order
   reverse(topological_mask.begin(), topological_mask.end());
   vector<int> reversed_order(topological_mask.size());
   for (int i = 0; i < topological_mask.size(); ++i) {
      reversed_order[topological_mask[i]] = i;
   }
   // get CP index
   int CP_area_no = 0;
   for (int i = 0; i < topological_mask.size(); ++i) {
      if (areas[topological_mask[i]].has_CP) {
         CP_area_no = i;
         break;
      }
   }

   set<int> res;
   vector<int> dist(areas.size(), 0);
   vector<vector<int>> parents_targe(areas.size(), vector<int>(1, -1));
   dist[0] = 0;
   // find trip to CP
   for (int i = 1; i <= CP_area_no; ++i) {
      int max_dist = -1;
      for (auto &edgeIn : areas[topological_mask[i]].in) {
         int position = reversed_order[edgeIn];
         if (dist[position] == max_dist) {
            parents_targe[i].push_back(position);
         }
         if (dist[position] > max_dist) {
            max_dist = dist[position];
            parents_targe[i].clear();
            parents_targe[i].push_back(position);
         }
      }
      max_dist += 1;
      dist[i] = max_dist;
   }

   // traversal of parents
   int max_trip = 0;
   vector<bool> visited_target(areas.size(), false);
   res.insert(topological_mask[CP_area_no]);
   max_trip += dist[CP_area_no];
   for (auto &parent : parents_targe[CP_area_no]) {
      through_parents(parents_targe, parent, res, topological_mask,
                      visited_target);
   }

   // find trip from CP
   dist[CP_area_no] = 0;
   vector<vector<int>> parents_source(areas.size());
   for (int i = areas.size() - 2; i >= CP_area_no; --i) {
      int max_dist = -1;
      vector<int> parent(1, -1);
      for (auto &edge_source : areas[topological_mask[i]].out) {
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
   // traversal of parents
   max_trip += dist[CP_area_no] + 1;
   vector<bool> visitedFrom(areas.size(), false);
   for (auto &parent : parents_source[CP_area_no]) {
      through_parents(parents_source, parent, res, topological_mask,
                      visitedFrom);
   }

   int kit_count = 0;
   for (auto &area : res) {
      kit_count += areas[area].points.size();
   }
   cout << max_trip << " " << kit_count << endl;
   return 0;
}