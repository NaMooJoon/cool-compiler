class A  {
  method(x:String, y:Int, z:Int) : D {
    {
      f <- 1;
      case x of a : Int => g + 100; b : String => b; esac;
      3 + 2;
    }
  };
  method(x:String, y:Int, x:Int) : C {
    {
      x <- 1;
      3 + 2;
    }
  };
  x : Int <- 2;
};

class B inherits A {
  ff(x:Object, y:Int, c:Int, d: A) : B {
    d.method("ff", 2, 4)
  };
  x : String <- 10;
};


class C {
  main() : Object {
    123
  };
};


