/*
 * class.c implements functions for building the class list.
 * Ethan Burns -- Created Thu Feb 23 17:22:59 EST 2006
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "class.h"
#include "ast.h"
#include "type.h"
#include "error.h"
#include "conversion.h"

/*
 * Forward declaration. 
 */
static void fillClasses(AstNode * n);

/*
 * Number of the argument we are currently parsing, the first
 * argument is 1.
 */
static int param_number = 0;

/*
 * List of defined classes. 
 */
ClassType *classes = NULL;

/*
 * The current class. 
 */
static ClassType *cur_class = NULL;

/*
 * The current method or constructor. 
 */
static MethodType *cur_method = NULL;

void
setCurrentClass(ClassType * t)
{
  if (t != NULL)
    assert(((Type *) t)->tag == CLASS_TYPE);

  cur_class = t;
}

void
setCurrentMethod(MethodType * t)
{
  cur_method = t;
}

ClassType *
getClass(char *name)
{
  ClassType *p;

  for (p = classes; p != NULL && strcmp(p->name, name) != 0; p = p->next);

  return p;
}

MethodType *
getMethod(char *name, Seq * params, ClassType * class)
{
  MethodType *mp;
  ClassType *cp;
  ParamType *pp;
  FormalParam *fpnode;
  Seq *sp;
  Type *a,
    *b;

  for (cp = class; cp != NULL; cp = cp->next) {
    for (mp = cp->methods; mp != NULL; mp = mp->next) {

      /*
       * If we have a matching method name, check the parameters. 
       */
      if (strcmp(mp->name, name) == 0) {

        for (sp = params, pp = mp->params;
             pp != NULL && sp != NULL;
             pp = pp->next, sp = (Seq *) sp->next) {

          assert(sp->super.type == SEQ_NODE);
          assert(sp->data->type == FORMALPARAM_NODE);
          fpnode = (FormalParam *) sp->data;

          a = fpnode->super.type;
          b = pp->type;

          if (equal_types(a, b) == 1)
            return mp;
        }

      }

    }
  }

  return NULL;
}

void
addClassType(ClassType * class)
{
  ClassType *p;

  if (classes == NULL) {
    classes = class;
    return;
  }

  for (p = classes; p->next != NULL; p = p->next);
  p->next = class;
}

void
addFieldType(FieldType * field)
{
  FieldType *p;

  cur_class->numFields++;

  if (cur_class->fields == NULL) {
    cur_class->fields = field;
    return;
  }

  for (p = cur_class->fields; p->next != NULL; p = p->next);
  p->next = field;
}

void
addMethodType(MethodType * method)
{
  MethodType *p;

  if (cur_class->methods == NULL) {
    cur_class->methods = method;
    return;
  }

  for (p = cur_class->methods; p->next != NULL; p = p->next);
  p->next = method;
}

void
addConstructorType(MethodType * cons)
{
  MethodType *p;

  if (cur_class->cons == NULL) {
    cur_class->cons = cons;
    return;
  }

  for (p = cur_class->cons; p->next != NULL; p = p->next);
  p->next = cons;
}

void
addParamType(ParamType * param)
{
  ParamType *p;

  cur_method->numParams++;

  if (cur_method->params == NULL) {
    cur_method->params = param;
    return;
  }

  for (p = cur_method->params; p->next != NULL; p = p->next);
  p->next = param;
}

/*
 * Returns the type of the given field in the current class, or NULL if
 * the field is not found. 
 */
static Type *
getFieldType(const char *name, ClassType * class)
{
  FieldType *p;

  assert(class != NULL);

  p = class->fields;
  while (p != NULL) {
    if (strcmp(p->name, name) == 0) {
      return p->type;
    }
    p = p->next;
  }

  return NULL;
}

Type *
searchFieldType(const char *name, ClassType * class)
{
  Type *type;

  assert(class != NULL);

  do {
    type = getFieldType(name, class);
    class = class->parent;
  } while (class != NULL && type == NULL);

  return type;
}

List *
searchMethods(const char *name, Seq * params, ClassType * class)
{
  List *ret = NULL;
  List *l;
  ClassType *p;
  MethodType *mp;

  for (p = class; p != NULL; p = p->parent) {
    for (mp = p->methods; mp != NULL; mp = mp->next) {

      if (strcmp(mp->name, name) == 0
          && validParams(params, mp->params) == 1) {

        /*
         * Scan the list to see if this method is already over ridden. 
         */
        for (l = ret; l != NULL; l = l->next) {
          if (same_sigs(mp, (MethodType *) l->data) == 1)
            break;
        }

        if (l == NULL)
          ret = list_add(ret, mp);
      }

    }
  }

  return ret;
}

