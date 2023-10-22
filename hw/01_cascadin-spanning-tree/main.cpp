#include <cstring>
#include <iostream>
#include <limits>
#include <queue>
#include <stdio.h>
using namespace std;

// #define DEBUG_PRINT
#define P_Q priority_queue<Edge, vector<Edge>, CompareEdge>

struct Edge {
   int target;
   int64_t value;
   Edge(int target, int64_t value) : target(target), value(value) {}
};
struct CompareEdge {
   bool operator()(Edge const &e1, Edge const &e2)
   {
      return e1.value > e2.value;
   }
};

struct BFS_Vertex {
   int no;
   int depth;
};

struct Edge_Raw {
   int target;
   int64_t value;
   // bool is_used;
};

struct Vertex {
   bool is_used = false;
   int depth = 0;
   bool visited = false; // for BFS - detect depth from root vertex
};

int added_vertices;
int64_t global_min_weight; // the weight of minimum cascading spanning tree
int64_t local_min_weight;

void markVertex(int i, Vertex *v)
{
   v[i].is_used = true;
   added_vertices++;
}

void printNeigbours(P_Q *n)
{
   printf("PRINTING NEIGHBOUR LIST\n");
   while (!n->empty()) {
      printf("%ld ", n->top().value);
      n->pop();
   }
   printf("\n");
}

void addNeighbours(int i, P_Q *neigbours, vector<Edge_Raw> *edges,
                   Vertex *vertecies)
{
   for (long unsigned int j = 0; j < edges[i].size(); ++j) {
      // is edge added into the P_Q?
      if (!vertecies[edges[i][j].target].is_used &&
          vertecies[i].depth + 1 == vertecies[edges[i][j].target].depth) {
         // added it
         neigbours->push(Edge(edges[i][j].target, edges[i][j].value));
      }
   }
}
void addCascadingNeighbours(int i, P_Q *cascading_neigbours,
                            vector<Edge_Raw> *edges, Vertex *vertecies)
{
   for (long unsigned int j = 0; j < edges[i].size(); ++j) {
      // is edge added into the P_Q?
      if (!vertecies[edges[i][j].target].is_used &&
          vertecies[i].depth == vertecies[edges[i][j].target].depth) {
         // added it
         cascading_neigbours->push(Edge(edges[i][j].target, edges[i][j].value));
      }
   }
}

void addNeighboursBFS(int i, queue<BFS_Vertex> *q, vector<Edge_Raw> *d,
                      int depth)
{
   for (long unsigned int j = 0; j < d[i].size(); ++j) {
      BFS_Vertex e;
      e.depth = depth;
      e.no = d[i][j].target;
      q->push(e);
   }
}

void calculateDepth(int root_id, Vertex *v, vector<Edge_Raw> *d,
                    int number_vertices)
{
   queue<BFS_Vertex> *q = new queue<BFS_Vertex>;
   v[root_id].depth = 0;
   v[root_id].visited = true;
   addNeighboursBFS(root_id, q, d, 0);
   int i = 0;
   while (!q->empty() && number_vertices > i) {
      int el = q->front().no;
      if (!v[el].visited) {
         v[el].depth = q->front().depth + 1;
         v[el].visited = true;
         addNeighboursBFS(el, q, d, v[el].depth);
         ++i;
      }
      q->pop();
   }
   delete q;
}

int main(int argc, char const *argv[])
{
   // LOAD DATA
   int number_vertices, number_edges;
   if (scanf("%d %d\n", &number_vertices, &number_edges) != 2)
      fprintf(stderr, "Can not load data\n");

   // Create data structs
   vector<Edge_Raw> *data = new vector<Edge_Raw>[number_vertices];

   // Load edges data
   for (int i = 0; i < number_edges; ++i) {
      int vertex, target, value;
      if (scanf("%d %d %d\n", &vertex, &target, &value) != 3)
         fprintf(stderr, "Can not load data\n");
      Edge_Raw tmp;
      // tmp.is_used = false;
      tmp.target = target - 1;
      tmp.value = value;
      data[vertex - 1].push_back(tmp);
      // save symtricity
      tmp.target = vertex - 1;
      data[target - 1].push_back(tmp);
   }

   // Calcucle minimal cascading spannig tree
   // 1. trought all vertices
   global_min_weight = INT64_MAX;
   for (int i = 0; i < number_vertices; ++i) {
      // allocated memory for vertex
      Vertex *vertices = new Vertex[number_vertices];
      P_Q *neigbours = new P_Q;
      P_Q *cascading_neighbours = new P_Q;

      // Add starting vertex
      markVertex(i, vertices);
      added_vertices = 1;
      local_min_weight = 0;

      // Calcule depth
      calculateDepth(i, vertices, data, number_vertices);

      // Add neighbours
      addNeighbours(i, neigbours, data, vertices);

#ifdef DEBUG_PRINT
      cout << "Root ID: " << i << endl;
#endif

      while (added_vertices != number_vertices) {
         int vertext_id = neigbours->top().target;
         if (!vertices[vertext_id].is_used) {
#ifdef DEBUG_PRINT
            printf("N: %d(%ld)\n", vertext_id, neigbours->top().value);
#endif
            markVertex(vertext_id, vertices);

            local_min_weight += neigbours->top().value;

            neigbours->pop();

            addNeighbours(vertext_id, neigbours, data, vertices);

            // calcule cacading area
            addCascadingNeighbours(vertext_id, cascading_neighbours, data,
                                   vertices);
            while (!cascading_neighbours->empty()) {
               vertext_id = cascading_neighbours->top().target;
               if (!vertices[vertext_id].is_used) {
#ifdef DEBUG_PRINT
                  printf("C: %d(%ld)\n", vertext_id,
                         cascading_neighbours->top().value);
#endif
                  markVertex(vertext_id, vertices);
                  local_min_weight += cascading_neighbours->top().value;
                  cascading_neighbours->pop();

                  addCascadingNeighbours(vertext_id, cascading_neighbours, data,
                                         vertices);
                  addNeighbours(vertext_id, neigbours, data, vertices);
               } else {
                  cascading_neighbours->pop();
               }
            }

         } else {
            neigbours->pop();
         }
      }
      if (global_min_weight > local_min_weight)
         global_min_weight = local_min_weight;

      delete cascading_neighbours;
      delete neigbours;
      delete[] vertices;

#ifdef DEBUG_PRINT
      cout << "local min: " << local_min_weight << endl << endl;
#endif
   }

   // free space
   delete[] data;
   printf("%ld\n", global_min_weight);
   return 0;
}