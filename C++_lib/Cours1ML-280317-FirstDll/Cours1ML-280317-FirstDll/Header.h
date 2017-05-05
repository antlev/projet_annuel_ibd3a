#pragma once
// changer en x64 en haut !!!!!
extern "C"
{
	//prefixe necessaire sur Windows pour pouvoir utiliser fonction
	__declspec(dllexport) int return42() {
		return 42;
	}

	__declspec(dllexport) double *linear_create_model(int inputDimension);

	__declspec(dllexport) void linear_remove_model(double *model);


	__declspec(dllexport) int linear_fit_regression(int *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize);


	__declspec(dllexport) int linear_fit_classification_hebb(int *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step);



	__declspec(dllexport) int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize, int iterationNumber, double step);


	__declspec(dllexport) bool learn_classification_rosenblatt(double *model, bool expected_result, const double* inputs, int inputSize, int learning_rate, double threshold);

	__declspec(dllexport) bool get_result(double *model, const double* inputs, int inputSize, double threshold);


	__declspec(dllexport) double dot_product(const double* inputs, int inputSize, const double* model);


	__declspec(dllexport) double linear_classify(double *model, double *input, int inputSize);

	__declspec(dllexport) double linear_predict(double *model, double *input, int inputSize);
}