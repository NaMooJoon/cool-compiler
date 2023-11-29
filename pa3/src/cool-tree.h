#ifndef COOL_TREE_H
#define COOL_TREE_H
//////////////////////////////////////////////////////////
//
// file: cool-tree.h
//
// This file defines classes for each phylum and constructor
//
//////////////////////////////////////////////////////////


#include "tree.h"
#include "cool-tree.handcode.h"
#include "semant.h"
#include "symtab.h"
#include <utility>

// global variable
class ClassTable;
extern ClassTable* g_ct;
// ostream& ClassTable::semant_error(Class_ c);

// define the class for phylum
// define simple phylum - Program
typedef class Program_class *Program;

class Program_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Program(); }
   virtual Program copy_Program() = 0;

#ifdef Program_EXTRAS
   Program_EXTRAS
#endif
};


// define simple phylum - Class_
typedef class Class__class *Class_;

class Class__class : public tree_node {
public:
   tree_node *copy()		 { return copy_Class_(); }
   virtual Class_ copy_Class_() = 0;
   virtual Symbol get_parent() = 0;
   virtual Symbol get_name() = 0;
   virtual Features get_features() = 0;
   virtual cool::SymbolTable<Symbol, Entry>* get_attr_symtab() = 0;
   virtual cool::SymbolTable<Symbol, Entry>* get_method_symtab() = 0;
   virtual void set_attr_symtab(cool::SymbolTable<Symbol, Entry>* s) {};
   virtual void set_method_symtab(cool::SymbolTable<Symbol, Entry>* s) {};
   virtual Feature get_feature_by_symbol(Symbol m) = 0;

#ifdef Class__EXTRAS
   Class__EXTRAS
#endif
};


// define simple phylum - Feature
typedef class Feature_class *Feature;

class Feature_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Feature(); }
   virtual Feature copy_Feature() = 0;
   virtual Symbol get_name() = 0;
   virtual Symbol get_type() = 0;
   virtual bool is_attr() = 0;
   virtual int check_scope(Class_, cool::SymbolTable<Symbol, Entry>*) = 0;
   virtual int check_type(Class_, cool::SymbolTable<Symbol, Entry>*) = 0;
   virtual Formals get_formals() = 0;

#ifdef Feature_EXTRAS
   Feature_EXTRAS
#endif
};


// define simple phylum - Formal
typedef class Formal_class *Formal;

class Formal_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Formal(); }
   virtual Formal copy_Formal() = 0;
   virtual Symbol get_name() = 0;
   virtual Symbol get_type() = 0;

#ifdef Formal_EXTRAS
   Formal_EXTRAS
#endif
};


// define simple phylum - Expression
typedef class Expression_class *Expression;

class Expression_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Expression(); }
   virtual Expression copy_Expression() = 0;
   virtual int check_scope(Class_, cool::SymbolTable<Symbol, Entry>*) { return 0; }
   virtual int check_type(Class_, cool::SymbolTable<Symbol, Entry>*) { return 0; };
   virtual Symbol infer_type(Class_, cool::SymbolTable<Symbol, Entry>*) {
      return NULL;
   }
   virtual Symbol infer_type2(Class_, cool::SymbolTable<Symbol, Entry>*) {
      return NULL;
   }

#ifdef Expression_EXTRAS
   Expression_EXTRAS
#endif
};


// define simple phylum - Case
typedef class Case_class *Case;

class Case_class : public tree_node {
public:
   tree_node *copy()		 { return copy_Case(); }
   virtual Case copy_Case() = 0;
   virtual int check_scope(Class_, cool::SymbolTable<Symbol, Entry>*) = 0;
   virtual Symbol infer_type(Class_, cool::SymbolTable<Symbol, Entry>*) = 0;

#ifdef Case_EXTRAS
   Case_EXTRAS
#endif
};


// define the class for phylum - LIST
// define list phlyum - Classes
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;


typedef list_node<Feature> Features_class;
typedef Features_class *Features;


// define list phlyum - Formals
typedef list_node<Formal> Formals_class;
typedef Formals_class *Formals;


// define list phlyum - Expressions
typedef list_node<Expression> Expressions_class;
typedef Expressions_class *Expressions;


// define list phlyum - Cases
typedef list_node<Case> Cases_class;
typedef Cases_class *Cases;


// define the class for constructors
// define constructor - program
class program_class : public Program_class {
protected:
   Classes classes;
public:
   program_class(Classes a1) {
      classes = a1;
   }
   Program copy_Program();
   void dump(ostream& stream, int n);

#ifdef Program_SHARED_EXTRAS
   Program_SHARED_EXTRAS
#endif
#ifdef program_EXTRAS
   program_EXTRAS
#endif
};


