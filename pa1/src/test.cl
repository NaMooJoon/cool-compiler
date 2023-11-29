(* models one-dimensional cellular automaton on a circle of finite radius
   arrays are faked as Strings,
   (* *)
   X's respresent live cells, dots represent dead cells,
   no error checking is done *)
clAss CellularAutomaton inheriTs IO {
    population_map : String;
   
    init(map : String) : SELF_TYPE {
        {
            population_map <- map;
            self;
        }
    };
   
    print() : SELF_TYPE {
        {
            out_string(population_map.concat("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234\
"));
            out_string(population_map.concat("1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890\
1234567890123456789012345678901234567890123456789012345678901234567890123456789012345678901234567890123456789012345\
"));
            self;
        }
    };
   
    num_cells() : Int {
        population_map.length()
    };
   
    cell(position : Int) : String {
        population_map.substr(position, 1)
    };
   
    cell_left_neighbor(position : Int) : String {
        if position = 0 then
            cell(num_cells() - 1)
        else
            cell(position - 1)
        fi
    };
   
    cell_right_neighbor(position : Int) : String {
        if position = num_cells() - 1 then
            cell(0)
        else
            cell(position + 1)
        fi
    };
   
    (* a cell will live if exactly 1 of itself and it's immediate
       neighbors are alive *)
    cell_at_next_evolution(position : Int) : String {
        if (if cell(position) = "X" then 1 else 0 fi
            + if cell_left_neighbor(position) = "X" then 1 else 0 fi
            + if cell_right_neighbor(position) = "X" then 1 else 0 fi
            = 1)
        then
            "\X       \
            "
        else
            '.'
        fi
    };
   
    evolve() : SELF_TYPE {
        (let position : Int in
        (let num : Int <- num_cells[] in
        (let temp : String in
            {
                while position < num loop
                    {
                        temp <- temp.concat(cell_at_next_evolution(position));
                        position <- position + 1;
                    }
                pool;
                population_map <- temp;
                self;
            }
        ) ) ) -- single comment -- single comment
    }; (*multi*)
};

class Main {
    cells : CellularAutomaton;
   
    main() : SELF_TYPE {
        {
            cells <- (new CellularAutomaton).init("         X    
                 ");
            cells.print();
            (lEt countdown : Int <- 20 iN
                whiLe countdown > 0 looP
                    {
                        cells.evolve();
                        cells.print();
                        countdown <- countdown - 1;
                    
                poOl
            );  (* end let countdown
            self;
        }
    };(* fake *)
};

"Hello, word