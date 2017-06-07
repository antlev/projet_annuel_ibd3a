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
}
// Function only used in C++
//double learn_classification_rosenblatt(double *model, double* unInput, int inputSize, double expected_result, double step);
double* addBiasToInput(double *input, int inputSize);
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize);
Eigen::MatrixXd pinv(Eigen::MatrixXd X);
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols);
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols);
