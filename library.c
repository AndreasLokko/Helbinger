#include "HCursor.h"
#include <unistd.h>
#include <z3.h>

void TestVisitor(pHCursor cursor)
{
    //TODO pretty printer
};

int main()
{
    ParseHeader(TestVisitor,"../library.h");
}

enum CXChildVisitResult TranslationUnitVisitor(CXCursor current,  CXCursor parent, HCursorVisitor visitor)
{
    if(clang_isDeclaration(clang_getCursorKind(current)))
        visitor(pHCursorFromCXCursor(current));
    return CXChildVisit_Continue;
};

void ParseHeader(HCursorVisitor visitor, char *header)
{
    if (access( header, F_OK ) == -1) {
        puts("Unable to find header. Quitting.");
        return;
    }

    CXIndex index = clang_createIndex(1, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
            index,
            header, NULL, 0,
            NULL, 0,
            CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_ForSerialization );

    clang_visitChildren(clang_getTranslationUnitCursor(unit), TranslationUnitVisitor, visitor);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}