

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "semant.h"
#include "utilities.h"
#include <set>
#include <queue>
#include <map>
#include "symtab.h"
#include "cool-tree.h"

std::map<Symbol, Class_> ClassTable::m;
int ClassTable::semant_errors_;
std::map<Symbol, std::vector<Symbol>> ClassTable::inheritance_tree;
ClassTable* g_ct;

extern int semant_debug;
extern char *curr_filename;

//////////////////////////////////////////////////////////////////////
//
// Symbols
//
// For convenience, a large number of symbols are predefined here.
// These symbols include the primitive type and method names, as well
// as fixed names used by the runtime system.
//
//////////////////////////////////////////////////////////////////////
Symbol
    arg,
    arg2,
    Bool,
    concat,
    cool_abort,
    copy,
    Int,
    in_int,
    in_string,
    IO,
    length,
    Main,
    main_meth,
    No_class,
    No_type,
    Object,
    out_int,
    out_string,
    prim_slot,
    self,
    SELF_TYPE,
    Str,
    str_field,
    substr,
    type_name,
    val;
//
// Initializing the predefined symbols.
//
static void initialize_constants(void)
{
    arg         = idtable.add_string("arg");
    arg2        = idtable.add_string("arg2");
    Bool        = idtable.add_string("Bool");
    concat      = idtable.add_string("concat");
    cool_abort  = idtable.add_string("abort");
    copy        = idtable.add_string("copy");
    Int         = idtable.add_string("Int");
    in_int      = idtable.add_string("in_int");
    in_string   = idtable.add_string("in_string");
    IO          = idtable.add_string("IO");
    length      = idtable.add_string("length");
    Main        = idtable.add_string("Main");
    main_meth   = idtable.add_string("main");
    //   _no_class is a symbol that can't be the name of any
    //   user-defined class.
    No_class    = idtable.add_string("_no_class");
    No_type     = idtable.add_string("_no_type");
    Object      = idtable.add_string("Object");
    out_int     = idtable.add_string("out_int");
    out_string  = idtable.add_string("out_string");
    prim_slot   = idtable.add_string("_prim_slot");
    self        = idtable.add_string("self");
    SELF_TYPE   = idtable.add_string("SELF_TYPE");
    Str         = idtable.add_string("String");
    str_field   = idtable.add_string("_str_field");
    substr      = idtable.add_string("substr");
    type_name   = idtable.add_string("type_name");
    val         = idtable.add_string("_val");
}

// Define basic classes as global variable
static Class_
    Object_class,
    IO_class,
    Int_class,
    Bool_class,
    Str_class;

