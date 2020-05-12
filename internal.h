//
// Created by lokko on 09.05.20.
//

#ifndef HELBINGER_INTERNAL_H
#define HELBINGER_INTERNAL_H

#include "library.h"
#include <clang-c/Index.h>
#include <z3.h>
#include <unistd.h>
#include <stdlib.h>

struct HCursor {
    CXType self;
    CXCursor parent;
    int indirections;
};

void VisitStructInternal(pHCursor cursor, HStructVisitor visitor, void* userData);
void VisitEnumInternal(pHCursor cursor, HEnumVisitor visitor, void* userData);
pHCursor pHCursorFromCXCursor(CXCursor cursor);

#endif //HELBINGER_INTERNAL_H
