#include <iostream>
#include <stack>
#include <stdlib.h>
#include <vector>

// #define PRINT_LOADED_DATA
// #define PRINT_LOWLINK

struct Area {
   std::vector<int> *nodes;
   // int tracks_cout; can use nodes.size()

   Area(std::vector<int> *nodes /*, int tracks_cout*/)
       : nodes(nodes) /*,tracks_cout(tracks_cout)*/
   {
   }
};

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
      std::vector<int> *t = new std::vector<int>;
      area.push_back(Area(t));
      int k = area.size() - 1;

      int j;
      while (stack.top() != i) {
         j = stack.top();
         in_stack[j] = false;
         stack.pop();

         // Add point to area
         // ++area[k].tracks_cout;
         area[k].nodes->push_back(j);
         if (j == cp_no)
            cp_area = k;
      }
      j = stack.top();
      in_stack[j] = false;
      stack.pop();

      // Add point to area
      // ++area[k].tracks_cout;
      area[k].nodes->push_back(j);
      if (j == cp_no)
         cp_area = k;
   }
#ifdef PRINT_LOWLINK
   for (long unsigned int l = 0; l < lowlink.size(); ++l)
      printf("%3d ", lowlink.at(l));
   printf("\n");
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
   std::vector<bool> in_stack(points_count, false);
   std::vector<int> lowlink(points_count);
   std::vector<int> indexes(points_count, 0);
   std::vector<Area> area;
   int cp_area;
   int index = 0;

   for (auto i = 0; i < points_count; ++i) {
      if (indexes[i] == 0) // is not visited
      {
         find_scc(i, stack, lowlink, indexes, in_stack, data, area, index,
                  cp_area, cp_no);
         // std::cout << "Start from node: " << i << std::endl;
      }
   }

   // remove struct
   for (long unsigned int i = 0; i < area.size(); ++i) {
      delete area[i].nodes;
   }

   return 0;
}
