#include "Header.h"
#include <iostream>
#include <stdlib.h>
void baseTest(double* inputs, double* expected_outputs, int inputSize) {
	inputs = (double*)malloc(inputSize * sizeof(double) * 20);
	expected_outputs = (double*)malloc(sizeof(double) * 20);
	int i;
	for (i = 0; i<10; i++) {
		// x entre -1 et 1
		*inputs = rand() % 10000 / 5000. - 1.;
		inputs++;
		// z entre 0 et 1
		*inputs = rand() % 10000 / 10000.;
		inputs++;
	}
	for (i = 0; i<10; i++) {
		// x entre -1 et 1
		*inputs = rand() % 10000 / 5000. - 1.;
		inputs++;
		// z entre -1 et 0
		*inputs = rand() % 10000 / 10000. - 1.;
		inputs++;
	}
	for (i = 0; i<10; i++) {
		// y = 1
		*expected_outputs = 1;
		expected_outputs++;
	}
	for (i = 0; i<10; i++) {
		// y = -1
		*expected_outputs = -1;
		expected_outputs++;
	}
}
int main() {

	int inputSize = 2;
	double step(1);

	double* model = linear_create_model(2);

	showModel(model);

	double* inputs;
	double* expected_outputs;
	int i;


	baseTest(inputs, expected_outputs, inputSize);

	std::cout << "Learning hebb !" << std::endl;

	/*    for(i=0; i<20 ;i++){
	std::cout << "totorea" << std::endl;
	learn_classification_hebb(model, inputs, *expected_outputs, inputSize, step);
	inputs+=2;
	expected_outputs++;
	}*/
	linear_fit_classification_hebb(model, inputs, 40, inputSize, expected_outputs, 1000, 1);
	baseTest(inputs, expected_outputs, inputSize);

	for (i = 0; i<20; i++) {
		std::cout << "res :" << linear_classify(model, inputs, inputSize) << std::endl;
		inputs += 2;
	}

	showModel(model);
	free(inputs);
	free(expected_outputs);


	return 0;
}

void showModel(double* model) {
	std::cout << "- Model -" << std::endl;
	std::cout << "w1:" << model[0] << std::endl;
	std::cout << "w2:" << model[1] << std::endl;
}
int test(double* test, int testSize) {
	int i(0);
	for (i = 0; i<testSize; ++i) {
		std::cout << "test[" << i << "] = " << test[i] << std::endl;
	}
	return 1;
}

int toto() {
	std::cout << "toto!!!!!!!" << std::endl;
	return 1;
}

// G�n�re un mod�le al�atoirement en "settant" tous les poids
// � une valeur pseudo-al�atoire entre -1 et 1
double *linear_create_model(int inputDimension) {
	// Cr�ation du mod�le en m�moire
	double* ptr = (double *)std::malloc(sizeof(double) * inputDimension);
	int i;
	// On affecte les poids � une valeur entre -1 et 1
	for (i = 0; i<inputDimension; ++i) {
		ptr[i] = rand() % 10000 / 5000. - 1.;
	}
	// Neurone de biais initialis� � 1
	//  ptr[inputDimension] = 1;

	return ptr;
};

// Supprime le modèle en m�moire
void linear_remove_model(double *model) {
	free(model);
}

//  ---------- APPLICATION ----------
// Return the Perceptron's response considering the inputs
double linear_classify(double *model, const double* input, int inputSize) {
	double somme_poids(0);
	int i;
	for (i = 0; i < inputSize; i++) {
		somme_poids += model[i] * input[i];
	}
	return (somme_poids < 0 ? -1 : 1);
}

