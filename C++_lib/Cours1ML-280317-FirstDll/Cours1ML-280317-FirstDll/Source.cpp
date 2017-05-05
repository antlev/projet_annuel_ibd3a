// Ne mettre que des elements présents en C - pas de classes
// changer en x64 en haut !!!!!
//extern "C"
//{
	//prefixe necessaire sur Windows pour pouvoir utiliser fonction
//	__declspec(dllexport) int return42() {
//		return 92;
//	}
//} 



#include "Header.h"
#include <iostream>
#include <stdlib.h>

// Génère un modèle aléatoirement en "settant" tous les poids
// à une valeur pseudo-aléatoire entre -1 et 1
double *linear_create_model(int inputDimension) {
	// Création du modèle en mémoire
	double* ptr = (double *)std::malloc(sizeof(double) * inputDimension);
	int i;
	// On affecte les poids à une valeur entre -1 et 1
	for (i = 0; i<inputDimension; ++i) {
		ptr[i] = rand() % 1 + -1;
	}
	// Neurone de biais initialisé à 1
	ptr[inputDimension] = 1;
	return ptr;
};

// Supprime le model en mémoire
void linear_remove_model(double *model) {
	free(model);
}

// ---------- APPRENTSSAGE ----------

int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize, int iterationNumber, double step) {
	if (model == nullptr) {
		return -1;
	}
	else {
		if (iterationNumber == 0)
			throw std::invalid_argument("The maximum number of iterations cannot be 0.");

		const double learning_rate(1);// TODO TOSET
		const double threshold(100);// TODO TOSET
		unsigned int iterations(0);

		while (true) {
			if (iterations > iterationNumber)
				break;
			else
				iterations++;

			int error_count = 0;
			int i(0);
			int count(0);
			// For each data
			for (i = 0; i<inputsSize; i+inputSize) {
				double* unInput = (double*)malloc(sizeof(double)*inputSize);
				int j(0);
				// On récupère les données correspondant à l'input
				for (j = 0; j<inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}

				bool output = learn_classification_rosenblatt(model, outputs[count], unInput, inputSize, learning_rate, threshold);

				if (output != outputs[i])
					error_count++;
				count++;
			}
			// Si l'apprentisage est finit et que le prerceptron renvoie la bonne sortie pour chaque input
			if (error_count == 0)
				break;
		}
		return 0;
	}
}
// Modify the weights using one data
bool learn_classification_rosenblatt(double *model, bool expected_result, const double* inputs, int inputSize, int learning_rate, double threshold)
{
	// Get the result given by the Perceptron
	bool result = get_result(model, inputs, inputSize, threshold);

	// If the Perceptron doesn't not return a correct response -> We modify the weights to adapt
	// If the Perceptron return a correct response -> Nothing to do
	if (result != expected_result) {
		// Convert boolean to a number
		double error = (expected_result ? 1 : 0) - (result ? 1 : 0);
		// We adapt every weight of our Perceptron using the formula
		for (int i = 0; i < inputSize + 1; i++) {
			model[i] += learning_rate * error * inputs[i];
		}
	}
	return result;
}

// Return the Perceptron's response considering the inputs
bool get_result(double *model, const double* inputs, int inputSize, double threshold)
{
	double toto;
	int i(0);
	for (i = 0; i < inputSize; i++) {
		toto += model[i] * inputs[i];
	}
	toto += threshold;

	return (toto < 0 ? -1 : 1);

	//return dot_product(inputs, inputSize, model) > threshold;
}

double dot_product(const double* inputs, int inputSize, const double* model)
{
	return 0;
	//return std::inner_product(inputs[0], inputs[inputSize], model[0], 0);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------------

int linear_fit_regression(int *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize) {
	if (model == nullptr) {
		return -1;
	}
	else {
		return 0;
	}
}


// Apprentissage NON SUPERVISE
int linear_fit_classification_hebb(int *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step) {
	if (model == nullptr) {
		return -1;
	}
	else {
		return 0;
	}
}


//  ---------- APPLICATION ----------
double linear_classify(double *model, double *input, int inputSize) {
	return 0;
}

double linear_predict(double *model, double *input, int inputSize) {
	return 0;
}
