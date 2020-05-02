//
// Created by lokko on 02.05.20.
//

#include <z3.h>
#include "functions.h"

void function(CXCursor const*const cursor){
    auto num = clang_Cursor_getNumArguments(*cursor);

    CXType type = clang_getCursorResultType(*cursor);
    fputs("function ", stdout);
    fputs(clang_getCString(clang_getCursorSpelling(*cursor)), stdout);

    if(num == 0)
        fputs(" takes 0 arguments and ", stdout);
    else{
        fputs(" takes ", stdout);
        for(unsigned int i = 0; i < num; i++){
            CXCursor arg = clang_Cursor_getArgument(*cursor, i);
            CXType argType = clang_getCursorType(arg);
            CXString argName = clang_getCursorSpelling(arg);
            fputs(clang_getCString(argName), stdout),
            fputs(" of type ", stdout),
            fputs(clang_getCString(clang_getTypeKindSpelling(argType.kind)), stdout),
            putchar(' ');
        }
    }

    if(type.kind == CXType_Pointer)
        fputs("returns pointer to ", stdout),
        puts(clang_getCString(clang_getTypeKindSpelling(clang_getPointeeType(type).kind)));
    else
        fputs("returns ", stdout),
        puts(clang_getCString(clang_getTypeKindSpelling(type.kind)));
}