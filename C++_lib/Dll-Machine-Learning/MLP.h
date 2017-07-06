#pragma once
//
// Created by antoine on 13/06/2017.
//
#include <cassert>
#include <cstdlib>
#include <iostream>

class MLP {
public:
	MLP(int *structure, int nbLayer);
	~MLP();
	double* MLP::classify(double *oneInput);
	void fitClassification(double *inputs, int nbData, double *expectedOutputs);
	void fitRegression(double *inputs, int nbData, double *expectedOutputs);
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