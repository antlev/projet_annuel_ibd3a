#include "Header.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>

void baseTest(double* inputs, double* expected_outputs, int inputSize) {

	int i;
	for (i = 0; i<20; i += 2) {
		// x entre -1 et 1
		inputs[i] = rand() % 10000 / 5000. - 1.;
		/*
		inputs++;
		*/
		// z entre 0 et 1
		inputs[i + 1] = rand() % 10000 / 10000.;
		/*
		inputs++;
		*/
	}
	for (i = 20; i<40; i += 2) {
		// x entre -1 et 1
		inputs[i] = rand() % 10000 / 5000. - 1.;
		// z entre -1 et 0
		inputs[i + 1] = rand() % 10000 / 10000. - 1.;
	}
	for (i = 0; i<10; i++) {
		// y = 1
		expected_outputs[i] = 1;
	}
	for (i = 10; i<20; i++) {
		// y = -1
		expected_outputs[i] = -1;
	}
}

void showInputs(double* inputs, int inputsSize) {
	for (int i = 0; i<inputsSize; ++i) {
		std::cout << "DEBUG inputs[" << i << "] >" << inputs[i] << "< " << std::endl;
	}
}
int main() {

	time_t now;
	time(&now);
	srand((unsigned int)now);

	int inputSize = 2;
	double step(1);


	double* model = linear_create_model(inputSize);

	showModel(model);

	double* inputs = new double[inputSize * 20];
	double* expected_outputs = new double[20];

	int i;

	// BASE APPRENTISSAGE
	baseTest(inputs, expected_outputs, inputSize);
	/*
	showInputs(inputs, inputSize*20);
	*/

	std::cout << "Learning hebb !" << std::endl;

	/*    for(i=0; i<20 ;i++){
	std::cout << "totorea" << std::endl;
	learn_classification_hebb(model, inputs, *expected_outputs, inputSize, step);
	inputs+=2;
	expected_outputs++;
	}*/
	linear_fit_classification_hebb(model, inputs, 40, inputSize, 200, 1);

	// BASE TEST ICI ON S EN BAT LES COUILLES DE L'OUTPUT
	baseTest(inputs, expected_outputs, inputSize);
	int k(0);
	double* unInput = new double[inputSize];

	// For each data
	for (i = 0; i<40; i += inputSize) {
		int j(0);
		// On r�cup�re les donn�es correspondant � l'input
		for (j = 0; j<inputSize; ++j) {
			unInput[j] = inputs[i + j];
		}
		std::cout << "res :" << linear_classify(model, unInput, inputSize) << "Expected : " << expected_outputs[k] << std::endl;

		k++;
	}

	showModel(model);

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
	double* ptr = new double[inputDimension];
	// On affecte les poids � une valeur entre -1 et 1
	for (int i = 0; i<inputDimension; ++i) {
		ptr[i] = rand() % 10000 / 5000. - 1.;
	}
	return ptr;
};

// Supprime le modèle en m�moire
void linear_remove_model(double *model) {
}

//  ---------- APPLICATION ----------
// Return the Perceptron's response considering the inputs
double linear_classify(double *model, const double* input, int inputSize) {
	double somme_poids_inputs(0);
	int i;
	for (i = 0; i < inputSize; i++) {
		somme_poids_inputs += model[i] * input[i];
	}
	somme_poids_inputs += 1; // neuronne de biais
	return (somme_poids_inputs < 0 ? -1 : 1);
}

// ---------- APPRENTSSAGE ----------
// Applique la règle de rosenblatt sur un perceptron avec un tableau d'inputs
// Applique la règle de hebb sur un perceptron avec un tableau d'inputs
// Retourne -1 si le model n'existe pas
// Retourne 0 si l'erreur est à 0
// Retourne 1 si le nombre d'iterations à stopper l'apprentissage
int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step) {
	if (model == nullptr) {
		return -1;
	}
	else {
		int iterations(0);
		double* unInput = new double[inputSize];

		while (true) {

			if (iterations > iterationNumber) {
				return 1;
			} else
				iterations++;

			int i(0);
			// For each data
			for (i = 0; i<inputsSize; i += inputSize) {
				int j(0);
				// On r�cup�re les donn�es correspondant � l'input
				for (j = 0; j<inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}
				learn_classification_hebb(model, unInput, inputSize, step);
			}
		}

	}
}
// Applique la règle de hebb sur un perceptron avec un input
int learn_classification_hebb(double *model, double *unInput, int inputSize, double step) {
	int i;
	for (i = 0; i < inputSize; i++) {
		// On modifie selon la règle de Hebb
		model[i] += step * unInput[i];
	}
	return 0;
}
int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int iterationNumber, double step) {
	if (model == nullptr) {
		return -1;
	}
	else {
		int iterations(0);
		double* unInput = new double[inputSize];

		while (true) {

			if (iterations > iterationNumber) {
				return 1;
			}
			else
				iterations++;

			int i(0);
			int error(0);
			int k(0);
			// For each data
			for (i = 0; i<inputsSize; i += inputSize) {
				int j(0);
				// On r�cup�re les donn�es correspondant � l'input
				for (j = 0; j<inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}
				error += (int) learn_classification_rosenblatt(model, unInput, inputSize, outputs[k], step);
				k++;
			}
			if (error == 0) {
				return 0;
			}
			error = 0;
		}
	}
}
// Applique la règle de rosenblatt sur un perceptron avec un input
double learn_classification_rosenblatt(double *model, double* unInput, int inputSize, double expected_result, double step){
	// Get the result given by the Perceptron
	double result = linear_classify(model, unInput, inputSize);

	// If the Perceptron doesn't not return a correct response -> We modify the weights to adapt
	// If the Perceptron return a correct response -> Nothing to do
	if (result != expected_result) {
		// Convert boolean to a number
		double error = (expected_result - result);
		int i;
		// We adapt every weight of our Perceptron using the formula
		for (i = 0; i < inputSize + 1; i++) {
			model[i] += step * error * unInput[i];
		}
	}
	return result;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------

int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs,int nb_iterations_max, double learning_rate){
	if (model == nullptr) {
		return -1;
	}
	else {
		int iterations(0);

		while (true) {
			int error_count(0);
			int i(0);
			int count(0);
			// For each data
			for (i = 0; i < inputsSize; i += inputSize) {
				double *unInput = new double[inputSize];
				int j(0);
				// On r�cup�re les donn�es correspondant � l'input
				for (j = 0; j < inputSize; ++j) {
					unInput[j] = inputs[i + j];
				}

				error_count += learn_regression(model, outputs[count], unInput, inputSize, learning_rate);
				count++;
			}
			// Si l'apprentisage est fini et que le perceptron renvoie la bonne sortie pour chaque input
			if (error_count == 0)
				break;
			if (iterations > nb_iterations_max) {
				break;
			}
			iterations++;
		}
		return 0;
	}
}
double learn_regression(double *model, double expected_result, double* input, int inputSize, double learning_rate) {

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
		return 1;
	}
	else {
		return 0;
	}
}
double linear_predict(double *model, const double *input, int inputSize) {
	double somme_poids_inputs(0);
	int i;
	for (i = 0; i < inputSize; i++) {
		somme_poids_inputs += model[i] * input[i];
	}
	somme_poids_inputs += 1; // neuronne de biais
	return somme_poids_inputs;
}