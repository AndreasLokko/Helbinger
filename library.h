#ifndef BINGER_LIBRARY_H
#define BINGER_LIBRARY_H

enum HType{
    HType_Void = 2,
    HType_Bool = 3,
    HType_UChar = 5,
    HType_UShort = 8,
    HType_UInt = 9,
    HType_ULong = 10,
    HType_ULongLong = 11,
    HType_UInt128 = 12,
    HType_SChar = 14,
    HType_WChar = 15,
    HType_Short = 16,
    HType_Int = 17,
    HType_Long = 18,
    HType_LongLong = 19,
    HType_Int128 = 20,
    HType_Float = 21,
    HType_Double = 22,
    HType_LongDouble = 23,
    HType_Float128 = 30,
    HType_Half = 31,
    HType_Float16 = 32,

    HType_Pointer = 101,
    HType_Record = 105,
    HType_Enum = 106,
    HType_Typedef = 107,
    HType_FunctionProto = 111,
    HType_ConstantArray = 112,
    HType_Elaborated = 119,
};

///These are here for demo purposes
struct a{
    int i;
    float f;
};

int v[5];
///end

typedef struct HCursor* pHCursor;

typedef void (*HCursorVisitor)(pHCursor cursor, void* userData);
typedef void (*HEnumVisitor)(pHCursor cursor, void* userData);
typedef void (*HStructVisitor)(pHCursor cursor, void* userData);

void VisitEnum(pHCursor enumCursor, HEnumVisitor visitor, void* userData);
void VisitStruct(pHCursor enumCursor, HStructVisitor visitor, void* userData);
void ParseHeader(char *header, HCursorVisitor visitor, void* userData);

int isPointer(pHCursor cursor);
int isPointedByPointer(pHCursor cursor);
int isConstQualified(pHCursor cursor);
int isFunction(pHCursor cursor);
int isVarDecl(pHCursor cursor);
int isTypedef(pHCursor cursor);
int isEnum(pHCursor cursor);
int isElaborated(pHCursor cursor);
int isStruct(pHCursor cursor);
int isArray(pHCursor cursor);
int numParameters(pHCursor cursor);
long long numArrayElements(pHCursor cursor);
pHCursor parameterCursor(pHCursor, int i);
pHCursor returnCursor(pHCursor cursor);
pHCursor finalPointedToCursor(pHCursor cursor);
pHCursor pointedToCursor(pHCursor cursor);
pHCursor pointedByPointer(pHCursor cursor);
pHCursor elaboratedCursor(pHCursor cursor);
pHCursor getTypedefCursor(pHCursor cursor);
enum HType cursorKind(pHCursor cursor);
const char *getNameSpelling(pHCursor cursor);
const char *getTypeSpelling(pHCursor cursor);
const char *typeSpelling(enum HType type);
enum HType enumIntegerType(pHCursor cursor);
pHCursor arrayElementTypeCursor(pHCursor cursor);
long long enumConstantDeclValue(pHCursor cursor);
unsigned long long enumConstantDeclUnsignedValue(pHCursor cursor);

#endif //BINGER_LIBRARY_H
