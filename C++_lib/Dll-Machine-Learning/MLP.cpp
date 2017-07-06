//
// Created by antoine on 13/06/2017.
//
#include "stdafx.h"
#include "MLP.h"
////////////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////// CLASSIFICATION ////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
MLP::MLP(int *structure, int nbLayer) : nbLayer(nbLayer) {
	assert(nbLayer >= 2);
	assert(structure != nullptr);
	for (int i = 0; i < nbLayer; ++i) {
		assert(structure[i] > 0);
	}
	inputSize = structure[0];
	outputSize = structure[nbLayer - 1];
	maxIterations = 10000;
	learningRate = 0.1;
	modelLearned = 0; //Model untrained
	weights = new double **[nbLayer - 1];
	neurons = new double *[nbLayer];
	gradient = new double *[nbLayer];
	this->structure = new int[nbLayer];
	for (int i = 0; i < nbLayer; ++i) {
		this->structure[i] = structure[i];
	}
	for (int layerNumber = 0; layerNumber < nbLayer; layerNumber++) {
		neurons[layerNumber] = new double[structure[layerNumber] + 1]; // Bias neuron
		gradient[layerNumber] = new double[structure[layerNumber] + 1]; // Bias neuron
		for (int leftNeuron = 0; leftNeuron < structure[layerNumber] + 1; leftNeuron++) {
			gradient[layerNumber][leftNeuron] = 0;
			neurons[layerNumber][leftNeuron] = 0;
		}
		neurons[layerNumber][structure[layerNumber]] = 1; // Bias
	}
	for (int layerNumber = 0; layerNumber < nbLayer - 1; layerNumber++) {
		weights[layerNumber] = new double *[structure[layerNumber] + 1]; // Bias neuron
		for (int leftNeuron = 0; leftNeuron < structure[layerNumber] + 1; leftNeuron++) {
			weights[layerNumber][leftNeuron] = new double[structure[layerNumber + 1]];
			for (int rightNeuron = 0; rightNeuron < structure[layerNumber + 1]; ++rightNeuron) {
				weights[layerNumber][leftNeuron][rightNeuron] = ((float)rand()) / ((float)RAND_MAX) * 2.0 - 1.0;
			}
		}
	}
}
MLP::~MLP() {
	for (int layerNumber = 0; layerNumber < nbLayer; layerNumber++) {
		delete neurons[layerNumber];
		delete gradient[layerNumber];
	}
	for (int layerNumber = 0; layerNumber < nbLayer - 1; layerNumber++) {
		delete weights[layerNumber];
		for (int leftNeuron = 0; leftNeuron < structure[layerNumber] + 1; leftNeuron++) {
			delete weights[layerNumber][leftNeuron];
		}
	}
}
// Function called from outside (the dll) taking all inputs and ouputs to fit the mlp classification model
// @param inputs : all inputs of the datas, that are concatenated in a single one entry array
// @param nbData : number of data in inputs
// @param expectedOutputs : array containing for each input all the output neuron expected response (ex : 2 inputs of 3 outputs : [i=1 o=1][i=1 o=2][i=1 o=2][i=2 o=1][i=2 o=2][i=2 o=2][i=2 o=1][i=3 o=2][i=3 o=3])
void MLP::fitClassification(double *inputs, int nbData, double *expectedOutputs) {
	assert(inputs);
	assert(expectedOutputs);
	assert(nbData > 0);
	int inputsSize = nbData * inputSize;
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
		classify(oneInput); // Classify
		fitClassifOneInput(oneInput, oneExpectedOutput); // Backpropagation
		if (iterations++ >= maxIterations) {
			break;
		}
	}
}
// Backpropagation for classification using a single input
// @param oneInput : input of one data
// @param oneOutput : array to write result
void MLP::fitClassifOneInput(double *oneInput, double* oneOutput) {
	assert(oneInput);
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
// @param oneInput : input of one data
// @return :array pointing on output
double* MLP::classify(double *oneInput) {
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
		return getOutputsforClassif();
	}
	else {
		std::cout << "Sorry you have to train a classification model to call classify" << std::endl;
		return nullptr;
	}
}
// Function to use to get the result after using classify
// @return :array pointing on output
double* MLP::getOutputsforClassif() {
	if (modelLearned == 1) {
		double* outputToReturn = new double[outputSize];
		for (auto i = 0; i < outputSize; i++) {
			outputToReturn[i] = (neurons[nbLayer - 1][i] < 0) ? -1 : 1;
		}
		return outputToReturn;
	}
	else {
		std::cout << "Sorry you have to train a classification model to call classify" << std::endl;
		return nullptr;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////// REGRESSION //////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Function called from outside (the dll) taking all inputs and ouputs to fit the mlp regression model
// @param inputs : all inputs of the datas, that are concatenated in a single one entry array
// @param nbData : number of data in inputs
// @param expectedOutputs : array containing for each input all the output neuron expected response (ex : 2 inputs of 3 outputs : [i=1 o=1][i=1 o=2][i=1 o=2][i=2 o=1][i=2 o=2][i=2 o=2][i=2 o=1][i=3 o=2][i=3 o=3])
void MLP::fitRegression(double *inputs, int nbData, double *expectedOutputs) {
	assert(inputs);
	assert(expectedOutputs);
	assert(nbData > 0);
	int inputsSize = nbData * inputSize;
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
		predict(oneInput);
		fitRegreOneInput(oneInput, oneExpectedOutput);
		if (iterations++ >= maxIterations) {
			break;
		}
	}
}
// Backpropagation for regression using a single input
// @param oneInput : input of one data
// @param oneOutput : array to write result
void MLP::fitRegreOneInput(double *oneInput, double *oneOutput) {
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
// @param oneInput : input of one data
// @return :array pointing on output
double* MLP::predict(double* oneInput) {
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
		return getOutputsforRegression();
	}
	else {
		std::cout << "Please train a regression model to call predict" << std::endl;
		return nullptr;
	}
}
// Function to use to get the result after using predict
// @return :array pointing on output
double* MLP::getOutputsforRegression() {
	if (modelLearned == 2) {
		return neurons[nbLayer - 1];
	}
	else {
		std::cout << "Please train a regression model to call predict" << std::endl;
		return nullptr;
	}
}
////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////// UTILS ////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
// Calculate the sum of the weights * the previous neuron layer, using the model struct, layerNb and neuronNb to set
// @param layerNb : layer number of the neuron to calculate sum (col)
// @param neuronNb : number of the neuron to calculate sum (line)
// @return : the value of the sum
double MLP::sum(int layerNb, int neuronNb) {
	double sum = 0;
	for (int i = 0; i < structure[layerNb - 1] + 1; ++i) {
		sum += neurons[layerNb - 1][i] * weights[layerNb - 1][i][neuronNb];
	}
	return sum;
}