ClassTable::ClassTable(Classes classes) : semant_errors(0) , error_stream(cerr) {
    install_basic_classes();
    g_ct = this;

    // Construct a mpa: Symbol -> Class_
    m[SELF_TYPE] = NULL;
    m[Object] = Object_class;
    m[Int] = Int_class;
    m[IO] = IO_class;
    m[Str] = Str_class;
    m[Bool] = Bool_class;
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Symbol class_name = c->get_name();
        if (class_name == Object    || 
            class_name == Int       || 
            class_name == Str       || 
            class_name == Bool      || 
            class_name == SELF_TYPE) 
        {
            semant_error(c) << "Redefinition of basic class " << class_name << "." << endl;
        }
        else if (m.find(class_name) != m.end()) {
            semant_error(c) << "Class " << class_name << " was previously defined.\n";
        }
        else {
            m[class_name] = c;
        }
    }

    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        // TODO: Detect undefined parent class
        if (m.count(c->get_parent()) == 0) {
            semant_error(c) << "Class " << c->get_name() << " inherits from an undefined class " << c->get_parent() << "." << endl;    
        }
        // TODO: Cannot inherit from SELF_TYPE
        if(c->get_parent() == SELF_TYPE) {
            semant_error(c) << "Class " << c->get_name() << " cannot inherit class SELF_TYPE." << endl;
        }
    }

    // TODO: detect class redefinition
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        if (classes->more(i)) {
            for(int j = classes->next(i); classes->more(j); j = classes->next(j)) {
                Class_ d = classes->nth(j);
                if (c->get_name() == d->get_name()) {
                    semant_error(c) << "Redefinition of class " << c->get_name() << "." << endl;
                }
            }
        }
    }
    
    /*
    // ?Save scope for each class?
    // Save attributes and methods for each class
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        c->get_attr_symtab()->addid(self, SELF_TYPE);
        while (c->get_name() != Object) {
            Features fl = c->get_features();
            std::set<Symbol> class_attrs;
            std::set<Symbol> class_methods;

            // c->get_attr_symtab()->enterscope(); // 맨 처음에는 enterscope를 하지 않아도 되는가?
            // c->get_method_symtab()->enterscope();


            for(int j = fl->first(); fl->more(j); j = fl->next(j)) {
                Feature f = fl->nth(j);
                // TODO: check if redefinition
                if (f->is_attr()) { // atrribute
                
                    if (class_attrs.find(f->get_name()) != class_attrs.end()) { // ensure class attributes are unique
                        semant_error(c) << "The attribute :" << f->get_name() << " has already been defined!\n";
                    } 
                    else {
                        class_attrs.insert(f->get_name());
                        c->get_attr_symtab()->addid(f->get_name(), f->get_type());
                    }

                } else {    // method
                
                    // ensure class methods are unique
                    if (class_methods.find(f->get_name()) != class_methods.end()) {
                        semant_error(c) << "The method :" << f->get_name() << " has already been defined!\n";
                    } 
                    else {
                        class_methods.insert(f->get_name());
                        c->get_method_symtab()->addid(f->get_name(), f->get_type());
                    }

                }            
            }

            if (c->get_parent() != Object) {
                c->get_attr_symtab()->enterscope();
                c->get_method_symtab()->enterscope();   
            }
            c = m[c->get_parent()];
            
            class_attrs.clear();
            class_methods.clear();
        }
    }

    // Check scope for each class (Interpreter Pattern applied)
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Features fl = c->get_features();

        for(int j = fl->first(); fl->more(j); j = fl->next(j)) {
            Feature f = fl->nth(j);

            cool::SymbolTable<Symbol, Entry>* object_symtab = new cool::SymbolTable<Symbol, Entry>();
            object_symtab->enterscope();

            semant_errors += f->check_scope(c, object_symtab);
        }
    }

    // Check type for each class (Interpreter Pattern applied)
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Features fl = c->get_features();

        for(int j = fl->first(); fl->more(j); j = fl->next(j)) {
            Feature f = fl->nth(j);

            cool::SymbolTable<Symbol, Entry>* object_symtab = new cool::SymbolTable<Symbol, Entry>();
            object_symtab->enterscope();

            semant_errors += f->check_type(c, object_symtab);
        }
    }
    */
}

