#include "Header.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include "Eigen/Dense"

class MatrixXd;

class MatrixXd;

using namespace std;

// G�n�re un mod�le al�atoirement en "settant" tous les poids
// � une valeur pseudo-al�atoire entre -1 et 1
double *linear_create_model(int inputDimension, int outputDimension) {
	assert(inputDimension > 0);
	assert(outputDimension > 0);
	// Cr�ation du mod�le en m�moire
	// On a autant de poids que d'input (sans oublier le neurone de biais) fois le nombre d'output
	double* model = new double[(inputDimension + 1) * outputDimension];
	// On affecte les poids � une valeur entre -1 et 1
	for (int i = 0; i < (inputDimension + 1) * outputDimension; ++i) {
		model[i] = ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
	}
	return model;
};
// Supprime le modèle en m�moire
void linear_remove_model(double *model) {
	assert(model != NULL);
	delete model;
}
double* addBias(double* input, int inputSize) {
	assert(input != NULL);
	assert(inputSize > 0);
	double* newInput = new double[inputSize + 1];
	newInput[0] = 1;
	for (int i = 1; i<inputSize + 1; i++) {
		newInput[i] = input[i - 1];
	}
	return newInput;
}
//  ---------- APPLICATION ----------
// Return the Perceptron's response considering the inputs
// @param model : model used to classify
// @param input : input to use
// @param inputSize : size of input array
// @param outputs : array to write results of all the outputs neurons
// @param outputDimension : size of output array
void linear_classify(double *model, double* input, int inputSize, double* output, int outputDimension) {
	assert(model != NULL);
	assert(input != NULL);
	assert(inputSize > 0);
	assert(output != NULL);
	assert(outputDimension > 0);
	double sum_weigths_inputs;
	double* inputWithBias = addBias(input, inputSize);
	for (int outputIterator = 0; outputIterator < outputDimension; outputIterator++) {
		//        cout << "Debug outputIterator >" << outputIterator << "< " << endl;

		sum_weigths_inputs = 0;
		for (int modelIterator = outputIterator, inputIterator = 0; modelIterator < (inputSize + 1)*outputDimension && inputIterator < inputSize + 1; inputIterator++, modelIterator += outputDimension) {
			//            cout << "Debug modelIterator >" << modelIterator << "< inputIterator >" << inputIterator << "< " << endl;
			sum_weigths_inputs += model[modelIterator] * inputWithBias[inputIterator];
		}
		output[outputIterator] = (sum_weigths_inputs >= 0 ? 1 : -1);
	}
}
// ---------- APPRENTSSAGE ----------
// Function called from C# taking all inputs and ouputs to learn rosenblatt to a model
// @param model : model used to classify
// @param inputs :
// @param inputsSize :
// @param inputSize :
// @param expectedOutputs : array containing for each input all the output neuron expected response (ex : 2 inputs of 3 outputs : [i=1 o=1][i=1 o=2][i=1 o=2][i=2 o=1][i=2 o=2][i=2 o=2][i=2 o=1][i=3 o=2][i=3 o=3])
// @param outputSize : size oh output neurons
// @param iterationMax : number of max of iterations
// @param step : step
int linear_fit_classification_rosenblatt(double *model, double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize, int iterationMax, double step) {
	assert(model != NULL);
	assert(inputs != NULL);
	assert(inputSize > 0);
	assert(inputsSize >= inputSize);
	assert(inputSize > 0);
	assert(expectedOutputs != NULL);
	assert(outputSize > 0);
	assert(iterationMax > 0);
	assert(step > 0);
	int iterations(0);
	int error;
	while (true) {
		if (iterations > iterationMax) {
			return 1;
		}
		iterations++;

		error = 0;
		double* oneInput = new double[inputSize];
		double* oneOutput = new double[outputSize];
		int inputIterator;
		// For each data passed in parameter of the function
		for (int inputsIterator = 0, inputIterator = 0; inputsIterator < inputsSize; inputsIterator += inputSize, inputIterator++) {
			// recovering inputs one by one
			for (int i = 0; i<inputSize; i++) {
				oneInput[i] = inputs[inputsIterator + i];
			}
			// Get the result given by the Perceptron
			linear_classify(model, oneInput, inputSize, oneOutput, outputSize);
			cout << "TEST RETURN CLASSIFY " << endl;
			for (int i = 0; i<outputSize; i++) {
				cout << "output[" << i << "] >" << oneOutput[i] << "<" << endl;

			}
			oneInput = addBias(oneInput, inputSize);

			bool* outputIsGood = new bool[outputSize];
			double* outputError = new double[outputSize];
			bool allOutputsAreGood = true;
			// For each output neuron we check that response from perceptron is correct
			for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
				cout << "DEBUG outputIterator>" << outputIterator << "< " << endl;
				outputIsGood[outputIterator] = true; // Initialising all response at true
													 // If one of the output neuron doesn't give the good answer
				cout << "DEBUG oneOutput[outputIterator]>" << oneOutput[outputIterator] << "<  expectedOutputs[" << inputIterator*outputSize + outputIterator << "] >" << expectedOutputs[inputIterator*outputSize + outputIterator] << "< " << endl;
				if (oneOutput[outputIterator] != expectedOutputs[inputIterator*outputSize + outputIterator]) {
					cout << "DEBUG output nb " << outputIterator << " is false" << endl;
					outputIsGood[outputIterator] = false;
					if (allOutputsAreGood) { allOutputsAreGood = false; }
					// Storing the output error in a tab
					outputError[outputIterator] = expectedOutputs[inputIterator*outputSize + outputIterator] - oneOutput[outputIterator];
					cout << "DEBUG outputError[" << outputIterator << "] >" << outputError[outputIterator] << "< " << endl;
				}
				else {
					cout << "DEBUG output nb " << outputIterator << " is ok" << endl;
				}
			}
			// If at least one of the Perceptron's response is not good
			if (!allOutputsAreGood) {
				error++; // We increment the error of the current Perceptron on the data given
				for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
					// For each output, if the Perceptron's response ins't good -> We modify the weights affecting the output
					if (!outputIsGood[outputIterator]) {
						//                        cout << "DEBUG modifying weight affecting output nb >" << outputIterator << "< " << endl;
						// We adapt every weight of our Perceptron using the Rsoenblatt formula
						for (int modelIterator = outputIterator; modelIterator < (inputSize + 1)*outputSize; modelIterator += outputSize) {
							if (modelIterator < outputSize) {
								cout << "modelIterator >" << modelIterator << "< model[" << modelIterator << "]>" << model[modelIterator] << "<" << endl;
								model[modelIterator] += step * outputError[outputIterator] * 1; // bias neuron
							}
							else {
								cout << "modelIterator >" << modelIterator << "< model[" << modelIterator << "]>" << model[modelIterator] << "< outputIterator >" << outputIterator << "< outputError[outputIterator] >" << outputError[outputIterator] << "< modelIterator - outputIterator >" << modelIterator - outputIterator << "< oneInput[modelIterator - outputIterator] >" << oneInput[modelIterator - outputIterator] << "< " << endl;
								model[modelIterator] += step * outputError[outputIterator] * oneInput[modelIterator - outputIterator];
							}
						}
					}
					else {
						cout << "DEBUG NOT MODIFYING weight affecting output nb >" << outputIterator << "< " << endl;
					}
				}
			}
		}
		if (error == 0) {
			return 0;
		}
	}
}

