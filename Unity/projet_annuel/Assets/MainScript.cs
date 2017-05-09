using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;

public class MainScript : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public static int iterationNumber = 100;
    public static int step = 1;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

	public void create_model(){
		if (model == null) {
			model = LibWrapperMachineLearning.linear_create_model (inputSize);
			Debug.Log ("Model created !" + model);
		} else {
			Debug.Log ("A model has been created, please delete it if you want to create another one" + model);
		}
	}	
	public void erase_model(){
		if (model != null) {
			LibWrapperMachineLearning.linear_remove_model (model);
			Debug.Log ("Model removed !");
		} else {
			Debug.Log ("There is no model in memory");
		}
	}
	public void classify(Transform[] UnitytObjects){
		if (model != null) {
			Debug.Log ("Classify");
			double[] input = new double[inputSize];
			foreach(var unityObject in UnitytObjects){
				getInputs (baseTest, input);
				var inputPtr = default(GCHandle);
				try
				{
					inputPtr = GCHandle.Alloc(input, GCHandleType.Pinned);
					unityObject.position = new Vector3 (unityObject.position.x, (float) LibWrapperMachineLearning.linear_classify (model, inputPtr.AddrOfPinnedObject(), inputSize), unityObject.position.z);
				}
				finally
				{
					if (inputPtr.IsAllocated) inputPtr.Free();
				}
			}
		} else {
			Debug.Log ("Aucun modèle en mémoire");
		}
	}