List *
searchConstructor(Seq * params, ClassType * class)
{
  List *ret = NULL;
  MethodType *mp;

  for (mp = class->cons; mp != NULL; mp = mp->next) {

    if (validParams(params, mp->params) == 1) {
      ret = list_add(ret, mp);
    }
  }

  return ret;
}

int
validParams(Seq * params, ParamType * mparms)
{
  Seq *p;
  ParamType *mp;
  Type *src,
    *dest;

  for (mp = mparms, p = params;
       p != NULL && mp != NULL; p = (Seq *) p->next, mp = mp->next) {

    assert(p->super.type == SEQ_NODE);
    assert(isExpr(p->data) == 1);

    src = ((Expr *) p->data)->type;
    dest = mp->type;

    if (meth_convertable(src, dest) == 0)
      return 0;
  }

  if (p != NULL || mp != NULL)
    return 0;

  return 1;
}

void
printMethod(const MethodType * t)
{
  ParamType *parms;

  printf("%s(", t->name);

  if (t->params == NULL) {
    printf(")");
  } else {
    for (parms = t->params; parms != NULL; parms = parms->next) {
      printType(parms->type);
      printf(" %s,", parms->name);
    }
    printf("\b)");
  }
}

ClassType *
getCurClass()
{
  return cur_class;
}

MethodType *
getCurMethod()
{
  return cur_method;
}

ParamType *
getParameter(char *name, MethodType * method)
{
  ParamType *p;

  for (p = method->params; p != NULL; p = p->next) {
    if (strcmp(p->name, name) == 0)
      return p;
  }

  return NULL;
}

/*
 * Returns 1 if there is a field with the give name declared in the
 * current class. 
 */
static int
fieldIsDeclared(const char *name)
{
  FieldType *p;

  assert(cur_class != NULL);
  for (p = cur_class->fields; p != NULL; p = p->next) {
    if (strcmp(name, p->name) == 0) {
      return 1;
    }
  }

  return 0;
}

/*
 * Returns 1 if there is already a class with the given name declared. 
 */
static int
classIsDeclared(const char *name)
{
  ClassType *p;

  for (p = classes; p != NULL; p = p->next) {
    if (strcmp(name, p->name) == 0) {
      return 1;
    }
  }

  return 0;
}

/*
 * Returns 1 if there is already a parameter with the given name declared
 * in the current method or constructor. 
 */
static int
paramIsDeclared(const char *name)
{
  ParamType *p;

  for (p = cur_method->params; p != NULL; p = p->next) {
    if (strcmp(name, p->name) == 0) {
      return 1;
    }
  }

  return 0;
}

/*
 * Finds all CLASS_NODES and creates an empty ClassType for them. 
 */
static AstNode *
findClassNames(AstNode * n)
{
  Seq *seq;
  Class *class;
  ClassType *class_type;

  if (n == NULL)
    return n;

  switch (n->type) {
  case SEQ_NODE:
    seq = (Seq *) n;
    assert(seq->super.type == SEQ_NODE);

    seq->data = findClassNames(seq->data);
    seq->next = findClassNames(seq->next);
    return n;

  case CLASS_NODE:
    class = (Class *) n;
    assert(class->super.type == CLASS_NODE);

    /*
     * ident = (NameId *) class->ident; assert(ident->super.type ==
     * NAMEID_NODE);
     * 
     * class_type = makeClassType(ident->name); 
     */
    class_type = makeClassType(class->name);

    /* Print the error for the user added Object class, not the compiler
     * added object, or for redeclarations of classes.  Also prune the
     * duplicated class. */
    if (classIsDeclared(class->name) == 1 || 
	(strcmp(class->name, OBJECT_NAME) == 0 && class->super.line != 0)) {
      error("redeclaration of class", &class->super);
      return NULL;
    }

    addClassType(class_type);
    return n;

  default:
    return n;
  }
}

/*
 * Parses a list of field declarators. 
 */
