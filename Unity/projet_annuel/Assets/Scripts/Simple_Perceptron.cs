using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;

public class Simple_Perceptron : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public static int iterationNumber = 100000;
	public static double step = 0.3f;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

	public Color blue = Color.blue;
	public Color red = Color.red;
	public Color green = Color.green;
	public static int colorBlue = 0;
	public static int colorRed = 1;
	public static int colorGreen = 2;

	public int nbColor;

	public static bool transformInput = false;
	public static string transformButtonString = "Use Transformation";

	public Camera cam1;

	/// <summary>
	/// Indique si un algorithme est en cours d'exécution
	/// </summary>
	private bool _isRunning = false;

	public void Start(){
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
		GUI.TextArea (new Rect (950, 475, 40, 30), " step");
		GUI.TextArea (new Rect (1050, 475, 75, 30), "iterations");


        if (GUILayout.Button("TEST"))
        {
            if (!_isRunning)
            {
                executeAllTests();
            }
        }
        if (GUILayout.Button("Create Model for classification"))
        {
            if (!_isRunning)
            {
                createLinearModelClassif();
            }
        }
        if (GUILayout.Button("Erase Model for Classification"))
        {
            if (!_isRunning)
            {
                eraseLinearModel();
            }
        }
        if (GUILayout.Button("Learn Rosenblatt Classification"))
        {
            if (!_isRunning)
            {
                fitClassificationRosenblatt();
            }
        }
        if (GUILayout.Button("Classify Test Set"))
        {
            if (!_isRunning)
            {
                classifyTestSet();
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
		GUILayout.TextArea ("     step >" + step + "<");
		GUILayout.TextArea ("     iterations >" + iterationNumber + "<");
		GUILayout.TextArea ("     size of input >" + inputSize + "<");
		GUILayout.TextArea ("     size of output >" + nbColor + "<");


		// Fin de la liste de composants visuels verticale
		GUILayout.EndVertical();
	}
	void clean(){
		foreach (var data in baseTest) {
			data.position = new Vector3(data.position.x, 5, data.position.z);
		}
	}
	public void createLinearModelClassif(){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			model = LibWrapperMachineLearning.createLinearModelClassif (inputSize, nbColor);
			Debug.Log ("Model created !");
		} else {
			Debug.Log ("A model has been created, please delete it if you want to create another one ");
		}
		_isRunning = false;
	}	
	public void eraseLinearModel(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			LibWrapperMachineLearning.eraseLinearModel (model);
			model = System.IntPtr.Zero;
			Debug.Log ("Model removed !" + model);
		} else {
			Debug.Log ("There is no model in memory");
		}
		_isRunning = false;
	}
	public void fitClassificationRosenblatt(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			double[] inputs = new double[baseApprentissage.Length*inputSize];
			double[] expectedOutputs = new double[baseApprentissage.Length*nbColor];
			getInputsOutputs(baseApprentissage, inputs, expectedOutputs);
			int returnValue = LibWrapperMachineLearning.linearFitClassificationRosenblatt (model, inputs, baseApprentissage.Length, expectedOutputs, iterationNumber, step);
			if(returnValue == 1){
				Debug.Log ("Learning stop by iterations");
			}else{
				Debug.Log ("Learning stop because the error on learning set has reach 0");			
			}
		} else {
			Debug.Log ("There is no model in memory");
		}
		_isRunning = false;
	}

	public void classifyTestSet(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			generateBaseTest (baseTest, 15);
			Debug.Log ("Classification with a test base of " + baseTest.Length + " marbles");
			double[] input = new double[inputSize];
			double[] output = new double[nbColor];
			System.IntPtr pRes;
			foreach(var unityObject in baseTest){
				getInput (unityObject, input);
				pRes = LibWrapperMachineLearning.linearClassify (model, input);
    			Marshal.Copy (pRes, output, 0, nbColor);
				for (int i = 0; i < output.Length; i++) {
					if(output[i] == 1){
						if(i%nbColor == colorBlue){
							unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.blue;
						}else if(i%nbColor == colorRed){
							unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.red;
						}else{
							unityObject.GetComponent<Renderer>().material.color = UnityEngine.Color.green;
						}
						// Just to position the balls somewhere we can see them
	                    unityObject.position = new Vector3(unityObject.position.x, 0, unityObject.position.z);
					}
				}
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
		if (inputs.Length < objetsUnity.Length * inputSize) {
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
			if (data.GetComponent<Renderer> ().material.color == UnityEngine.Color.blue) {
				for (int k = 0; k < nbColor; k++) {
					if (k % nbColor == colorBlue) {
						outputs [j+k] = 1;
					} else {
						outputs [j+k] = -1;
					}
				}
			} else if(data.GetComponent<Renderer> ().material.color == UnityEngine.Color.red){
				for (int k = 0; k < nbColor; k++) {
					if (k % nbColor == colorRed) {
						outputs [j+k] = 1;
					} else {					
						outputs [j+k] = -1;
					}
				}
			} else{
				for (int k = 0; k < nbColor; k++) {
					if (k % nbColor == colorGreen) {
						outputs [j+k] = 1;
					} else {
						outputs [j+k] = -1;
					}
				}
			}
			j += nbColor;
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
    // TEST FUNCTION
    private void executeAllTests(){
        int code;
        if(return42Test() == 0){
            Debug.Log("Test >return42< : OK");
        }else{
            Debug.Log("Test >return42< : FAILED");
        }
        if(objectTest() == 0){
            Debug.Log("Test >object< : OK");
        }else{
            Debug.Log("Test >object< : FAILED");
        }
        if(simplePerceptronClassificationTest() == 0){
            Debug.Log("Test >simplePerceptronClassification< : OK");
        }else{
            Debug.Log("Test >simplePerceptronClassification< : FAILED");
        }
    }
    private int return42Test(){
        if(LibWrapperMachineLearning.return42() == 42){
            return 0;
        }
        return 1;
    }
    private int objectTest(){
        System.IntPtr toto = System.IntPtr.Zero;
        toto = LibWrapperMachineLearning.createToto();
        int titi = LibWrapperMachineLearning.getTiti(toto);
        int[] test = new int[2] { 2, 2 };
        if(toto != System.IntPtr.Zero && titi == 42){
            return 0;
        }
        return 1;
    }
    private int simplePerceptronClassificationTest(){
        int linear_inputSize = 2;
        int linear_outputSize = 1;
        int linear_nbData = 4;
        double[] linear_inputs = new double[linear_inputSize * linear_nbData];
        double[] linear_input = new double[linear_inputSize];
        double[] linear_outputs = new double[linear_outputSize * linear_nbData];

        int linear_maxIterations = 10000;
        double linear_step = 0.01;

        linear_inputs[0] = 0;
        linear_inputs[1] = 0;
        linear_outputs[0] = -1;

        linear_inputs[2] = 0;
        linear_inputs[3] = 1;
        linear_outputs[1] = -1;

        linear_inputs[4] = 1;
        linear_inputs[5] = 1;
        linear_outputs[2] = 1;

        linear_inputs[6] = 1;
        linear_inputs[7] = 0;
        linear_outputs[3] = 1;

        System.IntPtr linearModel = LibWrapperMachineLearning.createLinearModelClassif(linear_inputSize, linear_outputSize);
        LibWrapperMachineLearning.linearFitClassificationRosenblatt(linearModel, linear_inputs, linear_nbData * linear_inputSize, linear_outputs, linear_maxIterations, linear_step);

        System.IntPtr pRes;
        double[] res = new double[linear_outputSize];
        linear_input[0] = 0;
        linear_input[1] = 0;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != -1){ return 1; }
        linear_input[0] = 0;
        linear_input[1] = 1;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != -1){ return 1; }
        linear_input[0] = 1;
        linear_input[1] = 1;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != 1){ return 1; }
        linear_input[0] = 1;
        linear_input[1] = 0;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != 1){ return 1; }
        return 0;
    }
}
