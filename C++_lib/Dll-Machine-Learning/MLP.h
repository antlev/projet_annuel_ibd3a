#pragma once
//
// Created by antoine on 13/06/2017.

#include <cassert>
#include <cstdlib>
//#include <tgmath.h>
#include <iostream>

class MLP {
public:
	MLP(int *structure, int nbLayer) : nbLayer(nbLayer) {
		assert(nbLayer >= 2);
		for (int i = 0; i < nbLayer; ++i) {
			assert(structure[i] > 0);
		}
		maxIterations = 10000;
		learningRate = 0.1;
		modelLearned = 0; //Model untrained
		weights = new double **[nbLayer - 1];
		neurons = new double *[nbLayer];
		gradient = new double *[nbLayer];
		this->structure = new int[nbLayer];
		for (int i = 0; i<nbLayer; ++i) {
			this->structure[i] = structure[i];
		}
		for (int layerNumber = 0; layerNumber < nbLayer; layerNumber++) {
			neurons[layerNumber] = new double[structure[layerNumber] + 1]; // Bias neuron
			gradient[layerNumber] = new double[structure[layerNumber] + 1]; // Bias neuron
			for (int leftNeuron = 0; leftNeuron < structure[layerNumber] + 1; leftNeuron++) {
				gradient[layerNumber][leftNeuron] = 0;
				neurons[layerNumber][leftNeuron] = 0;
			}
			neurons[layerNumber][structure[layerNumber]] = 1; // Bias
		}
		for (int layerNumber = 0; layerNumber < nbLayer - 1; layerNumber++) {
			weights[layerNumber] = new double *[structure[layerNumber] + 1]; // Bias neuron
			for (int leftNeuron = 0; leftNeuron < structure[layerNumber] + 1; leftNeuron++) {
				weights[layerNumber][leftNeuron] = new double[structure[layerNumber + 1]];
				for (int rightNeuron = 0; rightNeuron < structure[layerNumber + 1]; ++rightNeuron) {
					weights[layerNumber][leftNeuron][rightNeuron] = ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
				}
			}
		}
	}
	~MLP() {
		for (int layerNumber = 0; layerNumber < nbLayer; layerNumber++) {
			delete neurons[layerNumber];
			delete gradient[layerNumber];
		}
		for (int layerNumber = 0; layerNumber < nbLayer - 1; layerNumber++) {
			delete weights[layerNumber];
			for (int leftNeuron = 0; leftNeuron < structure[layerNumber] + 1; leftNeuron++) {
				delete weights[layerNumber][leftNeuron];
			}
		}
	}
	void classify(double *oneInput, int inputSize);
	void fitClassification(double *inputs, int inputSize, int inputsSize, double *expectedOutputs,
		int outputSize);
	void fitRegression(double *inputs, int inputSize, int inputsSize, double *expectedOutputs,
		int outputSize);
	void predict(double* oneInput, int inputSize);
	double getOutputsforClassif();
	double getOutputsforRegression();

private:
	double ***weights;
	double **neurons;
	double **gradient;
	int *structure;
	double learningRate;

	int maxIterations;
	int nbLayer;
	int modelLearned;

	void fitClassifOneInput(double *oneInput, int inputSize, double *oneOutput, int outputSize);
	void fitRegreOneInput(double *oneInput, int inputSize, double *oneOutput, int outputSize);
	double sum(int layerNb, int neuronNb);
};