// define constructor - class_
class class__class : public Class__class {
protected:
   Symbol name;
   Symbol parent;
   Features features;
   Symbol filename;
   cool::SymbolTable<Symbol, Entry>* attr_symtab;
   cool::SymbolTable<Symbol, Entry>* method_symtab;
public:
   class__class(Symbol a1, Symbol a2, Features a3, Symbol a4) {
      name = a1;
      parent = a2;
      features = a3;
      filename = a4;
      attr_symtab = new cool::SymbolTable<Symbol, Entry>();
      method_symtab = new cool::SymbolTable<Symbol, Entry>();
      
      attr_symtab->enterscope();
      method_symtab->enterscope();
   }
   Class_ copy_Class_();
   void dump(ostream& stream, int n);
   Symbol get_parent() { return parent; }
   Symbol get_name() { return name; }
   Features get_features() { return features; }
   cool::SymbolTable<Symbol, Entry>* get_attr_symtab() { return attr_symtab; }
   cool::SymbolTable<Symbol, Entry>* get_method_symtab() { return method_symtab; }
   void set_attr_symtab(cool::SymbolTable<Symbol, Entry>* s) { attr_symtab = s; }
   void set_method_symtab(cool::SymbolTable<Symbol, Entry>* s) { method_symtab = s; }
   Feature get_feature_by_symbol(Symbol m) {
      for (int i = features->first(); features->more(i); i = features->next(i)) {
         Feature f = features->nth(i);
         if (f->get_name() == m) {
            return f;
         }
      }
      return NULL;
   }

#ifdef Class__SHARED_EXTRAS
   Class__SHARED_EXTRAS
#endif
#ifdef class__EXTRAS
   class__EXTRAS
#endif
};


// define constructor - method
class method_class : public Feature_class {
protected:
   Symbol name;
   Formals formals;
   Symbol return_type;
   Expression expr;
   bool _is_attr;
public:
   method_class(Symbol a1, Formals a2, Symbol a3, Expression a4) {
      name = a1;
      formals = a2;
      return_type = a3;
      expr = a4;
      _is_attr = false;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);
   Symbol get_name() { return name; }
   Symbol get_type() { return return_type; }
   bool is_attr() { return _is_attr; }
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {

      // if (o->lookup(name) != nullptr) {
      //    g_ct->semant_error(c->get_filename(), this);
      //    cerr << "Method " << name << " is multiply defined.222" << endl;
      //    return 0;
      // }
      o->addid(name, return_type);

      Symbol parent = c->get_parent();
      Class_ p = g_ct->m[parent];
      if (p->get_method_symtab()->lookup(name) != nullptr) {
         Feature f = p->get_feature_by_symbol(name);
         if (f->get_type() != return_type) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "In redefined method " << name << ", return type " << return_type << " is different from original return type " << f->get_type() << "." << endl;
         }
         else if (f->get_formals()->len() != formals->len()) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "Incompatible number of formal parameters in redefined method " << name << "." << endl;
         }
         else {
            for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
               Formal f1 = formals->nth(i);
               Formal f2 = f->get_formals()->nth(i);
               if (f1->get_type() != f2->get_type()) {
                  g_ct->semant_error(c->get_filename(), this);
                  cerr << "In redefined method " << name << ", parameter type " << f1->get_type() << " is different from original type " << f2->get_type() << "." << endl;
               }
            }
         }
      }
      
      return 0;
   }
   int check_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      int errors = 0;
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      o->enterscope(); // enter method scope
      // iterate through formals
      if (formals->len() > 0) {
         for (int i = formals->first(); formals->more(i); i = formals->next(i)) {
            Formal f = formals->nth(i);
            
            if (o->probe(f->get_name()) != NULL) { // check if formal is already defined
               errors++;
               g_ct->semant_error(c->get_filename(), f);
               cerr  << "Formal parameter " << f->get_name() << " is multiply defined." << endl;
            }
            else {
               o->addid(f->get_name(), f->get_type());
            }
         }
      }

      // TODO: return type scope check
      if (!g_ct->m.count(return_type)) {
         errors++;
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Undefined return type " << return_type << " in method " << name << "." << endl;
      } 
      // check expression scope
      errors += expr->check_scope(c, o);
      errors += expr->check_type(c, o);
      Symbol expr_type = expr->infer_type(c, o);
      o->exitscope(); // exit method scope


      Symbol expr_type2 = expr_type;
      Symbol return_type2 = return_type;
      if (expr_type == SELF_TYPE) {
         expr_type2 = expr->infer_type2(c, o);
         if (expr_type2 == NULL) {
            expr_type2 = c->get_name();
         }
      }
      if (return_type == SELF_TYPE) {
         return_type2 = c->get_name();
      }
      // if expr_type is undefined.
      if (!g_ct->m.count(expr_type2) || !g_ct->m.count(return_type2)) {
         return errors;
      }
      if (!g_ct->partial_ordered(expr_type2, return_type2)) {
         errors++;
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Inferred return type " << expr_type << " of method " << name << " does not conform to declared return type " << return_type << "." << endl;
         return errors; // error
      }


      return errors;   // no error
   }
   Formals get_formals() {
      return formals;
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef method_EXTRAS
   method_EXTRAS
#endif
};


// define constructor - attr
class attr_class : public Feature_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression init;
   bool _is_attr;
