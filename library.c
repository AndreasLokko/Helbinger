#include "library.h"
#include "functions.h"
#include <clang-c/Index.h>
#include <unistd.h>
#include <z3.h>

int main(){
    ParseHeader("hello.h");
}

enum CXChildVisitResult TranslationUnitVisitor(CXCursor current,  CXCursor parent, CXClientData clientData)
{
    auto kind = clang_getCursorKind(current);
    if(!clang_isDeclaration(kind))
        return CXChildVisit_Continue;
    switch (clang_getCursorKind(current)) {
        case CXCursor_StructDecl:
            puts("Struct");
            break;
        case CXCursor_UnionDecl:
            puts("Union");
            break;
//                    case CXCursor_ClassDecl:
//                        puts("Class");
//                        break;
        case CXCursor_EnumDecl:
            puts("Enum");
            break;
        case CXCursor_FunctionDecl:
            function(&current);
            break;
        default: break;
    }
    return CXChildVisit_Continue;
};

void ParseHeader(char *header) {

    if (access( header, F_OK ) == -1) {
        puts("Unable to find header. Quitting.");
        return;
    }

    CXIndex index = clang_createIndex(1, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        header, NULL, 0,
        NULL, 0,
        CXTranslationUnit_SkipFunctionBodies | CXTranslationUnit_ForSerialization);

    //DEBUGGING ONLY
    //puts(clang_getCString(clang_TargetInfo_getTriple(clang_getTranslationUnitTargetInfo(unit))));

    clang_visitChildren(clang_getTranslationUnitCursor(unit), TranslationUnitVisitor, 0);

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
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