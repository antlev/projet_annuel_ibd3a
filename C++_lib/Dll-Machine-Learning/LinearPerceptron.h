#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"
Eigen::MatrixXd pinv(Eigen::MatrixXd X);
double* addBiasToInput(double *input, int inputSize);
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize);
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols);
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols);

class LinearPerceptronClassif {
public:
	LinearPerceptronClassif(int inputSize, int outputSize);
	~LinearPerceptronClassif();
	// Classification
	int linear_fit_classification_rosenblatt(double *inputs, int nbData, double *expectedOutputs, int iterationMax, double step);
	double* linear_classify(double* input);
private:
	double* model;
	int inputSize;
	int outputSize;
};
class LinearPerceptronRegression {
public:
	LinearPerceptronRegression(double *inputs, int inputSize, int inputsSize, double *expectedOutputs, int outputSize) {
		assert(inputSize > 0);
		assert(outputSize > 0);
		// Create the model
		assert(inputs != NULL);
		assert(inputSize > 0);
		assert(expectedOutputs != NULL);
		assert(outputSize > 0);
		inputs = addBiasToInputs(inputs, &inputsSize, &inputSize);
		// Build X and Y
		int nbInput = inputsSize / inputSize;
		Eigen::MatrixXd X(nbInput, inputSize);
		Eigen::MatrixXd Y(nbInput, outputSize);

		tabToMatrix(&X, inputs, nbInput, inputSize);
		tabToMatrix(&Y, expectedOutputs, nbInput, outputSize);
		// return the calculated result as a matrix
		model = new Eigen::MatrixXd(pinv(X) * Y);
	}
	~LinearPerceptronRegression() {
		if (model) { delete model; }
	}
	// Regression
	double* linearPredict(double* input);

private:
	Eigen::MatrixXd* model;
	int inputSize;
	int outputSize;
};