public:
   attr_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      init = a3;
      _is_attr = true;
   }
   Feature copy_Feature();
   void dump(ostream& stream, int n);
   Symbol get_name() { return name; }
   Symbol get_type() { return type_decl; }
   bool is_attr() { return _is_attr; }
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // Check the scope of the attirbute's initialization expression

      if (o->lookup(name) != nullptr) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Attribute " << name << " is multiply defined in class." << endl;
         return 1; // error
      }

      Symbol parent = c->get_parent();
      Class_ p = g_ct->m[parent];
      if (p->get_attr_symtab()->lookup(name) != nullptr) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Attribute " << name << " is an attribute of an inherited class." << endl;
         return 1; // error
      }
      o->addid(name, type_decl);

      o->enterscope();
      int errors = init->check_scope(c, o);
      o->exitscope();

      return errors;
   }
   int check_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {

      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");
      Symbol No_type = idtable.add_string("_no_type");

      Symbol expr_type = init->infer_type(c, o);

      Symbol attr_type = type_decl;

      Symbol expr_type2 = expr_type;
      if (expr_type == SELF_TYPE) {
         expr_type2 = init->infer_type2(c, o);
         if (expr_type2 == NULL) {
            expr_type2 = c->get_name();
         }
      }
      if (attr_type == SELF_TYPE) {
         attr_type = c->get_name();
      }
      if (!g_ct->m.count(expr_type) || !g_ct->m.count(attr_type)) {
         return 0;
      }
      if (expr_type != No_type) {
         if (!g_ct->partial_ordered(expr_type2, attr_type)) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "Inferred type " << expr_type << " of initialization of attribute " << name << " does not conform to declared type " << type_decl << "." << endl;
            return 1; // error
         }
      }

      return 0;   // no error
   }
   Formals get_formals() {
      return list_node<Formal>::nil();
   }

#ifdef Feature_SHARED_EXTRAS
   Feature_SHARED_EXTRAS
#endif
#ifdef attr_EXTRAS
   attr_EXTRAS
#endif
};


// define constructor - formal
class formal_class : public Formal_class {
protected:
   Symbol name;
   Symbol type_decl;
public:
   formal_class(Symbol a1, Symbol a2) {
      name = a1;
      type_decl = a2;
   }
   Formal copy_Formal();
   void dump(ostream& stream, int n);
   Symbol get_name() { return name; }
   Symbol get_type() { return type_decl; }

#ifdef Formal_SHARED_EXTRAS
   Formal_SHARED_EXTRAS
#endif
#ifdef formal_EXTRAS
   formal_EXTRAS
#endif
};


// define constructor - branch
class branch_class : public Case_class {
protected:
   Symbol name;
   Symbol type_decl;
   Expression expr;
public:
   branch_class(Symbol a1, Symbol a2, Expression a3) {
      name = a1;
      type_decl = a2;
      expr = a3;
   }
   Case copy_Case();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      o->enterscope();
      o->addid(name, type_decl);
      int errors = expr->check_scope(c, o);

      o->exitscope();

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // TODO
      if (o->lookup(type_decl) == nullptr) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Class " << type_decl << " of case branch is undefined." << endl;
      }

      o->enterscope();
      o->addid(name, type_decl);

      Symbol expr_type = expr->infer_type(c, o);

      o->exitscope();

      return expr_type;
   }

#ifdef Case_SHARED_EXTRAS
   Case_SHARED_EXTRAS
#endif
#ifdef branch_EXTRAS
   branch_EXTRAS
#endif
};


// define constructor - assign
class assign_class : public Expression_class {
protected:
   Symbol name;
   Expression expr;
public:
   assign_class(Symbol a1, Expression a2) {
      name = a1;
      expr = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;

      if (o->lookup(name) == NULL) { // OBJECTID not in object scope
         if (c->get_attr_symtab()->lookup(name) == NULL) { // OBJECTID not in class scope
            errors++;
            g_ct->semant_error(c->get_filename(), this);
            cerr << "Assignment to undeclared variable " << name << "." << endl;
         }
      }
      errors += expr->check_scope(c, o);

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o){
      Symbol expr_type = expr->infer_type(c, o);

      Symbol No_type = idtable.add_string("_no_type");
      Symbol Object = idtable.add_string("Object");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol var_type;
      if (o->lookup(name) != NULL) {
         var_type = o->lookup(name);
      } else if (c->get_attr_symtab()->lookup(name) != NULL){
         var_type = c->get_attr_symtab()->lookup(name);
      } else {
         return No_type;
      }

      Symbol var_type2 = var_type;
      if (var_type == SELF_TYPE) {
         var_type2 = c->get_name();
      }
      Symbol expr_type2 = expr_type;
      if (expr_type == SELF_TYPE) {
         expr_type2 = expr->infer_type2(c, o);
         if (expr_type2 == NULL) 
            expr_type2 = c->get_name();
      }
      if (!g_ct->m.count(expr_type2) || !g_ct->m.count(var_type2)) 
         return No_type;

      // check if the inferred type of the expression conforms to the type of the variable
      if (!g_ct->partial_ordered(expr_type2, var_type2)) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Type " << expr_type << " of assigned expression does not conform to declared type " << var_type << " of identifier " << name << "." << endl;
         return No_type;
      }
      set_type(var_type);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef assign_EXTRAS
   assign_EXTRAS
#endif
};


