class C {
	a : Int;
	b : Bool;
	c : Object;
	d : String;
	init(x : Int, y : Bool) : C {
           {
		a <- x;
		b <- y;
		c <- x;
		self;
           }
	};
};

Class B {
	a : Int;
	b : Bool;
	c : Object;
	d : String;
	init(x : Int, y : Bool) : B {
		   {
		a <- x;
		b <- y;
		c <- x;
		self;
		   }
	};
};


Class Main {
	main():B {
	  (new C).init(1,true)
	};
};
