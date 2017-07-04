// Dll-Machine-Learning.cpp : Defines the exported functions for the DLL application.
//
#include "stdafx.h"
#include "LinearPerceptron.h"
#include "MLP.h"
#include "RBF.h"

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
	__declspec(dllexport) LinearPerceptron* createLinearModel(int inputDimension, int outputDimension) { return new LinearPerceptron(inputDimension, outputDimension); }
	__declspec(dllexport) void eraseLinearModel(LinearPerceptron* pmodel) { delete pmodel; }
		// Classification
	__declspec(dllexport) int linearFitClassificationRosenblatt(LinearPerceptron* pmodel, double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize, int iterationMax, double step) {
		return pmodel->LinearPerceptron::linear_fit_classification_rosenblatt(inputs, inputsSize, inputSize, expectedOutputs, outputSize, iterationMax, step);
	}
	__declspec(dllexport) double* linearClassify(LinearPerceptron* pmodel, double* input, int inputSize, int outputDimension) {
		return pmodel->LinearPerceptron::linear_classify(input, inputSize, outputDimension);
	}
		// Regression
	__declspec(dllexport) void linearCreateAndFitRegression(LinearPerceptron* pmodel, double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize) {
		pmodel->LinearPerceptron::linear_create_and_fit_regression(inputs, inputsSize, inputSize, expectedOutputs, outputSize);
	}
	__declspec(dllexport) double* linearPredict(LinearPerceptron* pmodel, double* input, int inputSize, double* output, int outputSize) {
		return pmodel->LinearPerceptron::linearPredict(input, inputSize, outputSize);
	}
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
	// NAIVE RBF
	__declspec(dllexport) NAIVE_RBF* createNaiveRbfModel(int nbExamples, double gamma, double* X, int inputSize, double* Y) { return new NAIVE_RBF(nbExamples, gamma, X, inputSize, Y); }
	__declspec(dllexport) void naiveRbfGetResponse(NAIVE_RBF* pNaiveRBF, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples) { pNaiveRBF->getRbfResponse(gamma, input, inputSize, output, X, nbExamples); }
	// RBF
	__declspec(dllexport) RBF* createRbfModel(int nbExamples, double gamma, double* X, int inputSize, double* Y, int nbRepresentatives) { return new RBF(nbExamples, gamma, X, inputSize, Y, nbRepresentatives); }
	__declspec(dllexport) void getRbfResponse(RBF* pRBF, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples) { pRBF->getRbfResponse(gamma, input, inputSize, output, X, nbExamples); }
	__declspec(dllexport) void lloydAlgorithm(RBF* pRBF, double* inputs, int inputSize, int nbData, int nbRepresentatives) { pRBF->lloydAlgorithm(inputs, inputSize, nbData, nbRepresentatives);  }
	__declspec(dllexport) void showRepresentative(RBF* pRBF, int inputSize) { pRBF->showRepresentative(inputSize); }
}


