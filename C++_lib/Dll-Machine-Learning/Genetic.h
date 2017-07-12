#pragma once

#include <ostream>
#include <sstream>
#include <random>
#include <iostream>
#include <algorithm>
#include <cassert>
#include <fstream>
#include <stdio.h>
#include <unistd.h>
#include "RBF.h"

struct Trait;
struct MyRandom {
	static constexpr int ASCII_DOWN_LIMIT = 32;
	static constexpr int ASCII_UP_LIMIT = 126;

	MyRandom() : r(), gen(r()), get_real_between_one_and_zero(0, 1), get_int_ascii(ASCII_DOWN_LIMIT, ASCII_UP_LIMIT), get_int_between_one_and_zero(0, 1) {
	}

	MyRandom& operator=(const MyRandom& myRandom) {
		gen = myRandom.gen;
		get_real_between_one_and_zero = myRandom.get_real_between_one_and_zero;
		get_int_ascii = myRandom.get_int_ascii;
		return *this;
	}

	// Return a real between 1 and 0
	double getRealBetweenOneAndZero() {
		return get_real_between_one_and_zero(gen);
	}
	// Return a char between the 2 limits of ascii char
	char getIntAsciiChar() {
		return get_int_ascii(gen);
	}
	// Return a random int between downlimit and uplimit included
	int getIntRange(int downlimit, int uplimit) {
		return std::uniform_int_distribution<int>(downlimit, uplimit)(gen);
	}
	// Return a random double between downlimit and uplimit included
	double getRealRange(double downlimit, double uplimit) {
		return std::uniform_real_distribution<double>(downlimit, uplimit)(gen);
	}
private:
	std::random_device r;
	std::mt19937 gen;
	std::uniform_real_distribution<double> get_real_between_one_and_zero;
	std::uniform_int_distribution<int> get_int_ascii;
	std::uniform_int_distribution<int> get_int_between_one_and_zero;
};
template<typename _Trait>
struct Chromosome : _Trait::Evaluate {
	using Gene = typename _Trait::Gene;
	using Random = typename _Trait::Random;

	Random& random;
	typename std::vector<Gene> genes;
	double fitness;


	Chromosome(Random& random) : random(random), genes(), fitness(_Trait::MAX_ERROR) {
		std::cout << "Chromosome::Chromosome()" << std::endl;
		for (int i = 0; i < _Trait::NB_GENES; ++i) {
			genes.push_back(Gene(random));
		}
	}

	Chromosome& operator=(const Chromosome& c) {
		random = c.random;
		genes = c.genes;
		fitness = c.fitness;
		return *this;
	}

	void evaluateFitness() {
		// fitness = _Trait::Evaluate::evaluate(*this);
		fitness = _Trait::Evaluate::evaluate(*this);
	}
	std::string toString() const {
		std::stringstream ss;
		ss << "fitness >" << std::to_string(fitness) << "< chromosome : ";
		for (size_t i = 0; i<genes.size(); i++) {
			ss << genes[i].toString() << " ";
		}
		return ss.str();;
	}
	std::string toSaveForm() const {

		std::stringstream ss;
		for (size_t i = 0; i<genes.size(); i++) {
			ss << genes[i].toString() << " ";
		}
		return ss.str();;
	}
	void saveState() {
		std::ofstream myfile;
		myfile.open(_Trait::SAVE_FILE_PATH, std::ios_base::app);
		myfile << (*this).toSaveForm() << "\n";
		myfile.close();
	}
	void recoverChromosome(char* line) {
		for (size_t i = 0; i<_Trait::NB_GENES; ++i) {
			for (size_t j = 0; j<_Trait::SIZE_OF_MASK; ++j) {
				(*this).genes[i].mask[j] = line[i*_Trait::SIZE_OF_MASK + j];
			}
		}
	}
};
template<typename _Trait>
struct RbfGene {
	MyRandom& random;
	double gamma;

