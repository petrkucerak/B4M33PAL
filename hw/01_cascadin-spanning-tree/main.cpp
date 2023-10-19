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
   // load data
   int number_vertices, number_edges;
   if (scanf("%d %d\n", &number_vertices, &number_edges) != 2)
      fprintf(stderr, "Can not load data\n");

   printf("Loaded data %d %d\n", number_vertices, number_edges);

   priority_queue<Edge, vector<Edge>, CompareEdge> q;
   for (int i = 0; i < 20; ++i) {
      q.push(Edge(0, 20 - i));
   }
   for (int i = 0; i < 20; ++i) {
      Edge tmp = q.top();
      q.pop();
      printf("%d ", tmp.value);
   }
   printf("\n");

   // calcucle minimal cascading
   return 0;
}
