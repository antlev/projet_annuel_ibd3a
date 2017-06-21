#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"
#include <iostream>
class RBF {
public:
	RBF(int nbRepresentatives) {
		assert(nbRepresentatives > 0);
		(*this).nbRepresentatives = nbRepresentatives;
	}
	void naiveLearnModel(int nbExamples, double gamma, double* X, int inputSize, double* Y);

	void learnRbfModel(int nbExamples, double gamma, double* X, double* Y);

	void getRbfResponse(double gamma, double* input, int inputSize, double* output, double* X, int nbExamples);

private:
	int nbRepresentatives;
	double* representatives;

	Eigen::MatrixXd* weights;

	void lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives);
	void showRepresentative(int inputSize);
};

double distance(double * A, double* B, int inputSize);

