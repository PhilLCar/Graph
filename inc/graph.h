#ifndef GRAPH_H
#define GRAPH_H

#include <diagnostic.h>
#include <oop.h>
#include <matrix.h>
#include <set.h>
#include <list.h>
#include <iterator.h>

#define TYPENAME Graph

/* 
 * This class is meant to represent a weighted graph:
 * Use negative weights for unreacheable nodes, 0 is an instant link
 */

OBJECT (int vertices) INHERIT (Matrix)
  Set *labels;
END_OBJECT (2);

Graph *STATIC (From)(int vertices, const char *labels[vertices]);
Graph *STATIC (FromLabels) (void *enumerable);

int     _(Key)(const char *label);
int     _(Label)(const String *label);
String *_(NameOf)(int number);
int     _(AddKey)(const char *label);
int     _(AddLabel)(String *label);
void    _(RemoveKey)(const char *label);
void    _(Remove)(const String *label);
void    _(Set)(int i, int j, double weight);
void    _(SetKey)(const char *li, const char *lj, double weight);
void    _(SetLabel)(const String *li, const String *lj, double weight);
Matrix *_(AdjacencyMatrix)();
double  _(Weight)(int i, int j);
double  _(WeightKey)(const char *li, const char *lj);
double  _(WeightLabel)(const String *li, const String *lj);
int     _(Reaches)(int i, int j);
int     _(ReachesKey)(const char *li, const char *lj);
int     _(ReachesLabel)(const String *li, const String *lj);

#undef TYPENAME

#endif