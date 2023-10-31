#include <iostream>
#include <stack>
#include <stdlib.h>
#include <vector>

// #define PRINT_LOADED_DATA
#define PRINT_LOWLINK

void find_scc(int i, std::stack<int> &stack, std::vector<int> &lowlink,
              std::vector<int> &indexes, std::vector<std::vector<int>> &data,
              int &areas, int &package, int &index)
{
   indexes[i] = lowlink[i] = index;
   ++index;
   stack.push(i);
   // std::cout << indexes[i] << std::endl;
   for (long unsigned int n = 0; n < data[i].size(); ++n) {
      int target = data[i][n];
      //    std::cout << target << std::endl;
      if (indexes[target] == 0) { // not yet visited
         find_scc(target, stack, lowlink, indexes, data, areas, package, index);
         lowlink[i] = std::min(lowlink[i], lowlink[target]);
      } else {
         lowlink[i] = std::min(lowlink[i], indexes[target]);
      }
   }

   if (lowlink[i] == indexes[i]) { // point is head of SCC (area)
      ++areas;
      std::cout << "Area: " << lowlink[i] << std::endl;
      int j;
      do {
         j = stack.top();
         stack.pop();
         ++package;
         // add j node (point) to the strongly connected component
      } while (i == j); // output the current strongly connected component
   }
#ifdef PRINT_LOWLINK
   if (i == 0) {
      for (long unsigned int l = 0; l < lowlink.size(); ++l)
         printf("%3d ", lowlink.at(l));
      printf("\n");
   }
#endif // end of PRINT_LOWLINK
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
   // implement Tarjan's Algorithm

   // define variables
   std::stack<int> stack;
   std::vector<int> lowlink(points_count);
   std::vector<int> indexes(points_count, 0);
   int index = 0;
   int areas = 0;   // Count of SCC
   int package = 0; // Count of points in the SCC

   for (auto i = 0; i < points_count; ++i) {
      if (indexes[i] == 0) // is not visited
         find_scc(i, stack, lowlink, indexes, data, areas, package, index);
   }

   printf("Areas: %d Packages: %d\n", areas, package);

   return 0;
}
