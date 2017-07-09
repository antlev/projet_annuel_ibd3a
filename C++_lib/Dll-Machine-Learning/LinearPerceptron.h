#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"
Eigen::MatrixXd pinv(Eigen::MatrixXd X);
double* addBiasToInput(double *input, int inputSize);
double* addBiasToInputs(double *inputs, int nbData, int *inputSize);
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
	LinearPerceptronRegression(double *inputs, int inputSize, int nbData, double *expectedOutputs, int outputSize);
	~LinearPerceptronRegression();
	// Regression
	double* linearPredict(double* input);

private:
	Eigen::MatrixXd* model;
	int inputSize;
	int outputSize;
};