void ClassTable::install_basic_classes() {

    // The tree package uses these globals to annotate the classes built below.
    curr_lineno  = 0;
    Symbol filename = stringtable.add_string("<basic class>");

    // The following demonstrates how to create dummy parse trees to
    // refer to basic Cool classes.  There's no need for method
    // bodies -- these are already built into the runtime system.

    // IMPORTANT: The results of the following expressions are
    // stored in local variables.  You will want to do something
    // with those variables at the end of this method to make this
    // code meaningful.

    //
    // The Object class has no parent class. Its methods are
    //        abort() : Object    aborts the program
    //        type_name() : Str   returns a string representation of class name
    //        copy() : SELF_TYPE  returns a copy of the object
    //
    // There is no need for method bodies in the basic classes---these
    // are already built in to the runtime system.

    Object_class =
	class_(Object,
	       No_class,
	       append_Features(
			       append_Features(
					       single_Features(method(cool_abort, nil_Formals(), Object, no_expr())),
					       single_Features(method(type_name, nil_Formals(), Str, no_expr()))),
			       single_Features(method(copy, nil_Formals(), SELF_TYPE, no_expr()))),
	       filename);
    Object_class->get_method_symtab()->addid(cool_abort, Object);
    Object_class->get_method_symtab()->addid(type_name, Str);
    Object_class->get_method_symtab()->addid(copy, SELF_TYPE);
    Object_class->get_attr_symtab()->addid(self, SELF_TYPE);

    //
    // The IO class inherits from Object. Its methods are
    //        out_string(Str) : SELF_TYPE       writes a string to the output
    //        out_int(Int) : SELF_TYPE            "    an int    "  "     "
    //        in_string() : Str                 reads a string from the input
    //        in_int() : Int                      "   an int     "  "     "
    //
    IO_class =
	class_(IO,
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       single_Features(method(out_string, single_Formals(formal(arg, Str)),
										      SELF_TYPE, no_expr())),
							       single_Features(method(out_int, single_Formals(formal(arg, Int)),
										      SELF_TYPE, no_expr()))),
					       single_Features(method(in_string, nil_Formals(), Str, no_expr()))),
			       single_Features(method(in_int, nil_Formals(), Int, no_expr()))),
	       filename);
    IO_class->get_method_symtab()->addid(out_string, SELF_TYPE);
    IO_class->get_method_symtab()->addid(out_int, SELF_TYPE);
    IO_class->get_method_symtab()->addid(in_string, Str);
    IO_class->get_method_symtab()->addid(in_int, Int);
    IO_class->get_attr_symtab()->addid(self, SELF_TYPE);

    //
    // The Int class has no methods and only a single attribute, the
    // "val" for the integer.
    //
    Int_class =
	class_(Int,
	       Object,
	       single_Features(attr(val, prim_slot, no_expr())),
	       filename);
    Int_class->get_attr_symtab()->addid(val, prim_slot);
    Int_class->get_attr_symtab()->addid(self, SELF_TYPE);

    //
    // Bool also has only the "val" slot.
    //
    Bool_class =
	class_(Bool, Object, single_Features(attr(val, prim_slot, no_expr())),filename);
    Bool_class->get_attr_symtab()->addid(val, prim_slot);
    Bool_class->get_attr_symtab()->addid(self, SELF_TYPE);

    //
    // The class Str has a number of slots and operations:
    //       val                                  the length of the string
    //       str_field                            the string itself
    //       length() : Int                       returns length of the string
    //       concat(arg: Str) : Str               performs string concatenation
    //       substr(arg: Int, arg2: Int): Str     substring selection
    //
    Str_class =
	class_(Str,
	       Object,
	       append_Features(
			       append_Features(
					       append_Features(
							       append_Features(
									       single_Features(attr(val, Int, no_expr())),
									       single_Features(attr(str_field, prim_slot, no_expr()))),
							       single_Features(method(length, nil_Formals(), Int, no_expr()))),
					       single_Features(method(concat,
								      single_Formals(formal(arg, Str)),
								      Str,
								      no_expr()))),
			       single_Features(method(substr,
						      append_Formals(single_Formals(formal(arg, Int)),
								     single_Formals(formal(arg2, Int))),
						      Str,
						      no_expr()))),
	       filename);
    Str_class->get_attr_symtab()->addid(val, Int);
    Str_class->get_attr_symtab()->addid(str_field, prim_slot);
    Str_class->get_method_symtab()->addid(length, Int);
    Str_class->get_method_symtab()->addid(concat, Str);
    Str_class->get_method_symtab()->addid(substr, Str);
    Str_class->get_attr_symtab()->addid(self, SELF_TYPE);
}

bool ClassTable::build_inheritance_tree() {

    for (const auto& x : m) {

        Symbol class_name = x.first;

        if (class_name == Object || class_name == SELF_TYPE) { // Object is root.
            continue;
        }

        // SELF_TYPE, Int, Bool, Str can not be parent.
        Symbol parent_name = x.second->get_parent();
        if (parent_name == SELF_TYPE ||
            parent_name == Int ||
            parent_name == Bool ||
            parent_name == Str) {
            semant_error(x.second) << "Class " << class_name << " cannot inherit class " << parent_name << "." << endl;
            return false;
        }

        // Build the inheritance tree.
        if (inheritance_tree.find(parent_name) == inheritance_tree.end()) {
            inheritance_tree[parent_name] = std::vector<Symbol>();
        }
        this->inheritance_tree[parent_name].push_back(class_name);
    }
    return true;
}