static void
parseFields(Type * type, AstNode * decls)
{
  Seq *p;
  Type *t;

  p = (Seq *) decls;
  assert(p->super.type == SEQ_NODE);

  while (p != NULL) {
    t = copyArrays(type);
    t = appendArray(t, (ArrayType *) getDims(p->data));

    if (fieldIsDeclared(getIdent(p->data)) == 1)
      error("redeclaration of field", p->data);

    addFieldType(makeFieldType(getIdent(p->data), t));

    p = (Seq *) p->next;
    if (p != NULL)
      assert(p->super.type == SEQ_NODE);
  }
}

/*
 * Parses a parameter list.  Returns 1 on success and 0 on error.
 */
static int
parseParams(AstNode * n)
{
  Seq *p;
  FormalParam *fparam;
  Type *type;
  char *name;
  ParamType *param_type;

  p = (Seq *) n;
  if (p != NULL)
    assert(p->super.type == SEQ_NODE);

  while (p != NULL) {
    fparam = (FormalParam *) p->data;
    assert(fparam->super.super.type == FORMALPARAM_NODE);

    name = getIdent(fparam->decl);

    if (paramIsDeclared(name) == 1) {
      error("redefinition of parameter", &fparam->super.super);
      return 0;
    }

    /*
     * If there are no dimentions, the result of
     * buildType(fparam->type) is returned by this appention. 
     */
    type = appendArray(buildType(fparam->type),
                       (ArrayType *) getDims(fparam->decl));

    fparam->super.type = type;
    param_type = makeParamType(name, type, param_number);
    fparam->num = param_number++;
    addParamType(param_type);

    p = (Seq *) p->next;
    if (p != NULL)
      assert(p->super.type == SEQ_NODE);
  }

  return 1;
}

/*
 * Parses a method declaration.  Returns the created MethodType, or an
 * ERROR_TYPE if no method was added.
 */
static Type *
parseMethod(Type * type, AstNode * n)
{
  MethDeclator *mdecl;
  MethodType *method_type;
  MethodType *p;

  if (n == NULL)
    return makeErrorType();

  switch (n->type) {
  case ARRAY_NODE:
    parseMethod(&makeArrayType(type)->super, ((Array *) n)->child);
    break;
  case METHDECLATOR_NODE:
    mdecl = (MethDeclator *) n;
    assert(mdecl->super.type == METHDECLATOR_NODE);

    method_type = makeMethodType(mdecl->name, type);
    method_type->class = getCurClass();

    setCurrentMethod(method_type);
    param_number = 1;
    if (parseParams(mdecl->param) == 0) {
      setCurrentMethod(NULL);
      return makeErrorType();
    }
    setCurrentMethod(NULL);

    /*
     * Make sure this method's signature isn't already defined stated
     * in 4.4.1 of the T-spec v2. 
     */
    for (p = cur_class->methods; p != NULL; p = p->next) {
      if (same_sigs(p, method_type) == 1)
        error("duplicate method signature", &mdecl->super);
    }

    addMethodType(method_type);
    return &method_type->super;

  default:
    break;
  }

  return makeErrorType();
}

/*
 * Parses a method declaration. 
 */
static Type *
parseConstructor(AstNode * n)
{
  MethDeclator *mdecl;
  MethodType *method_type;
  MethodType *p;

  if (n == NULL)
    return makeErrorType();

  switch (n->type) {
  case CONSTDECLATOR_NODE:
    mdecl = (MethDeclator *) n;
    assert(mdecl->super.type == CONSTDECLATOR_NODE);

    /*
     * Make the constructor has the correct name. 
     */
    if (strcmp(mdecl->name, cur_class->name) != 0)
      error("constructor name is not the class name", &mdecl->super);

    method_type = makeConstructorType(mdecl->name);
    method_type->class = getCurClass();

    setCurrentMethod(method_type);
    param_number = 1;
    parseParams(mdecl->param);
    setCurrentMethod(NULL);

    /*
     * Check for duplicate constructor signature. 
     */
    for (p = cur_class->cons; p != NULL; p = p->next) {
      if (same_sigs(p, method_type) == 1)
        error("duplicate constructor signature", &mdecl->super);
    }

    addConstructorType(method_type);
    return &method_type->super;

  default:
    break;
  }

  return makeErrorType();
}

