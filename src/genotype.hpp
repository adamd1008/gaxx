#ifndef __GENOTYPE_HPP__
#define __GENOTYPE_HPP__

/*
 * Genotype describes the individual, whom has a set of chromosomes.
 */

namespace gaxx
{
    /**
     * TFT - target fitness type, i.e. the type of the variable passed into
     *       calcFitness() to calculate the fitness
     */
    template <typename TFT>
    class Genotype
    {
    public:
        Genotype<TFT>() = default;
        virtual ~Genotype<TFT>() = default;

        virtual void init() = 0;
        virtual void initWithMutateCrossover(
                const Genotype<TFT>&,
                const Genotype<TFT>&) = 0;
    };
}

#endif
