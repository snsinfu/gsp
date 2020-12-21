#include <cstddef>
#include <cstdint>
#include <iostream>
#include <random>

#include <gsp.hpp>


struct simulation_config
{
    std::size_t   gene_count;
    double        on_rate;
    double        off_rate;
    double        expr_rate;
    double        decay_rate;
    long          simulation_steps;
    std::uint64_t random_seed;
};

void show_state(gsp::state const& state, simulation_config const& config);


int
main()
{
    // Model and simulation parameters.
    simulation_config const config = {
        .gene_count       = 11,
        .on_rate          = 0.1,
        .off_rate         = 1.0,
        .expr_rate        = 1.0,
        .decay_rate       = 1.0,
        .simulation_steps = 10000,
        .random_seed      = 0,
    };

    // The system consists of "genes." We assign three species to each gene:
    // gene_off, gene_on and protein. Gene_on and gene_off are both singular
    // and transform each other, and gene_on produces proteins. We start with
    // all genes on with no proteins.
    gsp::state init;

    for (std::size_t i = 0; i < config.gene_count; i++) {
        init.species.push_back(0); // gene_off
        init.species.push_back(1); // gene_on
        init.species.push_back(0); // protein
    }

    gsp::simulation sim(init);

    // Define reactions. The network here consists of negative feedbacks
    // connected in circular way. Association of i-th gene with (i-1)-th
    // protein turns off the i-th gene:
    //
    //      gene_on(i) + protein(i-1)  <-->  gene_off(i) .
    //
    // And, when i-th gene is on, i-th protein is generated:
    //
    //           gene_on(i)
    //      *  -------------->  protein(i) .
    //
    // The generated proteins decay at constant rate;
    //
    //      protein(i)  -->  * .
    //
    // This way, a cyclic chain reaction is formed. When i-th gene is on,
    // (i+1)-th gene gets turned off, which in turn activates (i+2)-th gene.
    // So, eventually on and off states interleave along the chain:
    //
    //      ... --> on --> off --> on --> off --> on --> ...
    //
    // However, if the length of the cycle is odd, there must be a flustrated
    // region like on --> on or off --> off. So the system cannot become stable
    // and oscillates forever.

    for (std::size_t i = 0; i < config.gene_count; i++) {
        auto const gene_off = i * 3;
        auto const gene_on = i * 3 + 1;
        auto const protein = i * 3 + 2;
        auto const prev = (i + config.gene_count - 1) % config.gene_count;
        auto const repressor = prev * 3 + 2;

        // gene_on(i) + protein(i-1) --> gene_off(i)
        sim.add_reaction(
            gsp::simple_association({
                .reactant1 = gene_on,
                .reactant2 = repressor,
                .product   = gene_off,
                .rate      = config.off_rate,
            })
        );

        // gene_off(i) --> gene_on(i) + protein(i-1)
        sim.add_reaction(
            gsp::simple_dissociation({
                .reactant = gene_off,
                .product1 = gene_on,
                .product2 = repressor,
                .rate     = config.on_rate,
            })
        );

        // * --> protein(i)
        sim.add_reaction(
            gsp::linear_mediated_generation({
                .species  = protein,
                .rate     = config.expr_rate,
                .mediator = gene_on,
            })
        );

        // protein(i) --> *
        sim.add_reaction(
            gsp::simple_decay({
                .species  = protein,
                .rate     = config.decay_rate,
            })
        );
    }

    // Simulation.
    std::mt19937 random(config.random_seed);

    show_state(sim.state(), config);
    for (int step = 1; step <= config.simulation_steps; step++) {
        sim.step(random);
        show_state(sim.state(), config);
    }
}


void
show_state(gsp::state const& state, simulation_config const& config)
{
    std::cout << state.time << '\t';

    // Print only gene_on states.
    for (std::size_t i = 0; i < config.gene_count; i++) {
        std::cout << state.species[i * 3 + 1];
    }

    std::cout << '\n';
}
