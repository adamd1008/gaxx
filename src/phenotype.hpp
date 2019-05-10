#ifndef __PHENOTYPE_HPP__
#define __PHENOTYPE_HPP__

/*
 * Phenotype describes the individual in the population. The phenotype has a
 * genotype (chromosomes).
 */

#include "genotype.hpp"

namespace gaxx
{
    /**
     * TFT - target fitness type, i.e. the type of the variable passed into
     *       calcFitness() to calculate the fitness
     */
    template <typename TFT>
    class Phenotype
    {
    public:
        Phenotype<TFT>() = default;
        virtual ~Phenotype<TFT>() = default;


        void init()
        {
            getGenotype().init();
        }


        void initWithMutateCrossover(Phenotype<TFT>& pt1, Phenotype<TFT>& pt2)
        {
            getGenotype().initWithMutateCrossover(
                    pt1.getGenotype(), pt2.getGenotype());
        }

        virtual bool isFitterThan(const Phenotype<TFT>&, TFT) const = 0;
        virtual bool isOptimalFitness(TFT) const = 0;

    protected:
        virtual Genotype<TFT>& getGenotype() = 0;

        /* XXX: I have *no idea* why this can't be a getter for the other
         * getGenotype() here - it doesn't compile in implemented classes. */
        virtual const Genotype<TFT>& getGenotype() const = 0;
    };
}

#endif
