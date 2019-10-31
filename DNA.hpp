#ifndef DNA
#define DNA


#define GENE_COUNT 10
#define ENV_COUNT 100
#define PHENO_COUNT 11
#define INTMAX 0xFFFFFFFFFFFFFFFF // 64 bits of int


#include <cstring>
#include <random>
#include <functional>

class Dna;
class Environment;

class Phenotype {
    public:
    Phenotype(std::function<double (Dna *, Environment *)> evaluator) {
        this->evaluate = evaluator;
    }
    std::function<double (Dna *, Environment *)> evaluate;
};

class Environment {
    private:
    double weights[ENV_COUNT];

    public:
    Environment() {

    }

    unsigned int getWeight(short i) {
        return weights[i];
    }
};

class Dna {
    private:
    double weights[GENE_COUNT];
    

    public:
    Dna() {
        std::uniform_real_distribution<double> dist(0, 1);
        std::random_device rd;
        std::mt19937 e2(rd());
        for(short i = 0; i < GENE_COUNT; i++) {
            this->weights[i] = dist(e2);
        }
        printDNA();
        
    }
    double getWeight(short i) {
        return weights[i];
    }

    void printDNA() {
        std::cout << "PRINTING DNA" << std::endl;
        for(int i = 0; i < GENE_COUNT; i++) {
            std::cout << i << " -=- " << weights[i] << std::endl;
        }
        std::cout << "---------------------" << std::endl;
    }
    

};

class EvolutionContainer {
    public:
    std::unordered_map<std::string, Phenotype *> pheno;
    Environment env;
    Dna dna;
    
};

class PhenotypeEvaluators {
    private:

    public:
    static std::function<double (Dna *, Environment *)> Basic(short index) {
        return [index](Dna * d, Environment * e) {
            return (double)d->getWeight(index);
        };
    }
    static std::function<double (Dna *, Environment *)> Scaled(short index, double scalar) {
        return [index,scalar](Dna * d, Environment * e) {
            return (double)scalar * d->getWeight(index);
        };
    }
    static std::function<double (Dna *, Environment *)> ScaledOffset(short index, double scalar, double offset) {
        return [index,scalar,offset](Dna * d, Environment * e) {
            return (double)scalar * d->getWeight(index) + offset;
        };
    }
};

#endif