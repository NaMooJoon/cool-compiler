class A {
f():Int{{
a+b-c;
a-b+c;

a+b*c;
a*b+c;

a+b/c;
a/b+c;

a-b*c;
a*b-c;

a-b/c;
a/b-c;

a*b/c;
a/b*c;
}};
};(*
   The class A2I provides integer-to-string and string-to-integer
conversion routines.  To use these routines, either inherit them
in the class where needed, have a dummy variable bound to
something of type A2I, or simpl write (new A2I).method(argument).
*)


(*
   c2i   Converts a 1-character string to an integer.  Aborts
         if the string is not "0" through "9"
*)
class A2I {

     c2i(char : String) : Int {
	if char = "0" then 0 else
	if char = "1" then 1 else
	if char = "2" then 2 else
        if char = "3" then 3 else
        if char = "4" then 4 else
        if char = "5" then 5 else
        if char = "6" then 6 else
        if char = "7" then 7 else
        if char = "8" then 8 else
        if char = "9" then 9 else
        { abort(); 0; }  -- the 0 is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   i2c is the inverse of c2i.
*)
     i2c(i : Int) : String {
	if i = 0 then "0" else
	if i = 1 then "1" else
	if i = 2 then "2" else
	if i = 3 then "3" else
	if i = 4 then "4" else
	if i = 5 then "5" else
	if i = 6 then "6" else
	if i = 7 then "7" else
	if i = 8 then "8" else
	if i = 9 then "9" else
	{ abort(); ""; }  -- the "" is needed to satisfy the typchecker
        fi fi fi fi fi fi fi fi fi fi
     };

(*
   a2i converts an ASCII string into an integer.  The empty string
is converted to 0.  Signed and unsigned strings are handled.  The
method aborts if the string does not represent an integer.  Very
long strings of digits produce strange answers because of arithmetic 
overflow.

*)
     a2i(s : String) : Int {
        if s.length() = 0 then 0 else
	if s.substr(0,1) = "-" then ~a2i_aux(s.substr(1,s.length()-1)) else
        if s.substr(0,1) = "+" then a2i_aux(s.substr(1,s.length()-1)) else
           a2i_aux(s)
        fi fi fi
     };

(*
  a2i_aux converts the usigned portion of the string.  As a programming
example, this method is written iteratively.
*)
     a2i_aux(s : String) : Int {
	(let int : Int <- 0 in	
           {	
               (let j : Int <- s.length() in
	          (let i : Int <- 0 in
		    while i < j loop
			{
			    int <- int * 10 + c2i(s.substr(i,1));
			    i <- i + 1;
			}
		    pool
		  )
	       );
              int;
	    }
        )
     };

(*
    i2a converts an integer to a string.  Positive and negative 
numbers are handled correctly.  
*)
    i2a(i : Int) : String {
	if i = 0 then "0" else 
        if 0 < i then i2a_aux(i) else
          "-".concat(i2a_aux(i * ~1)) 
        fi fi
    };
	
(*
    i2a_aux is an example using recursion.
*)		
    i2a_aux(i : Int) : String {
        if i = 0 then "" else 
	    (let next : Int <- i / 10 in
		i2a_aux(next).concat(i2c(i - next * 10))
	    )
        fi
    };

};
class z {
  F(X : Int) : Object {
    self
  };;
}

class B {
  y : Idqwdqwdqwqwdnt;
  B : DASD;
djqwpdqwo
};
class X{
 y: ADSA;
};
class Test {
  foo:test;
  bar():Int { case foo.bar() of Z:Int => 3;
                                z:string => 4;
                                x:Test => 5; esac };
};

class Hello inherits goodbye {

   foo:Int;
   
};
class Test {
  foo:Test;
  bar():Int { {x == (3 < 4) ;}};
};
class Test {
  foo:test;
  bar():aa { if x = 3 then {if x < 2 then new Foo else isvoid baz fi;} else false fi};
};
class Test {
  foo:Test;
  bar(x:int,y:int):nnt {if true then 4 else 4 fi};
};
class Foo {
	bar():Int{{let A:Int in (a + b);
		(let a:int in a) + b;
		let a:Int in (a) + (b);}};
};class A {
  x : Int;
  y : Bool;
  z : Object;
  f() : Bool {
    true
  };
  a : String;
  a : IO;
};
class Test {
  foo:Test;
  bar():Int {5};
};

class Test extends Object {
  foo:test;


class Test {
  foo:yest;
  bar():Int {5};
};

class Test implements Nothing{
  foo:Test;
  bar():Int {5};
};


class A {
  f() : Int { {
    g();
    h(Z dqwe)zz;
    f(2, 3);
    f(3, 4, 5);
    z(1, 2, 3, 4, 5);
  } };
};
class A {
  f(x : Object) : b {
    zz 5 + 3
  };
};

class Foo {
	bar():Int{{isvoid a + b * c;
		not a < b;
		~a + b;
		}};
}
;class A {
f():Int{{
-- valid
while 1 loop 1 pool;
while 1 loop while 2 loop 2 pool pool;

-- not valid
while 1 loop 1 loop;
while 1 pool 1 pool
while id pool;
}};
};
