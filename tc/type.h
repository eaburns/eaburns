/*
 * type.h defines structures that represent types.
 * Ethan Burns -- Created Wed Feb 22 19:29:27 2006
 */

#ifndef _TYPE_H_
#define _TYPE_H_

/*
 * Different kinds of types. 
 */
enum type_tag {
  INT_TYPE, ARRAY_TYPE, CLASS_TYPE, FIELD_TYPE, METHOD_TYPE,
  CONSTRUCTOR_TYPE, PARAMETER_TYPE, NULL_TYPE, ERROR_TYPE
};

/*
 * Represents types. 
 */
typedef struct {
  enum type_tag tag;
} Type;

/*
 * Represents an array type. 
 */
typedef struct {
  Type super;
  Type *next;                   /* Arrays can be linked to arrays or
                                 * types. */
} ArrayType;

/*
 * Represents a field of a class. 
 */
typedef struct FieldType {
  Type super;
  char *name;                   /* Name of the field. */
  Type *type;                   /* Type of the field. */
  unsigned int num;             /* Field number in current class. */
  struct ClassType *class;
  struct FieldType *next;
} FieldType;

/*
 * Represents a parameter of a method. 
 */
typedef struct ParamType {
  Type super;
  char *name;                   /* The name of the parameter. */
  Type *type;                   /* The type of the parameter. */
  unsigned int num;             /* Parameter number in current method. */
  struct ParamType *next;
} ParamType;

/*
 * Represents a method of a class, or a constructor if ret is NULL. 
 */
typedef struct MethodType {
  Type super;
  char *name;
  Type *type;
  ParamType *params;
  unsigned int numParams;
  struct MethodType *next;
  struct ClassType *class;

  /*
   * Extras for constructors.
   */
  int def;                      /* 1 if this is a default constructor. */
  struct MethodType *invokedConst;      /* `this' constructor invoked in
                                         * the body of this constructor,
                                         * or NULL. */
} MethodType;

/*
 * Represents a class type. 
 */
typedef struct ClassType {
  Type super;                   /* Parent structure. */
  char *name;                   /* Name of the class. */
  FieldType *fields;            /* List of fields. */
  unsigned int numFields;       /* Number of fields. */
  MethodType *methods;          /* List of methods. */
  MethodType *cons;             /* List of constructors. */
  struct ClassType *parent;     /* This class's parent class. */
  struct ClassType *next;
} ClassType;

/*
 * Makes an error type.
 */
Type *makeErrorType();

/*
 * Makes an integer type. 
 */
Type *makeIntType();

/*
 * Makes a null type. 
 */
Type *makeNullType();

/*
 * Makes an array type. 
 */
ArrayType *makeArrayType(Type * next);

/*
 * Creates an empty class type. 
 */
ClassType *makeClassType(char *name);

/*
 * Makes a field type. 
 */
FieldType *makeFieldType(char *name, Type * type);

/*
 * Makes a parameter type. 
 */
ParamType *makeParamType(char *name, Type * type, int num);

/*
 * Makes a method with no parameters. 
 */
MethodType *makeMethodType(char *name, Type * ret);

/*
 * Makes a constructor with no parameters. 
 */
MethodType *makeConstructorType(char *name);

/*
 * Returns 1 if a and b have the same signature. 
 */
int same_sigs(MethodType * a, MethodType * b);

/*
 * Returns 1 if type a and type b are the same. 
 */
int equal_types(Type * a, Type * b);

/*
 * Returns t appended to a.  If a is NULL, t is returned. 
 */
Type *appendArray(Type * t, ArrayType * a);

/*
 * Copies all of the array types so that the pointers can be changed and
 * the origional copies are not destroyed. 
 */
Type *copyArrays(Type * t);

/*
 * Returns the base type of an array type.
 */
Type *arrayBaseType(ArrayType * t);

/*
 * Returns the number of dimensions in an array.
 */
unsigned int arrayDims(ArrayType * t);

/*
 * Returns 1 if the given type is a reference type (CLASS_TYPE ||
 * ARRAY_TYPE || NULL_TYPE), else 0 is returned. 
 */
int isReference(Type * t);

/*
 * Prints a type to stdout. 
 */
void printType(Type * t);

/*
 * Returns a string containing the type name.
 * The return value must be freed.
 */
char *getTypeName(Type *t);

/*
 * Initialize type memory tracking. 
 */
void initTypes();

/*
 * Free all memory associated with types. 
 */
void freeTypes();

#endif                          /* !_TYPE_H_ */
