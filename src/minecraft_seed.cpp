#include <cassert>
#include <cctype>
#include <climits>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <ctime>
#include "population.hpp"

// Including null-terminator
// TODO: make length a template parameter?
#define SEED_LEN 40

////////////////////////////////////////////////////////////////////////////////
class SeedGenotype final : public gaxx::Genotype<int32_t>
{
public:
    SeedGenotype()
        : Genotype<int32_t>()
    {
        _val = new char[SEED_LEN];
        memset(_val, 0, SEED_LEN);
    }


    virtual ~SeedGenotype()
    {
        delete[] _val;
    }


    SeedGenotype(const SeedGenotype& other)
        : Genotype<int32_t>()
    {
        _val = new char[SEED_LEN];
        copyValFrom(other.getValue());
        _hash = other._hash;
    }


    SeedGenotype& operator=(const SeedGenotype& other)
    {
        copyValFrom(other.getValue());
        _hash = other._hash;
        return *this;
    }


    void init() override
    {
        size_t len = (rand() % (SEED_LEN - 1)) + 1;

        for (size_t i = 0; i < len; i++)
            _val[i] = getNewChar();

        _val[len] = 0;

        // Update fitness
        _hash = hashCode(_val);
    }


    void initWithMutateCrossover(
            const Genotype<int32_t>& _other1,
            const Genotype<int32_t>& _other2) override
    {
        const SeedGenotype& other1 = static_cast<const SeedGenotype&>(_other1);
        const char* other1Val = other1.getValue();
        size_t other1Len = strlen(other1Val);
        int thresh;
        assert(other1Len > 0);

        for (uint32_t i = 0; i < other1Len; i++)
        {
            // Probability of mutating a character
            thresh = RAND_MAX / 2;

            if (rand() < thresh)
            {
                // Keep trying to mutate a bit until char is print
                unsigned char out = 0;

                do
                {
                    // Pick a bit between 1-8 (actually use a 0-7 shift)
                    int r = (rand() & 0xff) / 32;
                    int mask = 1 << r;
                    out = other1Val[i] ^ mask;
                } while (!isprint(out));

                _val[i] = out;
            }
            else
            {
                _val[i] = other1Val[i];
            }
        }

        // Now test if the string should be extended/reduced
        thresh = RAND_MAX / 2;

        if (rand() < thresh)
        {
            int test = rand();
            thresh = RAND_MAX / 2;

            if ((test < thresh) && (other1Len < (SEED_LEN - 1)))
            {
                int noToAdd = (rand() % ((SEED_LEN - 1) - other1Len)) + 1;
                assert(noToAdd > 0);
                assert((other1Len + noToAdd) < SEED_LEN);

                for (int i = 0; i < noToAdd; i++)
                {
                    _val[other1Len + i] = getNewChar();
                }

                _val[other1Len + noToAdd] = 0;
            }
            else if ((test >= thresh) && (other1Len > 1))
            {
                int newLen = (rand() % (other1Len - 1)) + 1;
                assert(newLen > 0);
                _val[newLen] = 0;
            }
        }

        // TODO crossover

        // Update fitness
        _hash = hashCode(_val);
    }


    /* C++ implementation of Java's String.hashCode() */
    static int32_t hashCode(const char* const str)
    {
        assert(str != nullptr);
        int32_t hash = 0;
        int32_t len = strlen(str);

        if (len > 0)
        {
            for (int32_t i = 0; i < (len - 1); i++)
            {
                int32_t temp = 31;

                for (int32_t j = 0; j < (len - (i + 2)); j++)
                    temp *= 31;

                hash += str[i] * temp;
            }

            hash += str[len - 1];
        }

        //printf("Input string: \"%s\"\n", str);
        //printf("Hash: %d\n", hash);

        return hash;
    }


    uint64_t getFitness(int32_t target) const
    {
        uint64_t diff = abs(_hash - target);
        return diff;
    }


    const char* getValue() const
    {
        return _val;
    }


protected:
    static char getNewChar()
    {
        int r = 0;

        do
        {
            r = rand() % 0xff;
        } while (!isprint(r));

        return static_cast<char>(r);
    }


private:
    void copyValFrom(const char* otherVal)
    {
        size_t otherLen = strlen(otherVal);
        assert(otherLen > 0);
        assert(otherLen < SEED_LEN);

        strncpy(_val, otherVal, SEED_LEN);
        assert(_val[SEED_LEN - 1] == 0);
    }


    char* _val;
    int32_t _hash;
};


