//
// Created by lokko on 12.05.20.
//

#include "library.h"
#include <z3.h>

void putTypeSpelling(pHCursor cursor);

static void enumVisitor(pHCursor cursor, void* userData) {
    printf("\n    %s = %lld,", getNameSpelling(cursor), enumConstantDeclValue(cursor));
}

static void structVisitor(pHCursor cursor, void* userData) {
    fputs("\n\t", stdout);
    putTypeSpelling(cursor);
    putchar(' ');
    fputs(getNameSpelling(cursor), stdout);
    putchar(';');
}
void putTypeSpelling(pHCursor cursor) {
    if(isVarDecl(cursor))
        fputs(getTypeSpelling(cursor), stdout);
    else if(isTypedef(cursor))
        fputs(getTypeSpelling(cursor), stdout);
    else if(isElaborated(cursor))
        putTypeSpelling(elaboratedCursor(cursor));
    else if(isEnum(cursor))
        fputs(getTypeSpelling(cursor), stdout);
    else if(isStruct(cursor))
        fputs(getTypeSpelling(cursor), stdout);
    else if(isPointer(cursor)) {
        fputs("pointer to ", stdout);
        putTypeSpelling(pointedToCursor(cursor));
    }
    else if(isFunction(cursor)) {
        printf("function that has %d parameters and returns " , numParameters(cursor));
        putTypeSpelling(returnCursor(cursor));
        for(int i = 0; i < numParameters(cursor); i++){
            printf(" |parameter %d ", i + 1);
            putTypeSpelling(parameterCursor(cursor, i));
        }
    }
}

void TestVisitor(pHCursor cursor, void* userData) {
    if(isVarDecl(cursor)) {
        fputs("\nVarDecl ", stdout);
        fputs(getNameSpelling(cursor), stdout);
        putchar(' ');
        if(isElaborated(cursor)) {
            pHCursor c = elaboratedCursor(cursor);
            fputs(getTypeSpelling(c), stdout);
        }
        else
            fputs(getTypeSpelling(cursor), stdout);
    }
    else if(isPointer(cursor)) {
        fputs("\nPointer ", stdout);
        fputs(getNameSpelling(cursor), stdout);
        fputs(" ultimately points to ", stdout);
        fputs(getTypeSpelling(finalPointedToCursor(cursor)), stdout);
    }
    else if(isTypedef(cursor)) {
        fputs("\nTypedef ", stdout);
        fputs(getNameSpelling(cursor), stdout);
        fputs(" is an alias for ", stdout);
        putTypeSpelling(getTypedefCursor(cursor));
    }
    else if(isFunction(cursor)) {
        printf("\nFunction %s that has %d parameters and returns " , getNameSpelling(cursor), numParameters(cursor));
        putTypeSpelling(returnCursor(cursor));
        for(int i = 0; i < numParameters(cursor); i++){
            printf(" |parameter %d ", i + 1);
            putTypeSpelling(parameterCursor(cursor, i));
        }
    }
    else if(isEnum(cursor)) {
        fputs("\nEnum ", stdout);
        fputs(getNameSpelling(cursor), stdout);
        putchar(' ');
        typeSpelling(enumIntegerType(cursor));
        putchar('{');
        VisitEnum(cursor, enumVisitor, NULL);
        fputs("\n}", stdout);
    }
    else if(isStruct(cursor)) {
        printf("\nStruct %s ", getNameSpelling(cursor));
        putchar('{');
        VisitStruct(cursor, structVisitor, NULL);
        fputs("\n}", stdout);
    }
    else if(isArray(cursor)) {
        printf("\nArray %s of ", getNameSpelling(cursor));
        putTypeSpelling(arrayElementTypeCursor(cursor));
        printf(" of %lld elements",numArrayElements(cursor));
    }
}

int main(){
    ParseHeader("../library.h", TestVisitor, NULL);
}