	RbfGene(MyRandom& random) : random(random) {
		gamma = random.getRealRange(0, 10);
	}
	RbfGene(const RbfGene& rbfGene) : random(rbfGene.random), gamma(gamma) {
	}
	RbfGene& operator=(const RbfGene& rbfGene) {
		random = rbfGene.random;
		gamma = rbfGene.gamma;
		return *this;
	}
	std::string toString() const noexcept {
		std::stringstream ss;
		ss << gamma;
		return ss.str();
	}
	void mutation() {
		gamma = random.getRealRange(0, 10);
	}

};
template<typename _Trait>
struct EvalRbf
{
	double evaluate(Chromosome<_Trait>& chromosome) {
		int nbData = 2;
		int nbRepresentatives = 1;
		int inputSize = 1;
		// set gamma array using chromosome
		double* gamma = new double[nbRepresentatives];
		for (int i = 0; i < nbRepresentatives; i++) {
			gamma = chromosome.genes[i];
		}
		double* learningSample = new double[nbData*inputSize];
		double* learningSampleExpectedOutputs = new double[nbData];
		int nbTestSample = 50;
		double* testSample = new double[nbTestSample*inputSize];
		double* testSampleExpectedOutput = new double[nbTestSample];
		double* input = new double[inputSize];
		double fitness = 0;

		learningSample[0] = 1;
		learningSample[1] = 2;
		learningSampleExpectedOutputs[0] = 1;
		learningSampleExpectedOutputs[1] = 1;

		testSample[0] = 0.5;
		testSample[1] = 1;
		testSample[2] = 1.5;
		testSample[3] = 2;
		testSampleExpectedOutput[0] = 0.5;
		testSampleExpectedOutput[1] = 1;
		testSampleExpectedOutput[2] = 1.3;
		testSampleExpectedOutput[3] = 1;

		// Instantiate a new rbf to solve the problem
		RBF* rbf = new RBF(nbData, gamma, learningSample, inputSize, learningSampleExpectedOutputs, nbRepresentatives);
		// For each testSample data, add the difference between the value found and the value expected to the fitness
		double response;
		for (int i = 0; i < nbTestSample; i++) {
			input = testSample + i*inputSize;
			if (response = rbf->getRbfResponseClassif(testSample) > testSampleExpectedOutput[i]) {
				fitness += response - testSampleExpectedOutput[i];
			}
			else {
				fitness += testSampleExpectedOutput[i] - response;
			}
		}
		return fitness;
	}
};
std::ostream& operator<<(std::ostream& os, const Chromosome<Trait>& c) {
	os << c.toString();
	return os;
}
////////////////////////////////////////// SORT //////////////////////////////////////////
template<typename _Trait>
struct Minimise
{
	static bool sort(const Chromosome<_Trait>& c1, const Chromosome<_Trait>& c2) {
		return c1.fitness < c2.fitness;
	}
};
template<typename _Trait>
struct Maximise
{
	static bool sort(Chromosome<_Trait>& c1, Chromosome<_Trait>& c2) {
		return c1.fitness > c2.fitness;
	}
};

////////////////////////////////////////// SELECTION //////////////////////////////////////////
template<typename _Trait>
struct SimpleSelection {
	// Take a sorted population and select one randomly using the BEST_SELECTION_RATE variable
	static std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&> selection(std::vector<Chromosome<_Trait>>& sortedPopulation, MyRandom* random) {
		int rdm = random->getIntRange(0, _Trait::POP_SIZE - 1);
		int rdm2 = random->getIntRange(0, _Trait::POP_SIZE - 1);
		assert(rdm >= 0 && rdm < _Trait::POP_SIZE);
		assert(rdm2 >= 0 && rdm < _Trait::POP_SIZE);
		return std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&>(sortedPopulation[rdm], sortedPopulation[rdm2]);
	}
};
template<typename _Trait>
struct SimpleSelectionOfBests {
	static std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&> selection(std::vector<Chromosome<_Trait>>& sortedPopulation, MyRandom* random) {
		int rdm = random->getIntRange(0, _Trait::POP_SIZE*_Trait::BEST_SELECTION_RATE - 1);
		int rdm2 = random->getIntRange(0, _Trait::POP_SIZE*_Trait::BEST_SELECTION_RATE - 1);
		return std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&>(sortedPopulation[rdm], sortedPopulation[rdm2]);
	}
};
template<typename _Trait>
struct SelectionOfFirstBests {
	static std::pair<Chromosome<_Trait>, Chromosome<_Trait>> selection(std::vector<Chromosome<_Trait>> sortedPopulation, int i) {
		return std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&>(sortedPopulation[i], sortedPopulation[i + 1]);
	}
};