// define constructor - static_dispatch
class static_dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol type_name;
   Symbol name;
   Expressions actual;
public:
   static_dispatch_class(Expression a1, Symbol a2, Symbol a3, Expressions a4) {
      expr = a1;
      type_name = a2;
      name = a3;
      actual = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // TODO
      int errors = 0;

      errors += expr->check_scope(c, o);
     
      for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
         errors += actual->nth(i)->check_scope(c, o);
      }

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol No_class = idtable.add_string("_no_class");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");
      Symbol Object = idtable.add_string("Object");

      Symbol expr_type = expr->infer_type(c, o);

      set_type(expr_type);
      if (expr_type == SELF_TYPE) {
         expr_type = c->get_name();
      }

      if (!g_ct->partial_ordered(expr_type, type_name)) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Expression type " << expr_type << " does not conform to declared static dispatch type " << type_name << "." << endl;
         set_type(Object);
         return Object;
      }

      Class_ dispatch_class = g_ct->m[type_name];
      if (dispatch_class == NULL) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch on undefined class " << expr_type << "." << endl;
         set_type(Object);
         return get_type();
      }


      Feature f;
      while (expr_type != No_class) {
         f = dispatch_class->get_feature_by_symbol(name);
         if (f != NULL) {
            break;
         }
         expr_type = dispatch_class->get_parent();
         dispatch_class = g_ct->m[expr_type];
      }
      if (expr_type == No_class) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch to undefined method " << name << "." << endl;
         set_type(Object);
         return Object;
      }
      
      Formals formals = f->get_formals();
      if (formals->len() != actual->len()) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Method " << name << " called with wrong number of arguments." << endl;
         set_type(Object);
         return Object;
      }
      for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
         Symbol actual_type = actual->nth(i)->infer_type(c, o);
         Symbol formal_type = formals->nth(i)->get_type();
         Symbol actual_type2 = actual_type;
         Symbol formal_type2 = formal_type;
         if (actual_type == SELF_TYPE) {
            actual_type2 = actual->nth(i)->infer_type2(c, o);
            if (actual_type2 == NULL) 
               actual_type2 = c->get_name();
         }
         if (formal_type == SELF_TYPE) {
               formal_type2 = c->get_name();
         }
         if (!g_ct->partial_ordered(actual_type2, formal_type2)) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "1In call of method " << name << ", type " << actual_type << " of parameter " << formals->nth(i)->get_name() << " does not conform to declared type " << formal_type << "." << endl;
            set_type(Object);
            return Object;
         }
      }
      Symbol return_type = f->get_type();
      // if (return_type == SELF_TYPE) {
      //    return get_type();
      // }
      set_type(return_type);
      return get_type();
   }
   Symbol infer_type2(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol No_class = idtable.add_string("_no_class");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");
      Symbol Object = idtable.add_string("Object");

      Symbol expr_type = expr->infer_type(c, o);

      if (expr_type == SELF_TYPE) {
         expr_type = c->get_name();
      }
      Symbol temp_type = expr_type;

      if (!g_ct->partial_ordered(expr_type, type_name)) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Expression type " << expr_type << " does not conform to declared static dispatch type " << type_name << "." << endl;
         set_type(Object);
         return Object;
      }

      Class_ dispatch_class = g_ct->m[type_name];
      if (dispatch_class == NULL) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch on undefined class " << expr_type << "." << endl;
         set_type(Object);
         return get_type();
      }


      Feature f;
      while (expr_type != No_class) {
         f = dispatch_class->get_feature_by_symbol(name);
         if (f != NULL) {
            break;
         }
         expr_type = dispatch_class->get_parent();
         dispatch_class = g_ct->m[expr_type];
      }
      if (expr_type == No_class) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch to undefined method " << name << "." << endl;
         set_type(Object);
         return Object;
      }
      
      Formals formals = f->get_formals();
      if (formals->len() != actual->len()) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Method " << name << " called with wrong number of arguments." << endl;
         set_type(Object);
         return Object;
      }
      for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
         Symbol actual_type = actual->nth(i)->infer_type(c, o);
         Symbol formal_type = formals->nth(i)->get_type();
         
         if (!g_ct->partial_ordered(actual_type, formal_type)) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "2thod " << name << ", type " << actual_type << " of parameter " << formals->nth(i)->get_name() << " does not conform to declared type " << formal_type << "." << endl;
            set_type(Object);
            return Object;
         }
      }
      Symbol return_type = f->get_type();
      if (return_type == SELF_TYPE) {
         return temp_type;
      }
      return return_type;
      // set_type(return_type);
      // return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef static_dispatch_EXTRAS
   static_dispatch_EXTRAS
#endif
};


