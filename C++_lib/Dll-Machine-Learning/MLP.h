#pragma once
//
// Created by antoine on 13/06/2017.
//
#include <cassert>
#include <cstdlib>
#include <iostream>

class MLP {
public:
	MLP(int *structure, int nbLayer) : nbLayer(nbLayer) {
		assert(nbLayer >= 2);
		assert(structure != nullptr);
		for (int i = 0; i < nbLayer; ++i) {
			assert(structure[i] > 0);
		}
		inputSize = structure[0];
		outputSize = structure[nbLayer - 1];
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
	double* MLP::classify(double *oneInput);
	void fitClassification(double *inputs, int inputsSize, double *expectedOutputs);
	void fitRegression(double *inputs, int inputsSize, double *expectedOutputs);
	double* predict(double* oneInput);

private:
	double ***weights;
	double **neurons;
	double **gradient;
	int *structure;
	
	double learningRate;
	int inputSize;
	int outputSize;

	int maxIterations;
	int nbLayer;
	int modelLearned;

	double* getOutputsforClassif();
	double* getOutputsforRegression();
	void fitClassifOneInput(double *oneInput, double* oneOutput);
	void fitRegreOneInput(double *oneInput, double *oneOutput);
	double sum(int layerNb, int neuronNb);
};