//
// Created by lokko on 03.05.20.
//

#include <stddef.h>
#include <z3.h>
#include "HCursor.h"

#define retType pointedToType_returnType_elementType
#define ptdToType pointedToType_returnType_elementType

enum Kind
{
    Array,
    Struct,
    FUNCTION,
    POINTER
};

struct HCursor
{ //TODO maybe overload some fields
    enum Kind kind;
    struct HCursor *pointedToType_returnType_elementType;
    struct HCursor *pointedByPointer;
    int isConstQualified;
};

pHCursor returnType(pHCursor cursor)
{
    struct HCursor result;
    return  cursor->retType;
}

pHCursor finalPointedToType(pHCursor cursor)
{
    struct HCursor result;
    while(isPointerType(cursor))
        cursor = cursor->ptdToType;
    return  cursor->ptdToType;
}

pHCursor pointedToType(pHCursor cursor)
{
    struct HCursor result;
    return  cursor->ptdToType;
}

pHCursor pointedByPointer(pHCursor cursor)
{
    struct HCursor result;
    return  cursor->retType;
}

int isPointerType(pHCursor cursor)
{
    return  cursor->kind == POINTER;
}

int isPointedByPointer(pHCursor cursor)
{
    return  cursor->pointedByPointer != NULL;
}

int isConstQualified(pHCursor cursor)
{
    return  cursor->isConstQualified;
}

int isFunction(pHCursor cursor)
{
    return  cursor->kind == FUNCTION;
}

int isForwardDecl(pHCursor cursor)
{
    return 0;
}

pHCursor pHCursorFromCXCursor(CXCursor cursor)
{
    auto kind = clang_getCursorKind(cursor);
    switch (kind)
    {
        case CXCursor_StructDecl:
            if(!clang_isCursorDefinition(cursor)) return 0;
            fputs("Struct ", stdout);
            break;
        case CXCursor_UnionDecl:
            fputs("Union ", stdout);
            break;
        case CXCursor_EnumDecl:
            if(!clang_isCursorDefinition(cursor)) return 0;
            fputs("Enum ", stdout);
            break;
        case CXCursor_FunctionDecl:
            fputs("Function ", stdout);
            break;
        case CXCursor_TypedefDecl:
            fputs("Typedef ", stdout);
            break;
        default: break;
    }
    puts(clang_getCString(clang_getCursorSpelling(cursor)));
    //TODO CONVERTER
}

//INTERESTING FUNCTIONS
/*
 * clang_TargetInfo_getPointerWidth
 * CXCursorKind
 * clang_getTranslationUnitCursor
 * clang_getCursorKind
 * clang_isDeclaration TODO get more documentation
 * clang_isAttribute
 *
 * clang_getCanonicalCursor //TODO super important
 * clang_getCanonicalType
 * clang_equalTypes
 * clang_equalCursors
 * clang_isCursorDefinition
 * clang_getCursorDefinition
 * clang_isPODType
 * clang_getPointeeType
 * CXTypeKind TODO define subset
 * clang_getTypedefDeclUnderlyingType TODO subsumed by canonical type?
 * clang_getEnumDeclIntegerType
 * clang_getEnumConstantDeclValue
 * clang_getEnumConstantDeclUnsignedValue
 * clang_getFunctionTypeCallingConv
 * clang_getFieldDeclBitWidth
 * clang_isConstQualifiedType
 * clang_isVolatileQualifiedType
 * clang_isRestrictQualifiedType
 * clang_getResultType
 * clang_getCursorResultType
 * clang_getNumArgTypes
 * clang_getArgType
 * clang_getElementType
 * clang_getNumElements
 * clang_getArraySize
 * clang_EvalResult...
 * clang_getTypeDeclaration
 *
 *
 * clang_Cursor_getNumArguments
 * clang_Cursor_getArgument
 * clang_Cursor_hasAttrs TODO what attributes?
 * clang_Cursor_getOffsetOfField
 * clang_Cursor_isAnonymous
 * clang_Cursor_isAnonymousRecordDecl
 * clang_Cursor_getStorageClass
 * clang_Cursor_isVariadic
 * clang_Cursor_isExternalSymbol
 * clang_Cursor_isBitField
 * clang_Cursor_Evaluate
 *
 * clang_Type_getAlignOf
 * clang_Type_getSizeOf
 * clang_Type_visitFields
 * clang_Type_getNullability
 *
 * clang_Type_getClassType
 * clang_Type_getNamedType
 * clang_Type_getCXXRefQualifier TODO IGNORE
 *
 *
 * CXLinkageKind TODO look up what it means
 * clang_getCursorLinkage
 * CXVisibilityKind TODO look up what it means
 * clang_getCursorVisibility
 * CXAvailabilityKind TODO look up what it means
 * clang_getCursorAvailability
 * clang_getCursorPlatformAvailability TODO what is entity availability
 * CXLanguageKind TODO restrict to C?
 * clang_getCursorTLSKind
 * clang_getCursorType
 *
 * clang_getTypedefName
 * clang_EnumDecl_isScoped TODO is C++?
 * clang_toggleCrashRecovery
 * */