bool ClassTable::is_inheritance_tree_acyclic() {
    // initialize all nodes to UNVISITED.
    std::map<Symbol, State> state;
    for (const auto& x : m) {
        state[x.first] = State::UNVISITED;
    }

    // DFS.
    bool is_acyclic = true;
    state[SELF_TYPE] = State::VISITED;
    for (const auto& x : m) {
        Symbol curr_class = x.first;
        if (state[curr_class] != State::VISITED) {
            if (has_cycle(curr_class, state)) {
                semant_error(x.second) << "Class " << curr_class << ", or an ancestor of " << curr_class << ", is involved in an inheritance cycle." << endl;
                is_acyclic = false;
            }
        }
    }
    return is_acyclic;
}

bool ClassTable::has_cycle(Symbol curr_class, std::map<Symbol, State>& state) {
    
    state[curr_class] = State::VISITING;

    for (const auto& child_class : inheritance_tree[curr_class]) {
        if (state[child_class] == State::VISITING) {
            return true;
        }
        if (state[child_class] == State::UNVISITED) {
            if (has_cycle(child_class, state)) {
                return true;
            }
        }
    }
    state[curr_class] = State::VISITED;

    return false;
}

void ClassTable::save_scope_for_each_class(Classes classes) {
    for (const auto& x : m) {
        if (x.first == SELF_TYPE) {
            continue;
        }
        Class_ curr_class = x.second;
        cool::SymbolTable<Symbol, Entry>* attr_symtab = new cool::SymbolTable<Symbol, Entry>();
        cool::SymbolTable<Symbol, Entry>* method_symtab = new cool::SymbolTable<Symbol, Entry>();
        
        // save parent scope, recursively.
        attr_symtab->enterscope();
        method_symtab->enterscope();
        save_scope_parent_class_first(curr_class->get_name(), attr_symtab, method_symtab);
        
        curr_class->set_attr_symtab(attr_symtab);
        curr_class->set_method_symtab(method_symtab);
    }
}

void ClassTable::save_scope_parent_class_first(Symbol curr_class_name, cool::SymbolTable<Symbol, Entry>* attr_symtab, cool::SymbolTable<Symbol, Entry>* method_symtab) {

    Class_ curr_class = m[curr_class_name];

    if (curr_class_name != Object) {
        save_scope_parent_class_first(curr_class->get_parent(), attr_symtab, method_symtab);
    }

    attr_symtab->addid(self, SELF_TYPE);

    std::set<Symbol> class_methods;

    Features fl = curr_class->get_features();
    for (int j = fl->first(); fl->more(j); j = fl->next(j)) {
        Feature f = fl->nth(j);
        if (f->is_attr()) {
            attr_symtab->addid(f->get_name(), f->get_type());
        } else {
            // if (class_methods.find(f->get_name()) != class_methods.end()) {
            //     semant_error(curr_class) << "Method " << f->get_name() << " is multiply defined." << endl;
            //     continue;
            // }
            if (class_methods.find(f->get_name()) != class_methods.end()) 
                continue;
            
            method_symtab->addid(f->get_name(), f->get_type());
        }
    }
}


void ClassTable::check_scope_for_each_class(Classes classes) {
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Features fl = c->get_features();

        cool::SymbolTable<Symbol, Entry>* object_symtab = new cool::SymbolTable<Symbol, Entry>();
        object_symtab->enterscope();
        for(int j = fl->first(); fl->more(j); j = fl->next(j)) {
            Feature f = fl->nth(j);

            semant_errors += f->check_scope(c, object_symtab);
        }
        object_symtab->exitscope();
    }
}

void ClassTable::check_type_for_each_class(Classes classes) {
    // Check type for each class (Interpreter Pattern applied)
    for(int i = classes->first(); classes->more(i); i = classes->next(i)) {
        Class_ c = classes->nth(i);
        Features fl = c->get_features();

        for(int j = fl->first(); fl->more(j); j = fl->next(j)) {
            Feature f = fl->nth(j);

            cool::SymbolTable<Symbol, Entry>* object_symtab = new cool::SymbolTable<Symbol, Entry>();
            object_symtab->enterscope();

            semant_errors += f->check_type(c, object_symtab);
        }
    }
}