// define constructor - dispatch
class dispatch_class : public Expression_class {
protected:
   Expression expr;
   Symbol name;
   Expressions actual;
public:
   dispatch_class(Expression a1, Symbol a2, Expressions a3) {
      expr = a1;
      name = a2;
      actual = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;

      errors += expr->check_scope(c, o);

      for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
         errors += actual->nth(i)->check_scope(c, o);
      }

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol No_class = idtable.add_string("_no_class");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");
      Symbol Object = idtable.add_string("Object");

      Symbol expr_type = expr->infer_type(c, o);
      set_type(expr_type);
      if (expr_type == SELF_TYPE) {
         expr_type = c->get_name();
      }
      // std::cout << "dispatch1] expr_type: " << expr_type << std::endl;
      Class_ dispatch_class = g_ct->m[expr_type];
      if (dispatch_class == NULL) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch on undefined class " << expr_type << "." << endl;
         set_type(Object);
         return get_type();
      }

      Feature f;
      while (expr_type != No_class) {
         f = dispatch_class->get_feature_by_symbol(name);
         if (f != NULL) {
            break;
         }
         expr_type = dispatch_class->get_parent();
         dispatch_class = g_ct->m[expr_type];
      } 

      if (expr_type == No_class) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch to undefined method " << name << "." << endl;
         set_type(Object);
         return get_type();
      }
      // std::cout << "method: " << name  << " 찾는 중" << std::endl;
      // std::cout << "dispatch2] expr_type: " << expr_type << std::endl;

      Formals formals = f->get_formals();
      if (formals->len() != actual->len()) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Method " << name << " called with wrong number of arguments." << endl;
         set_type(Object);
         return get_type();
      }

      for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
         Symbol actual_type = actual->nth(i)->infer_type(c, o);
         Symbol formal_type = formals->nth(i)->get_type();
         Symbol actual_type2 = actual_type;
         Symbol formal_type2 = formal_type;
         if (actual_type == SELF_TYPE) {
            actual_type2 = actual->nth(i)->infer_type2(c, o);
            if (actual_type2 == NULL) 
               actual_type2 = c->get_name();
         }
         if (formal_type == SELF_TYPE) {
               formal_type2 = c->get_name();
         }
         if (!g_ct->partial_ordered(actual_type2, formal_type2)) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "In call of method " << name << ", type " << actual_type << " of parameter " << formals->nth(i)->get_name() << " does not conform to declared type " << formal_type << "." << endl;
            set_type(Object);
            return get_type();
         }
      }
      Symbol return_type = f->get_type();
      // if (return_type == SELF_TYPE) {
      //    return get_type();
      // }
      set_type(return_type);
      // std::cout << "dispatch3] expr_type: " << get_type()  << "\n" << std::endl;
      return get_type();
   }
   Symbol infer_type2(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol No_class = idtable.add_string("_no_class");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");
      Symbol Object = idtable.add_string("Object");

      Symbol expr_type = expr->infer_type(c, o);
      if (expr_type == SELF_TYPE) {
         expr_type = c->get_name();
      }
      Symbol temp_type = expr_type;
      // std::cout << "dispatch1] expr_type: " << expr_type << std::endl;
      Class_ dispatch_class = g_ct->m[expr_type];
      if (dispatch_class == NULL) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch on undefined class " << expr_type << "." << endl;
         set_type(Object);
         return get_type();
      }

      Feature f;
      while (expr_type != No_class) {
         f = dispatch_class->get_feature_by_symbol(name);
         if (f != NULL) {
            break;
         }
         expr_type = dispatch_class->get_parent();
         dispatch_class = g_ct->m[expr_type];
      } 

      if (expr_type == No_class) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Dispatch to undefined method " << name << "." << endl;
         set_type(Object);
         return get_type();
      }
      // std::cout << "method: " << name  << " 찾는 중" << std::endl;
      // std::cout << "dispatch2] expr_type: " << expr_type << std::endl;

      Formals formals = f->get_formals();
      if (formals->len() != actual->len()) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Method " << name << " called with wrong number of arguments." << endl;
         set_type(Object);
         return get_type();
      }

      for (int i = actual->first(); actual->more(i); i = actual->next(i)) {
         Symbol actual_type = actual->nth(i)->infer_type(c, o);
         Symbol formal_type = formals->nth(i)->get_type();
         Symbol actual_type2 = actual_type;
         Symbol formal_type2 = formal_type;
         if (actual_type == SELF_TYPE) {
            actual_type2 = actual->nth(i)->infer_type2(c, o);
            if (actual_type2 == NULL) 
               actual_type2 = c->get_name();
         }
         if (formal_type == SELF_TYPE) {
               formal_type2 = c->get_name();
         }
         if (!g_ct->partial_ordered(actual_type2, formal_type2)) {
            g_ct->semant_error(c->get_filename(), this);
            cerr << "In call of method " << name << ", type " << actual_type << " of parameter " << formals->nth(i)->get_name() << " does not conform to declared type " << formal_type << "." << endl;
            set_type(Object);
            return get_type();
         }
      }
      Symbol return_type = f->get_type();
      if (return_type == SELF_TYPE) {
         return temp_type;
      }
      return return_type;
      // std::cout << "dispatch3] expr_type: " << get_type()  << "\n" << std::endl;
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef dispatch_EXTRAS
   dispatch_EXTRAS
