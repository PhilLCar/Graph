#ifndef GRAPHFILE_H
#define GRAPHFILE_H

#include <graph.h>
#include <jsonfile.h>

#define TYPENAME GraphFile

OBJECT(const char *filename, AccessModes modes) INHERIT (Graph)
  char        *filename;
  AccessModes  mode;
END_OBJECT(NULL, ACCESS_READ);

Graph *STATIC (FromJSON)(JSON *representation);

JSON   *_(ToJSON)();


#undef TYPENAME

#endif