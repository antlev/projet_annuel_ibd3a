#pragma once
extern "C" {
	// Functions called from C#
	//prefixe necessaire sur Windows pour pouvoir utiliser fonction
	__declspec(dllexport) int return42() { return 42; }

	__declspec(dllexport) double *linear_create_model(int inputDimension);

	__declspec(dllexport) void linear_remove_model(double *model);

	__declspec(dllexport) int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs,int nb_iterations_max, double learning_rate);

	__declspec(dllexport) int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step);

	__declspec(dllexport) int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int iterationNumber, double step);

	__declspec(dllexport) double linear_classify(double *model, double* input, int inputSize);

	__declspec(dllexport) double linear_predict(double *model, double *input, int inputSize);
}
// Function only used in C++
double learn_classification_rosenblatt(double *model, double* unInput, int inputSize, double expected_result, double step);
int learn_classification_hebb(double *model, double *unInput, int inputSize, double step);
double learn_regression(double *model, double expected_result, double* input, int inputSize, double learning_rate);
void showModel(double* model);