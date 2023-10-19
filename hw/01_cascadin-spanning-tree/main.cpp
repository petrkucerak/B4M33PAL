#include <queue>
#include <stdio.h>
using namespace std;

struct Edge {
   int target;
   int value;
   Edge(int target, int value) : target(target), value(value) {}
};
struct CompareEdge {
   bool operator()(Edge const &e1, Edge const &e2)
   {
      return e1.value > e2.value;
   }
};

int main(int argc, char const *argv[])
{
   // LOAD DATA
   int number_vertices, number_edges;
   if (scanf("%d %d\n", &number_vertices, &number_edges) != 2)
      fprintf(stderr, "Can not load data\n");

   // create array for pririty queues
   priority_queue<Edge, vector<Edge>, CompareEdge> *data =
       new priority_queue<Edge, vector<Edge>, CompareEdge>[number_vertices];

   // load edges data
   for (int i = 0; i < number_edges; ++i) {
      int vertex, target, value;
      if (scanf("%d %d %d\n", &vertex, &target, &value) != 3)
         fprintf(stderr, "Can not load data\n");
      data[vertex - 1].push(Edge(target - 1, value));
      // save symtricity
      data[target - 1].push(Edge(vertex - 1, value - 1));
   }

   // calcucle minimal cascading

   // free space
   delete[] data;
   return 0;
}