////////////////////////////////////////////////////////////////////////////////
class SeedPhenotype final : public gaxx::Phenotype<int32_t>
{
public:
    SeedPhenotype() = default;
    virtual ~SeedPhenotype() = default;


    bool isFitterThan(
            const Phenotype<int32_t>& _other,
            int32_t target) const override
    {
        const SeedPhenotype& other = static_cast<const SeedPhenotype&>(_other);
        return getFitness(target) < other.getFitness(target);
    }


    bool isOptimalFitness(int32_t target) const override
    {
        return getFitness(target) == 0;
    }


    gaxx::Genotype<int32_t>& getGenotype() override
    {
        return _gt;
    }


    /* XXX: See phenotype.hpp for serious question... */
    const gaxx::Genotype<int32_t>& getGenotype() const override
    {
        return _gt;
    }


    uint64_t getFitness(int32_t target) const
    {
        return _gt.getFitness(target);
    }


private:
    SeedGenotype _gt;
};


////////////////////////////////////////////////////////////////////////////////
class SeedPopulation final : public gaxx::Population<SeedPhenotype, int32_t>
{
public:
    SeedPopulation(uint64_t count, int32_t target)
        : Population<SeedPhenotype, int32_t>(count, target)
    {
    }


    virtual ~SeedPopulation() = default;


    void cbUpdate(
            uint32_t gen,
            const std::vector<SeedPhenotype>& indiv,
            int32_t target) override
    {
        uint32_t bestFitIdx = 0;

        //printf("----------------------------------------"
        //        "----------------------------------------\n");

        for (uint32_t i = 0; i < indiv.size(); i++)
        {
            if (indiv[i].isFitterThan(indiv[bestFitIdx], target))
                bestFitIdx = i;

            //const SeedPhenotype& pt =
            //    static_cast<const SeedPhenotype&>(indiv[i]);
            //const SeedGenotype& gt =
            //    static_cast<const SeedGenotype&>(pt.getGenotype());
            //printf("[G%u]\t| fit = %lu\t| \"%s\"\n",
            //        gen, gt.getFitness(target), gt.getValue());
        }

        const SeedPhenotype& pt =
            static_cast<const SeedPhenotype&>(indiv[bestFitIdx]);
        const SeedGenotype& gt =
            static_cast<const SeedGenotype&>(pt.getGenotype());

        //printf("*\n[G%u]\t| BEST = %lu\t| \"%s\"\n",
        //        gen, gt.getFitness(target), gt.getValue());

        uint64_t thisFit = gt.getFitness(target);

        if (thisFit < _bestFit)
        {
            printf("[G%u]\t| BEST = %lu\t| \"%s\"\n",
                    gen, thisFit, gt.getValue());
            _bestFit = thisFit;
        }
    }

    uint64_t _bestFit = std::numeric_limits<uint64_t>::max();
};


////////////////////////////////////////////////////////////////////////////////
int main(int argc, char** argv)
{
    if ((argc == 4) || (argc == 5))
    {
        long popSize = strtol(argv[1], nullptr, 10);
        long breedCount = strtol(argv[2], nullptr, 10);
        long genCount, hash;

        if (argc == 4)
        {
            hash = strtol(argv[3], nullptr, 10);
        }
        else
        {
            genCount = strtol(argv[3], nullptr, 10);
            hash = strtol(argv[4], nullptr, 10);
        }

        /* - breed# must be at least 2.
         * - pop# must be at least one more than breed#.
         * - gen# must be at least 1. */

        if ((popSize < 3) || (popSize > INT_MAX) ||
                (breedCount < 2) || (breedCount > INT_MAX) ||
                (hash < INT_MIN) || (hash > INT_MAX) ||
                (breedCount >= popSize) ||
                ((argc == 5) && ((genCount < 1) || (genCount > INT_MAX))))
        {
            fprintf(stderr, "Invalid input\n");
            return EXIT_FAILURE;
        }
        else
        {
            srand(time(nullptr));

            SeedPopulation pop(
                    static_cast<uint64_t>(popSize),
                    static_cast<int32_t>(hash));

            if (argc == 5)
                pop.runForGenerations(
                        static_cast<uint32_t>(genCount),
                        static_cast<uint32_t>(breedCount));
            else
                pop.run(static_cast<uint32_t>(breedCount));

            return EXIT_SUCCESS;
        }
    }
    else
    {
        fprintf(stderr, "Usage: %s <pop#> <breed#> [gen#] <hash>\n", argv[0]);
        return EXIT_FAILURE;
    }
}
