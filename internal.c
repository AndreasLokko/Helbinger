//
// Created by lokko on 09.05.20.
//

#include "internal.h"

struct TranslationUnitVisitorUserData {
    HCursorVisitor visitor;
    void* userData;
};

enum CXChildVisitResult TranslationUnitVisitor(CXCursor current, CXCursor parent, struct TranslationUnitVisitorUserData *visitorUserData) {
    enum CXCursorKind kind = clang_getCursorKind(current);
    if(clang_isDeclaration(kind)){
        switch (kind)
        {
            case CXCursor_StructDecl:
            case CXCursor_EnumDecl:
                if(!clang_isCursorDefinition(current))
                    return CXChildVisit_Continue;
                break;
            default:
                break;
        }
        visitorUserData->visitor(pHCursorFromCXCursor(current), visitorUserData->userData);
    }

    return CXChildVisit_Continue;
}

void ParseHeader(char *header, HCursorVisitor visitor, void* userData)
{
    if (access( header, F_OK ) == -1) {
        puts("Unable to find header. Quitting.");
        return;
    }

    struct TranslationUnitVisitorUserData vud = {visitor, userData};

    CXIndex index = clang_createIndex(1, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
            index,
            header, NULL, 0,
            NULL, 0,
            CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_ForSerialization );

    clang_visitChildren(clang_getTranslationUnitCursor(unit), (CXCursorVisitor) TranslationUnitVisitor, &vud);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}

struct enumVisitorUserData {
    HEnumVisitor visitor;
    void* userData;
};

enum CXChildVisitResult enumVisitor(CXCursor current, CXCursor parent, struct enumVisitorUserData *visitorUserData) {
    visitorUserData->visitor(pHCursorFromCXCursor(current), visitorUserData->userData);
    return CXChildVisit_Continue;
}

void VisitEnumInternal(pHCursor cursor, HEnumVisitor visitor, void* userData){
    struct enumVisitorUserData vud = {visitor, userData};
    clang_visitChildren(cursor->parent, (CXCursorVisitor) enumVisitor, &vud);
}

struct structVisitorUserData {
    HStructVisitor visitor;
    void* userData;
};

enum CXChildVisitResult structVisitor(CXCursor current, CXCursor parent, struct structVisitorUserData *visitorUserData) {
    visitorUserData->visitor(pHCursorFromCXCursor(current), visitorUserData->userData);
    return CXChildVisit_Continue;
}

void VisitStructInternal(pHCursor cursor, HStructVisitor visitor, void* userData){
    struct structVisitorUserData vud = {visitor, userData};
    clang_visitChildren(cursor->parent, (CXCursorVisitor) enumVisitor, &vud);
}