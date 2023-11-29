#ifndef SEMANT_H_
#define SEMANT_H_

#include <assert.h>
#include <iostream>
// #include "cool-tree.h"
#include "stringtab.h"
#include "symtab.h"
#include "list.h"
#include <map>
#include <vector>

#include "tree.h"

#define TRUE 1
#define FALSE 0

class ClassTable;
typedef ClassTable *ClassTableP;

typedef class Class__class *Class_;
typedef list_node<Class_> Classes_class;
typedef Classes_class *Classes;
typedef class Feature_class *Feature;

// This is a structure that may be used to contain the semantic
// information such as the inheritance graph.  You may use it or not as
// you like: it is only here to provide a container for the supplied
// methods.

class ClassTable {
private:
  int semant_errors;
  void install_basic_classes();
  ostream& error_stream;

  enum class State {
    UNVISITED,
    VISITING,
    VISITED
  };
  bool has_cycle(Symbol c, std::map<Symbol, State>& state);
  void construct_class_scopes_rec(Symbol c);
  void save_scope_parent_class_first(Symbol c, cool::SymbolTable<Symbol, Entry>* attr_symtab, cool::SymbolTable<Symbol, Entry>* method_symtab);

public:
  static int semant_errors_;
  static std::map<Symbol, Class_> m;
  static std::map<Symbol, std::vector<Symbol>> inheritance_tree;

public:
  ClassTable(Classes);

  bool build_inheritance_tree();
  bool is_inheritance_tree_acyclic();
  void save_scope_for_each_class(Classes classes);
  void check_scope_for_each_class(Classes classes);
  // int check_feature_scope(Class_ c, Feature f, cool::SymbolTable<Symbol, Entry>* object_symtab);

  void check_type_for_each_class(Classes classes);

  int errors() { return semant_errors + semant_errors_; }
  ostream& semant_error();
  ostream& semant_error(Class_ c);
  ostream& semant_error(Symbol filename, tree_node *t);
  void increase_semant_errors();
  static void semant_error(Class_ c, tree_node* t, char* err_msg);
  static bool partial_ordered(Symbol, Symbol);
  static Symbol lub(Symbol, Symbol, Symbol);
};

#endif

