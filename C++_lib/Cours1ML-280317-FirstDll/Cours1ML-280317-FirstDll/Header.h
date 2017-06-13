#pragma once

#include "Eigen/Dense"

extern "C" {
	// Functions called from C#
	//prefixe necessaire sur Windows pour pouvoir utiliser fonction
	__declspec(dllexport) int return42() { return 42; }

	__declspec(dllexport) int testSum(int* tab, int tabLength) {
		auto sum = 0;
		for (auto i = 0; i < tabLength; ++i) {
			sum += tab[i];
		}
		return sum;
	}

	__declspec(dllexport) double *linear_create_model(int inputDimension, int outputDimension);

	__declspec(dllexport) void linear_remove_model(double *model);

	__declspec(dllexport) Eigen::MatrixXd* linear_fit_regression(double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize);

	__declspec(dllexport) int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputSize, int iterationNumber, double step);

	__declspec(dllexport) void linear_classify(double *model, double* input, int inputSize, double* outputs, int outputDimension);

	__declspec(dllexport) void linearPredict(Eigen::MatrixXd* model, double* input, int inputSize, double* output, int outputSize);

	__declspec(dllexport)  void pmcFit(double**** modelWeights, double*** modelNeurons, double*** modelError, int* modelStruct, int nbLayer, double* inputs, int inputSize, int inputsSize, double* expectedOutputs, int outputSize, double learningRate, int maxIteraions, int option);

	__declspec(dllexport) void pmcCreateModel(int *modelStruct, int nbLayer, double ****modelWeights, double ***modelNeurons, double ***modelError);



}
// Function only used in C++
//double learn_classification_rosenblatt(double *model, double* unInput, int inputSize, double expected_result, double step);
double* addBiasToInput(double *input, int inputSize);
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize);
Eigen::MatrixXd pinv(Eigen::MatrixXd X);
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols);
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols);
void pmcFitOneInput(double**** modelWeights, double** modelNeurons, double*** modelError, int* modelStruct, int nbLayer, double* oneInput, int inputSize, double* oneOutput, int outputSize, double learningRate, int option);
double sum(double*** modelWeights, double** modelNeurons, int* modelStruct, int layerNb, int neuronNb);
void pmcClassifyOneInput(double*** modelWeights, double*** modelNeurons, int* modelStruct, int nbLayer, double* oneInput, int inputSize, double** oneOutput, int outputSize);
void pmcPredictOneInput(double*** modelWeights, double*** modelNeurons, int* modelStruct, int nbLayer, double* oneInput, int inputSize, double** oneOutput, int outputSize);