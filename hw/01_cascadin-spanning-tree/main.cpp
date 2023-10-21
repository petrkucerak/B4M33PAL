#include <cstring>
#include <iostream>
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

struct BFS_Vertex {
   int no;
   int depth;
};

struct Edge_Raw {
   int target;
   int value;
   bool is_used;
};

struct Vertex {
   bool is_used = false;
   int depth = 0;
   bool visited = false; // for BFS - detect depth from root vertex
};

int added_vertices;
int global_min_weight; // the weight of minimum cascading spanning tree
int local_min_weight;

void markVertex(int i, Vertex *v)
{
   v[i].is_used = true;
   added_vertices++;
}

void printNeigbours(P_Q *n)
{
   printf("PRINTING NEIGHBOUR LIST\n");
   while (!n->empty()) {
      printf("%d ", n->top().value);
      n->pop();
   }
   printf("\n");
}

void addNeighbours(int i, P_Q *neigbours, vector<Edge_Raw> *edges)
{
   for (long unsigned int j = 0; j < edges[i].size(); ++j) {
      // is edge added into the P_Q?
      if (!edges[i][j].is_used) {
         // added it
         neigbours->push(Edge(edges[i][j].target, edges[i][j].value));
         // mark edge as done
         edges[i][j].is_used = true;
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
      tmp.is_used = false;
      tmp.target = target - 1;
      tmp.value = value;
      data[vertex - 1].push_back(tmp);
      // save symtricity
      tmp.target = vertex - 1;
      data[target - 1].push_back(tmp);
   }

   // Print loaded data
   {
      // for (int i = 0; i < number_vertices; ++i) {
      //    printf("%d: ", i+1);
      //    for (int j = 0; j < data[i].size(); ++j) {
      //       printf("%d(%d) ", data[i][j].value, data[i][j].target);
      //    }
      //    printf("\n");
      // }
   }

   // Calcucle minimal cascading spannig tree
   // 1. trought all vertices
   global_min_weight = INT32_MAX;
   for (int i = 0; i < number_vertices; ++i) {
      // allocated memory for vertex
      Vertex *vertices = new Vertex[number_vertices];
      P_Q *neigbours = new P_Q;

      // Add starting vertex
      markVertex(i, vertices);
      added_vertices = 1;
      local_min_weight = 0;

      // Add neighbours
      addNeighbours(i, neigbours, data);

      // Calcule depth
      calculateDepth(i, vertices, data, number_vertices);

      while (added_vertices != number_vertices) {
         if (neigbours->top().value == 9)
            return 0;

         int vertext_id = neigbours->top().target;
         if (!vertices[vertext_id].is_used) {

            markVertex(vertext_id, vertices);

            local_min_weight += neigbours->top().value;

            neigbours->pop();

            addNeighbours(vertext_id, neigbours, data);
         } else {
            neigbours->pop();
         }
      }
      if (global_min_weight > local_min_weight)
         global_min_weight = local_min_weight;

      delete neigbours;
      delete vertices;

      // clean verticies as not used
      for (int i = 0; i < number_vertices; ++i) {
         for (long unsigned int j = 0; j < data[i].size(); ++j) {
            data[i][j].is_used = false;
         }
      }
   }
   // free space
   delete[] data;
   printf("%d\n", global_min_weight);
   return 0;
}
