#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"

class LinearPerceptron {
public:
	LinearPerceptron(int inputDimension, int outputDimension){
		assert(inputDimension > 0);
		assert(outputDimension > 0);
		modelLearned = 1;
		// Create the model
		classifModel = new double[(inputDimension + 1) * outputDimension];
		for (int i = 0; i < (inputDimension + 1) * outputDimension; ++i) {
			classifModel[i] = ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
		}
	}
	~LinearPerceptron() {
		modelLearned = 0;
		if (classifModel) { delete classifModel; }
		if (modelLearned) { delete regressionModel; }
	}
	// Classification
	int linear_fit_classification_rosenblatt(double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize, int iterationMax, double step);
	double linear_classify(double* input, int inputSize, double* output, int outputDimension);
	// Regression
	void linear_create_and_fit_regression(double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize);
	void linearPredict(double* input, int inputSize, double* output, int outputSize);

private:
	double* classifModel;
	Eigen::MatrixXd* regressionModel;
	int modelLearned;
};
Eigen::MatrixXd pinv(Eigen::MatrixXd X);
double* addBiasToInput(double *input, int inputSize);
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize);
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols);
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols);