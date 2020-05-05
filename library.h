#ifndef BINGER_LIBRARY_H
#define BINGER_LIBRARY_H

typedef struct HCursor* pHCursor;
typedef enum CXChildVisitResult (*HCursorVisitor)(pHCursor cursor);
void ParseHeader(HCursorVisitor visitor, char *header);

pHCursor returnType(pHCursor type); //type should be function. consider using exit();
pHCursor finalPointedToType(pHCursor type); //returns self if not pointer
pHCursor pointedToType(pHCursor type); //returns self if not pointer
pHCursor pointedByPointer(pHCursor type); //returns self if not pointed to by pointer
int isPointerType(pHCursor type); //Should be obvious without comment
int isConstQualified(pHCursor type); //ditto
int isFunction(pHCursor type); //ditto

#endif //BINGER_LIBRARY_H
