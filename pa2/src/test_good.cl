class A {
};

Class BB__ inherits A {
};

class Cia {
	x : Int <- 10;
	doSomething(a : Int,) : Int {
		let a : Int <- 4 in
			self@ParentClass.doSomething() + a
	};
};
