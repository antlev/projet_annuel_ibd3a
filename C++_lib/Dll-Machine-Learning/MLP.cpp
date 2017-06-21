#include "stdafx.h"
#include "MLP.h"
//
// Created by antoine on 13/06/2017.
//
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// CLASSIFICATION ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void MLP::fitClassification(double *inputs, int inputSize, int inputsSize, double *expectedOutputs,
	int outputSize) {
	assert(inputs);
	assert(expectedOutputs);
	assert(inputSize > 0);
	assert(inputsSize > 0);
	assert(outputSize > 0);
	int nbData = inputsSize / inputSize;
	int iterations(0);
	int indexOfRdmData;
	double* oneInput = new double[inputSize + 1]; // Bias
	double* oneExpectedOutput = new double[outputSize];
	modelLearned = 1;
	while (1) {
		indexOfRdmData = rand() % (nbData);
		for (int i = indexOfRdmData*inputSize, j = 0; i < (indexOfRdmData + 1)*inputSize; ++i, ++j) {
			oneInput[j] = inputs[i];
		}
		for (int i = indexOfRdmData*outputSize, j = 0; i < (indexOfRdmData + 1)*outputSize; ++i, ++j) {
			oneExpectedOutput[j] = expectedOutputs[i];
		}
		// Classification
		classify(oneInput, inputSize);
		fitClassifOneInput(oneInput, inputSize,
			oneExpectedOutput, outputSize);
		if (iterations++ >= maxIterations) {
			break;
		}
	}
}
void MLP::fitClassifOneInput(double *oneInput, int inputSize, double *oneOutput, int outputSize) {
	assert(oneInput);
	assert(oneOutput);
	assert(inputSize > 0);
	assert(outputSize > 0);
	double sum;
	// CALCULATE ERROR FOR LAST LAYER
	// Classification
	for (int neuronNb = 0; neuronNb < structure[nbLayer - 1]; ++neuronNb) {
		gradient[nbLayer - 1][neuronNb] =
			(1 - (neurons[nbLayer - 1][neuronNb] * neurons[nbLayer - 1][neuronNb])) *(neurons[nbLayer - 1][neuronNb] - oneOutput[neuronNb]);
	}
	// CALCULATE ERROR FOR ALL OTHER LAYERS
	for (int layerNb = nbLayer - 1; layerNb > 0; --layerNb) {
		for (int leftNeuron = 0; leftNeuron < structure[layerNb - 1]; ++leftNeuron) {
			sum = 0;
			for (int neuronNb = 0; neuronNb < structure[layerNb] /*Bias*/; ++neuronNb) {
				sum += weights[layerNb - 1][leftNeuron][neuronNb] * gradient[layerNb][neuronNb];
			}
			gradient[layerNb - 1][leftNeuron] =
				(1 - (neurons[layerNb - 1][leftNeuron] * neurons[layerNb - 1][leftNeuron])) * sum;
		}
	}
	// UPDATE WEIGHTS
	for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
		for (int leftNeuronNb = 0; leftNeuronNb < structure[layerNb - 1] + 1 /*Bias*/; ++leftNeuronNb) {
			for (int rightNeuronNb = 0; rightNeuronNb < structure[layerNb]; ++rightNeuronNb) {
				weights[layerNb - 1][leftNeuronNb][rightNeuronNb] -= learningRate * neurons[layerNb - 1][leftNeuronNb] * gradient[layerNb][rightNeuronNb];
			}
		}
	}
}
// Set all the neurons considering the input and weight
// Set the oneOutput variable
void MLP::classify(double *oneInput, int inputSize) {
	assert(oneInput);
	assert(inputSize > 0);
	if (modelLearned == 1) {
		// set the input layer with the input given plus the bias neuron
		for (int i = 0; i< inputSize; ++i) {
			neurons[0][i] = oneInput[i];
		}
		neurons[0][inputSize] = 1; // Bias
								   // Set all the neurons of the model
		for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
			for (int neuronNb = 0; neuronNb < structure[layerNb] + 1; ++neuronNb) {
				if (neuronNb == structure[layerNb]) { // Bias
					neurons[layerNb][neuronNb] = 1;
				}
				else {
					neurons[layerNb][neuronNb] = tanh(sum(layerNb, neuronNb));
				}
			}
		}
	}
	else {
		std::cout << "Sorry you have to train a classification model to call classify" << std::endl;
	}
}
double MLP::getOutputsforClassif() {
	if (modelLearned == 1) {
		return (neurons[nbLayer - 1][0] < 0) ? -1 : 1;
	}
	else {
		std::cout << "Sorry you have to train a classification model to call classify" << std::endl;
		return -1;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// REGRESSION //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
void MLP::fitRegression(double *inputs, int inputSize, int inputsSize, double *expectedOutputs,
	int outputSize) {
	assert(inputs);
	assert(expectedOutputs);
	assert(inputSize > 0);
	assert(inputsSize > 0);
	assert(outputSize > 0);
	int nbData = inputsSize / inputSize;
	int iterations(0);
	int indexOfRdmData;
	double* oneInput = new double[inputSize + 1]; // Bias
	double* oneExpectedOutput = new double[outputSize];
	modelLearned = 2;
	while (1) {
		indexOfRdmData = rand() % (nbData);
		for (int i = indexOfRdmData*inputSize, j = 0; i < (indexOfRdmData + 1)*inputSize; ++i, ++j) {
			oneInput[j] = inputs[i];
		}
		for (int i = indexOfRdmData*outputSize, j = 0; i < (indexOfRdmData + 1)*outputSize; ++i, ++j) {
			oneExpectedOutput[j] = expectedOutputs[i];
		}
		// Regression
		predict(oneInput, inputSize);
		fitRegreOneInput(oneInput, inputSize,
			oneExpectedOutput, outputSize);
		if (iterations++ >= maxIterations) {
			break;
		}
	}
}
void MLP::fitRegreOneInput(double *oneInput, int inputSize, double *oneOutput, int outputSize) {
	double sum;
	assert(oneInput);
	assert(oneOutput);
	assert(inputSize > 0);
	assert(outputSize > 0);
	// CALCULATE ERROR FOR LAST LAYER
	// Regression
	for (int neuronNb = 0; neuronNb < structure[nbLayer - 1]; ++neuronNb) {
		gradient[nbLayer - 1][neuronNb] =
			(neurons[nbLayer - 1][neuronNb] - oneOutput[neuronNb]);
	}
	// CALCULATE ERROR FOR ALL OTHER LAYERS
	for (int layerNb = nbLayer - 1; layerNb > 0; --layerNb) {
		for (int leftNeuron = 0; leftNeuron < structure[layerNb - 1]; ++leftNeuron) {
			sum = 0;
			for (int neuronNb = 0; neuronNb < structure[layerNb] /*Bias*/; ++neuronNb) {
				sum += weights[layerNb - 1][leftNeuron][neuronNb] * gradient[layerNb][neuronNb];
			}
			gradient[layerNb - 1][leftNeuron] =
				(1 - (neurons[layerNb - 1][leftNeuron] * neurons[layerNb - 1][leftNeuron])) * sum;
		}
	}
	// UPDATE WEIGHTS
	for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
		for (int leftNeuronNb = 0; leftNeuronNb < structure[layerNb - 1] + 1 /*Bias*/; ++leftNeuronNb) {
			for (int rightNeuronNb = 0; rightNeuronNb < structure[layerNb]; ++rightNeuronNb) {
				weights[layerNb - 1][leftNeuronNb][rightNeuronNb] -= learningRate * neurons[layerNb - 1][leftNeuronNb] * gradient[layerNb][rightNeuronNb];
			}
		}
	}
}
// Set all the neurons considering the input and weight
// Set the oneOutput variable
void MLP::predict(double* oneInput, int inputSize) {
	assert(oneInput);
	assert(inputSize > 0);
	if (modelLearned == 2) {
		// set the input layer with the input given plus the bias neuron
		for (int i = 0; i < inputSize; ++i) {
			neurons[0][i] = oneInput[i];
		}
		neurons[0][inputSize] = 1;
		// Set all the neurons of the model
		for (int layerNb = 1; layerNb < nbLayer; ++layerNb) {
			for (int neuronNb = 0; neuronNb < structure[layerNb] + 1; ++neuronNb) {
				if (neuronNb == structure[layerNb]) { // Bias
					neurons[layerNb][neuronNb] = 1;
				}
				else {
					if (layerNb == nbLayer - 1) {
						neurons[layerNb][neuronNb] = sum(layerNb, neuronNb);
					}
					else {
						neurons[layerNb][neuronNb] = tanh(sum(layerNb, neuronNb));
					}
				}
			}
		}
	}
	else {
		std::cout << "Please train a regression model to call predict" << std::endl;
	}
}
double MLP::getOutputsforRegression() {
	if (modelLearned == 2) {
		return neurons[nbLayer - 1][0];
	}
	else {
		std::cout << "Please train a regression model to call predict" << std::endl;
		return -1;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// UTILS ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate the sum of the weights * the previous neuron layer, using the model struct, layerNb and neuronNb to set
double MLP::sum(int layerNb, int neuronNb) {
	double sum = 0;
	for (int i = 0; i < structure[layerNb - 1] + 1; ++i) {
		sum += neurons[layerNb - 1][i] * weights[layerNb - 1][i][neuronNb];
	}
	return sum;
}
