//
// Created by antoine on 14/06/2017.
//
#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <cmath>
#include "LinearPerceptron.h"

class MatrixXd;
using namespace std;
// Instantiate a simple Perceptron (without any hidden layer)
// @param inputSize : number of neuron input of the perceptron
// @param outputSize : number of neuron output of the perceptron
LinearPerceptronClassif::LinearPerceptronClassif(int inputSize, int outputSize)  : inputSize(inputSize), outputSize(outputSize){
	assert(inputSize > 0);
	assert(outputSize > 0);
	// Create the model
	model = new double[(inputSize + 1) * outputSize];
	for (int i = 0; i < (inputSize + 1) * outputSize; ++i) {
		model[i] = ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
	}
}
// Delete a perceptron from the memory
LinearPerceptronClassif::~LinearPerceptronClassif() {
	if (model) { delete model; }
}
// Return the MLP's response considering the inputs
// @param input : input of one data
// @param inputSize : size of input array
// @param outputSize : size of output array
double* LinearPerceptronClassif::linear_classify(double* input){
	assert(input != NULL);
	assert(this->model != nullptr);
	double sum_weigths_inputs;
	double* inputWithBias = addBiasToInput(input, inputSize);
	double* output = new double[outputSize];
	for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
		sum_weigths_inputs = 0;
		for (int modelIterator = outputIterator, inputIterator = 0; 
				modelIterator < (inputSize + 1)*outputSize && inputIterator < inputSize + 1;
				inputIterator++, modelIterator += outputSize) {
			sum_weigths_inputs += model[modelIterator] * inputWithBias[inputIterator];
		}
		output[outputIterator] = (sum_weigths_inputs >= 0 ? 1 : -1);
	}
	return output;
}
// Function called from outside (the dll) taking all inputs and ouputs to learn rosenblatt to a model
// @param inputs : all inputs of the datas, that are concatenated in a single one entry array
// @param inputsSize : size of the whole array
// @param inputSize : size of one input
// @param expectedOutputs : array containing for each input all the output neuron expected response (ex : 2 inputs of 3 outputs : [i=1 o=1][i=1 o=2][i=1 o=2][i=2 o=1][i=2 o=2][i=2 o=2][i=2 o=1][i=3 o=2][i=3 o=3])
// @param outputSize : size of output neurons
// @param iterationMax : number of max of iterations
// @param step : step that will be use for learning
int LinearPerceptronClassif::linear_fit_classification_rosenblatt(double *inputs, int nbData, double *expectedOutputs, int iterationMax, double step) {
	assert(this->model != NULL);
	assert(inputs != NULL);
	assert(nbData > 0);
	assert(expectedOutputs != NULL);
	assert(iterationMax > 0);
	assert(step > 0);
	int iterations(0);
	int error;
	int inputsSize = nbData * inputSize;
	while (true) {
		if (iterations > iterationMax) {
			return 1;
		}
		iterations++;
		error = 0;
		double* oneInput = new double[inputSize];
		double* oneOutput = new double[outputSize];
		// For each data passed in parameter of the function
		for (int inputsIterator = 0, inputIterator = 0; inputsIterator < inputsSize; inputsIterator += inputSize, inputIterator++) {
			// recovering inputs one by one
			for (int i = 0; i<inputSize; i++) {
				oneInput[i] = inputs[inputsIterator + i];
			}
			// Get the result given by the MLP
			double* oneOutput = linear_classify(oneInput);
			oneInput = addBiasToInput(oneInput, inputSize);
			bool* outputIsGood = new bool[outputSize];
			double* outputError = new double[outputSize];
			bool allOutputsAreGood = true;
			// For each output neuron we check that response from MLP is correct
			for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
				outputIsGood[outputIterator] = true; // Initialising all response at true
													 // If one of the output neuron doesn't give the good answer
				if (oneOutput[outputIterator] != expectedOutputs[inputIterator*outputSize + outputIterator]) {
					outputIsGood[outputIterator] = false;
					if (allOutputsAreGood) { allOutputsAreGood = false; }
					// Storing the output error in a tab
					outputError[outputIterator] = expectedOutputs[inputIterator*outputSize + outputIterator] - oneOutput[outputIterator];
				}
			}
			// If at least one of the MLP's response is not good
			if (!allOutputsAreGood) {
				error++; // We increment the error of the current MLP on the data given
				for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
					// For each output, if the MLP's response ins't good -> We modify the weights affecting the output
					if (!outputIsGood[outputIterator]) {
						// We adapt every weight of our MLP using the Rsoenblatt formula
						for (int modelIterator = outputIterator, inputIterator = 0; modelIterator < (inputSize + 1)*outputSize; modelIterator += outputSize, inputIterator++) {
							model[modelIterator] += step * outputError[outputIterator] * oneInput[inputIterator];
						}
					}
				}
			}
		}
		if (error == 0) {
			return 0;
		}
	}
}
// Instantiate and learn a dataSet for a simple linear Percetron
// @param inputs : array containing flatten inputs data
// @param inputSize : size of input array
// @param nbData : number of data
// @param expectedOutputs : array containing flatten outputs data
// @param outputSize : size of output array
LinearPerceptronRegression::LinearPerceptronRegression(double *inputs, int inputSize, int nbData, double *expectedOutputs, int outputSize){
	assert(inputSize > 0);
	assert(outputSize > 0);
	// Create the model
	assert(inputs != NULL);
	assert(inputSize > 0);
	assert(expectedOutputs != NULL);
	assert(outputSize > 0);
	inputs = addBiasToInputs(inputs, nbData, &inputSize);
	// Build X and Y
	Eigen::MatrixXd X(nbData, inputSize);
	Eigen::MatrixXd Y(nbData, outputSize);

	tabToMatrix(&X, inputs, nbData, inputSize);
	tabToMatrix(&Y, expectedOutputs, nbData, outputSize);
	// return the calculated result as a matrix
	model = new Eigen::MatrixXd(pinv(X) * Y);
}
// Delete the mlp
LinearPerceptronRegression::~LinearPerceptronRegression() {
	if (model) { delete model; }
}
// For a trained model, this function calculate the output of the single input passed as a parameter
// @param input : input of one data
// @param inputSize : size of input array
// @param output : array to write result
// @param outputSize : size of output array
double* LinearPerceptronRegression::linearPredict(double* input) {
	assert(input != nullptr);
	assert(model != nullptr);
	double* inputWithBias = addBiasToInput(input, inputSize);
	double* output = new double[outputSize];

	Eigen::MatrixXd inputMatrix(1, inputSize + 1);
	for (int i = 0; i<inputSize + 1; i++) {
		inputMatrix(0, i) = inputWithBias[i];
	}
	Eigen::MatrixXd outputMatrix(outputSize, 1);
	outputMatrix = inputMatrix * (*model);
	matrixToTab(outputMatrix, output, 1, outputSize);
	return output;
}