//	public void predict(){
//		if (model != null) {
//			Debug.Log ("Predict");
//			Debug.Log ("to be implemented");
//			double[] inputs = new double[inputSize * baseTest.Length];
//			getInputs (baseTest, inputs);
//			int i = 0;
//			foreach (var input in inputs){
////				baseTest[i].position.y = LibWrapperMachineLearning.linear_predict (model, input, inputSize);
//				i++;
//			}
//		} else {
//			Debug.Log ("Aucun modèle en mémoire");
//		}
//	}
//	public void linear_fit_regression(){
//		if (model != null) {
//			Debug.Log ("linear_fit_regression");
//			Debug.Log ("to be implemented");
//			double[] inputs = new double[inputSize * baseApprentissage.Length];
//			getInputs (baseApprentissage, inputs);
//
//		} else {
//			Debug.Log ("Aucun modèle en mémoire");
//		}
//	}	
//
//	public void linear_fit_classification_hebb(){
//		if (model != null) {
//			Debug.Log ("linear_fit_classification_hebb");
//			Debug.Log ("to be implemented");
//			double[] inputs = new double[inputSize * baseApprentissage.Length];
//			getInputs (baseApprentissage, inputs);
////			LibWrapperMachineLearning.linear_fit_classification_hebb (model, inputs, inputs.Length, inputSize, iterationNumber, step);
//		} else {
//			Debug.Log ("Aucun modèle en mémoire");
//		}
//	}
//
//
//    public void linear_fit_classification_rosenblatt()
//    {
//		if (model != null) {
//			Debug.Log ("linear_fit_classification_rosenblatt");
//			Debug.Log ("to be implemented");
//			double[] inputs = new double[inputSize * baseApprentissage.Length];
//			double[] outputs = new double[inputSize * baseApprentissage.Length];
//			getInputsOutputs (baseApprentissage, inputs, outputs);
//
////			LibWrapperMachineLearning.linear_fit_classification_rosenblatt (model, inputs, inputs.Length, inputSize, outputs, outputs.Length, iterationNumber, step);
//		} else {
//			Debug.Log ("Aucun modèle en mémoire");
//		}
//	}

	// Rempli le tableau inputs passé en paramètre avec les coordonnées x et y de l'objetsUnity
	private void getInputs(Transform[] objetsUnity, double[] inputs){
		int i = 0;
		foreach (var data in objetsUnity)
		{
			inputs[i] = data.position.x;
			i++;
			inputs[i] = data.position.z;
			i++;
		}
	}
	// Rempli le tableau inputs passé en paramètre avec les coordonnées x et y du tableau d'objetsUnity
	// ainsi que le tableau outputs avec les coordonées z du tableau d'objetsUnity
	private void getInputsOutputs(Transform[] objetsUnity, double[] inputs, double[] outputs){
		int i = 0, j = 0;
		foreach (var data in objetsUnity)
		{
			inputs[i] = data.position.x;
			i++;
			inputs[i] = data.position.z;
			i++;
			outputs[j] = data.position.y;
			j++;
		}
	}


	public void test(){

		LibWrapperMachineLearning.return42 ();

		Debug.Log("LAUNCHING TEST FUNCTION");
		Debug.Log("DEBUG Test C++ function return 42 >" + LibWrapperMachineLearning.return42() + "<");
		Debug.Log("DEBUG baseApprentissage >" + baseApprentissage.Length + "<");
		Debug.Log("DEBUG baseTest >" + baseTest.Length + "<");

		double[] inputs = new double[inputSize * baseApprentissage.Length];
        double[] outputs = new double[inputSize * baseApprentissage.Length];
        int i = 0;
        int j = 0;
		// Remplissage input output
        foreach (var data in baseApprentissage)
        {
            inputs[i] = data.position.x;
            i++;
            inputs[i] = data.position.z;
            i++;
            outputs[j] = data.position.y;
			j++;
        }
        int inputsSize = inputs.Length;
        int outputsSize = outputs.Length;

		// Create model
		create_model();

		// Création des poiteurs
		var inputsPtr = default(GCHandle);
		var outputsPtr = default(GCHandle);
        try
        {
            inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
            outputsPtr = GCHandle.Alloc(outputs, GCHandleType.Pinned);

//			Debug.Log("Learning Rosenblatt to model ! step > "  + step);
//		    LibWrapperMachineLearning.linear_fit_classification_rosenblatt(model, gchX.AddrOfPinnedObject(), inputsSize, inputSize, gchY.AddrOfPinnedObject(), outputsSize, iterationNumber, step);
			Debug.Log("Learning hebb to model ! step > "  + step);
		    LibWrapperMachineLearning.linear_fit_classification_hebb(model, inputsPtr.AddrOfPinnedObject(), inputsSize, inputSize,1000,1, outputsPtr.AddrOfPinnedObject(), outputsSize);
		}
        finally
        {
            if (inputsPtr.IsAllocated) inputsPtr.Free();
            if (outputsPtr.IsAllocated) outputsPtr.Free();
        }

		Debug.Log("Generating testBase !");        
        double[] input = new double[inputSize];
		generateBaseTest (baseTest, 5);

        i = 0;
		foreach (var data in baseTest)
        {
            input[i] = data.position.x;
            i++;
            input[i] = data.position.z;
			var inputsPtr2 = default(GCHandle);
            try
			{
                inputsPtr2 = GCHandle.Alloc(inputs, GCHandleType.Pinned);
				data.position = new Vector3(data.position.x, (float) LibWrapperMachineLearning.linear_classify(model, inputsPtr2.AddrOfPinnedObject(), inputSize), data.position.z);
                Debug.Log("Position x : " + data.position.x + " z : " + data.position.z + " y : " + data.position.y);
            }
            finally
            {
                if (inputsPtr2.IsAllocated) inputsPtr2.Free();
                if (outputsPtr.IsAllocated) outputsPtr.Free();
            }
            i = 0;
        }

// Test avec la base d'apprentissage a nouveau
//        foreach (var data in baseApprentissage)
//        {
//            input[i] = data.position.x;
//            i++;
//            input[i] = data.position.z;
//
//            var ghX = default(GCHandle);
//            try
//            {
//                ghX = GCHandle.Alloc(inputs, GCHandleType.Pinned);
////				data.position.Set(data.position.x, (float)LibWrapperMachineLearning.linear_classify(model, ghX.AddrOfPinnedObject(), inputSize), data.position.z);
//				data.position = new Vector3(data.position.x, (float)LibWrapperMachineLearning.linear_classify(model, ghX.AddrOfPinnedObject(), inputSize), data.position.z);
//                Debug.Log("Position x : "+ data.position.x + " z : " + data.position.z + " y : " + data.position.y);
//            }
//            finally
//            {
//                if (gchX.IsAllocated) gchX.Free();
//                if (gchY.IsAllocated) gchY.Free();
//            }
//            i = 0;
//        }

        LibWrapperMachineLearning.linear_remove_model(model);
		Debug.Log ("Test Function finished !");


    }

	public void generateBaseTest(Transform[] testObject, int separation){
		if (testObject.Length != separation * separation) {
			Debug.Log ("Le nombre d'objet envoyé ne correspond pas à la séparation demandée");
		} else {
			var x = -1.0f;
			var z = -1.0f;
			int count = 0;
			foreach (var data in testObject) {
				data.position = new Vector3(x,data.position.y, z);
				count++;
				Debug.Log ("Boule positionnée en (" + x + ";" + data.position.y + ";" + z + ")"); 
				if (count % separation == 0) {
					x = -1.0f;
					z += 2.0f / separation;
				} else {
					x += 2.0f / separation;
				}
			}
			Debug.Log ("Les boules de test sont placé selon une sépartion de " + separation + "x" + separation + " sur les axes (x;z)");

		}
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



