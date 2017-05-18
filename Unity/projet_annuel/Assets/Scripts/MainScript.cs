﻿using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;


public class MainScript : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public static int iterationNumber = 10000000;
    public static double step = 0.5;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

	public Color marblecolor;
	public Color blue = Color.blue;
	public Color red = Color.red;
	public Color green = Color.green;

	/// <summary>
	/// Indique si un algorithme est en cours d'exécution
	/// </summary>
	private bool _isRunning = false;

	/// <summary>
	/// Méthode utilisée pour gérer les informations et 
	/// boutons de l'interface utilisateur
	/// </summary>
	public void OnGUI()
	{
		// Démarrage d'une liste de composants visuels verticale
		GUILayout.BeginVertical();

		if (GUILayout.Button("Create Model")) {
			if (!_isRunning) {
				create_model();
			}
		}
		if (GUILayout.Button("Erase Model")) {
			if (!_isRunning) {
				erase_model();
			}
		}
		if (GUILayout.Button("Linear_fit_classification_hebb")){
			if (!_isRunning)
			{
				linear_fit_classification_hebb();
			}
		}

		if (GUILayout.Button("Linear_fit_classification_rosenblatt")) {
			if (!_isRunning) {
				StartCoroutine ("linear_fit_classification_rosenblatt");
			}
		}
		if (GUILayout.Button("Classify")) {
			if (!_isRunning)
			{
				classify();
			}
		}
		if (GUILayout.Button("Linear_fit_regression")) {
			if (!_isRunning)
			{
				linear_fit_regression();
			}
		}
		if (GUILayout.Button("Predict")) {
			if (!_isRunning)
			{
				predict();
			}
		}
		if (GUILayout.Button("Test")) {
			if (!_isRunning)
			{
				StartCoroutine("test");
			}
		}
		// Fin de la liste de composants visuels verticale
		GUILayout.EndVertical();
	}

	public void create_model(){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			model = LibWrapperMachineLearning.linear_create_model (1, inputSize);
			Debug.Log ("Model created !" + model);
		} else {
			Debug.Log ("A model has been created, please delete it if you want to create another one ");
		}
		_isRunning = false;
	}	

	public void create_model(int nbCouche){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			model = LibWrapperMachineLearning.linear_create_model (nbCouche, inputSize);
			Debug.Log ("Model created !" + model);
		} else {
			Debug.Log ("A model has been created, please delete it if you want to create another one ");
		}
		_isRunning = false;
	}

	public void erase_model(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			LibWrapperMachineLearning.linear_remove_model (model);
			Debug.Log ("Model removed !");
		} else {
			Debug.Log ("There is no model in memory");
		}
		_isRunning = false;
	}
	public void classify(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			generateBaseTest (baseTest, 10);
			Debug.Log ("génère une base de test de 10x10 boules");
			Debug.Log ("Classify baseTest");
			double[] input = new double[inputSize];
			foreach(var unityObject in baseTest){
				getInput (unityObject, input);
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
		_isRunning = false;
	}

	public void predict(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			generateBaseTest (baseTest, 10);
			double[] input = new double[inputSize];
			Debug.Log ("Starting predicting outputs of baseTest...");
			foreach (var data in baseTest){
				getInput (data, input);

				var inputPtr = default(GCHandle);
				try
				{
					inputPtr = GCHandle.Alloc(input, GCHandleType.Pinned);
					data.position = new Vector3(data.position.x, (float) LibWrapperMachineLearning.linear_predict (model, inputPtr.AddrOfPinnedObject(), inputSize), data.position.z);
				}
				finally
				{
					if (inputPtr.IsAllocated) inputPtr.Free();
				}
			}
		} else {
			Debug.Log ("Aucun modèle en mémoire");
		}
		_isRunning = false;
	}
	public void linear_fit_regression(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {

			Debug.Log ("linear_fit_regression");
			double[] inputs = new double[inputSize * baseApprentissage.Length];
			double[] outputs = new double[baseApprentissage.Length];
			getInputsOutputs (baseApprentissage, inputs, outputs, false);
			// Création des pointeurs
			var inputsPtr = default(GCHandle);
			var outputsPtr = default(GCHandle);
			int learningResponse;
			try
			{
				inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
				outputsPtr = GCHandle.Alloc(outputs, GCHandleType.Pinned);
				Debug.Log("Start learning regression with baseApprentissage...");
				learningResponse = LibWrapperMachineLearning.linear_fit_classification_hebb(model, inputsPtr.AddrOfPinnedObject(), inputSize * baseApprentissage.Length, inputSize, outputsPtr.AddrOfPinnedObject(), iterationNumber, step);
				if(learningResponse == -1){
					Debug.Log("C++ >Aucun modèle en mémoire<");
				}else if(learningResponse == 0){
					Debug.Log("Learning stop by iterations");
				} else{
					Debug.Log("Learning stop beacause all case were correctly classified");
				}
			}
			finally
			{
				if (inputsPtr.IsAllocated) inputsPtr.Free();
				if (outputsPtr.IsAllocated) outputsPtr.Free();
			}
		} else {
			Debug.Log ("Aucun modèle en mémoire");
		}
		_isRunning = false;
	}	

	public void linear_fit_classification_hebb(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			Debug.Log ("linear_fit_classification_hebb");
			double[] inputs = new double[inputSize * baseApprentissage.Length];
			double[] outputs = new double[baseApprentissage.Length];
			getInputsOutputs (baseApprentissage, inputs, outputs, false);
			// Création des pointeurs
			var inputsPtr = default(GCHandle);
			var outputsPtr = default(GCHandle);
			int learningResponse;
			try
			{
				inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
				outputsPtr = GCHandle.Alloc(outputs, GCHandleType.Pinned);
				Debug.Log("Start learning classification hebb with baseApprentissage...");
				learningResponse = LibWrapperMachineLearning.linear_fit_classification_hebb(model, inputsPtr.AddrOfPinnedObject(), inputSize * baseApprentissage.Length, inputSize, outputsPtr.AddrOfPinnedObject(), iterationNumber, step);
				if(learningResponse == -1){
					Debug.Log("C++ >Aucun modèle en mémoire<");
				}else if(learningResponse == 0){
					Debug.Log("Learning stop by iterations");
				} else{
					Debug.Log("Learning stop beacause all case were correctly classified");
				}
			}
			finally
			{
				if (inputsPtr.IsAllocated) inputsPtr.Free();
				if (outputsPtr.IsAllocated) outputsPtr.Free();
			}
		} else {
			Debug.Log ("Aucun modèle en mémoire");
		}
		_isRunning = false;
	}


	public void linear_fit_classification_rosenblatt(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			Debug.Log ("linear_fit_classification_rosenblatt");
			double[] inputs = new double[inputSize * baseApprentissage.Length];
			double[] outputs = new double[baseApprentissage.Length];
			getInputsOutputs (baseApprentissage, inputs, outputs, false);
			// Création des pointeurs
			var inputsPtr = default(GCHandle);
			var outputsPtr = default(GCHandle);
			int learningResponse;
			try
			{
				inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
				outputsPtr = GCHandle.Alloc(outputs, GCHandleType.Pinned);
				learningResponse = LibWrapperMachineLearning.linear_fit_classification_rosenblatt(model, inputsPtr.AddrOfPinnedObject(), inputSize * baseApprentissage.Length, inputSize, outputsPtr.AddrOfPinnedObject(), iterationNumber, step);
				if(learningResponse == -1){
					Debug.Log("C++ >Aucun modèle en mémoire<");
				}else if(learningResponse == 0){
					Debug.Log("Leaning stop by iterations");
				} else{
					Debug.Log("Learning stop beacause all case were correctly classified");
				}
			}
			finally
			{
				if (inputsPtr.IsAllocated) inputsPtr.Free();
				if (outputsPtr.IsAllocated) outputsPtr.Free();
			}
		} else {
			Debug.Log ("Aucun modèle en mémoire");
		}
		_isRunning = false;
	}

	private void getInput(Transform objetUnity, double[] input){
		input[0] = objetUnity.position.x;
		input[1] = objetUnity.position.z;
	}
	// Rempli le tableau input passé en paramètre avec les coordonnées x et y de l'objetsUnity
	private void getInputs(Transform[] objetsUnity, double[] inputs){
		if (inputs.Length < objetsUnity.Length * 2) {
			Debug.Log ("Error : the array inputs given to the function isn't big enough");
		} else {
			int i;
			for(i=0; i<objetsUnity.Length*2; i+=2) {
				inputs [i] = objetsUnity[i/2].position.x;
				inputs [i+1] = objetsUnity[i/2].position.z;
			}
		}
	}