////////////////////////////////////////////////////////////////////
//
// semant_error is an overloaded function for reporting errors
// during semantic analysis.  There are three versions:
//
//    ostream& ClassTable::semant_error()
//
//    ostream& ClassTable::semant_error(Class_ c)
//       print line number and filename for `c'
//
//    ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
//       print a line number and filename
//
///////////////////////////////////////////////////////////////////

ostream& ClassTable::semant_error(Class_ c)
{
    return semant_error(c->get_filename(),c);
}

ostream& ClassTable::semant_error(Symbol filename, tree_node *t)
{
    error_stream << filename << ":" << t->get_line_number() << ": ";
    return semant_error();
}

ostream& ClassTable::semant_error()
{
    semant_errors++;
    return error_stream;
}

void ClassTable::semant_error(Class_ c, tree_node* t, char* err_msg) {
    cerr << c->get_filename() << ":" << t->get_line_number() << ": ";
    cerr << err_msg << endl;
    semant_errors_++;
}

bool ClassTable::partial_ordered(Symbol derived, Symbol base) {
    // TODO
    if (derived == base || base == Object)
        return true;

    // 제출 전 지우자.
    if (derived == SELF_TYPE || base == SELF_TYPE) {
        return false;
    }        
    if (derived == nullptr || base == nullptr) {
        return false;
    }
    Symbol curr_class = derived;
    while (curr_class != Object && curr_class != No_class) {
        if (curr_class == base)
            return true;
        curr_class = m[curr_class]->get_parent();
    }

    return false;
}

Symbol ClassTable::lub(Symbol s1, Symbol s2, Symbol curr_class) {
    // TODO
    if (s1 == SELF_TYPE) {
        s1 = curr_class;
    }
    if (s2 == SELF_TYPE) {
        s2 = curr_class;
    }

    if (s1 == s2) {
        return s1;
    }

    std::vector<Symbol> s1_ancestors, s2_ancestors;

    Symbol curr = s1;
    while (curr != Object && curr != No_class) {
        s1_ancestors.push_back(curr);
        curr = m[curr]->get_parent();
    }
    s1_ancestors.push_back(Object);

    curr = s2;
    while (curr != Object && curr != No_class) {
        s2_ancestors.push_back(curr);
        curr = m[curr]->get_parent();
    }
    s2_ancestors.push_back(Object);

    for (int i = 0; i < s1_ancestors.size(); i++) {
        for (int j = 0; j < s2_ancestors.size(); j++) {
            if (s1_ancestors[i] == s2_ancestors[j]) {
                return s1_ancestors[i];
            }
        }
    }

    return Object;
}

/*   This is the entry point to the semantic checker.

     Your checker should do the following two things:

     1) Check that the program is semantically correct
     2) Decorate the abstract syntax tree with type information
        by setting the `type' field in each Expression node.
        (see `tree.h')

     You are free to first do 1), make sure you catch all semantic
     errors. Part 2) can be done in a second stage, when you want
     to build mycoolc.
 */
void program_class::semant()
{
    initialize_constants();

    /* ClassTable constructor may do some semantic analysis */
    ClassTable *classtable = new ClassTable(classes);

    if (classtable->errors()) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }

    // TODO: check if Main class exists
    if (classtable->m.find(Main) == classtable->m.end()) {
        classtable->semant_error() << "Class Main is not defined." << endl;
    }
    
    /* some semantic analysis code may go here */
    classtable->build_inheritance_tree();

    if (!classtable->is_inheritance_tree_acyclic()) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }

    classtable->save_scope_for_each_class(classes);

    classtable->check_scope_for_each_class(classes);
 
    classtable->check_type_for_each_class(classes);


    if (classtable->errors()) {
        cerr << "Compilation halted due to static semantic errors." << endl;
        exit(1);
    }

}


