﻿using UnityEngine;
using System.Runtime.InteropServices;

public class MainScript : MonoBehaviour {

    public static int iterationNumber = 100;
    public static int step = 1;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

    // Use this for initialization
    void Start()
    {

    }

    // Update is called once per frame
    void Update()
    {

    }


    public void linear_fit_classification_rosenblatt()
    {
        Debug.Log("toto >" + LibWrapperMachineLearning.return42());

        Debug.Log("baseApprentissage >" + baseApprentissage.Length);
        Debug.Log("baseTest >" + baseTest.Length);



        int inputSize = 2; // x et z
        double[] inputs = new double[inputSize * baseApprentissage.Length];
        double[] outputs = new double[inputSize * baseApprentissage.Length];
        int i = 0;
        int j = 0;
        Debug.Log("DEBUG inputs >" + inputs.Length + "<");

        foreach (var data in baseApprentissage)
        {
            inputs[i] = data.position.x;
            i++;
            inputs[i] = data.position.z;
            i++;
            outputs[j] = data.position.y;
        }
        Debug.Log("DEBUG inputs >" + inputs.Length + "<");


		Debug.Log("DEBUG toto >" + LibWrapperMachineLearning.toto() + "<");


//        int inputsSize = inputs.Length;
//        int outputsSize = outputs.Length;
//
//        System.IntPtr model = LibWrapperMachineLearning.linear_create_model(inputSize);
//
//        var gchX = default(GCHandle);
//        var gchY = default(GCHandle);
//        try
//        {
//            gchX = GCHandle.Alloc(inputs, GCHandleType.Pinned);
//            gchY = GCHandle.Alloc(outputs, GCHandleType.Pinned);
////			Debug.Log ("test >" + LibWrapperMachineLearning.test(gchX.AddrOfPinnedObject(), inputs.Length) + "<");
////		    LibWrapperMachineLearning.linear_fit_classification_rosenblatt(model, gchX.AddrOfPinnedObject(), inputsSize, inputSize, gchY.AddrOfPinnedObject(), outputsSize, iterationNumber, step);
//        }
//        finally
//        {
//            if (gchX.IsAllocated) gchX.Free();
//            if (gchY.IsAllocated) gchY.Free();
//        }
//
//        double[] input = new double[inputSize];
//
//        i = 0;
//        foreach (var data in baseTest)
//        {
//            input[i] = data.position.x;
//            i++;
//            input[i] = data.position.z;
//            i++;
//            //			data.position.y = LibWrapperMachineLearning.linear_classify(model, input);
//        }

        //		LibWrapperMachineLearning.linear_remove_model(model);
    }
}



    //	public void linear_classify(){
    //
    //	}
    //

    //	double *linear_create_model(int inputDimension);
    //
    //	void linear_remove_model(double *model);
    //
    //
    //	int linear_fit_regression(int *model ,double *inputs, int inputsSize,int inputSize,double *outputs,int outputsSize);
    //
    //
    //	int linear_fit_classification_hebb(int *model, double *inputs, int inputsSize,int inputSize,int iterationNumber, double step);
    //
    //
    //	int linear_fit_classification_rosenblatt(int *model, double *inputs, int inputsSize, int inputSize,double *outputs, int outputsSize,int iterationNumber, double step);
    //
    //	bool learn_classification_rosenblatt(int *model, bool expected_result, const double* inputs, int inputSize, int learning_rate);
    //	bool get_result(int *model, const double* inputs, int inputSize);
    //
    //	double dot_product(const std::vector<double> &v1,
    //		const std::vector<double> &v2);
    //
    //
    //	double linear_classify(double *model, double *input, int inputSize);
    //
    //	double linear_predict(double *model, double *input, int inputSize);
    //



