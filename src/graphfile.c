#include <graphfile.h>

#define TYPENAME GraphFile

////////////////////////////////////////////////////////////////////////////////
GraphFile *_(Construct)(const char *filename, AccessModes mode)
{  
  if (this) {
    if (filename) {
      this->filename = malloc(strlen(filename) + 1);
      this->mode     = mode;

      strcpy(this->filename, filename);

      if (mode & ACCESS_READ) {
        JSONFile *file = NEW (JSONFile) (filename, ACCESS_READ);

        Graph *graph = GraphFile_FromJSON((JSON*)file);

        memcpy(this, graph, sizeof(Graph));
        tfree(graph);

        DELETE (file);
      } else {
        Graph_Construct(BASE(0), 0);
      }
    }
  }

  return this;
}

////////////////////////////////////////////////////////////////////////////////
void _(Destruct)()
{
  if (this->filename) {
    if (this->mode & ACCESS_WRITE) {
      FileStream *file           = FileStream_Open(this->filename, ACCESS_WRITE | ACCESS_CREATE);
      JSON       *representation = GraphFile_ToJSON(this);

      JSON_Serialize(representation, (CharStream*)file);

      DELETE (representation);
      DELETE (file);
    }

    free((void*)this->filename);
    this->filename = NULL;

    Graph_Destruct(BASE(0));
  }
}

////////////////////////////////////////////////////////////////////////////////
Graph *STATIC (FromJSON)(JSON *representation)
{
  List *labels  = Map_ValueAtKey((Map*)representation, "labels");
  List *weights = Map_ValueAtKey((Map*)representation, "weights");

  Graph *graph = Graph_FromLabels(labels);

  int i = 0; int j = 0;
  for (List *li = weights; !List_Empty(li); li = List_Next(li), ++i, j = 0) {
    for (List *lj = List_Head(li); !List_Empty(lj); lj = List_Next(lj), ++j) {
      Graph_Set(graph, i, j, *(double*)List_Head(lj));
    }
  }

  return graph;
}

////////////////////////////////////////////////////////////////////////////////
JSON *_(ToJSON)()
{
  JSON *json    = NEW (JSON)();
  List *labels  = NEW (List)();
  List *weights = NEW (List)();

  for (Iterator *it = NEW (Iterator)(BASE(0)->labels); !done(it); next(it)) {
    List_Add(labels, String_Copy(it->base));
  }

  Map_Set((Map*)json, NEW (String)("labels"),  labels);
  Map_Set((Map*)json, NEW (String)("weights"), weights);

  for (int i = 0; i < BASE(1)->rows; i++) {
    List *l = NEW (List)();

    for (int j = 0; j < BASE(1)->cols; j++) {
      List_AddValue(l, TYPEOF(double), &BASE(1)->base[i][j]);
    }

    List_Add(weights, l);
  }

  return json;
}

#undef TYPENAME