#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"
#include <iostream>
class NAIVE_RBF {
public:
	NAIVE_RBF(int nbExamples, double gamma, double* X, int inputSize, double* Y);
	double getRbfResponseClassif(double* input);
	double getRbfResponseRegression(double* input);
private:
	Eigen::MatrixXd naiveWeights;
	double gamma;
	int inputSize;
	int nbExamples;
	double* X;

	double getRbfResponse(double* input);
};
class RBF {
public:
	RBF(int nbExamples, double gamma, double* X, int inputSize, double* Y, int nbRepresentatives);
	void getRbfResponse(double gamma, double* input, int inputSize, double* output, double* X, int nbExamples);
	void lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives);
	void showRepresentative(int inputSize);
private:
	int nbRepresentatives;
	double* representatives;

	Eigen::MatrixXd expectedResults;
	Eigen::MatrixXd weights;
};
Eigen::MatrixXd pinv2(Eigen::MatrixXd X);
double distance(double * A, double* B, int inputSize);

