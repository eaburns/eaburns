/*
 * class.h function declaration for building the class list.
 * Ethan Burns -- Created Thu Feb 23 17:15:19 EST 2006
 */
#ifndef _CLASS_H_
#define _CLASS_H_

#include "ast.h"
#include "type.h"
#include "list.h"

/*
 * String representation of the name of the Object class. 
 */
#define OBJECT_NAME "Object"

/* The global class list. */
extern ClassType *classes;

/*
 * Sets the current class. 
 */
void setCurrentClass(ClassType * t);

/*
 * Sets the current method. 
 */
void setCurrentMethod(MethodType * t);

/*
 * Returns a pointer to a class given its name. 
 */
ClassType *getClass(char *name);

/*
 * Returns a method from the given class.
 */
MethodType *getMethod(char *name, Seq * params, ClassType * class);

/*
 * Adds a class to the list of classes. 
 */
void addClassType(ClassType * class);

/*
 * Adds a field to the current class. 
 */
void addFieldType(FieldType * field);

/*
 * Adds a method to the current class. 
 */
void addMethodType(MethodType * method);

/*
 * Adds a constructor to the current class. 
 */
void addConstructorType(MethodType * cons);

/*
 * Adds a parameter to the current method or constructor. 
 */
void addParamType(ParamType * param);

/*
 * Looks for the field in the current class and its parent classes.
 * The return value is the Type* of the first field with the given name found
 * on the way up the class-parent chain, or NULL if the field is not found.
 */
Type *searchFieldType(const char *name, ClassType * class);

/*
 * Returns a list of possible methods given a name and a paramater list.
 */
List *searchMethods(const char *name, Seq * params, ClassType * class);

/*
 * Returns a list of possible constructors given the parameter list.
 */
List *searchConstructor(Seq * params, ClassType * class);

/*
 * Returns 1 if the sequence of parameters `params' can be method converted
 * to the given method paramaters `mparms.'
 */
int validParams(Seq * params, ParamType * mparms);

/*
 * Prints the method to stdout.
 */
void printMethod(const MethodType * t);

/*
 * Returns a pointer to the current class, or NULL if there is not one.
 */
ClassType *getCurClass();

/*
 * Returns a pointer to the current method, or NULL if there is not one.
 */
MethodType *getCurMethod();

/*
 * Returns a pointer to the parameter with the given name in the given
 * method, or NULL if no such parameter exists.
 */
ParamType *getParameter(char *name, MethodType * method);

/*
 * Prints the list of classes. 
 */
void printClasses();

/*
 * Builds the list of classes. 
 */
void buildClasses(AstNode * n);

/*
 * Returns 1 if t is a super class of s, else returns 0. 
 */
int isSuper(ClassType * t, ClassType * s);

/*
 * Returns the munged method name.  The return value is allocated using malloc
 * and must be freed by the user with free(). 
 */
char *getMungedName(MethodType *t);

/*
 * Returns the name of the VMT for this class.  Malloced, must be freed.
 */
char *getVMTName(Type *t);

/*
 * Returns the name of the destructor for this class.  Malloced, must be freed.
 */
char *getDestructorName(const char *class);

/*
 * Returns a List of methods for the given class in the order that they are
 * to appear in the class's VMT.
 */
List *getMethods(ClassType *class);

/*
 * Returns the number of fields in the current class, including fields 
 * inherited from parents.
 */
unsigned int getNumFields(ClassType *class);

/*
 * Returns the number of the field given by the second argument in the class
 * given by the first argument.  Field numbers start at 0.
 */
unsigned int getFieldNum(ClassType *class, char *fieldName);

/*
 * Returns the number of the method given by the second argument in the class
 * given by the first argument.  Methods start at 0.
 */
unsigned int getMethodNum(ClassType *class, MethodType *method);

#endif                          /* !_CLASS_H_ */
