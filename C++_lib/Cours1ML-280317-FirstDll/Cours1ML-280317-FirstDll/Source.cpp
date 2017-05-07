#include "Header.h"
#include <iostream>
#include <stdlib.h>
int test(double* test, int testSize){
    int i(0);
    for(i=0;i<testSize;++i){
        std::cout << "test[" << i << "] = " << test[i] <<std::endl;
    }
    return 0;
}

int toto(){
    std::cout << "toto!!!!!!!"<<std::endl;
    return 0;
}

// G�n�re un mod�le al�atoirement en "settant" tous les poids
// � une valeur pseudo-al�atoire entre -1 et 1
double *linear_create_model(int inputDimension) {
    // Cr�ation du mod�le en m�moire
    double* ptr = (double *)std::malloc(sizeof(double) * inputDimension);
    int i;
    // On affecte les poids � une valeur entre -1 et 1
    for (i = 0; i<inputDimension; ++i) {
        ptr[i] = rand() % 1 + -1;
    }
    // Neurone de biais initialis� � 1
  //  ptr[inputDimension] = 1;
    return ptr;
};

// Supprime le modèle en m�moire
void linear_remove_model(double *model) {
    free(model);
}

// ---------- APPRENTSSAGE ----------
// Applique la règle de rosenblatt sur un perceptron avec un tableau d'inputs
int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int outputsSize, int iterationNumber, double step) {
    if (model == nullptr) {
        return -1;
    }
    else {
        int learning_rate(1);// TODO TOSET
        int threshold(100);// TODO TOSET
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
            for (i = 0; i<inputsSize; i+=inputSize) {
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
double learn_classification_rosenblatt(double *model, double expected_result, const double* input, int inputSize, int learning_rate, double threshold)
{
    // Get the result given by the Perceptron
    double result = linear_classify(model, input, inputSize);

    // If the Perceptron doesn't not return a correct response -> We modify the weights to adapt
    // If the Perceptron return a correct response -> Nothing to do
    if (result != expected_result) {
        // Convert boolean to a number
        double error = (expected_result  - result ) ;
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
        int learning_rate(1);// TODO TOSET
        int threshold(100);// TODO TOSET
        unsigned int iterations(0);

        while (true) {
            int error_count(0);
            int i(0);
            int count(0);
            // For each data
            for (i = 0; i < inputsSize; i += inputSize) {
                double *unInput = (double *) malloc(sizeof(double) * inputSize);
                int j(0);
                // On r�cup�re les donn�es correspondant � l'input
                for (j = 0; j < inputSize; ++j) {
                    unInput[j] = inputs[i + j];
                }

                double output = learn_regression(model, outputs[count], unInput, inputSize,
                                                 learning_rate, threshold);

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

double learn_regression(double *model, double expected_result, const double* input, int inputSize, int learning_rate, double threshold)
{
    // TODO
    // Get the result given by the Perceptron
    double result = linear_predict(model, input, inputSize);

    // If the Perceptron doesn't not return a correct response -> We modify the weights to adapt
    // If the Perceptron return a correct response -> Nothing to do
    if (result != expected_result) {
        // Convert boolean to a number
        double error = (expected_result  - result ) ;
        int i;
        // We adapt every weight of our Perceptron using the formula
        for (i = 0; i < inputSize + 1; i++) {
            model[i] += learning_rate * error * input[i];
        }
    }
    return result;
}

// Applique la règle de hebb sur un perceptron avec un tableau d'inputs
int linear_fit_classification_hebb(double *model, double *inputs, int inputsSize, int inputSize, int iterationNumber, double step, double *outputs, int outputSize) {
    if (model == nullptr) {
        return -1;
    }
    else {

        int iterations(0);
		double* unInput = (double*)malloc(sizeof(double)*inputSize);

        while (true) {
            if (iterations > iterationNumber)
                break;
 //           else
                //iterations++;


            int i(0);
            int count(0);
			int error(0);
            // For each data
            for (i = 0; i<inputsSize; i+=inputSize) {
                int j(0);
                // On r�cup�re les donn�es correspondant � l'input
                for (j = 0; j<inputSize; ++j) {
                    unInput[j] = inputs[i + j];
                }
				error += learn_classification_hebb(model, unInput, inputSize, step, outputs[i]);
			}
			if (error == 0) {
				break;
			}
		}
		free(unInput);
        return 0;
    }
}
// Applique la règle de hebb sur un perceptron avec un input
int learn_classification_hebb(double *model, const double *unInput, int inputSize, double step, double expected_output){
    // We adapt every weight of our Perceptron using the formula
    double output= linear_classify(model, unInput, inputSize);
	if (output != expected_output) {
		int i;
		for (i = 0; i < inputSize; i++) {
			// On modifie selon la règle de Hebb
			model[i] += step * output * unInput[i];
		}
		return 1;
	}
	else {
		return 0;
	}
}

//  ---------- APPLICATION ----------
// Return the Perceptron's response considering the inputs
double linear_classify(double *model, const double* input, int inputSize)
{
    double somme_poids(0);
    int i(0);
    for (i = 0; i < inputSize; i++) {
		somme_poids += model[i] * input[i];
    }
	//somme_poids += 1;

    return (somme_poids < 0 ? -1 : 1);
}

double linear_predict(double *model, const double *input, int inputSize) {
    // TODO
    return 0;
}
