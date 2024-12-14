#include <diagnostic.h>
#include <graph.h>

List *path(Matrix *base, Matrix *current, int step, int i, int j);

int main(void)
{
  CHECK_MEMORY

  double test[5][5] = {
    { 0, 1, 0, 0, 1 },
    { 0, 0, 0, 0, 1 },
    { 0, 1, 0, 0, 0 },
    { 1, 0, 0, 0, 0 },
    { 1, 0, 0, 0, 0 }
  };

  Matrix *ajd = Matrix_Fill(5, 5, test);

  List *test = path(ajd, NULL, 0, 0, 2);

  return 0;
}