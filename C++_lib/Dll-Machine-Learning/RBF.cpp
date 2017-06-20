#include "stdafx.h"
#include "RBF.h"
//
// Created by antoine on 14/06/2017.
//
#include <cstdlib>
//#include <tgmath.h>
#include <vector>
#include <cfloat>
#include <iostream>

class MatrixXd;

//double** initRbf(int inputSize, int nbExamples){
//    double** weights = new double*[inputSize];
//    for (int input = 0; input < inputSize; ++input) {
//        weights[input] = new double[nbExamples];
//        for (int example = 0; example < nbExamples; ++example) {
//            weights[input][example] = ((float) rand()) / ((float) RAND_MAX) * 2.0 - 1.0;
//        }
//    }
//    return weights;
//}
double distance(double * A, double* B, int inputSize) {
	double distance = 0;
	for (int i = 0; i<inputSize; i++) {
		distance += (B[i] - A[i])*(B[i] - A[i]);
	}
	return sqrt(distance);
}
double* lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives) {
	double* copyInputs = inputs;
	double* representative = new double[nbRepresentatives*inputSize];
	double* copyRepresentative = representative;
	int iterations = 0;

	std::vector< std::vector<int> > clusters(nbRepresentatives, std::vector<int>(0, 0));

	for (int i = 0; i<nbRepresentatives; i++) {
		for (int j = 0; j<inputSize; j++) {
			representative[i*inputSize + j] = (float)rand() / RAND_MAX * 2.0 - 1.0;
		}
	}
	while (iterations  > 10000) {
		// assocites representatives to clusters
		for (int dataNb = 0; dataNb<nbData; dataNb++) {
			double distanceToRepr;
			int reprNbToPutInCluster;

			double minDist = DBL_MAX;
			inputs = copyInputs + dataNb*inputSize;

			for (int representantNb = 0; representantNb<nbRepresentatives; representantNb++) {
				representative += inputSize;
				distanceToRepr = distance(inputs, representative, inputSize);
				if (distanceToRepr < minDist) {
					minDist = distanceToRepr;
					reprNbToPutInCluster = representantNb;
				}
			}
			clusters[reprNbToPutInCluster].push_back(dataNb);
			iterations++;
		}

		// elect n representatives
		for (int clusterNb=0; clusterNb<nbRepresentatives; clusterNb++) {
			double* sumOfX = new double[inputSize];
			for (int i = 0; i<inputSize; i++) {
				sumOfX[i] = 0;
			}
			for (int i = 0; i<clusters[clusterNb].size(); i++) {
				for (int i = 0; i<inputSize; i++) {
					sumOfX[i] += clusters[clusterNb][i];
				}
			}
			for (int i = 0; i<inputSize; i++) {
				representative[clusterNb*inputSize + i] = sumOfX[i] / clusters[clusterNb].size();
			}
		}
	}
	return representative; // Return a pointer on all kernels
}

//void classify(double* input, int inputSize, int* outputs, int outputSize, int gamma, double** weights, int nbNeurons){
//    double calc=0;
//    for(int i=0; i<outputSize;i++){
//        for (int j = 1; j < nbNeurons; ++j) {
//            calc += weights[j][?]* exp(-gamma*(X-X[nbNeurons])*(X-X[nbNeurons]));
//        }
//        outputs[i] = (calc < 0) ? -1 : 1;
//        calc=0;
//    }
//}
//void predict(double* input, int inputSize, int* outputs, int outputSize, int gamma, double** weights, int nbNeurons){
//    double calc=0;
//    for(int i=0; i<outputSize;i++){
//        for (int j = 1; j < nbNeurons; ++j) {
//            calc += weights[j][?]* exp(-gamma*(X-X[nbNeurons])*(X-X[nbNeurons]));
//        }
//        outputs[i] = calc;
//        calc=0;
//    }
//}
Eigen::MatrixXd learnModel(int nbExamples, double gamma, double* X, Eigen::MatrixXd Y) {
	Eigen::MatrixXd teta(nbExamples, nbExamples);
	for (int i = 0; i < nbExamples; ++i) {
		for (int j = 0; j < nbExamples; ++j) {
			teta(i, j) = exp(-gamma*(X[i] - X[j])*(X[i] - X[j]));
		}
	}

	return teta.inverse()*Y;
}
Eigen::MatrixXd naiveLearnWeights(int nbExamples, double gamma, double* X, int inputSize, double* Y) {
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
				distance += (X[j*inputSize + k] - X[i*inputSize + k])*(X[j*inputSize + k] - X[i*inputSize + k]);
			}
			teta(i, j) = exp(-gamma*distance);
		}
	}
	return teta.inverse()*YMatrix;
}
void getRBFResponse(Eigen::MatrixXd weights, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples) {
	double sum = 0;
	double* oneX = new double[inputSize];
	for (int i = 0; i<nbExamples; i++) {
		for (int j = 0; j<inputSize; j++) {
			oneX[j] = X[i*inputSize + j];
		}
		sum += weights(i, 0)*exp(-gamma*distance(input, oneX, inputSize)*distance(input, oneX, inputSize));
	}
	*output = (sum > 0) ? 1 : -1;
}