static void
parseClass(AstNode * n)
{
  Class *class;
  ClassType *class_type;
  ClassType *parent_type;
  MethodType *con;

  class = (Class *) n;
  assert(class->super.type == CLASS_NODE);

  class_type = getClass(class->name);
  assert(class_type != NULL);

  /*
   * Fill in the class's parent. 
   */
  if (class->pname == NULL && strcmp(class_type->name, OBJECT_NAME) != 0) {
    class->pname = OBJECT_NAME;
  }

  if (class->pname != NULL) {
    parent_type = getClass(class->pname);

    /*
     * 4.4.1 of the T-spec v2, classification of name according to 
     * * context. 
     */
    if (parent_type == NULL)
      error("class name required", &class->super);

    class_type->parent = parent_type;

    /*
     * Check for interdependent classes. 
     */
    if (isSuper(class_type, parent_type))
      error("self dependent class", &class->super);

  }

  /*
   * Recurse into this classes body. 
   */
  setCurrentClass(class_type);
  fillClasses(class->body);

  if (class_type->cons == NULL) {
    con = makeConstructorType(class_type->name);
    con->class = class_type;
    con->def = 1;
    addConstructorType(con);

    if (class->body != NULL) {
      appendSeq(makeDefaultConstructor(con), class->body);
    } else {
      class->body = makeDefaultConstructor(con);
    }
  }

  setCurrentClass(NULL);
}

/*
 * Fills in the empty class datastructures made by findClassNames. 
 */
static void
fillClasses(AstNode * n)
{
  Seq *seq;
  MemberDecl *member;

  if (n == NULL)
    return;

  switch (n->type) {
  case SEQ_NODE:
    seq = (Seq *) n;
    fillClasses(seq->data);
    fillClasses(seq->next);
    break;

  case CLASS_NODE:
    parseClass(n);
    break;

  case METHDECL_NODE:
    member = (MemberDecl *) n;
    assert(member->super.type == METHDECL_NODE);

    assert(cur_class != NULL);
    member->t = parseMethod(buildType(member->type), member->decl);
    break;

  case CONSTDECL_NODE:
    member = (MemberDecl *) n;
    assert(member->super.type == CONSTDECL_NODE);

    assert(cur_class != NULL);
    member->t = parseConstructor(member->decl);
    break;

  case FIELDDECL_NODE:
    member = (MemberDecl *) n;
    assert(member->super.type == FIELDDECL_NODE);

    assert(cur_class != NULL);
    parseFields(buildType(member->type), member->decl);
    break;

  default:
    break;
  }

}

void
buildClasses(AstNode * n)
{
  /*
   * Build the primordial Object class, and add it to the tree.
   */
  appendSeq(makeSeq(makeObject()), ast);

  /*
   * Find class names and allocate them to the list so that we can point
   * to them later. 
   */
  findClassNames(n);

  /*
   * Fill in the information for class structures. 
   */
  fillClasses(n);
}

static void
printFields(FieldType * fields)
{
  FieldType *p;

  for (p = fields; p != NULL; p = p->next) {
    printf("\t");
    printType(p->type);
    printf(" %s\n", p->name);
  }
}

static void
printParams(ParamType * params)
{
  ParamType *p;

  for (p = params; p != NULL; p = p->next) {
    printType(p->type);
    printf(" %s", p->name);
    if (p->next != NULL)
      printf(", ");
  }
}

static void
printMethods(MethodType * methods)
{
  MethodType *p;

  for (p = methods; p != NULL; p = p->next) {
    printf("\t");
    printType(p->type);
    printf(" %s(", p->name);
    printParams(p->params);
    printf(") { ... }\n");
  }
}

static void
printConstructors(MethodType * consts)
{
  MethodType *p;

  for (p = consts; p != NULL; p = p->next) {
    printf("\t");
    printf("%s(", p->name);
    printParams(p->params);
    printf(") { ... }\n");
  }
}

void
printClasses()
{
  ClassType *p;

  for (p = classes; p != NULL; p = p->next) {

    if (p->parent == NULL) {
      printf("class %s\n", p->name);
    } else {
      printf("class %s extends %s {", p->name, p->parent->name);
    }

    if (p->fields != NULL)
      printf("\n");
    printFields(p->fields);

    if (p->cons != NULL)
      printf("\n");
    printConstructors(p->cons);

    if (p->methods != NULL)
      printf("\n");
    printMethods(p->methods);

    printf("}\n");

  }

}

int
isSuper(ClassType * t, ClassType * s)
{
  ClassType *p;

  for (p = s; p != NULL; p = p->parent)
    if (p == t)
      return 1;

  return 0;
}

