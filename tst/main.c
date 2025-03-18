#include <diagnostic.h>
#include <graphfile.h>
#include <print.h>
#include <collection.str.h>


/*    .3
B *-------+* C
  ++       +
  | \.2    |
  |  \     |
.8| E *    | .4
  |  +     |
  | /.3    |
  |+       |
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

  Graph *graph = (Graph*)NEW (GraphFile)("tst/graph.json", ACCESS_READ);

  print("%[%3g]O\n", graph);

  if (Graph_ReachesKey(graph, "A", "C")) {
    // Expects: 0.8
    print("Smallest weight: %g\n", Graph_WeightKey(graph, "A", "C"));
  }

  Graph_SetKey(graph, "A", "E", -1);

  if (Graph_ReachesKey(graph, "A", "C")) {
    // Expects: 0.9
    print("Weight without AE: %g\n", Graph_WeightKey(graph, "A", "C"));
  }

  Graph_SetKey(graph, "D", "C", -1);

  if (Graph_ReachesKey(graph, "A", "C")) {
    // Expects: 0.11
    print("Weight without DC: %g\n", Graph_WeightKey(graph, "A", "C"));
  }

  Graph_SetKey(graph, "A", "B", -1);

  if (Graph_ReachesKey(graph, "A", "C")) {
    // Should not print (doesn't reach)
    print("Weight without AB: %g\n", Graph_WeightKey(graph, "A", "C"));
  }

  DELETE (graph);

  CHECK_MEMORY
  STOP_WATCHING

  return 0;
}