#include <diagnostic.h>
#include <graph.h>
#include <print.h>
#include <collection.str.h>


/*    .3
B *-------+* C
  |+       +
  | \.2    |
  |  \     |
.8| E *    | .4
  |  +     |
  | /.3    |
  ++       |
A *-------+* D
     .5
*/

List *path(Matrix *base, Matrix *current, int step, int i, int j);

int main(void)
{
  /* NOTE:
   * For now the method "Reaches" and "Weight" use the same algorithm
   * which minimizes the number of edges and not the weight, which is
   * a mistake, eventually, we will need to separate them
   */
  CHECK_MEMORY

  Graph *graph = Graph_Fill(5, (const char*[]){ "A", "B", "C", "D", "E"});

  Graph_SetKey(graph, "A", "D", 0.5);
  Graph_SetKey(graph, "A", "E", 0.3);
  Graph_SetKey(graph, "B", "A", 0.8);
  Graph_SetKey(graph, "B", "C", 0.3);
  Graph_SetKey(graph, "D", "C", 0.4);
  Graph_SetKey(graph, "E", "B", 0.2);
  Graph_SetKey(graph, "E", "A", 0.3);

  print("%[%3g]O\n", graph);

  print("Weight with DC: %g\n", Graph_WeightKey(graph, "A", "C"));

  Graph_SetKey(graph, "D", "C", -1);

  print("Weight without DC: %g\n", Graph_WeightKey(graph, "A", "C"));

  DELETE (graph);

  CHECK_MEMORY
  STOP_WATCHING

  return 0;
}