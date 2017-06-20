#pragma once
//
// Created by antoine on 14/06/2017.
//
#include "Eigen/Dense"
#include <iostream>
class RBF {
public:
	double* lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives);
	Eigen::MatrixXd learnModel(int nbExamples, double gamma, double* X, Eigen::MatrixXd Y);
	Eigen::MatrixXd naiveLearnWeights(int nbExamples, double gamma, double* X, int inputSize, double* Y);
	void getRBFResponse(Eigen::MatrixXd weights, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples);
	int nbRepresentatives;
private:
	double* representatives;

	double distance(double * A, double* B, int inputSize);
	void showRepresentative(int inputSize) {
		for (int i = 0; i<RBF::nbRepresentatives; i += inputSize) {
			std::cout << "Representant " << i << " = (" << representatives[i] << ";" << representatives[i + 1] << ")" << std::endl;
		}
	}

};