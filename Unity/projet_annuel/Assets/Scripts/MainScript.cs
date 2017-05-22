using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;


public class MainScript : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public static int iterationNumber = 1000;
	public static double step = 0.5;
	public static double learning_rate = 0.5;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

    public static bool testWithColor = true;
    public static string colorButtonString = "Use height";
    public Color marblecolor;
	public Color blue = Color.blue;
	public Color red = Color.red;
	public Color green = Color.green;

	public static bool transformInput = false;
	public static string transformButtonString = "Use Transformation";


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
		if (GUILayout.Button("Clean")) {
			if (!_isRunning)
			{
				clean();
			}
		}
        if (GUILayout.Button(colorButtonString))
        {
            if (!_isRunning)
            {
                if (testWithColor)
                {
                    testWithColor = false;
                    colorButtonString = "Use color";
                }
                else
                {
                    testWithColor = true;
                    colorButtonString = "Use height";
                }
            }
        }
		if (GUILayout.Button(transformButtonString))
        {
            if (!_isRunning)
            {
				if (transformInput)
				{
					transformInput = false;
					transformButtonString = "Use Transformation";
				}
				else
				{
					transformInput = true;
					transformButtonString = "Don't Use Transformation";
				}            }
        }
        if (GUILayout.Button("Test")) {
			if (!_isRunning)
			{
				test();
			}
		}
		// Fin de la liste de composants visuels verticale
		GUILayout.EndVertical();
	}
	void clean(){
		foreach (var data in baseTest) {
			data.position = new Vector3(data.position.x, 5, data.position.z);
		}
	}
	public void create_model(){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			model = LibWrapperMachineLearning.linear_create_model (inputSize);
			Debug.Log ("Model created !" + model);
		} else {
			Debug.Log ("A model has been created, please delete it if you want to create another one ");
		}
		_isRunning = false;
	}	

	public void create_model(int nbCouche){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			model = LibWrapperMachineLearning.linear_create_model (inputSize);
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
			model = System.IntPtr.Zero;
			Debug.Log ("Model removed !" + model);
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
                    if (testWithColor) {
                        if((float)LibWrapperMachineLearning.linear_classify(model, inputPtr.AddrOfPinnedObject(), inputSize) == 1) {
							unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.red;
                        } else {
							unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.blue;
                        }
                        unityObject.position = new Vector3(unityObject.position.x, 0, unityObject.position.z);
                    }
                    else {
                        unityObject.position = new Vector3(unityObject.position.x, (float)LibWrapperMachineLearning.linear_classify(model, inputPtr.AddrOfPinnedObject(), inputSize), unityObject.position.z);
                    }
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
			double[] outputs = new double[baseTest.Length];
			Debug.Log ("Starting predicting outputs of baseTest...");
			int i = 0;


			foreach (var data in baseTest){
				getInput (data, input);

				var inputPtr = default(GCHandle);
				try
				{
					inputPtr = GCHandle.Alloc(input, GCHandleType.Pinned);
					outputs[i] = LibWrapperMachineLearning.linear_predict (model, inputPtr.AddrOfPinnedObject(), inputSize);
					i++;

				}
				finally
				{
					if (inputPtr.IsAllocated) inputPtr.Free();
				}
			}
			serialiseData2 (outputs);
			for (i = 0; i < 100; i++) {

				baseTest[i].position = new Vector3 (baseTest[i].position.x, (float)outputs [i], baseTest[i].position.z);

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
			getInputsOutputs (baseApprentissage, inputs, outputs);
			// Création des pointeurs
			var inputsPtr = default(GCHandle);
			var outputsPtr = default(GCHandle);
			int learningResponse;
			try
			{
				inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
				outputsPtr = GCHandle.Alloc(outputs, GCHandleType.Pinned);
				Debug.Log("Start learning regression with baseApprentissage...");
				learningResponse = LibWrapperMachineLearning.linear_fit_regression(model, inputsPtr.AddrOfPinnedObject(), inputSize * baseApprentissage.Length, inputSize, outputsPtr.AddrOfPinnedObject(), iterationNumber, learning_rate);
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
			getInputs (baseApprentissage, inputs);
			// Création des pointeurs
			var inputsPtr = default(GCHandle);
			int learningResponse;
			try
			{
				inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
				Debug.Log("Start learning classification hebb with baseApprentissage...");
				learningResponse = LibWrapperMachineLearning.linear_fit_classification_hebb(model, inputsPtr.AddrOfPinnedObject(), inputSize * baseApprentissage.Length, inputSize, iterationNumber, step);
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
			getInputsOutputs (baseApprentissage, inputs, outputs);
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
		if (transformInput) {
			transformInputs (input);
		}
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
		if (transformInput) {
			transformInputs (inputs);
		}
	}
//	// Rempli le tableau inputs passé en paramètre avec les coordonnées x et y du tableau d'objetsUnity
//	// ainsi que le tableau outputs avec les coordonées z du tableau d'objetsUnity
	private void getInputsOutputs(Transform[] objetsUnity, double[] inputs, double[] outputs){
		int i = 0, j = 0;
		foreach (var data in objetsUnity)
		{
			inputs[i] = data.position.x;
			i++;
			inputs[i] = data.position.z;
			i++;
			if (testWithColor) {
				if (data.GetComponent<Renderer> ().material.color == UnityEngine.Color.blue) {
					outputs [j] = -1;
				} else {
					outputs [j] = 1 ;
				}
			} else {
				outputs [j] = data.position.y;
			}
			j++;
		}
		if (transformInput) {
			transformInputs (inputs);
		}
	}

	// Transforme les inputs pour certains cas non linérement séparable mais séparables par leur carré
	public void transformInputs(double[] inputs){
		for (int i = 0; i < inputs.Length; i++) {
			inputs[i] *= inputs[i];
		}
		for (int i = 0; i < inputs.Length; i++) {
			inputs[i] *= inputs[i];
		}


	}

	// Use the min / max method to serialise inputs
	public int serialiseData(double[] data){
		double minX = data[0], maxX = data[0];
		double minZ = data[1], maxZ = data[1];
		for (int i = 2; i < data.Length; i += 2) {
			if(data[i] < minX) { minX = data[i]; }
			if(data[i] > maxX) { maxX = data[i]; }
		}
		for (int i = 3; i < data.Length; i += 2) {
			if(data[i] < minZ) { minZ = data[i]; }
			if(data[i] > maxZ) { maxZ = data[i]; }
		}
		Debug.Log ("min x :" + minX + " max x :" + maxX + " min z :" + minZ + " max z" + maxZ);
		for (int i = 0; i < data.Length; i += 2) {
			data[i] = (float) (-1.0 + 2.0 * (double) ( (double) (data [i] - minX) / (double) (maxX - minX)));
		}
		for (int i = 1; i < data.Length; i += 2) {
			data[i] = (float) (-1.0 + 2.0 * (double) ( (double) (data [i] - minZ) / (double) (maxZ - minZ)));
		}
		return 0;
	}
	// Use the min / max method to serialise inputs
	public int serialiseData2(double[] data){
		double minX = data[0], maxX = data[0];
		for (int i = 1; i < data.Length; i ++) {
			if(data[i] < minX) { minX = data[i]; }
			if(data[i] > maxX) { maxX = data[i]; }
		}
		for (int i = 0; i < data.Length; i ++) {
			data[i] = (float) (-1.0 + 2.0 * (double) ( (double) (data [i] - minX) / (double) (maxX - minX)));
		}
		return 0;
	}
	// TEST FUNCTION		
	public void test(){
		_isRunning = true;
		Debug.Log("LAUNCHING TEST FUNCTION");

//		Debug.Log("DEBUG baseApprentissage >" + baseApprentissage.Length + "<");
//		Debug.Log("DEBUG baseTest >" + baseTest.Length + "<");
//
//		double[] inputs = new double[inputSize * baseApprentissage.Length];
//        double[] outputs = new double[baseApprentissage.Length];
//
//		getInputs (baseApprentissage, inputs);
//		Debug.Log ("before");
//		foreach (var data in inputs) {
//
//			Debug.Log (data);
//		}
//		double[] inputs = new double[] {10, 1, 7.5, 2, 5, 3, 2.5, 4, 0, 5};
//		serialiseInputs (inputs);
//		Debug.Log ("after");
//
//		foreach (var data in inputs) {
//				
//			Debug.Log (data);
//		}
//        int inputsSize = inputs.Length;
//
//		// Create model
//		create_model();
//
////		// Création des pointeurs
//		var inputsPtr = default(GCHandle);
//        try
//        {
//            inputsPtr = GCHandle.Alloc(inputs, GCHandleType.Pinned);
//			Debug.Log("Learning hebb to model ! step > "  + step);
//			LibWrapperMachineLearning.linear_fit_classification_hebb(model, inputsPtr.AddrOfPinnedObject(), inputsSize, inputSize, iterationNumber, step);
//		}
//        finally
//        {
//            if (inputsPtr.IsAllocated) inputsPtr.Free();
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




}