// Add bias to the inputs
// Modify inputSize and inputsSize
// @param input : input of one data
// @param inputSize : size of input array
// @return the new input
double* addBiasToInputs(double *inputs, int nbData, int *inputSize) {
	assert(inputs != NULL);
	assert(nbData >= 0);
	assert(inputSize > 0);
	*inputSize += 1; // Bias
	double* newInputs = new double[*inputSize*nbData];
	int inputIterator = 0;
	for (int newinputIterator = 0; newinputIterator < (*inputSize*nbData); newinputIterator++) {
		if (newinputIterator % (*inputSize) == 0) {
			newInputs[newinputIterator] = 1;
			newinputIterator++;
		}
		newInputs[newinputIterator] = inputs[inputIterator];
		inputIterator++;
	}
	return newInputs;
}
// Add bias to one input
// Do not modify inputSize and inputsSize
// @param input : input of one data
// @param inputSize : size of input array
// @return the new input
double* addBiasToInput(double *input, int inputSize) {
	assert(input != NULL);
	assert(inputSize > 0);
	double* newInput = new double[2 + 1];
	newInput[0] = 1;
	for (int i = 1; i<inputSize + 1; i++) {
		newInput[i] = input[i - 1];
	}
	return newInput;
}
// Transform an array of double into a matrix
// @param matrix : Eigen matrix to fill
// @param tab : array to convert to matrix
// @param nbRow : number of rows of the matrix to fill 
// @param nbCols : number of columns of the matrix to fill 
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols) {
	assert(matrix != NULL);
	assert(tab != NULL);
	assert(nbRow > 0 && nbCols > 0);
	int iterator = 0;
	for (int i = 0; i<nbRow; i++) {
		for (int j = 0; j<nbCols; ++j) {
			(*matrix)(i, j) = tab[iterator];
			iterator++;
		}
	}
}
// Transform a matrix into a array of double
// @param matrix : Eigen matrix to fill
// @param tab : array to fill
// @param nbRow : number of rows of the matrix
// @param nbCols : number of columns of the matrix
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols) {
	assert(tab != NULL);
	assert(nbRow > 0 && nbCols > 0); 
	//assert(matrix.row == nbRow);// TODO
	//assert(matrix.col == nbCols); 
	int iterator = 0;
	for (int i = 0; i<nbRow; ++i) {
		for (int j = 0; j<nbCols; ++j) {
			tab[iterator] = matrix(i, j);
			iterator++;
		}
	}
}
// Return the pseudo inverse of the matrix passed as a parameter
// @param X : matrix to manipulate
// @return : pseudo-inverse of the given matrix
Eigen::MatrixXd pinv(Eigen::MatrixXd X) {
	Eigen::MatrixXd X_Transp = X.transpose();
	return (((X_Transp * X).inverse()) * X_Transp);
}