#include "Header.h"
#include "MLP.h"
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <cassert>
#include "Eigen/Dense"
#include <cmath>
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
// Return the MLP's response considering the inputs
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
		sum_weigths_inputs = 0;
		for (int modelIterator = outputIterator, inputIterator = 0; modelIterator < (inputSize + 1)*outputDimension && inputIterator < inputSize + 1; inputIterator++, modelIterator += outputDimension) {
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
		// For each data passed in parameter of the function
		for (int inputsIterator = 0, inputIterator = 0; inputsIterator < inputsSize; inputsIterator += inputSize, inputIterator++) {
			// recovering inputs one by one
			for (int i = 0; i<inputSize; i++) {
				oneInput[i] = inputs[inputsIterator + i];
			}
			// Get the result given by the MLP
			linear_classify(model, oneInput, inputSize, oneOutput, outputSize);
			oneInput = addBiasToInput(oneInput, inputSize);
			bool* outputIsGood = new bool[outputSize];
			double* outputError = new double[outputSize];
			bool allOutputsAreGood = true;
			// For each output neuron we check that response from MLP is correct
			for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
				outputIsGood[outputIterator] = true; // Initialising all response at true
													 // If one of the output neuron doesn't give the good answer
				if (oneOutput[outputIterator] != expectedOutputs[inputIterator*outputSize + outputIterator]) {
					outputIsGood[outputIterator] = false;
					if (allOutputsAreGood) { allOutputsAreGood = false; }
					// Storing the output error in a tab
					outputError[outputIterator] = expectedOutputs[inputIterator*outputSize + outputIterator] - oneOutput[outputIterator];
				}
			}
			// If at least one of the MLP's response is not good
			if (!allOutputsAreGood) {
				error++; // We increment the error of the current MLP on the data given
				for (int outputIterator = 0; outputIterator < outputSize; outputIterator++) {
					// For each output, if the MLP's response ins't good -> We modify the weights affecting the output
					if (!outputIsGood[outputIterator]) {
						// We adapt every weight of our MLP using the Rsoenblatt formula
						for (int modelIterator = outputIterator, inputIterator = 0; modelIterator < (inputSize + 1)*outputSize; modelIterator += outputSize, inputIterator++) {
							model[modelIterator] += step * outputError[outputIterator] * oneInput[inputIterator];
						}
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
	Eigen::MatrixXd Y(nbInput, outputSize);

	tabToMatrix(&X, inputs, nbInput, inputSize);
	tabToMatrix(&Y, expectedOutputs, nbInput, outputSize);
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
	Eigen::MatrixXd outputMatrix(outputSize, 1);
	outputMatrix = inputMatrix * (*model);
	matrixToTab(outputMatrix, output, 1, outputSize);
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
// ModelStruct corresponds to the structure of the model to generate
// Each case correspond to the number of neurons of the hidden layer(s) and the output
// It does not contain the input neurons
// example modelstruct = [2][3] -> input of size 2, no hidden layers only an output composed of 3 neurons
// example modelstruct = [2][3][2][3] -> input of size 2, 2 hidden layers of 3 and 2 neurons, and an output composed of 3 neurons
// nbLayer correspond to the modelStruct size
// The model is then easy to use to get the weight corresponding to the layer 2  linking the left 2nd neuron to the 3rd right is model[2][2][3]
void pmcCreateModel(int *modelStruct, int nbLayer, double ****modelWeights, double ***modelNeurons, double ***modelError) {
	assert(modelStruct[0] > 0 && modelStruct[1] > 0);
	assert(nbLayer >= 2);
	for (int i = 0; i < nbLayer; ++i) {
		assert(modelStruct[i] > 0);
	}
	(*modelWeights) = new double**[nbLayer - 1];
	(*modelNeurons) = new double*[nbLayer];
	(*modelError) = new double*[nbLayer];
	for (int layerNumber = 0; layerNumber<nbLayer; layerNumber++) {
		(*modelNeurons)[layerNumber] = new double[modelStruct[layerNumber] + 1]; // Bias neuron
		(*modelError)[layerNumber] = new double[modelStruct[layerNumber] + 1]; // Bias neuron
		for (int leftNeuron = 0; leftNeuron<modelStruct[layerNumber] + 1; leftNeuron++) {
			(*modelError)[layerNumber][leftNeuron] = 0;
			(*modelNeurons)[layerNumber][leftNeuron] = 0;
		}
		(*modelNeurons)[layerNumber][modelStruct[layerNumber]] = 1; // Bias
	}
	for (int layerNumber = 0; layerNumber<nbLayer - 1; layerNumber++) {
		(*modelWeights)[layerNumber] = new double*[modelStruct[layerNumber] + 1]; // Bias neuron
		for (int leftNeuron = 0; leftNeuron<modelStruct[layerNumber] + 1; leftNeuron++) {
			(*modelWeights)[layerNumber][leftNeuron] = new double[modelStruct[layerNumber + 1]];
			for (int rightNeuron = 0; rightNeuron < modelStruct[layerNumber + 1]; ++rightNeuron) {
				(*modelWeights)[layerNumber][leftNeuron][rightNeuron] = ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
			}
		}
	}
}
void pmcFit(double**** modelWeights, double*** modelNeurons, double*** modelError, int* modelStruct, int nbLayer, double* inputs, int inputSize, int inputsSize, double* expectedOutputs, int outputSize, double learningRate, int maxIteraions, int option) {
	int nbData = inputsSize / inputSize;
	int iterations(0);
	int indexOfRdmData;
	double* oneInput = new double[inputSize + 1]; // Bias
	double* oneExpectedOutput = new double[outputSize];
	while (1) {
		indexOfRdmData = rand() % (nbData);
		for (int i = indexOfRdmData*inputSize, j = 0; i < (indexOfRdmData + 1)*inputSize; ++i, ++j) {
			oneInput[j] = inputs[i];
		}
		for (int i = indexOfRdmData*outputSize, j = 0; i < (indexOfRdmData + 1)*outputSize; ++i, ++j) {
			oneExpectedOutput[j] = expectedOutputs[i];
		}
		if (option == 0) { // Classification
			pmcClassifyOneInput(*modelWeights, modelNeurons, modelStruct, nbLayer, oneInput, inputSize, &oneExpectedOutput, outputSize);
		}
		else { // Regression
			pmcPredictOneInput(*modelWeights, modelNeurons, modelStruct, nbLayer, oneInput, inputSize, &oneExpectedOutput, outputSize);
		}
		pmcFitOneInput(modelWeights, *modelNeurons, modelError, modelStruct, nbLayer, oneInput, inputSize, oneExpectedOutput, outputSize, learningRate, option);
		if (iterations++ >= maxIteraions) {
			break;
		}
	}
}
void pmcFitOneInput(double**** modelWeights, double** modelNeurons, double*** modelError, int* modelStruct, int nbLayer, double* oneInput, int inputSize, double* oneOutput, int outputSize, double learningRate, int option) {
	double sum;
	// CALCULATE ERROR FOR LAST LAYER
	if (option == 0) { // Classification
		for (int neuronNb = 0; neuronNb < modelStruct[nbLayer - 1]; ++neuronNb) {
			(*modelError)[nbLayer - 1][neuronNb] =
				(1 - (modelNeurons[nbLayer - 1][neuronNb] * modelNeurons[nbLayer - 1][neuronNb])) *(modelNeurons[nbLayer - 1][neuronNb] - oneOutput[neuronNb]);
		}
	}
	else { // Regression
		for (int neuronNb = 0; neuronNb < modelStruct[nbLayer - 1]; ++neuronNb) {
			(*modelError)[nbLayer - 1][neuronNb] =
				(modelNeurons[nbLayer - 1][neuronNb] - oneOutput[neuronNb]);
		}
	}
	// CALCULATE ERROR FOR ALL OTHER LAYERS
	for (int layerNb = nbLayer - 1; layerNb > 0; --layerNb) {
		for (int leftNeuron = 0; leftNeuron < modelStruct[layerNb - 1]; ++leftNeuron) {
			sum = 0;
			for (int neuronNb = 0; neuronNb < modelStruct[layerNb] /*Bias*/; ++neuronNb) {
				sum += (*modelWeights)[layerNb - 1][leftNeuron][neuronNb] * (*modelError)[layerNb][neuronNb];
			}
			(*modelError)[layerNb - 1][leftNeuron] =
				(1 - (modelNeurons[layerNb - 1][leftNeuron] * modelNeurons[layerNb - 1][leftNeuron])) * sum;
		}
	}
	// UPDATE WEIGHTS
	for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
		for (int leftNeuronNb = 0; leftNeuronNb < modelStruct[layerNb - 1] + 1 /*Bias*/; ++leftNeuronNb) {
			for (int rightNeuronNb = 0; rightNeuronNb < modelStruct[layerNb]; ++rightNeuronNb) {
				(*modelWeights)[layerNb - 1][leftNeuronNb][rightNeuronNb] -= learningRate * modelNeurons[layerNb - 1][leftNeuronNb] * (*modelError)[layerNb][rightNeuronNb];
			}
		}
	}
}
// Calculate the sum of the weights * the previous neuron layer, using the model struct, layerNb and neuronNb to set
double sum(double*** modelWeights, double** modelNeurons, int* modelStruct, int layerNb, int neuronNb) {
	double sum = 0;
	for (int i = 0; i < modelStruct[layerNb - 1] + 1; ++i) {
		sum += modelNeurons[layerNb - 1][i] * modelWeights[layerNb - 1][i][neuronNb];
	}
	return sum;
}
// Set all the neurons considering the input and weight
// Set the oneOutput variable
void pmcClassifyOneInput(double*** modelWeights, double*** modelNeurons, int* modelStruct, int nbLayer, double* oneInput, int inputSize, double** oneOutput, int outputSize) {
	// set the input layer with the input given plus the bias neuron
	for (int i = 0; i< inputSize; ++i) {
		(*modelNeurons)[0][i] = oneInput[i];
	}
	(*modelNeurons)[0][inputSize] = 1;
	// Set all the neurons of the model
	for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
		for (int neuronNb = 0; neuronNb < modelStruct[layerNb] + 1; ++neuronNb) {
			if (neuronNb == modelStruct[layerNb]) { // Bias
				(*modelNeurons)[layerNb][neuronNb] = 1;
			}
			else {
				(*modelNeurons)[layerNb][neuronNb] = tanh(sum(modelWeights, *modelNeurons, modelStruct, layerNb, neuronNb));
			}
		}
	}
}
// Set all the neurons considering the input and weight
// Set the oneOutput variable
void pmcPredictOneInput(double*** modelWeights, double*** modelNeurons, int* modelStruct, int nbLayer, double* oneInput, int inputSize, double** oneOutput, int outputSize) {
	// set the input layer with the input given plus the bias neuron
	for (int i = 0; i< inputSize; ++i) {
		(*modelNeurons)[0][i] = oneInput[i];
	}
	(*modelNeurons)[0][inputSize] = 1;
	// Set all the neurons of the model
	for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
		for (int neuronNb = 0; neuronNb < modelStruct[layerNb] + 1; ++neuronNb) {
			if (neuronNb == modelStruct[layerNb]) { // Bias
				(*modelNeurons)[layerNb][neuronNb] = 1;
			}
			else {
				if (layerNb == nbLayer - 1) {
					(*modelNeurons)[layerNb][neuronNb] = sum(modelWeights, *modelNeurons, modelStruct, layerNb, neuronNb);
				}
				else {
					(*modelNeurons)[layerNb][neuronNb] = tanh(sum(modelWeights, *modelNeurons, modelStruct, layerNb, neuronNb));
				}
			}
		}
	}
}
void pmc_remove_model(double*** model, int* modelStruct, int modelStructSize, int inputSize) {
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
		// z entre 0 et 1
		inputs[i + 1] = rand() % 10000 / 10000.;
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

int main() {

	time_t now;
	time(&now);
	srand((unsigned int)now);

	/*    int inputSize = 2;
	int outputNeuronsSize = 1;
	int nbData = 4;

	int inputsSize = inputSize * nbData;
	int outputsSize = outputNeuronsSize*nbData;
	double* inputs = new double[inputsSize];
	double* expectedOutputs = new double[outputsSize];
	//
	//    inputs[0] = 0;
	//    inputs[1] = -0.5;
	//    expectedOutputs[0] = 0.25;
	//    expectedOutputs[1] = 0.1;
	//    expectedOutputs[2] = -1;
	//
	//    inputs[2] = 0.5;
	//    inputs[3] = 0.375;
	//    expectedOutputs[3] = 0.6;
	//    expectedOutputs[4] = -0.3;
	//    expectedOutputs[5] = 0.2;
	//
	//    inputs[4] = -0.625;
	//    inputs[5] = 0.25;
	//    expectedOutputs[6] = 0.1;
	//    expectedOutputs[7] = -0.1;
	//    expectedOutputs[8] = -0.8;
	//
	//    Eigen::MatrixXd* model = linear_fit_regression(inputs, inputsSize, inputSize, expectedOutputs, outputNeuronsSize);
	//
	//    cout << "model : " << *model << " (" << model->rows() << "," << model->cols() << ")" << endl;
	double* input = new double[inputSize];
	double* output = new double[outputNeuronsSize];
	//    input[0] = 0;
	//    input[1] = -0.5;
	//    linearPredict(model, input, inputSize, output, outputNeuronsSize);
	//
	//    input[0] = 0.5;
	//    input[1] = 0.375;
	//    linearPredict(model, input, inputSize, output, outputNeuronsSize);
	//
	//    input[0] = -0.625;
	//    input[1] = 0.25;
	//    linearPredict(model, input, inputSize, output, outputNeuronsSize);

	double*** modelWeights;
	double** modelNeurons;
	double** modelError;
	int nbLayer =3;
	int modelStruct[3] = { 2, 2, 1 };
	int iterationsMax = 100000;
	double learningRate = 0.01;

	inputs[0] = -1;
	inputs[1] = -1;
	expectedOutputs[0] = 1;
	//    expectedOutputs[1] = 0.1;
	//    expectedOutputs[2] = 1;

	inputs[2] = 1;
	inputs[3] = 1;
	expectedOutputs[1] = 1;

	//    expectedOutputs[3] = 0.6;
	//    expectedOutputs[4] = 0.3;
	//    expectedOutputs[5] = 0.2;

	inputs[4] = -1;
	inputs[5] = 1;
	expectedOutputs[2] = -1;

	//    expectedOutputs[6] = 0.1;
	//    expectedOutputs[7] = 0.1;
	//    expectedOutputs[8] = 0.8;

	inputs[6] = 1;
	inputs[7] = -1;
	expectedOutputs[3] = -1;

	pmcCreateModel(modelStruct, nbLayer, &modelWeights, &modelNeurons, &modelError);

	pmcFit(&modelWeights, &modelNeurons, &modelError, modelStruct, nbLayer, inputs, inputSize, inputsSize, expectedOutputs, outputNeuronsSize, learningRate, iterationsMax, 0);

	input[0] = 1;
	input[1] = 1;
	pmcClassifyOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}

	input[0] = -1;
	input[1] = -1;
	pmcClassifyOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}

	input[0] = 1;
	input[1] = -1.0;
	pmcClassifyOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}

	input[0] = -1;
	input[1] = 1.0;
	pmcClassifyOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}

	inputs[0] = 0;
	inputs[1] = 0;
	expectedOutputs[0] = 0;
	//    expectedOutputs[1] = 0.1;
	//    expectedOutputs[2] = 1;

	inputs[2] = 0;
	inputs[3] = 1;
	expectedOutputs[1] = 0;

	//    expectedOutputs[3] = 0.6;
	//    expectedOutputs[4] = 0.3;
	//    expectedOutputs[5] = 0.2;

	inputs[4] = 1;
	inputs[5] = 1;
	expectedOutputs[2] = 0.5;
	cout << "Regression ! " << endl;
	pmcFit(&modelWeights, &modelNeurons, &modelError, modelStruct, nbLayer, inputs, inputSize, inputsSize, expectedOutputs, outputNeuronsSize, learningRate, iterationsMax, 0);


	input[0] = 0;
	input[1] = 0;
	pmcPredictOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}

	input[0] = 0;
	input[1] = 1;
	pmcPredictOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}

	input[0] = 1;
	input[1] = 1.0;
	pmcPredictOneInput(modelWeights, &modelNeurons, modelStruct, nbLayer, input, inputSize, &output, outputNeuronsSize);
	for (int i = 0; i < outputNeuronsSize; ++i) {
	cout << "TEST Output[" << i << "] >" << modelNeurons[nbLayer - 1][i] << "<" << endl;
	}*/
	int nbLayer = 3;
	int modelStruct[3] = { 2, 2, 1 };
	int inputSize = 2;
	int outputSize = 1;

	int nbData = 3;

	double* inputs = new double[inputSize*nbData];
	double* expectedOutputs = new double[outputSize*nbData];
	double* oneInput = new double[inputSize];
	double* oneOutput = new double[outputSize];

	MLP* regressionPerceptron = new MLP(modelStruct, nbLayer);

	inputs[0] = 0;
	inputs[1] = 0;

	inputs[2] = 0;
	inputs[3] = 1;

	inputs[4] = 1;
	inputs[5] = 1;

	expectedOutputs[0] = 0;
	expectedOutputs[1] = 0;
	expectedOutputs[2] = 0.5;

	std::cout << "Fitting regression model..." << std::endl;
	regressionPerceptron->fitRegression(inputs, inputSize, inputSize*nbData, expectedOutputs, outputSize);

	std::cout << "Testing Regression" << std::endl;
	oneInput[0] = 0;
	oneInput[1] = 0;
	regressionPerceptron->predict(oneInput, inputSize, &oneOutput, outputSize);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << oneOutput[0] << "<" << std::endl;

	oneInput[0] = 0;
	oneInput[1] = 1;
	regressionPerceptron->predict(oneInput, inputSize, &oneOutput, outputSize);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << oneOutput[0] << "<" << std::endl;

	oneInput[0] = 1;
	oneInput[1] = 1;
	regressionPerceptron->predict(oneInput, inputSize, &oneOutput, outputSize);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << oneOutput[0] << "<" << std::endl;


	MLP* classificationPerceptron = new MLP(modelStruct, nbLayer);

	inputs[0] = 0;
	inputs[1] = 0;

	inputs[2] = 0;
	inputs[3] = 1;

	inputs[4] = 1;
	inputs[5] = 1;

	expectedOutputs[0] = 0;
	expectedOutputs[1] = 0;
	expectedOutputs[2] = 1;

	std::cout << "Fitting Classification model..." << std::endl;
	regressionPerceptron->fitClassification(inputs, inputSize, inputSize*nbData, expectedOutputs, outputSize);

	std::cout << "Testing Classification" << std::endl;
	oneInput[0] = 0;
	oneInput[1] = 0;
	regressionPerceptron->classify(oneInput, inputSize, &oneOutput, outputSize);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << oneOutput[0] << "<" << std::endl;

	oneInput[0] = 0;
	oneInput[1] = 1;
	regressionPerceptron->classify(oneInput, inputSize, &oneOutput, outputSize);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << oneOutput[0] << "<" << std::endl;

	oneInput[0] = 1;
	oneInput[1] = 1;
	regressionPerceptron->classify(oneInput, inputSize, &oneOutput, outputSize);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << oneOutput[0] << "<" << std::endl;

	std::cout << "Testing RBF..." << std::endl;
	int nbExamples = 4;
	double gamma = 1;
	inputSize = 2;
	double* rbfInputs = new double[nbExamples*inputSize];
	double* rbfOutputs = new double[nbExamples];
	double rbfOneOutput;
	rbfInputs[0] = 0.2;
	rbfInputs[1] = 0.5;
	rbfOutputs[0] = 1;

	rbfInputs[2] = 0.7;
	rbfInputs[3] = 0.8;
	rbfOutputs[1] = 1;

	rbfInputs[4] = 0.6;
	rbfInputs[5] = 0.2;
	rbfOutputs[2] = -1;

	rbfInputs[6] = 0.9;
	rbfInputs[7] = 0.5;
	rbfOutputs[3] = -1;

	Eigen::MatrixXd weights = naiveLearnWeights(nbExamples, gamma, rbfInputs, inputSize, rbfOutputs);

	oneInput[0] = 0.2;
	oneInput[1] = 0.5;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected 1" << std::endl;
	oneInput[0] = 0.7;
	oneInput[1] = 0.8;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected 1" << std::endl;
	oneInput[0] = 0.6;
	oneInput[1] = 0.2;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected -1" << std::endl;
	oneInput[0] = 0.9;
	oneInput[1] = 0.5;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected -1" << std::endl;


	oneInput[0] = 0.2;
	oneInput[1] = 0.7;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected 1" << std::endl;
	oneInput[0] = 0.4;
	oneInput[1] = 0.8;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected 1" << std::endl;
	oneInput[0] = 0.5;
	oneInput[1] = 0.1;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected -1" << std::endl;
	oneInput[0] = 0.7;
	oneInput[1] = 0.5;
	getRBFResponse(weights, gamma, oneInput, inputSize, &rbfOneOutput, rbfInputs, nbExamples);
	std::cout << "Response for input = [" << oneInput[0] << "][" << oneInput[1] << "] ->" << rbfOneOutput << "< expected -1" << std::endl;
	return 1;
}


Eigen::MatrixXd naiveLearnWeights(int nbExamples, double gamma, double* X, int inputSize, double* Y) {
	double distance;
	Eigen::MatrixXd teta(nbExamples, nbExamples);
	Eigen::MatrixXd YMatrix(nbExamples, 1);
	for (int i = 0; i<nbExamples; i++) {
		YMatrix(i, 0) = Y[i];
	}
	for (int i = 0; i < nbExamples; ++i) {
		for (int j = 0; j < nbExamples; ++j) {
			distance = 0;
			for (int k = 0; k<inputSize; k++) {
				distance += (X[j*inputSize + k] - X[i*inputSize + k])*(X[j*inputSize + k] - X[i*inputSize + k]);
			}
			teta(i, j) = exp(-gamma*distance);
		}
	}
	return teta.inverse()*YMatrix;
}
double distance(double * A, double* B, int inputSize) {
	double distance = 0;
	for (int i = 0; i<inputSize; i++) {
		distance += (B[i] - A[i])*(B[i] - A[i]);
	}
	return sqrt(distance);
}
void getRBFResponse(Eigen::MatrixXd weights, double gamma, double* input, int inputSize, double* output, double* X, int nbExamples) {
	double sum = 0;
	double* oneX = new double[inputSize];
	for (int i = 0; i<nbExamples; i++) {
		for (int j = 0; j<inputSize; j++) {
			oneX[j] = X[i*inputSize + j];
		}
		sum += weights(i, 0)*exp(-gamma*distance(input, oneX, inputSize)*distance(input, oneX, inputSize));
	}
	*output = (sum > 0) ? 1 : -1;
}
