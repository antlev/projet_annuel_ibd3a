#include "stdafx.h"
#include <iostream>
#include <stdlib.h>
#include <cassert>
#include <cmath>
#include "LinearPerceptron.h"

class MatrixXd;

using namespace std;

//  ---------- APPLICATION ----------
// Return the MLP's response considering the inputs
// @param model : model used to classify
// @param input : input of one data
// @param inputSize : size of input array
// @param outputs : array to write results of all the outputs neurons
// @param outputDimension : size of output array
void LinearPerceptron::linear_classify(double* input, int inputSize, double* output, int outputDimension) {
	assert(input != NULL);
	assert(inputSize > 0);
	assert(output != NULL);
	assert(outputDimension > 0);
	double sum_weigths_inputs;
	double* inputWithBias = addBiasToInput(input, inputSize);
	for (int outputIterator = 0; outputIterator < outputDimension; outputIterator++) {
		sum_weigths_inputs = 0;
		for (int modelIterator = outputIterator, inputIterator = 0; modelIterator < (inputSize + 1)*outputDimension && inputIterator < inputSize + 1; inputIterator++, modelIterator += outputDimension) {
			sum_weigths_inputs += classifModel[modelIterator] * inputWithBias[inputIterator];
		}
		output[outputIterator] = (sum_weigths_inputs >= 0 ? 1 : -1);
	}
}
// ---------- APPRENTSSAGE ----------
// Function called from C# taking all inputs and ouputs to learn rosenblatt to a model
// @param model : model used to classify
// @param inputs :
// @param inputsSize :
// @param inputSize :
// @param expectedOutputs : array containing for each input all the output neuron expected response (ex : 2 inputs of 3 outputs : [i=1 o=1][i=1 o=2][i=1 o=2][i=2 o=1][i=2 o=2][i=2 o=2][i=2 o=1][i=3 o=2][i=3 o=3])
// @param outputSize : size oh output neurons
// @param iterationMax : number of max of iterations
// @param step : step
int LinearPerceptron::linear_fit_classification_rosenblatt(double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize, int iterationMax, double step) {
	assert(classifModel != NULL);
	assert(inputs != NULL);
	assert(inputSize > 0);
	assert(inputsSize >= inputSize);
	assert(inputSize > 0);
	assert(expectedOutputs != NULL);
	assert(outputSize > 0);
	assert(iterationMax > 0);
	assert(step > 0);
	int iterations(0);
	int error;
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
			linear_classify(oneInput, inputSize, oneOutput, outputSize);
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
							classifModel[modelIterator] += step * outputError[outputIterator] * oneInput[inputIterator];
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

// Fit a model with all the inputs and outputs WITHOUT bias and concatenate in an double* array
// Return a pointer on W matrix
void LinearPerceptron::linear_create_and_fit_regression(double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize) {
	inputs = addBiasToInputs(inputs, &inputsSize, &inputSize);
	// Build X and Y
	int nbInput = inputsSize / inputSize;
	Eigen::MatrixXd X(nbInput, inputSize);
	Eigen::MatrixXd Y(nbInput, outputSize);

	tabToMatrix(&X, inputs, nbInput, inputSize);
	tabToMatrix(&Y, expectedOutputs, nbInput, outputSize);
	// return the calculated result as a matrix
	regressionModel = new Eigen::MatrixXd(pinv(X) * Y);
}

// For a trained model, this function calculate the output of the single input passed as a paramter
// The output array will be filled
void LinearPerceptron::linearPredict(double* input, int inputSize, double* output, int outputSize) {
	//	assert(model != NULL);
	assert(input != NULL);
	assert(inputSize > 0);
	assert(output != NULL);
	assert(outputSize > 0);
	double* inputWithBias = addBiasToInput(input, inputSize);

	Eigen::MatrixXd inputMatrix(1, inputSize + 1);
	for (int i = 0; i<inputSize + 1; i++) {
		inputMatrix(0, i) = inputWithBias[i];
	}
	Eigen::MatrixXd outputMatrix(outputSize, 1);
	outputMatrix = inputMatrix * (*regressionModel);
	matrixToTab(outputMatrix, output, 1, outputSize);
}

// Add bias to the inputs
// Modify inputSize and inputsSize
// Return the new input
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize) {
	int nbData = *inputsSize / *inputSize;
	*inputsSize += nbData;
	double* newInputs = new double[*inputsSize + nbData];
	int inputIterator = 0;
	for (int newinputIterator = 0; newinputIterator < (*inputsSize); newinputIterator++) {
		if (newinputIterator % (*inputSize + 1) == 0) {
			newInputs[newinputIterator] = 1;
			newinputIterator++;
		}
		newInputs[newinputIterator] = inputs[inputIterator];
		inputIterator++;
	}
	*inputSize += 1;
	return newInputs;
}
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
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols) {
	int iterator = 0;
	for (int i = 0; i<nbRow; i++) {
		for (int j = 0; j<nbCols; ++j) {
			(*matrix)(i, j) = tab[iterator];
			iterator++;
		}
	}
}
// Transform a matrix into a array of double
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols) {
	int iterator = 0;
	for (int i = 0; i<nbRow; ++i) {
		for (int j = 0; j<nbCols; ++j) {
			tab[iterator] = matrix(i, j);
			iterator++;
		}
	}
}
// Return the pseudo inverse of the matrix passed as a parameter
Eigen::MatrixXd pinv(Eigen::MatrixXd X) {
	Eigen::MatrixXd X_Transp = X.transpose();
	return (((X_Transp * X).inverse()) * X_Transp);
}