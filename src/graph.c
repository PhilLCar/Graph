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
  int added = this->labels->base.base.size;
  int index = Set_Add(this->labels, label);

  added -= this->labels->base.base.size;

  if (added) {
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

#undef TYPENAME
/*============================================================================*/
/* PRIVATE CLASS                                                              */
/*============================================================================*/
#define TYPENAME Head

/******************************************************************************/
OBJECT (Matrix *state, int vertex, double distance) NOBASE
  Matrix *state;
  int     vertex;
  double  distance;
END_OBJECT(NULL, 0, 0.0);

/******************************************************************************/
Head *_(Construct)(Matrix *state, int vertex, double distance)
{
  if (this) {
    this->state    = Matrix_Copy(state);
    this->vertex   = vertex;
    this->distance = distance;
  }

  return this;
}

/******************************************************************************/
Head *_(Split)(int vertex)
{
  Head   *new      = NULL;
  double  distance = this->state->base[this->vertex][vertex];

  if (distance > 0) {
    new = NEW (Head) (this->state, vertex, this->distance + distance);

    new->state->base[this->vertex][vertex] = -1;
  }

  return new;
}

/******************************************************************************/
void _(Destruct)()
{
  DELETE (this->state);
}

#undef TYPENAME
/*============================================================================*/
#define TYPENAME Graph

////////////////////////////////////////////////////////////////////////////////
double _(Weight)(int s, int e)
{
  List   *heads    = List_Push(NEW (List)(), NEW (Head) (BASE(0), s, 0));
  List   *nexts    = NEW (List)();
  double  distance = -1;

  while (List_Size(heads) > 0) {
    for (List *l = heads; !List_Empty(l); l = List_Next(l)) {
      Head *h = List_Head(l);

      for (int i = 0; i < h->state->rows; i++) {
        Head *next = Head_Split(h, i);

        if (next && (distance == -1 || next->distance < distance)) {
          if (i == e) {
            distance = next->distance;
          } else {
            nexts = List_Push(nexts, next);
            next  = NULL;
          }
        }

        DELETE (next);
      }
    }

    DELETE (heads);
    heads = nexts;
    nexts = NEW (List)();
  }

  DELETE (heads);
  DELETE (nexts);

  return distance;
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
int _(Reaches)(int s, int e)
{
  Matrix *adjacency   = Graph_AdjacencyMatrix(this);
  Matrix *accumulator = Matrix_Copy(adjacency);  
  int     reaches     = 0;

  for (int i = 0;!(reaches |= accumulator->base[s][e] > 0) && i < this->base.rows; i++) {
     accumulator = Matrix_Cross(accumulator, Matrix_Copy(adjacency));
  }

  DELETE (accumulator);
  DELETE (adjacency);

  return reaches;
}

////////////////////////////////////////////////////////////////////////////////
int _(ReachesKey)(const char *li, const char *lj)
{
  int i = Graph_Key(this, li);
  int j = Graph_Key(this, lj);

  return Graph_Reaches(this, i, j);
}

////////////////////////////////////////////////////////////////////////////////
int _(ReachesLabel)(const String *li, const String *lj)
{
  int i = Graph_Label(this, li);
  int j = Graph_Label(this, lj);

  return Graph_Reaches(this, i, j);
}

#undef TYPENAME