template<typename _Trait>
struct TournamentSelection {
	static std::pair<Chromosome<_Trait>, Chromosome<_Trait>> selection(std::vector<Chromosome<_Trait>> sortedPopulation, MyRandom* random) {
		// TODO
	}
};
template<typename _Trait>
struct RouletteSelection {
	static std::pair<Chromosome<_Trait>, Chromosome<_Trait>> selection(std::vector<Chromosome<_Trait>> sortedPopulation, MyRandom* random) {
		// TODO
	}
};
////////////////////////////////////////// CROSSOVER //////////////////////////////////////////
template<typename _Trait>
struct SinglePointCrossover {
	static std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&> crossover(std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&>& parents, MyRandom* random) {
		int randomCursor = random->getIntRange(1, _Trait::NB_GENES - 2);
		Chromosome<_Trait>& child1 = parents.first;
		Chromosome<_Trait>& child2 = parents.second;
#ifndef NDEBUG
		std::cout << "Before crossover child 1 : " << child1.toString() << " child 2 : " << child2.toString() << std::endl;
#endif
		// for(int i=0; i<randomCursor; i++){
		// 	child1.genes[i] = parents.first.genes[i];
		// 	child2.genes[i] = parents.second.genes[i];
		// }


		for (int i = randomCursor; i<_Trait::NB_GENES; i++) {
			typename _Trait::Gene tmp = child1.genes[i];
			child1.genes[i] = parents.second.genes[i];
			child2.genes[i] = tmp;
		}
#ifndef NDEBUG
		std::cout << " After crossover child 1 : " << child1.toString() << " child 2 : " << child2.toString() << std::endl;
#endif
		return std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&>(child1, child2);
	}
};
// template<typename _Trait>
// struct TwoPointCrossover{
// 	static std::pair<Chromosome<_Trait>, Chromosome<_Trait>> crossover(std::pair<Chromosome<_Trait>, Chromosome<_Trait>> parents, MyRandom* random){
// 		std::pair<Chromosome<_Trait>, Chromosome<_Trait>> children;
// 		int randomCursor = *random->getIntRange(1, _Trait::NB_GENES - 3);
// 		int randomCursor2 = *random->getIntRange(randomCursor+1, _Trait::NB_GENES - 2);

