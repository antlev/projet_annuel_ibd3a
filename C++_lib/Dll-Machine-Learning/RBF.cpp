#include "stdafx.h"
#include "RBF.h"
//
// Created by antoine on 14/06/2017.
//
#include <cstdlib>
#include <vector>
#include <cfloat>
#include <iostream>

class MatrixXd;

// Implementation of a native RBF that consider all inputs given
NAIVE_RBF::NAIVE_RBF(int nbExamples, double gamma, double* X, int inputSize, double* Y) {
	assert(nbExamples > 0);
	assert(gamma > 0);
	assert(inputSize > 0);
	assert(X != nullptr);
	assert(Y != nullptr);
	this->nbExamples = nbExamples;
	this->gamma = gamma;
	this->X = X;
	this->inputSize = inputSize;

	double distance;
	Eigen::MatrixXd teta(nbExamples, nbExamples);
	Eigen::MatrixXd YMatrix(nbExamples, 1);
	for (int i = 0; i<nbExamples; i++) {
		YMatrix(i, 0) = Y[i];
	}
	for (int i = 0; i < nbExamples; ++i) {
		for (int j = 0; j < nbExamples; ++j) {
			distance = 0;
			for (int k = 0; k<inputSize; k++) {
				distance += (X[j*inputSize + k] - X[i*inputSize + k]) * (X[j*inputSize + k] - X[i*inputSize + k]);
			}
			teta(i, j) = exp(-gamma*distance);
		}
	}
	naiveWeights = teta.inverse()*YMatrix;
}
double NAIVE_RBF::getRbfResponseClassif(double* input) {
	return (getRbfResponse(input) > 0) ? 1 : -1;
}
double NAIVE_RBF::getRbfResponseRegression(double* input) {
	return getRbfResponse(input);
}
double NAIVE_RBF::getRbfResponse(double* input) {
	assert(input != nullptr);
	double sum = 0;
	double* oneX = new double[inputSize];
	for (int i = 0; i<nbExamples; i++) {
		for (int j = 0; j<inputSize; j++) {
			oneX[j] = X[i*inputSize + j];
		}
		sum += (naiveWeights)(i, 0)*exp(-gamma*distance(input, oneX, inputSize));
	}
	return sum;
}
// Implementation of RBF that uses Lloyd algorithm to select some representatives
RBF::RBF(int nbExamples, double gamma, double* X, int inputSize, double* Y, int nbRepresentatives) {
	assert(nbExamples > 0);
	assert(gamma > 0);
	assert(X != nullptr);
	assert(inputSize > 0);
	assert(Y != nullptr);
	assert(nbRepresentatives > 0);
	this->gamma = gamma;
	this->inputSize = inputSize;
	this->X = X;
	this->nbRepresentatives = nbRepresentatives;

	// Select representatives using LLoyd algorithm, representatives are set as object atribute
	lloydAlgorithm(X, inputSize, nbExamples, nbRepresentatives);

	double* XCopy;
	double* representativesCopy;
	Eigen::MatrixXd teta(nbExamples, nbRepresentatives);
	Eigen::MatrixXd YMatrix(nbExamples, 1);
	for (int i = 0; i<nbExamples; i++) {
		YMatrix(i, 0) = Y[i];
	}
	for (int i = 0; i < nbExamples; ++i) {
		XCopy = X + i*inputSize;
		for (int j = 0; j < nbRepresentatives; ++j) {
			representativesCopy = representatives + j*inputSize;
			teta(i, j) = exp(-gamma*distance(XCopy, representativesCopy, inputSize));
		}
	}
	weights = pinv2(teta)*YMatrix;
}
double RBF::getRbfResponseClassif(double* input) {
	return (getRbfResponse(input) > 0) ? 1 : -1;
}
double RBF::getRbfResponseRegression(double* input) {
	return getRbfResponse(input);
}

