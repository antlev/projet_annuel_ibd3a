#pragma once
extern "C" {
	// Functions called from C#
	//prefixe necessaire sur Windows pour pouvoir utiliser fonction
	__declspec(dllexport) int return42() { return 42; }

	__declspec(dllexport) double *linear_create_model(int nbCouches, int inputDimension);

	__declspec(dllexport) void linear_remove_model(double *model);

	__declspec(dllexport) int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs);

	__declspec(dllexport) int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, double* outputs, int iterationNumber, double step);

	__declspec(dllexport) int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int iterationNumber, double step);

	__declspec(dllexport) double linear_classify(double *model, const double* input, int inputSize);

	__declspec(dllexport) double linear_predict(double *model, const double *input, int inputSize);

	__declspec(dllexport) double new_pmc_model(double* model, double* input, int inputSize, int nbHiddenLayers);

	__declspec(dllexport) double pmc_linear_classify(double* model, double* input, int inputSize);

	__declspec(dllexport) double pmc_fit_linear_classification(double* model, double* inputs, int inputsSize, int inputSize, double* outputs, int iterationNumber, double learningRate);

	__declspec(dllexport) double pmc_linear_predict(double* model, double* input, int inputSize);

	__declspec(dllexport) double pmc_fit_linear_regression(double* model, double* input, int inputSize, double learningRate);
}
// Function only used in C++
double learn_classification_rosenblatt(double *model, double* unInput, int inputSize, double expected_result, double step);
int learn_classification_hebb(double *model, double *unInput, int inputSize, double expected_output, double step);
double learn_regression(double *model, double expected_result, const double* input, int inputSize, double learning_rate, double threshold);
void showModel(double* model);


double **mlp_classification_feed_forward(double ***model, int *modelStruct, int modelStructSize, double *inputs, int inputsSize);
void mlp_classification_fit(double ***model, int *modelStruct, int modelStructSize, double *inputs, int inputsSize, int *desiredOutput, double learningRate);

void mlp_classification_back_propagate(double ***model, int *modelStruct, int modelStructSize, double *inputs, int inputsSize, double **outputs, int *desiredOutputs, double &learningRate);
double *add_bias_input(double *inputs, int inputsSize);
double activation(double &x);
void mlp_update_weight(double &weight, double &learningRate, double &output, double &error);