#endif
};


// define constructor - cond
class cond_class : public Expression_class {
protected:
   Expression pred;
   Expression then_exp;
   Expression else_exp;
public:
   cond_class(Expression a1, Expression a2, Expression a3) {
      pred = a1;
      then_exp = a2;
      else_exp = a3;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;

      errors += pred->check_scope(c, o);
      errors += then_exp->check_scope(c, o);
      errors += else_exp->check_scope(c, o);

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool_type = idtable.add_string("Bool");

      Symbol pred_type = pred->infer_type(c, o);
      if (pred_type != Bool_type) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Predicate of 'if' does not have type Bool." << endl;
      }

      Symbol then_type = then_exp->infer_type(c, o);
      Symbol else_type = else_exp->infer_type(c, o);

      Symbol join_type = g_ct->lub(then_type, else_type, c->get_name());

      set_type(join_type);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef cond_EXTRAS
   cond_EXTRAS
#endif
};


// define constructor - loop
class loop_class : public Expression_class {
protected:
   Expression pred;
   Expression body;
public:
   loop_class(Expression a1, Expression a2) {
      pred = a1;
      body = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;

      errors += pred->check_scope(c, o);
      errors += body->check_scope(c, o);

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Object = idtable.add_string("Object");
      Symbol Bool_type = idtable.add_string("Bool");
      Symbol pred_type = pred->infer_type(c, o);

      if (pred_type != Bool_type) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Loop condition does not have type Bool." << endl;
      }

      body->infer_type(c, o);

      set_type(Object);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef loop_EXTRAS
   loop_EXTRAS
#endif
};


// define constructor - typcase
class typcase_class : public Expression_class {
protected:
   Expression expr;
   Cases cases;
public:
   typcase_class(Expression a1, Cases a2) {
      expr = a1;
      cases = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;

      errors += expr->check_scope(c, o);

      for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
         errors += cases->nth(i)->check_scope(c, o);
      }

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {

      expr->infer_type(c, o);

      std::vector<Symbol> case_types;

      for (int i = cases->first(); cases->more(i); i = cases->next(i)) {
         Symbol case_type = cases->nth(i)->infer_type(c, o);
         case_types.push_back(case_type);
      }

      int n = case_types.size();
      Symbol join_type = case_types[0];
      for (int i = 1; i < n; i++) {
         join_type = g_ct->lub(join_type, case_types[i], c->get_name());
      }

      set_type(join_type);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef typcase_EXTRAS
   typcase_EXTRAS
#endif
};


// define constructor - block
class block_class : public Expression_class {
protected:
   Expressions body;
public:
   block_class(Expressions a1) {
      body = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;
      o->enterscope();
      for (int i = body->first(); body->more(i); i = body->next(i)) {
         errors += body->nth(i)->check_scope(c, o);
      }
      o->exitscope();

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Object = idtable.add_string("Object");
      Symbol last_type = Object;

      for (int i = body->first(); body->more(i); i = body->next(i)) {
         last_type = body->nth(i)->infer_type(c, o);
      }

      set_type(last_type);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef block_EXTRAS
   block_EXTRAS
#endif
};


// define constructor - let
class let_class : public Expression_class {
protected:
   Symbol identifier;
   Symbol type_decl;
   Expression init;
   Expression body;
public:
   let_class(Symbol a1, Symbol a2, Expression a3, Expression a4) {
      identifier = a1;
      type_decl = a2;
      init = a3;
      body = a4;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      int errors = 0;

      errors += init->check_scope(c, o);

      o->enterscope();
      o->addid(identifier, type_decl);

      errors += body->check_scope(c, o);

      o->exitscope();  
      
      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol No_type = idtable.add_string("_no_type");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol init_type = init->infer_type(c, o);
      
      if (init_type != No_type) {
         Symbol init_type2 = init_type;
         if (init_type == SELF_TYPE) {
            init_type2 = init->infer_type2(c, o);
            if (init_type2 == NULL)
               init_type2 = c->get_name();
         }
         if (g_ct->m.count(type_decl)) { // type_decl 정의 유무 확인
            if (!g_ct->partial_ordered(init_type2, type_decl)) {
              g_ct->semant_error(c->get_filename(), this);
               cerr << "Inferred type " << init_type << " of initialization of " << identifier << " does not conform to identifier's declared type " << type_decl << "." << endl;
            }
         }
      }

      o->enterscope();
      o->addid(identifier, type_decl);
      Symbol body_type = body->infer_type(c, o);
      o->exitscope();

      set_type(body_type);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef let_EXTRAS
   let_EXTRAS
#endif
};


// define constructor - plus
class plus_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   plus_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);

      if (e1_type == SELF_TYPE) {
         e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = c->get_name();
      }
      
