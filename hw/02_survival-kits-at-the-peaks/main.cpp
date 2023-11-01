#include <iostream>
#include <stack>
#include <stdlib.h>
#include <vector>

// #define PRINT_LOADED_DATA
// #define PRINT_LOWLINK

struct Area {
   std::vector<int> *nodes;
   std::vector<int> *targets;
   // int tracks_cout; can use nodes.size()

   Area(std::vector<int> *nodes, std::vector<int> *targets)
       : nodes(nodes), targets(targets)
   {
   }
};

bool is_point_in_area(int target, std::vector<int> *area)
{
   for (long unsigned int r = 0; r < area->size(); ++r) {
      if (target == area->at(r))
         return true;
   }
   return false;
}

bool is_target_in_area(int target, std::vector<int> *area)
{
   for (long unsigned int r = 0; r < area->size(); ++r) {
      if (target == area->at(r))
         return true;
   }
   return false;
}

void find_scc(int i, std::stack<int> &stack, std::vector<int> &lowlink,
              std::vector<int> &indexes, std::vector<bool> &in_stack,
              std::vector<std::vector<int>> &data, std::vector<Area> &area,
              int &index, int &cp_area, int &cp_no)
{
   indexes[i] = lowlink[i] = ++index;
   stack.push(i);
   in_stack[i] = true;
   for (long unsigned int n = 0; n < data[i].size(); ++n) {
      int target = data[i][n];
      if (indexes[target] == 0) { // not yet visited
         find_scc(target, stack, lowlink, indexes, in_stack, data, area, index,
                  cp_area, cp_no);
         lowlink[i] = std::min(lowlink[i], lowlink[target]);
      } else if (in_stack[target]) {
         lowlink[i] = std::min(lowlink[i], indexes[target]);
      }
   }

   if (lowlink[i] == indexes[i]) { // point is head of SCC (area)
      std::vector<int> *n = new std::vector<int>;
      std::vector<int> *t = new std::vector<int>;
      int k = area.size() - 1;

      int j;
      while (stack.top() != i) {
         j = stack.top();
         in_stack[j] = false;
         stack.pop();

         // Add point to area
         // ++area[k].tracks_cout;
         n->push_back(j);
         if (j == cp_no)
            cp_area = k;
      }
      j = stack.top();
      in_stack[j] = false;
      stack.pop();

      // Add point to area
      // ++area[k].tracks_cout;
      n->push_back(j);
      if (j == cp_no)
         cp_area = k;

      // if target is not in same area add the edge to area list
      // TODO: this is extremly uneffective!
      for (long unsigned int p = 0; p < n->size(); ++p) {
         int tmp_point = n->at(p);
         for (long unsigned int q = 0; q < data[tmp_point].size(); ++q) {
            if (!is_point_in_area(data[tmp_point][q], n) &&
                !is_target_in_area(data[tmp_point][q], t))
               t->push_back(data[tmp_point][q]);
         }
      }
      area.push_back(Area(n, t));
   }
#ifdef PRINT_LOWLINK
   for (long unsigned int l = 0; l < lowlink.size(); ++l)
      printf("%3d ", lowlink.at(l));
   printf("\n");
#endif // end of PRINT_LOWLINK
}

void topological_sort(int i, std::vector<bool> &is_visited,
                      std::stack<int> &stack,
                      std::vector<std::vector<int>> &data,
                      std::vector<Area> &area)
{
   // Mark the current node as visited
   is_visited[i] = true;

   // Recur for all the vertices
   // adjacent to this vertex
   // for(int j )

   // Push current vertex to stack
   // which stores result
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

   std::vector<std::vector<int>> data(points_count);
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

   // TODO: start at CP?

   // define variables
   std::stack<int> stack;
   std::vector<bool> in_stack(points_count, false);
   std::vector<int> lowlink(points_count);
   std::vector<int> indexes(points_count, 0);
   std::vector<Area> area;
   int cp_area;
   int index = 0;

   // Tarjan's Algorithm to fnd CSS
   for (auto i = 0; i < points_count; ++i) {
      if (indexes[i] == 0) // is not visited
      {
         find_scc(i, stack, lowlink, indexes, in_stack, data, area, index,
                  cp_area, cp_no);
         // std::cout << "Start from node: " << i << std::endl;
      }
   }

   int areas_count = (int)area.size();
   // Create topological sort
   std::vector<bool> is_visited(areas_count, false);
   for (auto i = 0; i < areas_count; ++i) {
      if (!is_visited[i])
         // material: https://www.geeksforgeeks.org/topological-sorting/
         topological_sort(i, is_visited, stack, data, area);
   }

   // remove struct
   for (int i = 0; i < areas_count; ++i) {
      delete area[i].nodes;
      delete area[i].targets;
   }

   return 0;
}