Eigen::Matrix3f pinv(Eigen::Matrix3f X) {
	Eigen::Matrix3f X_Transp = X.transpose();
	return (((X_Transp * X).inverse()) * X_Transp);
}
int main() {

	time_t now;
	time(&now);
	srand((unsigned int)now);
	//    Eigen::Matrix3f A;
	//    A << 1, 2, 1,
	//            2, 1, 0,
	//            -1, 1, 2;
	//    cout << "Here is the matrix A:\n" << A << endl;
	//    cout << "The transposé of A is \n" << A.transpose() << endl;
	//    A.transpose();
	//
	//    cout << "Here is the matrix A:\n" << A << endl;
	//    cout << "Here is the pinv of A:\n" << pinv(A) << endl;
	//
	//    cout << "Expected Result : " << endl;
	//
	//    cout << "-0.6667   1.0000   0.3333\n1.3333  -1.0000  -0.6667\n-1.0000   1.0000   1.0000 " << endl;

	int inputSize = 2;
	int outputNeuronsSize = 3;
	int iterationsMax = 10000;
	double step(0.1);

	int modelSize = (inputSize + 1) * outputNeuronsSize;
	double* model = linear_create_model(inputSize, outputNeuronsSize);
	showModel(model, modelSize);

	double* inputs = new double[inputSize * 3];
	double* expected_outputs = new double[outputNeuronsSize * 3];

	inputs[0] = 0;
	inputs[1] = -0.5;
	expected_outputs[0] = -1;
	expected_outputs[1] = 1;
	expected_outputs[2] = -1;

	inputs[2] = 0.5;
	inputs[3] = 0.375;
	expected_outputs[3] = -1;
	expected_outputs[4] = -1;
	expected_outputs[5] = 1;

	inputs[4] = -0.625;
	inputs[5] = 0.25;
	expected_outputs[6] = 1;
	expected_outputs[7] = -1;
	expected_outputs[8] = -1;

	//    cout << "DEBUG Exepected_ouputs given to fit classification" << endl;
	//    for(int i=0; i<outputNeuronsSize*3; i++){
	//        cout << "DEBUG expected_ouputs[" << i << "] >" << expected_outputs[i] << "<" << endl;
	//    }
	linear_fit_classification_rosenblatt(model, inputs, inputSize * 3, inputSize, expected_outputs, outputNeuronsSize, iterationsMax, step);

	double* oneInput = new double[inputSize];
	double* oneOutput = new double[outputNeuronsSize];

	oneInput[0] = 0;
	oneInput[1] = -0.5;
	linear_classify(model, oneInput, inputSize, oneOutput, outputNeuronsSize);
	for (int i = 0; i< outputNeuronsSize; i++) {
		cout << "output[" << i << "] >" << oneOutput[i] << "<" << endl;
	}
	oneInput[0] = 0.5;
	oneInput[1] = 0.375;
	linear_classify(model, oneInput, inputSize, oneOutput, outputNeuronsSize);
	for (int i = 0; i< outputNeuronsSize; i++) {
		cout << "output[" << i << "] >" << oneOutput[i] << "<" << endl;
	}
	oneInput[0] = -0.625;
	oneInput[1] = 0.25;
	linear_classify(model, oneInput, inputSize, oneOutput, outputNeuronsSize);
	for (int i = 0; i< outputNeuronsSize; i++) {
		cout << "output[" << i << "] >" << oneOutput[i] << "<" << endl;
	}
	//// BASE APPRENTISSAGE
	//baseTest(inputs, expected_outputs, inputSize);
	///*
	//showInputs(inputs, inputSize*20);
	//*/



	showModel(model, modelSize);

	return 0;
}
//---------------------------------------------------------------------------------------------------------------------------------------------------------
// Function called by C# giving all inputs annd outputs of a database to learn regression
int linear_fit_regression(double *model, double *inputs, int inputsSize, int inputSize, double *outputs, int nb_iterations_max, double learning_rate) {
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
			if (error_count <= 0.0001)
				break;
			if (iterations > nb_iterations_max) {
				break;
			}
			iterations++;
		}
		return 0;
	}
}
// Learn a regression model using one set of input output
double learn_regression(double *model, double expected_result, double* input, int inputSize, double learning_rate) {

	// Get the result given by the Perceptron
	double result = linear_predict(model, input, inputSize);

	// Convert boolean to a number
	double error = (expected_result - result);
	int i;
	// We adapt every weight of our Perceptron using the formula
	for (i = 0; i < inputSize; i++) {
		model[i + 1] += learning_rate * error * input[i];
	}
	model[0] += learning_rate * error * 1.0;
	return error;
}
// Return the response of the Perceptron using regression
double linear_predict(double *model, double *input, int inputSize) {
	double somme_poids_inputs(0);
	int i;
	for (i = 0; i < inputSize; i++) {
		somme_poids_inputs += model[i + 1] * input[i];
	}
	somme_poids_inputs += model[0]; // neuronne de biais
	return somme_poids_inputs;
}
//////////////// PERCEPTRON MULTI-COUCHES ////////////////////////
//double*** pmc_create_model(double* modelStruct, int modelStructSize, int inputSize){
//    double*** model = new double**[modelStructSize];
//    for(int i=0; i<modelStructSize; i++){
//        model[i] = new double*[modelStruct[i]];
//        for(int j=0;j<modelStruct[i];j++) {
//            int previousLayerSize = i == 0 ? inputSize : modelStruct[i - 1];
//            model[i][j] = new double[previousLayerSize];
//            for (int k = 0; k < previousLayerSize; ++k) {
//                model[i][j][k] = ((float) rand()) / ((float) RAND_MAX) * 2.0 - 1.0;
//            }
//        }
//    }
//    return  model;
//}
void pmc_remove_model(double*** model, double* modelStruct, int modelStructSize, int inputSize) {
	for (int i = 0; i<modelStructSize; i++) {
		for (int j = 0; j<modelStruct[i]; j++) {
			delete(model[i][j]);
		}
		delete(model[i]);
	}
	delete(model);
}
// TEST
void baseTest(double* inputs, double* expected_outputs, int inputSize) {

	int i;
	for (i = 0; i < 20; i += 2) {
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
	for (i = 20; i < 40; i += 2) {
		// x entre -1 et 1
		inputs[i] = rand() % 10000 / 5000. - 1.;
		// z entre -1 et 0
		inputs[i + 1] = rand() % 10000 / 10000. - 1.;
	}
	for (i = 0; i < 10; i++) {
		// y = 1
		expected_outputs[i] = 1;
	}
	for (i = 10; i < 20; i++) {
		// y = -1
		expected_outputs[i] = -1;
	}
}

void showInputs(double* inputs, int inputsSize) {
	for (int i = 0; i < inputsSize; ++i) {
		std::cout << "DEBUG inputs[" << i << "] >" << inputs[i] << "< " << std::endl;
	}
}

void showModel(double* model, int modelSize) {
	std::cout << "- Model -" << std::endl;
	for (int i = 0; i<modelSize; ++i) {
		std::cout << "w" << i << ":" << model[i] << std::endl;
	}
}
int test(double* test, int testSize) {
	int i(0);
	for (i = 0; i < testSize; ++i) {
		std::cout << "test[" << i << "] = " << test[i] << std::endl;
	}
	return 1;
}

