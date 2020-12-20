// Chain reaction A --> B --> C with generated A and decaying C.
//
// Compile and run:
//
// $ c++ -std=c++14 -isystem ../include -O2 -o abc abc.cc
// $ ./abc
//

#include <iostream>
#include <random>

#include <gsp.hpp>


int main()
{
    // Use standard pseudo-random number generator. Here we use seed value 0.
    std::mt19937 random(0);

    // Initial simulation state. We start with (A, B, C) = (0, 1, 0). That is,
    // there is only one B molecule in the system.
    gsp::state init = {
        .species = {0, 1, 0}
    };
    gsp::simulation sim(init);

    // Now, define reactions.

    // Spontaneous generation of A molecule.
    sim.add_reaction(gsp::simple_generation({
        .species = 0, .rate = 0.1
    }));

    // A --> B
    sim.add_reaction(gsp::simple_transformation({
        .reactant = 0, .product = 1, .rate = 0.2
    }));

    // B --> C
    sim.add_reaction(gsp::simple_transformation({
        .reactant = 1, .product = 2, .rate = 0.3
    }));

    // Spontaneous destruction of C molecule.
    sim.add_reaction(gsp::simple_decay({
        .species = 2, .rate = 0.4
    }));

    // Then, let us start a simulation.

    // Print header and initial state.
    std::cout << "time A B C\n";
    std::cout << sim.state() << '\n';

    // Simulate 100 reactions, printing state after each reaction.
    for (int step = 1; step <= 100; step++) {
        sim.step(random);
        std::cout << sim.state() << '\n';
    }
}
