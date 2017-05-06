#pragma once
// changer en x64 en haut !!!!!
extern "C"
{
//prefixe necessaire sur Windows pour pouvoir utiliser fonction
__declspec(dllexport) int return42() {
    return 42;
}

__declspec(dllexport) int test(double* test, int testSize);

__declspec(dllexport) int toto();

__declspec(dllexport) double *linear_create_model(int inputDimension);

__declspec(dllexport) void linear_remove_model(double *model);


__declspec(dllexport) int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize);


__declspec(dllexport) int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step);

int learn_classification_hebb(double *model, const double *unInput, int inputSize, double step);


__declspec(dllexport) int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize, int iterationNumber, double step);


__declspec(dllexport) double learn_classification_rosenblatt(double *model, double expected_result, const double* inputs, int inputSize, int learning_rate, double threshold);

__declspec(dllexport) double linear_classify(double *model, const double* input, int inputSize);

__declspec(dllexport) double linear_predict(double *model, double *input, int inputSize);

}