// ---------- APPRENTSSAGE ----------
// Applique la règle de rosenblatt sur un perceptron avec un tableau d'inputs
// Applique la règle de hebb sur un perceptron avec un tableau d'inputs
int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, double* outputs, int iterationNumber, double step) {
	if (model == nullptr) {
		return -1;
	}
	else {
		int iterations(0);
		double* unInput = (double*)malloc(sizeof(double)*inputSize);

		while (true) {

			if (iterations > iterationNumber)
				break;
			else
				iterations++;

			int i(0);
			int count(0);
			int error(0);
			int k(0);

			// For each data
			for (i = 0; i<inputsSize; i += inputSize) {
				double* unInput = (double*)malloc(sizeof(double)*inputSize);
				int j(0);
				// On r�cup�re les donn�es correspondant � l'input
				for (j = 0; j<inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}
				error += learn_classification_hebb(model, unInput, outputs[k], inputSize, step);
				k++;
			}
			if (error == 0) {
				break;
			}
			error = 0;
		}
		free(unInput);
		return 0;
	}
}
// Applique la règle de hebb sur un perceptron avec un input
int learn_classification_hebb(double *model, double *unInput, double expected_output, int inputSize, double step) {
	// We adapt every weight of our Perceptron using the formula

	double output = linear_classify(model, unInput, inputSize);
	std::cout << "classify of >" << unInput[0] << " " << unInput[1] << "< returned >" << output << "<" << std::endl;

	if (output != expected_output) {
		int i;
		for (i = 0; i < inputSize; i++) {
			// On modifie selon la règle de Hebb
			std::cout << "modifying weight " << model[i] << std::endl;
			model[i] += step * expected_output * unInput[i];
			std::cout << "weight now " << model[i] << std::endl;

		}
		return 1;
	}
	else {
		return 0;
	}
}
int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize, int iterationNumber, double step) {
	if (model == nullptr) {
		return -1;
	}
	else {
		double learning_rate(1);// TODO TOSET
		double threshold(100);// TODO TOSET
		int iterations(0);

		while (true) {
			if (iterations > iterationNumber)
				break;
			else
				iterations++;

			int error_count(0);
			int i(0);
			int count(0);
			// For each data
			for (i = 0; i<inputsSize; i += inputSize) {
				double* unInput = (double*)malloc(sizeof(double)*inputSize);
				int j(0);
				// On r�cup�re les donn�es correspondant � l'input
				for (j = 0; j<inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}

				double output = learn_classification_rosenblatt(model, outputs[count], unInput, inputSize, learning_rate, threshold);

				if (output != outputs[i])
					error_count++;
				count++;
				free(unInput);
			}
			// Si l'apprentisage est fini et que le perceptron renvoie la bonne sortie pour chaque input
			if (error_count == 0)
				break;
		}
		return 0;
	}
}
// Applique la règle de rosenblatt sur un perceptron avec un input
double learn_classification_rosenblatt(double *model, double expected_result, double* input, int inputSize, double learning_rate, double threshold)
{
	// Get the result given by the Perceptron
	double result = linear_classify(model, input, inputSize);

	// If the Perceptron doesn't not return a correct response -> We modify the weights to adapt
	// If the Perceptron return a correct response -> Nothing to do
	if (result != expected_result) {
		// Convert boolean to a number
		double error = (expected_result - result);
		int i;
		// We adapt every weight of our Perceptron using the formula
		for (i = 0; i < inputSize + 1; i++) {
			model[i] += learning_rate * error * input[i];
		}
	}
	return result;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize) {
	if (model == nullptr) {
		return -1;
	}
	else {
		double learning_rate(1);// TODO TOSET
		double threshold(100);// TODO TOSET
		int iterations(0);

		while (true) {
			int error_count(0);
			int i(0);
			int count(0);
			// For each data
			for (i = 0; i < inputsSize; i += inputSize) {
				double *unInput = (double *)malloc(sizeof(double) * inputSize);
				int j(0);
				// On r�cup�re les donn�es correspondant � l'input
				for (j = 0; j < inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}

				double output = learn_regression(model, outputs[count], unInput, inputSize, learning_rate, threshold);

				if (output != outputs[i])
					error_count++;
				count++;
				free(unInput);
			}
			// Si l'apprentisage est fini et que le perceptron renvoie la bonne sortie pour chaque input
			if (error_count == 0)
				break;
		}
		return 0;
	}
}
double learn_regression(double *model, double expected_result, const double* input, int inputSize, double learning_rate, double threshold) {

	// TODO
	// Get the result given by the Perceptron
	double result = linear_predict(model, input, inputSize);

	// If the Perceptron doesn't not return a correct response -> We modify the weights to adapt
	// If the Perceptron return a correct response -> Nothing to do
	if (result != expected_result) {
		// Convert boolean to a number
		double error = (expected_result - result);
		int i;
		// We adapt every weight of our Perceptron using the formula
		for (i = 0; i < inputSize + 1; i++) {
			model[i] += learning_rate * error * input[i];
		}
	}
	return result;
}
double linear_predict(double *model, const double *input, int inputSize) {
	// TODO
	return 0;
}