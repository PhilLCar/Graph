#include <diagnostic.h>
#include <graph.h>
#include <print.h>
#include <collection.str.h>

List *path(Matrix *base, Matrix *current, int step, int i, int j);

int main(void)
{
  CHECK_MEMORY

  double test[5][5] = {
    { 0, 1, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 1, 0, 1, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 }
  };

  Matrix *ajd = Matrix_Fill(5, 5, test);

  List *list = path(ajd, ajd, 0, 2, 0);

  print("%d\n", List_Size(list));

  print("%Of\n", list);

  CHECK_MEMORY
  STOP_WATCHING

  return 0;
}