#pragma once
// changer en x64 en haut !!!!!
extern "C"
{



	//typedef void(__stdcall * DebugCallback) (const char * str);
	//DebugCallback gDebugCallback;

	//__declspec(dllexport) void RegisterDebugCallback(DebugCallback callback)
	//{
	//	if (callback)
	//	{
	//		gDebugCallback = callback;
	//	}
	//}

	// Functions called from C#
	//prefixe necessaire sur Windows pour pouvoir utiliser fonction
	__declspec(dllexport) int return42() { return 42; }

	__declspec(dllexport) double *linear_create_model(int inputDimension);

	__declspec(dllexport) void linear_remove_model(double *model);

	__declspec(dllexport) int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize);

	__declspec(dllexport) int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, double* outputs, int iterationNumber, double step);

	__declspec(dllexport) double learn_classification_rosenblatt(double *model, double expected_result, double* input, int inputSize, double learning_rate, double threshold);


	__declspec(dllexport) double linear_classify(double *model, const double* input, int inputSize);

	__declspec(dllexport) double linear_predict(double *model, const double *input, int inputSize);
}
// Function only used in C++
double learn_classification_rosenblatt(double *model, double expected_result, const double* inputs, int inputSize, double learning_rate, double threshold);
int learn_classification_hebb(double *model, double *unInput, double expected_output, int inputSize, double step);
double learn_regression(double *model, double expected_result, const double* input, int inputSize, double learning_rate, double threshold);
void showModel(double* model);
