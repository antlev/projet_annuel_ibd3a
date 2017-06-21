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
	__declspec(dllexport) int linear_fit_classification_rosenblatt(LinearPerceptron* pmodel, double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize, int iterationMax, double step) {
		return pmodel->LinearPerceptron::linear_fit_classification_rosenblatt(inputs, inputsSize, inputSize, expectedOutputs, outputSize, iterationMax, step);
	}
	__declspec(dllexport) void linear_classify(LinearPerceptron* pmodel, double* input, int inputSize, double* output, int outputDimension) {
		pmodel->LinearPerceptron::linear_classify(input, inputSize, output, outputDimension);
	}
	// Regression
	__declspec(dllexport) void linear_create_and_fit_regression(LinearPerceptron* pmodel, double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize) {
		pmodel->LinearPerceptron::linear_create_and_fit_regression(inputs, inputsSize, inputSize, expectedOutputs, outputSize);
	}
	__declspec(dllexport) void linearPredict(LinearPerceptron* pmodel, double* input, int inputSize, double* output, int outputSize) {
		pmodel->LinearPerceptron::linearPredict(input, inputSize, output, outputSize);
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


	// RBF
	__declspec(dllexport) RBF* createRbfModel(int nbRepresentatives) {
		return new RBF(nbRepresentatives);
	}
	__declspec(dllexport) void naiveLearnModel(RBF* pRBF, int nbExamples, double gamma, double* X, int inputSize, double* Y) {
		pRBF->RBF::naiveLearnModel(nbExamples, gamma, X, inputSize, Y);
	}
	__declspec(dllexport) void rbfLearnModel(RBF* pRBF, int nbExamples, double gamma, double* X, int inputSize, Eigen::MatrixXd Y) {
		pRBF->RBF::learnRbfModel(nbExamples, gamma, X, Y);
	}
	__declspec(dllexport) void getRbfResponse(RBF* pRBF, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples){
		pRBF->RBF::getRbfResponse(gamma, input, inputSize, output, X, nbExamples);
	}


}


