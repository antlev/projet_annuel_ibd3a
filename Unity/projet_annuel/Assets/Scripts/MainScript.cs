using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;


public class MainScript : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public static int iterationNumber = 100000;
	public static double step = 0.3f;
	public static double learning_rate = 0.5f;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

	public static string nbColorButtonString = "Change to 2 colors";
	public static bool testWithColor = true;
    public static string colorButtonString = "Use height";
    public Color marblecolor;
	public Color blue = Color.blue;
	public Color red = Color.red;
	public Color green = Color.green;
	public static int colorBlue = 0;
	public static int colorRed = 1;
	public static int colorGreen = 2;

	public static int nbColor = 3;

	public static bool transformInput = false;
	public static string transformButtonString = "Use Transformation";
	public static int nbOutputNeuron = 6;

	public Camera cam1;
	public Camera cam2;

	/// <summary>
	/// Indique si un algorithme est en cours d'exécution
	/// </summary>
	private bool _isRunning = false;

	public void Start(){
		cam1.enabled = true;
		cam2.enabled = false;
	}
	/// <summary>
	/// Méthode utilisée pour gérer les informations et 
	/// boutons de l'interface utilisateur
	/// </summary>
	public void OnGUI()
	{
		// Démarrage d'une liste de composants visuels verticale
		GUILayout.BeginVertical();

		step = GUI.VerticalScrollbar(new Rect(1000, 25, 10, 500), (float) step, 0.08F, 1.08F, 0.0F);
		iterationNumber = (int) GUI.VerticalScrollbar(new Rect(1025, 25, 250, 500), iterationNumber, 0.1F, 10000, 0);
		nbOutputNeuron = (int) GUI.HorizontalScrollbar(new Rect(15, 420, 175, 50), nbOutputNeuron, 0.1F, 1, 20);
		GUI.TextArea (new Rect (15, 440, 175, 90), "Si vous touchez au nombre de neurones de sortie, effacez le model puis recréez en un nouveau. Merci");
		GUI.TextArea (new Rect (950, 475, 40, 30), " step");
		GUI.TextArea (new Rect (1050, 475, 75, 30), "iterations");



		if (GUILayout.Button("Create Model")) {
			if (!_isRunning) {
				create_linear_model();
			}
		}
		if (GUILayout.Button("Erase Model")) {
			if (!_isRunning) {
				erase_model();
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
        if (GUILayout.Button("Switch Cam")) {
			if (!_isRunning) {

				if (cam1.enabled == true) {

					cam1.enabled = false;
					cam2.enabled = true;
				}else{
					cam1.enabled = true;
					cam2.enabled = false;		
				}
			}
		}
		GUILayout.TextArea ("     step >" + step + "<");
		GUILayout.TextArea ("     iterations >" + iterationNumber + "<");
		GUILayout.TextArea ("     size of input >" + inputSize + "<");
		GUILayout.TextArea ("     size of output >" + nbOutputNeuron + "<");
		if (testWithColor) {
			if (GUILayout.Button (nbColorButtonString)) {
				if (nbColor == 2) {
					nbColor = 3;
					nbColorButtonString = "Change to 2 colors";
				} else {
					nbColor = 2;
					nbColorButtonString = "Change to 3 colors";
				}
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
	public void create_linear_model(){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			model = LibWrapperMachineLearning.linear_create_model (inputSize, nbOutputNeuron);
			Debug.Log ("Model created !");
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
			Debug.Log ("Classification with a test base of " + baseTest.Length + " marbles");
			double[] input = new double[inputSize];
			double[] outputs = new double[nbOutputNeuron];
			foreach(var unityObject in baseTest){
				getInput (unityObject, input);
                if (testWithColor) {
					LibWrapperMachineLearning.linear_classify (model, input, inputSize, outputs, nbOutputNeuron);
					for (int i = 0; i < outputs.Length; i++) {
						if(outputs[i] == 1){
							if(i%nbColor == colorBlue){
								unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.blue;
							}else if(i%nbColor == colorRed){
								unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.red;
							}else{
								unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.green;
							}
							// Just to position the balls somewhere we can see them
		                    unityObject.position = new Vector3(unityObject.position.x, 2, unityObject.position.z);
						}
					}
                }
                else {
					unityObject.position = new Vector3(unityObject.position.x, (float)LibWrapperMachineLearning.linear_classify(model, input, inputSize, outputs, nbOutputNeuron), unityObject.position.z);
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
				outputs[i] = LibWrapperMachineLearning.linear_predict (model, input, inputSize);
				i++;

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
			getInputsOutputs (baseApprentissage, inputs, outputs, nbOutputNeuron);
			int learningResponse;
			Debug.Log("Start learning regression with baseApprentissage...");
			learningResponse = LibWrapperMachineLearning.linear_fit_regression(model, inputs, inputSize * baseApprentissage.Length, inputSize, outputs, iterationNumber, learning_rate);
			if(learningResponse == -1){
				Debug.Log("C++ >Aucun modèle en mémoire<");
			}else if(learningResponse == 0){
				Debug.Log("Learning stop by iterations");
			} else{
				Debug.Log("Learning stop beacause all case were correctly classified");
			}
		} else {
			Debug.Log ("Aucun modèle en mémoire");
		}
		_isRunning = false;
	}	
	public void linear_fit_classification_rosenblatt(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			Debug.Log ("linear_fit_classification_rosenblatt with a learning base of " + baseApprentissage.Length + " marbles");
			double[] inputs = new double[inputSize * baseApprentissage.Length];
			double[] outputs = new double[baseApprentissage.Length * nbOutputNeuron];
			getInputsOutputs (baseApprentissage, inputs, outputs, nbOutputNeuron);
			int learningResponse;
			learningResponse = LibWrapperMachineLearning.linear_fit_classification_rosenblatt(model, inputs, inputSize * baseApprentissage.Length, inputSize, outputs, nbOutputNeuron, iterationNumber, step);
			if(learningResponse == -1){
				Debug.Log("C++ >Aucun modèle en mémoire<");
			}else if(learningResponse == 0){
				Debug.Log("Leaning stop by iterations");
			} else{
				Debug.Log("Learning stop beacause all case were correctly classified");
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
	private void getInputsOutputs(Transform[] objetsUnity, double[] inputs, double[] outputs, int outputSize){
		int i = 0, j = 0;
		foreach (var data in objetsUnity)
		{
			inputs[i] = data.position.x;
			i++;
			inputs[i] = data.position.z;
			i++;
			if (testWithColor) {
				if (data.GetComponent<Renderer> ().material.color == UnityEngine.Color.blue) {
					for (int k = 0; k < nbOutputNeuron; k++) {
						if (k % nbColor == colorBlue) {
							outputs [j+k] = 1;
						} else {
							outputs [j+k] = -1;
						}
					}
				} else if(data.GetComponent<Renderer> ().material.color == UnityEngine.Color.red){
					for (int k = 0; k < nbOutputNeuron; k++) {
						if (k % nbColor == colorRed) {
							outputs [j+k] = 1;
						} else {					
							outputs [j+k] = -1;
						}
					}
				} else{
					for (int k = 0; k < nbOutputNeuron; k++) {
						if (k % nbColor == colorGreen) {
							outputs [j+k] = 1;
						} else {
							outputs [j+k] = -1;
						}
					}
				}
			} else {
				outputs [j] = data.position.y;
			}
			j += outputSize;
		}
		if (transformInput) {
			transformInputs (inputs);
		}
	}

	// Transforme les inputs pour certains cas non linérement séparable mais séparables par leur carré
	private void transformInputs(double[] inputs){
		for (int i = 0; i < inputs.Length; i++) {
			inputs[i] *= inputs[i];
		}
		for (int i = 0; i < inputs.Length; i++) {
			inputs[i] *= inputs[i];
		}
	}

	// Use the min / max method to serialise inputs
	private int serialiseData(double[] data){
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
	private int serialiseData2(double[] data){
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
	private void test(){
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
	private void generateBaseTest(Transform[] testObject, int separation){
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
		}
	}

	System.Random rdm = new System.Random ();
	private float generateRdm(double a, double b){
		return (float) (a + rdm.NextDouble () * (b - a));
	}

	private void generateLinear(){
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

	private void generateRegression(){
		float nbBallPerCote = 7.0f;
		float step = (2.0f / nbBallPerCote);
		if (baseApprentissage.Length == nbBallPerCote * nbBallPerCote) {
			float x = 1.0f;
			float z = 1.0f;
			float y = 1.0f;
			for (int i = 0; i < nbBallPerCote; i++) {
				for (int j = 0; j < nbBallPerCote; j++) {
					baseApprentissage[i*(int)nbBallPerCote+j].position = new Vector3 (x, y, z);
					x -= step;
				}
				y -= step;
				z -= step;
				x = 1.0f;
			}
		} else {
			Debug.Log ("baseApprentissage ne contien pas le bon nombre d'objets");
		}
	}


}