#include <iostream>
#include <random>

#include <gsp.hpp>

int main()
{
    // Chain reaction A --> B --> C with generated A and decaying C.
    gsp::state init = {
        .species = {0, 1, 0}
    };

    std::mt19937 random;
    gsp::simulation sim(init);

    // nothing --> A
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

    // C --> nothing
    sim.add_reaction(gsp::simple_decay({
        .species = 2, .rate = 0.4
    }));

    std::cout << "time A B C\n";
    std::cout << sim.state() << '\n';

    for (int step = 1; step <= 100; step++) {
        sim.step(random);
        std::cout << sim.state() << '\n';
    }
}