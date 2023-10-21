#include <cstring>
#include <queue>
#include <stdio.h>
using namespace std;

#define P_Q priority_queue<Edge, vector<Edge>, CompareEdge>

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

struct Vertex {
   bool is_used = false;
};

int added_vertices;
int global_min_weight; // the weight of minimum cascading spanning tree

void markVertex(int i, Vertex *v)
{
   v[i].is_used = true;
   added_vertices++;
}

void addNeighbours(int i, P_Q *neigbours, P_Q *edges)
{
   // is edge added into the P_Q?

   // if not add it
}

int main(int argc, char const *argv[])
{
   // LOAD DATA
   int number_vertices, number_edges;
   if (scanf("%d %d\n", &number_vertices, &number_edges) != 2)
      fprintf(stderr, "Can not load data\n");

   // Create array for pririty queues
   P_Q *data = new P_Q[number_vertices];

   // Load edges data
   for (int i = 0; i < number_edges; ++i) {
      int vertex, target, value;
      if (scanf("%d %d %d\n", &vertex, &target, &value) != 3)
         fprintf(stderr, "Can not load data\n");
      data[vertex - 1].push(Edge(target - 1, value));
      // save symtricity
      data[target - 1].push(Edge(vertex - 1, value - 1));
   }

   // Calcucle minimal cascading spannig tree
   // 1. trought all vertices
   for (int i = 0; i < number_vertices; ++i) {
      // allocated memory for vertex
      Vertex *vertices = new Vertex[number_vertices];
      P_Q *neigbours = new P_Q;

      // Add starting vertex
      markVertex(i, vertices);
      added_vertices = 0;
      global_min_weight = 0;

      // Add neighbours
      addNeighbours(i, neigbours, data);

      while (added_vertices != number_vertices) {
         // 2. find minimum edge
         // mark vertex as used
         vertices[i].is_used = true;
         // get cheaper edge cost
         // int cheapest_edge;
      }
      delete neigbours;
   }
   // free space
   delete[] data;
   return 0;
}