/*
 * Forms a string of the given parameter names.
 * Strings returned by this function must be freed with `free'
 */
static char *
getParamString(ParamType *head)
{
	char *param_string;
	char *retval;
	char *type_name;
	unsigned int len;

	if(head == NULL) {
		retval = malloc(sizeof(char));
		retval[0] = '\0';
		return retval;
	}

	type_name = getTypeName(head->type);

	param_string = getParamString(head->next);
	len = strlen(param_string) + strlen(type_name) + 2;

	retval = malloc(sizeof(char)*len);
	strncpy(retval, type_name, len);
	strncat(retval, "$", len);
	strncat(retval, param_string, len);

	free(type_name);
	free(param_string);
	return retval;
}

char *
getMungedName(MethodType *t)
{
  char *retval;
  char *class = t->class->name;
	char *param_string;
  unsigned int len;

	param_string = getParamString(t->params);
	len = strlen(class)+strlen(t->name)+strlen(param_string)+6; 

  retval = malloc(sizeof(char)*len);
  strncpy(retval, class, len);
  strncat(retval, "$", len);

  if(t->super.tag == CONSTRUCTOR_TYPE) 
    strncat(retval, "$", len);

  strncat(retval, t->name, len);
	strncat(retval, "$", len);
	strncat(retval, param_string, len);

	free(param_string);
  return retval;
}

char *getVMTName(Type *t)
{
  char *retval;
	ClassType *class_type;
  unsigned int len;

  if(t == NULL || t->tag != CLASS_TYPE) {
    len = 2;
  } else {
		class_type = (ClassType*)t;
    len =  strlen(class_type->name)+sizeof("$VMT")+1;
  }

  retval = malloc(sizeof(char) * len);

  if(t == NULL || t->tag != CLASS_TYPE) {
    strncpy(retval, "0", len);
  } else {
    strncpy(retval, class_type->name, len);
    strncat(retval, "$VMT", len);
  }

  return retval;
}

char *getDestructorName(const char *class)
{
  char *retval;
  unsigned int len = strlen(class)+sizeof("$$destructor")+1;

  retval = malloc(sizeof(char) * len);

  strncpy(retval, class, len);
  strncat(retval, "$$destructor", len);

  return retval;
}

List *getMethods(ClassType *class)
{
  MethodType *mp, *pmeth;
  unsigned int override;
  List *p;
  List *smeths; 
  List *tmeths = NULL;

  if(class == NULL) 
    return NULL;

  /* Get the parent's methods. */
  smeths = getMethods(class->parent);

  /* For each method of the current class. */
  for(mp = class->methods; mp != NULL; mp = mp->next) {

    /* For each parent method. */
    override = 0;
    for(p = smeths; p != NULL; p = p->next) {
      pmeth = (MethodType*)p->data;
	
      /* If the current method over rides a parent method.
       * replace the over ridden method in the list. */
      if(same_sigs(pmeth, mp) == 1) {
				p->data = mp;
				override = 1;
      }

    }

    /* If this method is not overriding another, add it
     * to the tmeths list. */
    if(override == 0) {
      tmeths = list_add(tmeths, mp);	
    }

  }

  /* The result is the this methods appended to the super methods. */
  return append_list(smeths, tmeths);
}

unsigned int getNumFields(ClassType *class)
{
  unsigned int pfields;
  
  if(class == NULL)
    return 0;

  pfields = getNumFields(class->parent);

  return class->numFields + pfields;
}

unsigned int getFieldNum(ClassType *class, char *fieldName)
{
  unsigned int pfields;
  unsigned int found;
  unsigned int n;
  FieldType *fp;

  pfields = getNumFields(class->parent);

  for(found = 0, n = pfields, fp = class->fields; fp != NULL; fp = fp->next) {
    if(strcmp(fp->name, fieldName) == 0) {
      found = 1;
      break;
    }

    n++;
  }

  if(found == 0) {
/*
    fprintf(stderr,"not found checking parent\n");
*/
    return getFieldNum(class->parent, fieldName);
  }

  return n;
}

unsigned int 
getMethodNum(ClassType *class, MethodType *method)
{
  List *l, *meths;
  unsigned int num = 0;

  meths = getMethods(class);
  for(l = meths; l != NULL; l = l->next) {
    if(same_sigs((MethodType*)l->data, method) == 1)
      return num;

    num++;
  }

  return -1;
}
