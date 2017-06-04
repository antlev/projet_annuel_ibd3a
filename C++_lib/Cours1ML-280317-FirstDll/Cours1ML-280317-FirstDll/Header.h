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

	__declspec(dllexport) int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int nb_iterations_max, double learning_rate);

	__declspec(dllexport) int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step);

	__declspec(dllexport) int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputSize, int iterationNumber, double step);

	__declspec(dllexport) void linear_classify(double *model, double* input, int inputSize, double* outputs, int outputDimension);

	__declspec(dllexport) void linearPredict(Eigen::MatrixXd model, double* input, int inputSize, double* output, int outputDimension);
}
// Function only used in C++
//double learn_classification_rosenblatt(double *model, double* unInput, int inputSize, double expected_result, double step);
int learn_classification_hebb(double *model, double *unInput, int inputSize, double step);
void showModel(double* model, int modelSize);
void showInputs(double* inputs, int inputSize);
double* addBiasToInput(double *input, int inputSize);
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize);
Eigen::MatrixXd pinv(Eigen::MatrixXd X);
void matrixToOutput(Eigen::MatrixXd outputMatrix, double* outputs, int nbData, int outputSize);
void inputTabToMatrix(Eigen::MatrixXd* inputsMatrix, double* inputs, int inputsSize, int inputSize);