//	// Rempli le tableau inputs passé en paramètre avec les coordonnées x et y du tableau d'objetsUnity
//	// ainsi que le tableau outputs avec les coordonées z du tableau d'objetsUnity
	private void getInputsOutputs(Transform[] objetsUnity, double[] inputs, double[] outputs, bool color){
		int i = 0, j = 0;
		foreach (var data in objetsUnity)
		{
			inputs[i] = data.position.x;
			i++;
			inputs[i] = data.position.z;
			i++;
			if (color) {
				if (data.GetComponent<Renderer> ().material.color == blue) {
					outputs [j] = -1;
				} else {
					outputs [j] = 1 ;
				}
			} else {
				outputs [j] = data.position.y;
			}
			j++;
		}
	}
	// TEST FUNCTION		
	public void test(){
		_isRunning = true;
		Debug.Log("LAUNCHING TEST FUNCTION");

		double[] test1 = {1,2,2,3,3,4};
		serialiseInputs (test1);
		double[] test2 = {2,3,4,5,6,7,8,9};
		serialiseInputs (test2);
		double[] test3 = {-12,0,24,3};
		serialiseInputs (test3);

		Debug.Log ("Test 1");
		foreach (var test in test1) {
			Debug.Log (">" + test + "<");
		}
		Debug.Log ("Test 2");
		foreach (var test in test2) {
			Debug.Log (">" + test + "<");
		}
		Debug.Log ("Test 2");
		foreach (var test in test3) {
			Debug.Log (">" + test + "<");
		}

//		generateLinear ();
//		Debug.Log ("sleeping 2sec...");
////		Thread.Sleep (2000);
//
//		Debug.Log("DEBUG baseApprentissage >" + baseApprentissage.Length + "<");
//		Debug.Log("DEBUG baseTest >" + baseTest.Length + "<");
//
//		double[] inputs = new double[inputSize * baseApprentissage.Length];
//        double[] outputs = new double[baseApprentissage.Length];
//
//		getInputsOutputs (baseApprentissage, inputs, outputs, false);
//
//        int inputsSize = inputs.Length;
//
//		// Create model
//		create_model();
//
////		// Création des pointeurs
//		var inputsPtr = default(GCHandle);
//		var outputsPtr = default(GCHandle);
//        try
//        {
//            inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
//            outputsPtr = GCHandle.Alloc(outputs, GCHandleType.Pinned);
//			Debug.Log("Learning hebb to model ! step > "  + step);
//			LibWrapperMachineLearning.linear_fit_classification_hebb(model, inputsPtr.AddrOfPinnedObject(), inputsSize, inputSize, outputsPtr.AddrOfPinnedObject(), iterationNumber, step);
//		}
//        finally
//        {
//            if (inputsPtr.IsAllocated) inputsPtr.Free();
//            if (outputsPtr.IsAllocated) outputsPtr.Free();
//        }
//
//		Debug.Log("Generating testBase !");        
//        double[] input = new double[inputSize];
//		generateBaseTest (baseTest, 10);
//
//		foreach (var data in baseTest)
//        {
//			var inputPtr = default(GCHandle);
//			getInputs (data, input);
//            try
//			{
//				inputPtr = GCHandle.Alloc(input, GCHandleType.Pinned);
//				data.position = new Vector3(data.position.x, (float) LibWrapperMachineLearning.linear_classify(model, inputPtr.AddrOfPinnedObject(), inputSize), data.position.z);
//            }
//            finally
//            {
//                if (inputPtr.IsAllocated) inputPtr.Free();
//            }
//        }
//		erase_model();
		Debug.Log ("Test Function finished !");
		_isRunning = false;
    }
	// Place a square with equally reparted marbles 
	// separation gives the number of marble by size f the square
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
//				Debug.Log ("Boule positionnée en (" + x + ";" + data.position.y + ";" + z + ")"); 
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

	System.Random rdm = new System.Random ();
	float generateRdm(double a, double b){
		return (float) (a + rdm.NextDouble () * (b - a));
	}

	public void generateLinear(){
		foreach (var data in baseApprentissage) {
			var z = generateRdm (-1, 1);
			if (z < 0) {
				data.position = new Vector3(generateRdm(-1,1), -1, z);
				data.GetComponent<Renderer> ().material.color = blue;
			} else {
				data.position = new Vector3(generateRdm(-1,1), 1, z);
				data.GetComponent<Renderer> ().material.color = red;
			}
		}
	}

	// Transforme les inputs pour certains cas non linérement séparable mais séparables par leur carré
	public void transformInputs(double[] inputs){
		for (int i = 0; i < inputs.Length; i++) {
			inputs[i] *= inputs[i];
		}
//		foreach(var input in inputs){
//			input *= input;
//		}
	}

	// Use the min / max method to serialise inputs
	public int serialiseInputs(double[] inputs){
		double minX = inputs[0], maxX = inputs[0];
		double minZ = inputs[1], maxZ = inputs[1];
		for (int i = 2; i < inputs.Length; i += 2) {
			if(inputs[i] < minX) { minX = inputs[i]; }
			if(inputs[i] > maxX) { maxX = inputs[i]; }
		}
		for (int i = 3; i < inputs.Length; i += 2) {
			if(inputs[i] < minZ) { minZ = inputs[i]; }
			if(inputs[i] > maxZ) { maxZ = inputs[i]; }
		}
		for (int i = 2; i < inputs.Length; i += 2) {
			inputs[i] = -1 + 2 * (inputs [i] - minX) / (maxX - minX);
		}
		for (int i = 3; i < inputs.Length; i += 2) {
			inputs[i] = -1 + 2 * (inputs [i] - minZ) / (maxZ - minZ);
		}
		return 0;
	}



}