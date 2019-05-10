#ifndef __POPULATION_HPP__
#define __POPULATION_HPP__

#include <cstdint>
#include <algorithm>
#include <utility>
#include <vector>
#include "phenotype.hpp"

namespace gaxx
{
    /**
     * P   - phenotype subclass
     * TFT - target fitness type, i.e. the type of the variable passed into
     *       calcFitness() to calculate the fitness
     */
    template <typename P, typename TFT>
    class Population
    {
    public:
        explicit Population(uint64_t count, TFT target)
            : _count(count), _target(target), _indiv()
        {
            for (uint64_t i = 0; i < _count; i++)
            {
                P pt;
                pt.init();
                _indiv.push_back(pt);
            }
        }


        virtual ~Population() = default;


        void runForGenerations(uint32_t gens, uint32_t breedCount)
        {
            assert(breedCount >= 2);
            assert(breedCount < _count);

            bool done = false;
            auto optimalFitFunc = [&](P obj) -> bool
            {
                return obj.isOptimalFitness(_target);
            };

            cbUpdate(0, _indiv, _target);

            for (uint32_t i = 1; (i <= gens) && (!done); i++)
            {
                if (std::any_of(_indiv.begin(), _indiv.end(), optimalFitFunc))
                {
                    done = true;
                }
                else
                {
                    doGen(breedCount);
                    cbUpdate(i, _indiv, _target);
                }
            }
        }


        void run(uint32_t breedCount)
        {
            assert(breedCount >= 2);
            assert(breedCount < _count);

            uint64_t i = 1;
            bool done = false;
            auto optimalFitFunc = [&](P obj) -> bool
            {
                return obj.isOptimalFitness(_target);
            };

            cbUpdate(0, _indiv, _target);

            while (!done)
            {
                if (std::any_of(_indiv.begin(), _indiv.end(), optimalFitFunc))
                {
                    done = true;
                }
                else
                {
                    doGen(breedCount);
                    cbUpdate(i, _indiv, _target);
                    i++;
                }
            }
        }


        uint32_t getCount() const
        {

            assert(_count == _indiv.size());
            return _count;
        }


        std::vector<P> getBest(const uint32_t breedCount)
        {
            std::vector<P> ret;
            assert(_count <= _indiv.size());

            // Populate ret with enough initial samples
            for (uint32_t i = 0; i < breedCount; i++)
                ret.push_back(_indiv[i]);

            for (uint32_t indivIdx = breedCount;
                    indivIdx < _indiv.size();
                    indivIdx++)
            {
                bool replaced = false;

                for (uint32_t retIdx = 0;
                        (retIdx < breedCount) && (!replaced);
                        retIdx++)
                {
                    if (_indiv[indivIdx].isFitterThan(ret[retIdx], _target))
                    {
                        auto it = ret.begin();
                        std::advance(it, retIdx);
                        ret.erase(it);
                        ret.push_back(_indiv[indivIdx]);
                        replaced = true;
                    }
                }
            }

            return ret;
        }


    protected:
        virtual void cbUpdate(uint32_t, const std::vector<P>&, TFT) = 0;


    private:
        void doGen(uint32_t breedCount)
        {
            std::vector<P> shortList = getBest(breedCount);
            assert(shortList.size() == breedCount);

            _indiv.clear();

            for (uint32_t j = 0; j < (_count - breedCount); j++)
            {
                uint32_t bestIndex1 = j % breedCount;
                uint32_t bestIndex2 = (j + 1) % breedCount;

                P pt;
                pt.initWithMutateCrossover(
                        shortList[bestIndex1],
                        shortList[bestIndex2]);

                _indiv.push_back(pt);
            }

            assert((_indiv.size() + breedCount) == _count);

            for (uint32_t j = 0; j < breedCount; j++)
            {
                auto it = shortList.begin();
                std::advance(it, j);
                _indiv.push_back(*it);
            }
        }


        std::vector<P> _indiv;
        TFT _target;
        const uint32_t _count;
    };
}

#endif
