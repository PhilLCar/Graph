#include <graph.h>

#define TYPENAME Graph

Graph *_(Construct)(int vertices)
{
  if (Matrix_Construct(BASE(0), vertices, vertices)) {
    this->labels = NEW (ObjectArray) (TYPEOF (String));

    // Initialize all nodes as separate from one another
    for (int i = 0; i < BASE(0)->rows; i++) {
      for (int j = 0; j < BASE(0)->cols; j++) {
        BASE(0)->base[i][j] = ((i == j) - 1);
      }
    }
  } else {
    this->labels = NULL;
  }

  return this;
}

void _(Destruct)()
{
  DELETE (this->labels);
  Matrix_Destruct(BASE(0));
}

Graph *STATIC (Fill)(int vertices, const char *labels[vertices])
{
  Graph *graph = NEW (Graph) (vertices);

  for (int i = 0; i < vertices; i++) {
    ObjectArray_Push(graph->labels, NEW (String) (labels[i]));
  }

  return graph;
}

Graph *STATIC (FromLabels)(ObjectArray *labels)
{
  Graph *graph = NEW (Graph) (labels->base.size);

  Array_Merge((Array*)graph->labels, (Array*)labels);

  return graph;
}

int _(L)(const char *label)
{
  return ObjectArray_IndexOfKey(this->labels, label);
}

int _(Label)(const String *label)
{
  return ObjectArray_IndexOf(this->labels, label);
}

void _(Set)(int i, int j, double weight)
{
  this->base.base[i][j] = weight;
}

void _(SetL)(const char *li, const char *lj, double weight)
{
  int i = Graph_L(this, li);
  int j = Graph_L(this, lj);

  return Graph_Set(this, i, j, weight);
}

void _(SetLabel)(const String *li, const String *lj, double weight)
{
  int i = Graph_Label(this, li);
  int j = Graph_Label(this, lj);

  return Graph_Set(this, i, j, weight);
}

List *path(Matrix *base, Matrix *current, int step, int i, int j)
{
  List *list = NULL;

  if (current->base[i][j]) {
    list = NEW (List) ();
  } else if (step < current->cols) {
    list = path(base, Matrix_Cross(Matrix_Copy(base), Matrix_Copy(current)), step + 1, i, j);

    if (list) {
      int prev = List_Empty(list) ? i : *(int*)List_Head(list);

      Vec *row = Matrix_Row(base, prev);
      Vec *col = Matrix_Col(current, j);

      for (int k = 0; k < row->dimension; k++) {
        if (row->base[k] && col->base[k])
        {
          list = List_PushValue(list, TYPEOF (int), &k);
          break;
        }
      }

      DELETE (row);
      DELETE (col);
    }
  }

  DELETE (current);

  return list;
}

double _(Weight)(int i, int j)
{
  //Matrix *check = NEW (Matrix)(this->base.rows, this->base.cols);

  return 0;
}

double _(WeightL)(const char *li, const char *lj)
{
  int i = Graph_L(this, li);
  int j = Graph_L(this, lj);

  return Graph_Weight(this, i, j);
}

double _(WeightLabel)(const String *li, const String *lj)
{
  int i = Graph_Label(this, li);
  int j = Graph_Label(this, lj);

  return Graph_Weight(this, i, j);
}

#undef TYPENAME