      if (e1_type != Int || e2_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "non-Int arguments: " << e1_type << " + " << e2_type << endl;
      }
      // set the type
      set_type(Int);

      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef plus_EXTRAS
   plus_EXTRAS
#endif
};


// define constructor - sub
class sub_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   sub_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);

      if (e1_type == SELF_TYPE) {
         e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = c->get_name();
      }
      
      if (e1_type != Int || e2_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "non-Int arguments: " << e1_type << " - " << e2_type << endl;
      }
      // set the type
      set_type(Int);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef sub_EXTRAS
   sub_EXTRAS
#endif
};


// define constructor - mul
class mul_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   mul_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);

      if (e1_type == SELF_TYPE) {
         e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = c->get_name();
      }

      if (e1_type != Int || e2_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "non-Int arguments: " << e1_type << " * " << e2_type << endl;
      }
      // set the type
      set_type(Int);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef mul_EXTRAS
   mul_EXTRAS
#endif
};


// define constructor - divide
class divide_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   divide_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);

      if (e1_type == SELF_TYPE) {
         e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = c->get_name();
      }
      
      if (e1_type != Int || e2_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "non-Int arguments: " << e1_type << " / " << e2_type << endl;
      }
      // set the type
      set_type(Int);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef divide_EXTRAS
   divide_EXTRAS
#endif
};


// define constructor - neg
class neg_class : public Expression_class {
protected:
   Expression e1;
public:
   neg_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      return e1->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol e1_type = e1->infer_type(c, o);
      if (e1_type == SELF_TYPE) {
         e1_type = e1->infer_type2(c, o);
         if (e1_type == NULL)
            e1_type = c->get_name();
      }
      if (e1_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "non-Int arguments: " << e1_type << endl;
      }
      // set the type
      set_type(Int);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef neg_EXTRAS
   neg_EXTRAS
#endif
};


// define constructor - lt
class lt_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   lt_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool = idtable.add_string("Bool");
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");   


      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);
      if (e1_type == SELF_TYPE) {
         e1_type = e1->infer_type2(c, o);
         if (e1_type == NULL)
            e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = e2->infer_type2(c, o);
         if (e2_type == NULL)
            e2_type = c->get_name();
      }
      
      if (e1_type != Int || e2_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Illegal comparison with a basic type." << endl;
      }
      // set the type
      set_type(Bool);
      return get_type();   
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef lt_EXTRAS
   lt_EXTRAS
#endif
};


// define constructor - eq
class eq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   eq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool = idtable.add_string("Bool");
      Symbol Int = idtable.add_string("Int");
      Symbol Object = idtable.add_string("Object");
      Symbol Str = idtable.add_string("String");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");

      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);
      if (e1_type == SELF_TYPE) {
         e1_type = e1->infer_type2(c, o);
         if (e1_type == NULL)
            e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = e2->infer_type2(c, o);
         if (e2_type == NULL)
            e2_type = c->get_name();
      }
      if ((e1_type == Int || e1_type == Bool || e1_type == Str) && e1_type != e2_type) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Illegal comparison with a basic type." << endl;
      } else if ((e2_type == Int || e2_type == Bool || e2_type == Str) && e1_type != e2_type) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Illegal comparison with a basic type." << endl;
      }
      // set the type
      set_type(Bool);
      return get_type(); 
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef eq_EXTRAS
   eq_EXTRAS
#endif
};


// define constructor - leq
class leq_class : public Expression_class {
protected:
   Expression e1;
   Expression e2;
public:
   leq_class(Expression a1, Expression a2) {
      e1 = a1;
      e2 = a2;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o) + e2->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool = idtable.add_string("Bool");
      Symbol Int = idtable.add_string("Int");
      Symbol SELF_TYPE = idtable.add_string("SELF_TYPE");   

      Symbol e1_type = e1->infer_type(c, o);
      Symbol e2_type = e2->infer_type(c, o);
      if (e1_type == SELF_TYPE) {
         e1_type = e1->infer_type2(c, o);
         if (e1_type == NULL)
            e1_type = c->get_name();
      }
      if (e2_type == SELF_TYPE) {
         e2_type = e2->infer_type2(c, o);
         if (e2_type == NULL)
            e2_type = c->get_name();
      }

      if (e1_type != Int || e2_type != Int) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Illegal comparison with a basic type." << endl;
      }
      // set the type
      set_type(Bool);
      return get_type(); 
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef leq_EXTRAS
   leq_EXTRAS
#endif
};


// define constructor - comp
class comp_class : public Expression_class {
protected:
   Expression e1;
public:
   comp_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool = idtable.add_string("Bool");
      Symbol e1_type = e1->infer_type(c, o);
      if (e1_type != Bool) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "non-Bool arguments: " << e1_type << endl;
      }
      // set the type
      set_type(Bool);
      return get_type(); 
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef comp_EXTRAS
   comp_EXTRAS
#endif
};


