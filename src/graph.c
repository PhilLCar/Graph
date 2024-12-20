#include <graph.h>

#define TYPENAME Graph

////////////////////////////////////////////////////////////////////////////////
Graph *_(Construct)(int vertices)
{
  if (Matrix_Construct(BASE(0), vertices, vertices)) {
    this->labels = NEW (Set) (TYPEOF (String));

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

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  DELETE (this->labels);
  Matrix_Destruct(BASE(0));
}

////////////////////////////////////////////////////////////////////////////////
Graph *STATIC (Fill)(int vertices, const char *labels[vertices])
{
  Graph *graph = NEW (Graph) (vertices);

  for (int i = 0; i < vertices; i++) {
    Set_Add(graph->labels, NEW (String) (labels[i]));
  }

  return graph;
}

////////////////////////////////////////////////////////////////////////////////
Graph *STATIC (FromLabels)(void *enumerable)
{
  Set *labels = NEW (Set) (TYPEOF (String));

  for (Iterator *it = NEW (Iterator)(enumerable); !done(it); next(it)) {
    Set_Add(labels, String_Copy(it->base));
  }

  Graph *graph = NEW (Graph) (((Array*)labels)->size);

  DELETE (graph->labels);
  graph->labels = labels;

  return graph;
}

////////////////////////////////////////////////////////////////////////////////
int _(Key)(const char *label)
{
  return Set_ContainsKey(this->labels, label);
}

////////////////////////////////////////////////////////////////////////////////
int _(Label)(const String *label)
{
  return Set_Contains(this->labels, label);
}

////////////////////////////////////////////////////////////////////////////////
String *_(NameOf)(int number)
{
  return ObjectArray_At((ObjectArray*)this->labels, number);
}

////////////////////////////////////////////////////////////////////////////////
int _(AddKey)(const char *label)
{
  return Graph_AddLabel(this, NEW (String)(label));
}

////////////////////////////////////////////////////////////////////////////////
int _(AddLabel)(String *label)
{
  int index = Set_Add(this->labels, label);

  if (index >= 0) {
    Matrix old = *BASE(0);

    Matrix_Construct(BASE(0), old.rows + 1, old.cols + 1);

    for (int i = 0, m = 0; i < old.rows; i++) {
      if (i == index) ++m;
      for (int j = 0, n = 0; j < old.cols; j++) {
        if (j == index) ++n;
        this->base.base[i + m][j + n] = old.base[i][j];
      }
    }

    for (int i = 0; i < this->base.rows; i++) {
      this->base.base[i][index] = (i == index) - 1;
    }
    
    for (int j = 0; j < this->base.cols; j++) {
      this->base.base[index][j] = (j == index) - 1;
    }

    Matrix_Destruct(&old);
  } else {
    DELETE (label);
  }

  return BASE(0)->rows;
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveKey)(const char *label)
{
  int index = Set_ContainsKey(this->labels, label);

  if (index >= 0 && Set_RemoveKey(this->labels, label)) {
    Matrix *new, *old = talloc(TYPEOF(Matrix));

    *old = *BASE(0);
    new  = Matrix_Rem(old, index, index);

    memcpy(this, new, sizeof(Matrix));

    tfree(new);
  }
}

////////////////////////////////////////////////////////////////////////////////
void _(RemoveLabel)(const String *label)
{
  Graph_RemoveKey(this, label->base);
}

////////////////////////////////////////////////////////////////////////////////
void _(Set)(int i, int j, double weight)
{
  BASE(0)->base[i][j] = weight;
}

////////////////////////////////////////////////////////////////////////////////
void _(SetKey)(const char *li, const char *lj, double weight)
{
  int i = Graph_Key(this, li);
  int j = Graph_Key(this, lj);

  return Graph_Set(this, i, j, weight);
}

////////////////////////////////////////////////////////////////////////////////
void _(SetLabel)(const String *li, const String *lj, double weight)
{
  int i = Graph_Label(this, li);
  int j = Graph_Label(this, lj);

  return Graph_Set(this, i, j, weight);
}

////////////////////////////////////////////////////////////////////////////////
Matrix *_(AdjacencyMatrix)()
{
  Matrix *m = NEW (Matrix) (BASE(0)->rows, BASE(0)->cols);

  for (int i = 0; i < m->rows; i++) {
    for (int j = 0; j < m->cols; j++) {
      m->base[i][j] = BASE(0)->base[i][j] >= 0;
    }
  }

  return m;
}


/******************************************************************************/
List *STATIC (nodes)(Matrix *base, Matrix *current, int step, int i, int j)
{
  List *list = NULL;

  // Initialize current with the base
  current = IFNULL(current, base);

  if (current->base[i][j]) {
    list = NEW (List) ();
  } else if (step < current->cols) {
    list = Graph_nodes(base, Matrix_Cross(Matrix_Copy(base), Matrix_Copy(current)), step + 1, i, j);

    if (list) {
      int prev = List_Empty(list) ? i : *(int*)List_Head(list);

      Vec *row = Matrix_Row(base, prev);
      Vec *col = Matrix_Col(current, j);

      for (int k = 0; k < row->dimension; k++) {
        if (row->base[k] && col->base[k])
        {
          List_AddValue(list, TYPEOF (int), &k);
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

////////////////////////////////////////////////////////////////////////////////
double _(Weight)(int i, int j)
{
  double  weight      = -1;
  List   *throughpath = Graph_nodes(Graph_AdjacencyMatrix(this), NULL, 0, i, j);

  if (throughpath) {
    int last = i;

    weight = 0;

    for (List *l = throughpath; !List_Empty(l); l = List_Next(l)) {
      int next = *(int*)List_Head(l);

      weight += BASE(0)->base[last][next];
      last    = next;
    }

    weight += BASE(0)->base[last][j];

    DELETE (throughpath);
  }

  return weight;
}

////////////////////////////////////////////////////////////////////////////////
double _(WeightKey)(const char *li, const char *lj)
{
  int i = Graph_Key(this, li);
  int j = Graph_Key(this, lj);

  return Graph_Weight(this, i, j);
}

////////////////////////////////////////////////////////////////////////////////
double _(WeightLabel)(const String *li, const String *lj)
{
  int i = Graph_Label(this, li);
  int j = Graph_Label(this, lj);

  return Graph_Weight(this, i, j);
}

////////////////////////////////////////////////////////////////////////////////
int _(Reaches)(int i, int j)
{
  List *throughpath = Graph_nodes(Graph_AdjacencyMatrix(this), NULL, 0, i, j);
  int   reaches     = throughpath != NULL;

  DELETE (throughpath);

  return reaches;
}

////////////////////////////////////////////////////////////////////////////////
int _(ReachesKey)(const char *li, const char *lj)
{
  int i = Graph_Key(this, li);
  int j = Graph_Key(this, lj);

  return Graph_Weight(this, i, j);
}

////////////////////////////////////////////////////////////////////////////////
int _(ReachesLabel)(const String *li, const String *lj)
{
  int i = Graph_Label(this, li);
  int j = Graph_Label(this, lj);

  return Graph_Weight(this, i, j);
}

#undef TYPENAME