double RBF::getRbfResponse(double* input) {
	assert(nbRepresentatives > 0);
	assert(input != nullptr);
	double sum = 0;
	double* oneX = new double[inputSize];
	for (int i = 0; i<nbRepresentatives; i++) {
		for (int j = 0; j<inputSize; j++) {
			oneX[j] = representatives[i*inputSize + j];
		}
		sum += (weights)(i, 0)*exp(-gamma*distance(input, oneX, inputSize));
	}
	return sum;
}
void RBF::lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives) {
	assert(nbData > 0);
	assert(nbRepresentatives > 0);
	assert(inputSize > 0);
	assert(inputs != nullptr);
	double* input = new double[inputSize];
	double* representatives = new double[nbRepresentatives*inputSize];
	double* copyRepresentatives = representatives;
	int iterations = 0;
	std::vector< std::vector<int> > clusters(nbRepresentatives, std::vector<int>(0, 0));
	for (int i = 0; i<nbRepresentatives; i++) {
		for (int j = 0; j<inputSize; j++) {
			representatives[i*inputSize + j] = (float)rand() / RAND_MAX;
		}
	}
	while (iterations  < 10000) {
		// associates representatives to clusters
		clusters.clear();
		for (int dataNb = 0; dataNb<nbData; dataNb++) {
			double distanceToRepr;
			int reprNbToPutInCluster;

			double minDist = DBL_MAX;
			input = inputs + dataNb*inputSize;
			representatives = copyRepresentatives;

			for (int representativeNb = 0; representativeNb<nbRepresentatives; representativeNb++) {
				distanceToRepr = distance(input, representatives, inputSize);
				if (distanceToRepr < minDist) {
					minDist = distanceToRepr;
					reprNbToPutInCluster = representativeNb;
				}
				representatives += inputSize;
			}
			clusters[reprNbToPutInCluster].push_back(dataNb);
		}
		representatives = copyRepresentatives;
		// elect n representatives
		for (int clusterNb = 0; clusterNb<nbRepresentatives; clusterNb++) {
			double* sumOfX = new double[inputSize];
			for (int i = 0; i<inputSize; i++) {
				sumOfX[i] = 0;
			}
			for (unsigned int i = 0; i<clusters[clusterNb].size(); i++) {
				for (int j = 0; j<inputSize; j++) {
					auto tmp = inputs[clusters[clusterNb][i] * inputSize + j];
					sumOfX[j] += tmp;
				}
			}
			for (int i = 0; i<inputSize; i++) {
				representatives[clusterNb*inputSize + i] = sumOfX[i] / (double)clusters[clusterNb].size();
			}
		}
		iterations++;
	}
	this->representatives = representatives;
}
void RBF::showRepresentative() {
	assert(inputSize > 0);
	for (int i = 0; i<RBF::nbRepresentatives*inputSize; i += inputSize) {
		std::cout << "Representant " << i / inputSize << " = (" << representatives[i] << ";" << representatives[i + 1] << ")" << std::endl;
	}
}
// Return the pseudo inverse of the matrix passed as a parameter
Eigen::MatrixXd pinv2(Eigen::MatrixXd X) {
	Eigen::MatrixXd X_Transp = X.transpose();
	return (((X_Transp * X).inverse()) * X_Transp);
}
double distance(double * A, double* B, int inputSize) {
	assert(A != nullptr);
	assert(B != nullptr);
	assert(inputSize > 0);
	double distance = 0;
	for (int i = 0; i<inputSize; i++) {
		distance += (B[i] - A[i])*(B[i] - A[i]);
	}
	return sqrt(distance);
}

void lloydAlgorithm2(double* inputs, int inputSize, int nbData, int nbRepresentatives) {
	assert(nbData > 0);
	assert(nbRepresentatives > 0);
	assert(inputSize > 0);
	assert(inputs != nullptr);
	double* input = new double[inputSize];
	double* representatives = new double[nbRepresentatives*inputSize];
	double* copyRepresentatives = representatives;
	int iterations = 0;
	std::vector< std::vector<int> > clusters(nbRepresentatives, std::vector<int>(0, 0));
	for (int i = 0; i<nbRepresentatives; i++) {
		for (int j = 0; j<inputSize; j++) {
			representatives[i*inputSize + j] = (float)rand() / RAND_MAX;
		}
	}
	while (iterations  < 10000) {
		// associates representatives to clusters
		clusters.clear();
		for (int dataNb = 0; dataNb<nbData; dataNb++) {
			double distanceToRepr;
			int reprNbToPutInCluster;

			double minDist = DBL_MAX;
			input = inputs + dataNb*inputSize;
			representatives = copyRepresentatives;

			for (int representativeNb = 0; representativeNb<nbRepresentatives; representativeNb++) {
				distanceToRepr = distance(input, representatives, inputSize);
				if (distanceToRepr < minDist) {
					minDist = distanceToRepr;
					reprNbToPutInCluster = representativeNb;
				}
				representatives += inputSize;
			}
			clusters[reprNbToPutInCluster].push_back(dataNb);
		}
		representatives = copyRepresentatives;
		// elect n representatives
		for (int clusterNb = 0; clusterNb<nbRepresentatives; clusterNb++) {
			double* sumOfX = new double[inputSize];
			for (int i = 0; i<inputSize; i++) {
				sumOfX[i] = 0;
			}
			for (unsigned int i = 0; i<clusters[clusterNb].size(); i++) {
				for (int j = 0; j<inputSize; j++) {
					auto tmp = inputs[clusters[clusterNb][i] * inputSize + j];
					sumOfX[j] += tmp;
				}
			}
			for (int i = 0; i<inputSize; i++) {
				representatives[clusterNb*inputSize + i] = sumOfX[i] / (double)clusters[clusterNb].size();
			}
		}
		iterations++;
	}
	std::cout << "TEST >>" << representatives[0] << " " << representatives[1] << std::endl;
	std::cout << "TEST >>" << representatives[2] << " " << representatives[3] << std::endl;
}