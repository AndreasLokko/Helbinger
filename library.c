#include "internal.h"
#include "library.h"

const char *getCStringFromPHCursor(pHCursor cursor){
    return clang_getCString(clang_getTypeSpelling(cursor->self));
}

pHCursor pHCursorFromCXType(CXType self, CXCursor parent, int ind){
    pHCursor p = malloc(sizeof(struct HCursor));
    *p = (struct HCursor){self, parent, ind};
    //puts(getCStringFromPHCursor(p));
    return p;
}

pHCursor pHCursorFromCXCursor(CXCursor c){
    return pHCursorFromCXType(clang_getCursorType(c), c, 0);
}

pHCursor parameterCursor(pHCursor cursor, int i) {
    return pHCursorFromCXType(clang_getArgType(cursor->self, i), cursor->parent, cursor->indirections);
}

pHCursor returnCursor(pHCursor cursor){
    return pHCursorFromCXType(clang_getResultType(cursor->self), cursor->parent, 0);
}

pHCursor finalPointedToCursor(pHCursor cursor){
    CXType t = cursor->self;
    int i = 0;
    while(t.kind == CXType_Pointer)
        t = clang_getPointeeType(t), i++;
    return pHCursorFromCXType(t, cursor->parent, i);
}

pHCursor pointedToCursor(pHCursor cursor){
    return pHCursorFromCXType(clang_getPointeeType(cursor->self), cursor->parent, cursor->indirections + 1);
}

pHCursor elaboratedCursor(pHCursor cursor){
    return pHCursorFromCXType(clang_Type_getNamedType(cursor->self), cursor->parent, cursor->indirections);
}

pHCursor getTypedefCursor(pHCursor cursor){ //TODO re-evaluate if this is correct way to handle typedefs
    if(clang_getCursorKind(cursor->parent) == CXCursor_TypedefDecl && cursor->indirections <= 0)
        return pHCursorFromCXType(clang_getTypedefDeclUnderlyingType(cursor->parent), cursor->parent,cursor->indirections);
    return pHCursorFromCXType(clang_getCanonicalType(cursor->self), cursor->parent,cursor->indirections);
}

pHCursor pointedByPointer(pHCursor cursor){
    CXType t = clang_getCursorType(cursor->parent);
    int i = cursor->indirections;
    while(--i)
        t = clang_getPointeeType(t);
    return pHCursorFromCXType(t, cursor->parent, cursor->indirections - 1);;
}

int isPointer(pHCursor cursor){
    return cursorKind(cursor) == HType_Pointer;
}

int isPointedByPointer(pHCursor cursor){
    return cursor->indirections > 0;
}

int isConstQualified(pHCursor cursor){
    return (int) clang_isConstQualifiedType(cursor->self);
}

int isFunction(pHCursor cursor){
    return cursorKind(cursor) == HType_FunctionProto;
}

int isTypedef(pHCursor cursor){
    return cursorKind(cursor) == HType_Typedef;
}

int isVarDecl(pHCursor cursor){
    enum CXTypeKind kind = cursor->self.kind;
    return CXType_FirstBuiltin <= kind && kind <= CXType_LastBuiltin;
}

int isEnum(pHCursor cursor){
    return cursorKind(cursor) == HType_Enum;
}

int isElaborated(pHCursor cursor){
    return cursorKind(cursor) == HType_Elaborated;
}

int isStruct(pHCursor cursor) {
    return cursorKind(cursor) == HType_Record;
}

int isArray(pHCursor cursor) {
    return cursorKind(cursor) == HType_ConstantArray;
}

int numParameters(pHCursor cursor) {
    return clang_getNumArgTypes(cursor->self);
}

long long numArrayElements(pHCursor cursor){
    return clang_getArraySize(cursor->self);
}

enum HType cursorKind(pHCursor cursor){
    return (enum HType) cursor->self.kind;
}

const char *getNameSpelling(pHCursor cursor){
    return clang_getCString(clang_getCursorSpelling(cursor->parent));
}

enum HType enumIntegerType(pHCursor cursor){
    if(!isEnum(cursor)){
        puts("\n ERROR: pHCursor does not point to Enum.");
        exit(1);
    }
    return (enum HType) clang_getEnumDeclIntegerType(cursor->parent).kind;
}

pHCursor arrayElementTypeCursor(pHCursor cursor){
    return pHCursorFromCXType(clang_getArrayElementType(cursor->self), cursor->parent, cursor->indirections);
};

const char *getTypeSpelling(pHCursor cursor){
    if(isElaborated(cursor)){
        puts("\n ERROR: pHCursor points to elaborated type");
        exit(1);
    }
    if(cursor->self.kind == CXType_Invalid){
        puts("\n ERROR: pHCursor points to invalid type");
        exit(1);
    }
    return clang_getCString(clang_getTypeSpelling(cursor->self));
}

long long enumConstantDeclValue(pHCursor cursor){
    return clang_getEnumConstantDeclValue(cursor->parent);
}

unsigned long long enumConstantDeclUnsignedValue(pHCursor cursor){
    return clang_getEnumConstantDeclUnsignedValue(cursor->parent);
}

void VisitEnum(pHCursor cursor, HEnumVisitor visitor, void* userData){
    if(!isEnum(cursor)){
        puts("\n ERROR: pHCursor does not point to an enum");
        exit(1);
    }
    VisitEnumInternal(cursor, visitor, userData);
}

void VisitStruct(pHCursor cursor, HStructVisitor visitor, void* userData) {
    if(!isStruct(cursor)){
        puts("\n ERROR: pHCursor does not point to a struct");
        exit(1);
    }
    VisitStructInternal(cursor, visitor, userData);
}

const char *typeSpelling(enum HType type) {
    return clang_getCString(clang_getTypeKindSpelling(type));
}

//INTERESTING FUNCTIONS
/*
 * clang_TargetInfo_getPointerWidth
 * clang_getTranslationUnitCursor
 * clang_getCursorKind
 * clang_isDeclaration
 * clang_isAttribute
 *
 * clang_getCanonicalCursor
 * clang_getCanonicalType
 * clang_equalTypes
 * clang_equalCursors
 * clang_isCursorDefinition
 * clang_getCursorDefinition
 * clang_isPODType
 * clang_getPointeeType
 * clang_getTypedefDeclUnderlyingType
 * clang_Type_getNamedType
 *
 * clang_getEnumDeclIntegerType
 * clang_getEnumConstantDeclValue
 * clang_getEnumConstantDeclUnsignedValue
 * clang_getFunctionTypeCallingConv
 *
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
 * clang_Cursor_getNumArguments
 * clang_Cursor_getArgument
 * clang_Cursor_hasAttrs
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
 * clang_Type_getCXXRefQualifier TODO IGNORE
 *
 * CXLinkageKind
 * clang_getCursorLinkage
 * CXVisibilityKind
 * clang_getCursorVisibility
 * CXAvailabilityKind
 * clang_getCursorAvailability
 * clang_getCursorPlatformAvailability
 * CXLanguageKind TODO restrict to C?
 * clang_getCursorTLSKind
 * clang_getCursorType
 *
 * clang_EnumDecl_isScoped is C++
 * clang_toggleCrashRecovery
 * */