// define constructor - int_const
class int_const_class : public Expression_class {
protected:
   Symbol token;
public:
   int_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Int = idtable.add_string("Int");
      // set the type
      set_type(Int);
      return get_type(); 
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef int_const_EXTRAS
   int_const_EXTRAS
#endif
};


// define constructor - bool_const
class bool_const_class : public Expression_class {
protected:
   Boolean val;
public:
   bool_const_class(Boolean a1) {
      val = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool = idtable.add_string("Bool");
      set_type(Bool);
      return get_type(); 
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef bool_const_EXTRAS
   bool_const_EXTRAS
#endif
};


// define constructor - string_const
class string_const_class : public Expression_class {
protected:
   Symbol token;
public:
   string_const_class(Symbol a1) {
      token = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Str = idtable.add_string("String");
      set_type(Str);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef string_const_EXTRAS
   string_const_EXTRAS
#endif
};


// define constructor - new_
class new__class : public Expression_class {
protected:
   Symbol type_name;
public:
   new__class(Symbol a1) {
      type_name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // No scope checking required for new expression
      return 0;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Object = idtable.add_string("Object");  
      if (!g_ct->m.count(type_name)) {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "new: " << type_name << " is undefined" << endl;
         set_type(Object);
      }
      set_type(type_name);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef new__EXTRAS
   new__EXTRAS
#endif
};


// define constructor - isvoid
class isvoid_class : public Expression_class {
protected:
   Expression e1;
public:
   isvoid_class(Expression a1) {
      e1 = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      // _TODO
      return e1->check_scope(c, o);
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Bool = idtable.add_string("Bool");
      e1->infer_type(c, o);
      set_type(Bool);
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef isvoid_EXTRAS
   isvoid_EXTRAS
#endif
};


// define constructor - no_expr
class no_expr_class : public Expression_class {
protected:
public:
   no_expr_class() {
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef no_expr_EXTRAS
   no_expr_EXTRAS
#endif
};


// define constructor - object
class object_class : public Expression_class {
protected:
   Symbol name;
public:
   object_class(Symbol a1) {
      name = a1;
   }
   Expression copy_Expression();
   void dump(ostream& stream, int n);
   int check_scope(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      int errors = 0;

      // if (o->lookup(name) == NULL) {
      //    if (c->get_attr_symtab()->lookup(name) == NULL) {
      //       errors++;
      //       // g_ct->semant_error(c);
      //       g_ct->semant_error(c->get_filename(), this);
      //       cerr << "[2]Undeclared identifier " << name << "." << endl;
      //    }
      // }

      return errors;
   }
   Symbol infer_type(Class_ c, cool::SymbolTable<Symbol, Entry>* o) {
      Symbol Object = idtable.add_string("Object");
      Symbol sym;

      if (sym = o->lookup(name)) {
         set_type(sym);
      }
      else if (sym = c->get_attr_symtab()->lookup(name)) {
         set_type(sym);
      }
      else {
         g_ct->semant_error(c->get_filename(), this);
         cerr << "Undeclared identifier " << name << "." << endl;
         set_type(Object);
      }
      return get_type();
   }

#ifdef Expression_SHARED_EXTRAS
   Expression_SHARED_EXTRAS
#endif
#ifdef object_EXTRAS
   object_EXTRAS
#endif
};


// define the prototypes of the interface
Classes nil_Classes();
Classes single_Classes(Class_);
Classes append_Classes(Classes, Classes);
Features nil_Features();
Features single_Features(Feature);
Features append_Features(Features, Features);
Formals nil_Formals();
Formals single_Formals(Formal);
Formals append_Formals(Formals, Formals);
Expressions nil_Expressions();
Expressions single_Expressions(Expression);
Expressions append_Expressions(Expressions, Expressions);
Cases nil_Cases();
Cases single_Cases(Case);
Cases append_Cases(Cases, Cases);
Program program(Classes);
Class_ class_(Symbol, Symbol, Features, Symbol);
Feature method(Symbol, Formals, Symbol, Expression);
Feature attr(Symbol, Symbol, Expression);
Formal formal(Symbol, Symbol);
Case branch(Symbol, Symbol, Expression);
Expression assign(Symbol, Expression);
Expression static_dispatch(Expression, Symbol, Symbol, Expressions);
Expression dispatch(Expression, Symbol, Expressions);
Expression cond(Expression, Expression, Expression);
Expression loop(Expression, Expression);
Expression typcase(Expression, Cases);
Expression block(Expressions);
Expression let(Symbol, Symbol, Expression, Expression);
Expression plus(Expression, Expression);
Expression sub(Expression, Expression);
Expression mul(Expression, Expression);
Expression divide(Expression, Expression);
Expression neg(Expression);
Expression lt(Expression, Expression);
Expression eq(Expression, Expression);
Expression leq(Expression, Expression);
Expression comp(Expression);
Expression int_const(Symbol);
Expression bool_const(Boolean);
Expression string_const(Symbol);
Expression new_(Symbol);
Expression isvoid(Expression);
Expression no_expr();
Expression object(Symbol);


#endif
