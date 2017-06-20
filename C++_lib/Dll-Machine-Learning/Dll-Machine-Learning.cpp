// Dll-Machine-Learning.cpp : Defines the exported functions for the DLL application.
//

#include "stdafx.h"
#include "MLP.h"

extern "C" {

class Toto {
private:
	int titi;
public:
	Toto() { titi = 42; }

	int getTiti() {
		return titi;
	}
};

	__declspec(dllexport) int return42() { return 42; } 

	__declspec(dllexport) Toto* createToto() { return new Toto() ; }

	__declspec(dllexport) int getTiti(Toto* pToto) { return pToto->getTiti(); }

	// WRAPPER FUNCTIONS
	// LinearPerceptron

	// MLP
	__declspec(dllexport) MLP* createMlp(int *structure, int nbLayer) { return new MLP(structure, nbLayer); }
	__declspec(dllexport) void classify(MLP* pMLP, double *oneInput, int inputSize) { pMLP->MLP::classify(oneInput, inputSize); }
	__declspec(dllexport) void fitClassification(MLP* pMLP, double *inputs, int inputSize, int inputsSize, double *expectedOutputs, int outputSize) {
		pMLP->MLP::fitClassification(inputs, inputSize, inputsSize, expectedOutputs, outputSize); 
	}
	__declspec(dllexport) double getOutputsforClassif(MLP* pMLP) { return pMLP->getOutputsforClassif(); }
	__declspec(dllexport) void fitRegression(MLP* pMLP, double *inputs, int inputSize, int inputsSize, double *expectedOutputs, int outputSize) {
		pMLP->MLP::fitRegression(inputs, inputSize, inputsSize, expectedOutputs, outputSize);
	}
	__declspec(dllexport) void predict(MLP* pMLP, double* oneInput, int inputSize) { pMLP->MLP::predict(oneInput, inputSize); }
	__declspec(dllexport) void eraseMlp(MLP* pMLP) { delete pMLP; }
	__declspec(dllexport) double getOutputsforRegression(MLP* pMLP) { return pMLP->getOutputsforRegression(); }


	// RBF
	//__declspec(dllexport) RBF* createRBF(int *structure, int nbLayer) { return new MLP(structure, nbLayer); }


	//double* lloydAlgorithm(double* inputs, int inputSize, int nbData, int nbRepresentatives);
	//Eigen::MatrixXd learnModel(int nbExamples, double gamma, double* X, Eigen::MatrixXd Y);
	//Eigen::MatrixXd naiveLearnWeights(int nbExamples, double gamma, double* X, int inputSize, double* Y);
	//void getRBFResponse(Eigen::MatrixXd weights, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples);
}


