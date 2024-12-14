#ifndef GRAPH_H
#define GRAPH_H

#include <diagnostic.h>
#include <oop.h>
#include <matrix.h>
#include <objectarray.h>
#include <list.h>

#define TYPENAME Graph

OBJECT (int vertices) INHERIT (Matrix)
  ObjectArray *labels;
END_OBJECT (2);

Graph *STATIC (Fill)(int vertices, const char *labels[vertices]);
Graph *STATIC (FromLabels) (ObjectArray *labels);

int    _(L)(const char *label);
int    _(Label)(const String *label);
void   _(Set)(int i, int j, double weight);
void   _(SetL)(const char *li, const char *lj, double weight);
void   _(SetLabel)(const String *li, const String *lj, double weight);
double _(Weight)(int i, int j);
double _(WeightL)(const char *li, const char *lj);
double _(WeightLabeL)(const String *li, const String *lj);

#undef TYPENAME

#endif