#include "Header.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include "Eigen/Dense"

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
double* addBiasToInput(double *input, int inputSize) {
	assert(input != NULL);
	assert(inputSize > 0);
	double* newInput = new double[2 + 1];
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
	double* inputWithBias = addBiasToInput(input, inputSize);
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
			oneInput = addBiasToInput(oneInput, inputSize);

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
						for (int modelIterator = outputIterator, inputIterator = 0; modelIterator < (inputSize + 1)*outputSize; modelIterator += outputSize, inputIterator++) {

							cout << "modelIterator >" << modelIterator << "< model[" << modelIterator << "]>" << model[modelIterator] << "< outputIterator >" << outputIterator << "< outputError[outputIterator] >" << outputError[outputIterator] << "< inputIterator >" << inputIterator << "< oneInput[inputIterator] >" << oneInput[inputIterator] << "< " << endl;
							model[modelIterator] += step * outputError[outputIterator] * oneInput[inputIterator];

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
// Fit a model with all the inputs and outputs WITHOUT bias and concatenate in an double* array
// Return a pointer on W matrix
Eigen::MatrixXd* linear_fit_regression(double *inputs, int inputsSize, int inputSize, double *expectedOutputs, int outputSize) {
	inputs = addBiasToInputs(inputs, &inputsSize, &inputSize);
	// Build X and Y
	int nbInput = inputsSize / inputSize;
	Eigen::MatrixXd X(nbInput, inputSize);
	//    cout << "X(" << nbInput << "," << inputSize << ")!" << endl;
	Eigen::MatrixXd Y(nbInput, outputSize);
	//    cout << "Y(" << nbInput << "," << outputSize << ")!" << endl;

	tabToMatrix(&X, inputs, nbInput, inputSize);
	tabToMatrix(&Y, expectedOutputs, nbInput, outputSize);
	//    for (int i = 0; i < nbInput; i++) {
	//        for (int j = 0; j < inputSize; j++) {
	//            cout << "X(" << i << "," << j << ") = " << X(i,j) << endl;
	//        }
	//    }
	//    for (int i = 0; i < nbInput; i++) {
	//        for (int j = 0; j < outputSize; j++) {
	//            cout << "Y(" << i << "," << j << ") = " << Y(i,j) << endl;
	//        }
	//    }
	// return the calculated result as a matrix
	return new Eigen::MatrixXd(pinv(X) * Y);
}
// Return the pseudo inverse of the matrix passed as a parameter
Eigen::MatrixXd pinv(Eigen::MatrixXd X) {
	Eigen::MatrixXd X_Transp = X.transpose();
	return (((X_Transp * X).inverse()) * X_Transp);
}
// For a trained model, this function calculate the output of the single input passed as a paramter
// The output array will be filled
void linearPredict(Eigen::MatrixXd* model, double* input, int inputSize, double* output, int outputSize) {
	//	assert(model != NULL);
	assert(input != NULL);
	assert(inputSize > 0);
	assert(output != NULL);
	assert(outputSize > 0);
	double* inputWithBias = addBiasToInput(input, inputSize);

	Eigen::MatrixXd inputMatrix(1, inputSize + 1);
	for (int i = 0; i<inputSize + 1; i++) {
		inputMatrix(0, i) = inputWithBias[i];
	}
	cout << "inputMatrix >" << inputMatrix << "<" << endl;
	Eigen::MatrixXd outputMatrix(outputSize, 1);
	outputMatrix = inputMatrix * (*model);
	cout << "outputVector >" << outputMatrix << "<" << endl;

	matrixToTab(outputMatrix, output, 1, outputSize);

	for (int i = 0; i<outputSize; i++) {
		cout << "output[" << i << "] = " << output[i] << endl;
	}
}
int main() {

	time_t now;
	time(&now);
	srand((unsigned int)now);

	int inputSize = 2;
	int outputNeuronsSize = 3;
	int nbData = 3;

	int inputsSize = inputSize * nbData;
	int outputsSize = outputNeuronsSize*nbData;
	double* outputs = new double[outputsSize];
	double* inputs = new double[inputsSize];
	double* expected_outputs = new double[outputsSize];

	inputs[0] = 0;
	inputs[1] = -0.5;
	expected_outputs[0] = 0.25;
	expected_outputs[1] = 0.1;
	expected_outputs[2] = -1;

	inputs[2] = 0.5;
	inputs[3] = 0.375;
	expected_outputs[3] = 0.6;
	expected_outputs[4] = -0.3;
	expected_outputs[5] = 0.2;

	inputs[4] = -0.625;
	inputs[5] = 0.25;
	expected_outputs[6] = 0.1;
	expected_outputs[7] = -0.1;
	expected_outputs[8] = -0.8;

	Eigen::MatrixXd* model = linear_fit_regression(inputs, inputsSize, inputSize, expected_outputs, outputNeuronsSize);

	cout << "model : " << *model << " (" << model->rows() << "," << model->cols() << ")" << endl;
	double* input = new double(inputSize);
	double* output = new double(outputNeuronsSize);
	input[0] = 0;
	input[1] = -0.5;
	linearPredict(model, input, inputSize, output, outputNeuronsSize);

	input[0] = 0.5;
	input[1] = 0.375;
	linearPredict(model, input, inputSize, output, outputNeuronsSize);

	input[0] = -0.625;
	input[1] = 0.25;
	linearPredict(model, input, inputSize, output, outputNeuronsSize);

	return 1;
}
// Add bias to the inputs
// Modify inputSize and inputsSize
// Return the new input
double* addBiasToInputs(double *inputs, int *inputsSize, int *inputSize) {
	int nbData = *inputsSize / *inputSize;
	*inputsSize += nbData;
	double* newInputs = new double[*inputsSize + nbData];
	int inputIterator = 0;
	for (int newinputIterator = 0; newinputIterator < (*inputsSize); newinputIterator++) {
		if (newinputIterator % (*inputSize + 1) == 0) {
			newInputs[newinputIterator] = 1;
			newinputIterator++;
		}
		newInputs[newinputIterator] = inputs[inputIterator];
		inputIterator++;
	}
	*inputSize += 1;
	return newInputs;
}
// Transform an array of double into a matrix
void tabToMatrix(Eigen::MatrixXd* matrix, double* tab, int nbRow, int nbCols) {
	int iterator = 0;
	for (int i = 0; i<nbRow; i++) {
		for (int j = 0; j<nbCols; ++j) {
			(*matrix)(i, j) = tab[iterator];
			iterator++;
		}
	}
}
// Transform a matrix into a array of double
void matrixToTab(Eigen::MatrixXd matrix, double *tab, int nbRow, int nbCols) {
	int iterator = 0;
	for (int i = 0; i<nbRow; ++i) {
		for (int j = 0; j<nbCols; ++j) {
			tab[iterator] = matrix(i, j);
			iterator++;
		}
	}
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
//void test(Eigen::MatrixXd* model, double* inputs, int inputsSize, int inputSize, double* outputs, int outputSize){
//    int nbData = inputsSize / inputSize;
//    inputs = addBiasToInputs(inputs, &inputsSize, &inputSize);
//
//    // Eigen::MatrixXd inputsMatrix(inputSize, nbData);
//
//    // int inputIterator = 0;
//    // for (int i = 0; i<nbData; i++) {
//    // 	for (int j = 0; j<inputSize; ++j) {
//    // 		inputsMatrix(i, j) = inputs[inputIterator];
//    // 		inputIterator++;
//    // 	}
//    // }
//    // Eigen::MatrixXd outputMatrix(outputSize, nbData);
//    // Calcul de l'output
//    //Eigen::MatrixXd outputMatrix = (*model).transpose() * inputsMatrix;
//
//    for(int i=0;i<nbData;i++){
//        int sum=0;
//        for(int j=0;j<inputsSize;j++){
//            sum += (*model)(i,j) * inputs[i*inputsSize+j];
//        }
//        outputs[i] = sum;
//    }
//
//
//    // matrixToTab(outputMatrix, outputs, nbData, outputSize);
//}