// 		children.first.genes = parents.first.genes;
// 		children.second.genes = parents.second.genes;
// 		assert(randomCursor < randomCursor2);
// 		for(int i=randomCursor; i<randomCursor2; i++){
// 			children.first.genes[i] = parents.second.genes[i];
// 			children.second.genes[i] = parents.first.genes[i];
// 		}
// 		return children;
// 	}
// };
template<typename _Trait>
struct MultiPointsCrossover {
	static std::pair<Chromosome<_Trait>, Chromosome<_Trait>> crossover(std::pair<Chromosome<_Trait>, Chromosome<_Trait>> parents, MyRandom* random) {
		// TODO
	}
};
////////////////////////////////////////// MUTATION //////////////////////////////////////////
template<typename _Trait>
struct SimpleMutation {
	static void mutation(Chromosome<_Trait>& chromosome, MyRandom* random) {
		int indexGene = random->getIntRange(0, _Trait::NB_GENES - 1);
		typename _Trait::Gene tmpGene = chromosome.genes[indexGene];
		chromosome.evaluateFitness();
		int tmpFitness = chromosome.fitness;
		chromosome.genes[indexGene].mutation();
		chromosome.evaluateFitness();
		if (tmpFitness < chromosome.fitness) {
			chromosome.genes[indexGene] = tmpGene;
		}
	}

};
template<typename _Trait>
struct BuildMyHeader {
	static void buildHeader() {
		assert(truncate(_Trait::SAVE_FILE_PATH, 0) == 0);
		std::ofstream saveStateFile;
		saveStateFile.open(_Trait::SAVE_FILE_PATH);
		saveStateFile << _Trait::NB_GENES << " " << _Trait::POP_SIZE << " " << _Trait::MAX_ITERATIONS << " " << _Trait::CROSSOVER_RATE << " " << _Trait::BEST_SELECTION_RATE << " " << _Trait::MUTATION_RATE << " " << _Trait::LIMIT_STAGNATION << "\n";
		saveStateFile.close();
	}
};
template<typename _Trait>
struct RecoverMyHeader {
	static void recoverHeader() {
		FILE *stream;
		char *line = NULL;
		size_t len = 0;
		ssize_t nread;

		stream = fopen(_Trait::SAVE_FILE_PATH, "r");
		if (stream == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}
		nread = getline(&line, &len, stream);
		fwrite(line, nread, 1, stdout);

		int nbGenes, popSize, maxIterations, stagnationLimit;
		float crossoverRate, bestSelectionRate, mutationRate;
		sscanf(line, "%d %d %d %f %f %f %d", &nbGenes, &popSize, &maxIterations, &crossoverRate, &bestSelectionRate, &mutationRate, &stagnationLimit);
		free(line);
		fclose(stream);

		if (nbGenes != _Trait::NB_GENES || popSize != _Trait::POP_SIZE || maxIterations != _Trait::MAX_ITERATIONS ||
			crossoverRate != _Trait::CROSSOVER_RATE || bestSelectionRate != _Trait::BEST_SELECTION_RATE || mutationRate != _Trait::MUTATION_RATE || stagnationLimit != _Trait::LIMIT_STAGNATION) {
			std::cout << "Header is not conform" << std::endl;
			exit(EXIT_FAILURE);
		}
	}
};

struct Trait {

	static constexpr int NB_GENES = 30;
	static constexpr const long MAX_ERROR = 100000;

	// Configuration
	static constexpr int POP_SIZE = 12;
	static constexpr long MAX_ITERATIONS = 10000;
	static constexpr double CROSSOVER_RATE = 0.90; // % of children produce from a crossover
	static constexpr double BEST_SELECTION_RATE = 0.25; // % of population consider as best
	static constexpr double MUTATION_RATE = 0.70;
	static constexpr int LIMIT_STAGNATION = 50000;

	using Random = MyRandom;

	using Selection = SimpleSelection<Trait>;
	using Crossover = SinglePointCrossover<Trait>;
	using Mutation = SimpleMutation<Trait>;
	using Sort = Minimise<Trait>;
	using Evaluate = EvalRbf<Trait>;
	using BuildHeader = BuildMyHeader<Trait>;
	using RecoverHeader = RecoverMyHeader<Trait>;
	using Gene = RbfGene<Trait>;

	static Random random;

	static constexpr const char* SAVE_FILE_PATH = "/tmp/state.save";

	static bool isFinished(long bestFitness, size_t iterations, int stagnation) {
		return false;
	}
};
Trait::Random Trait::random;

