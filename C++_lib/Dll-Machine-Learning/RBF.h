#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"
#include <iostream>
class NAIVE_RBF {
public:
	NAIVE_RBF(int nbExamples, double* gamma, double* X, int inputSize, double* Y);
	double getRbfResponseClassif(double* input);
	double getRbfResponseRegression(double* input);
private:
	Eigen::MatrixXd naiveWeights;
	double* gamma;
	int inputSize;
	int nbExamples;
	double* X;

	double getRbfResponse(double* input);
};
class RBF {
public:
	RBF(int nbExamples, double* gamma, double* X, int inputSize, double* Y, int nbRepresentatives);
	double getRbfResponseClassif(double* input);
	double getRbfResponseRegression(double* input);

	void lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives);
	void showRepresentative();
private:
	int nbRepresentatives;
	double* gamma;
	double* X;
	int inputSize;
	double* representatives;
	Eigen::MatrixXd expectedResults;
	Eigen::MatrixXd weights;

	double getRbfResponse(double* input);
};
Eigen::MatrixXd pinv2(Eigen::MatrixXd X);
double distance(double * A, double* B, int inputSize);