template<typename _Trait>
class GeneticAlgo : _Trait::Selection, _Trait::Crossover, _Trait::Mutation {
	using Random = typename _Trait::Random;
public:
	GeneticAlgo() : random(), population(), population2(), currentIterationCount(0), bestFitness(_Trait::MAX_ERROR), stagnation(0) {
		for (int i = 0; i < _Trait::POP_SIZE; ++i)
		{
			population.push_back(Chromosome<_Trait>(random));
		}
		population2.reserve(_Trait::POP_SIZE);
	};
	GeneticAlgo(int i) : population(_Trait::POP_SIZE), population2(), random() {
		population2.reserve(_Trait::POP_SIZE);
	};
	void start() {
		evaluate();
		sort();
#ifndef NDEBUG
		printAllRes();
#endif
		do {
#ifndef NDEBUG
			printAllRes();
			std::cout << "evaluation - sort" << std::endl;
#endif

			crossover();
#ifndef NDEBUG
			std::cout << "crossover" << std::endl;
			printAllRes();
#endif
			evaluate();
			sort();
#ifndef NDEBUG
			printAllRes();
#endif

			if (population[0].fitness >= bestFitness) {
				stagnation++;
			}
			else {
				stagnation = 0;
				bestFitness = population[0].fitness;
				std::cout << "New best answer >" << population[0].toString() << "< fitness :" << bestFitness << " iterations :" << currentIterationCount << std::endl;
				saveState();
			}
		} while (!_Trait::isFinished(bestFitness, currentIterationCount++, stagnation));
		evaluate();
		sort();
		printAllRes();
	}
	void recoverState() {
		_Trait::RecoverHeader::recoverHeader();

		int nbGenes, popSize, maxIterations, stagnationLimit;
		float crossoverRate, bestSelectionRate, mutationRate;

		FILE *stream;
		char *line = NULL;
		size_t len = 0;
		ssize_t nread;

		stream = fopen(_Trait::SAVE_FILE_PATH, "r");
		if (stream == NULL) {
			perror("fopen");
			exit(EXIT_FAILURE);
		}

		nread = getline(&line, &len, stream);
		fwrite(line, nread, 1, stdout);

		sscanf(line, "%d %d %d %f %f %f %d", &nbGenes, &popSize, &maxIterations, &crossoverRate, &bestSelectionRate, &mutationRate, &stagnationLimit);

		for (size_t i = 0; i<_Trait::POP_SIZE; i++) {
			nread = getline(&line, &len, stream);
			population[i].recoverChromosome(line);
		}
		free(line);
		fclose(stream);
	}
	long getBestFitness() { return bestFitness; }
	size_t getCurrentIterationCount() { return currentIterationCount; }
private:
	void printRes() {
		std::cout << "Best answer found in " << currentIterationCount << " iterations : " << population[0] << std::endl;
	}
	void printAllRes() {
		std::cout << "Best answer found in " << currentIterationCount << " iterations : " << population[0] << std::endl;
		for (int i = 0; i<_Trait::POP_SIZE; i++) {
			std::cout << "[" << i << "] >" << population[i].toString() << std::endl;
		}
	}
	Random random;
	std::vector<Chromosome<_Trait>> population;
	std::vector<Chromosome<_Trait>> population2;
	size_t currentIterationCount;
	long bestFitness;
	int stagnation;

	void evaluate() {
		for (int i = 0; i<_Trait::POP_SIZE; ++i) {
			population[i].evaluateFitness();
		}
	}
	void mutation(Chromosome<_Trait>& ch) {
		if (random.getRealBetweenOneAndZero() < _Trait::MUTATION_RATE) {
#ifndef NDEBUG
			std::cout << "MUTATION !!! " << std::endl;
#endif
			_Trait::Mutation::mutation(ch, &random);
		}
	}
	void crossover() {
		assert(population2.size() == 0);
		assert(population.size() != 0);

		while (population2.size() <= _Trait::POP_SIZE*_Trait::CROSSOVER_RATE) {
			std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&> parents = _Trait::Selection::selection(population, &random);
			std::pair<Chromosome<_Trait>&, Chromosome<_Trait>&> children = _Trait::Crossover::crossover(parents, &random);
#ifndef NDEBUG
			// std::cout << "Children produced by crossover  : child 1 : " << children.first.toString() << " child 2 : " << children.second.toString() << std::endl;
#endif
			mutation(children.first);
			mutation(children.second);
#ifndef NDEBUG
			// std::cout << "        Children after mutation : child 1 : " << children.first.toString() << " child 2 : " << children.second.toString() << std::endl;
#endif
			population2.push_back(children.first);
			population2.push_back(children.second);

		}
		while (population2.size() <= _Trait::POP_SIZE) {
			std::pair< Chromosome<_Trait>, Chromosome<_Trait>> nonChangeIndiv = _Trait::Selection::selection(population, &random);
			population2.push_back(nonChangeIndiv.first);
			population2.push_back(nonChangeIndiv.second);

		}
		population.swap(population2);
		population2.clear();
	}
	void sort() {
		std::sort(population.begin(), population.end(), _Trait::Sort::sort);
	}
	void saveState() {
		// Build header
		_Trait::BuildHeader::buildHeader();

		for (size_t i = 0; i<_Trait::POP_SIZE; i++) {
			population[i].saveState();
		}
	}
};
