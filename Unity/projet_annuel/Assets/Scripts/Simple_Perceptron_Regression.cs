using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;

public class Simple_Perceptron_Regression : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public Transform[] baseApprentissage;
    public Transform[] baseTest;

	public static bool transformInput = false;
	public static string transformButtonString = "Use Transformation";

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

        if (GUILayout.Button("Create Model and fit regression"))
        {
            if (!_isRunning)
            {
                linearCreateAndFitRegression();
            }
        }
        if (GUILayout.Button("Predict Test Set"))
        {
            if (!_isRunning)
            {
                linearPredictTestSet();
            }
        }

		GUILayout.TextArea ("     size of input >" + inputSize + "<");

		// Fin de la liste de composants visuels verticale
		GUILayout.EndVertical();
	}
	void clean(){
		foreach (var data in baseTest) {
			data.position = new Vector3(data.position.x, 5, data.position.z);
		}
	}
	public void linearCreateAndFitRegression(){
		_isRunning = true;
		if (model == System.IntPtr.Zero) {
			double[] inputs = new double[baseApprentissage.Length*inputSize];
			double[] expectedOutputs = new double[baseApprentissage.Length];
			getInputsOutputs(baseApprentissage, inputs, expectedOutputs);
			model = LibWrapperMachineLearning.linearCreateAndFitRegression (inputs, baseApprentissage.Length, inputSize, expectedOutputs, 1);
			Debug.Log ("Model created and fit !");
		} else {
			Debug.Log ("A model has been created, please delete it if you want to create another one ");
		}
		_isRunning = false;
	}	

	public void linearPredictTestSet(){
		_isRunning = true;
		if (model != System.IntPtr.Zero) {
			generateBaseTest (baseTest, 15);
			Debug.Log ("Predict with a test base of " + baseTest.Length + " marbles");
			double[] input = new double[inputSize];
			double[] output = new double[1];
			System.IntPtr pRes;
			foreach(var unityObject in baseTest){
				getInput (unityObject, input);
				pRes = LibWrapperMachineLearning.linearPredict (model, input);
    			Marshal.Copy (pRes, output, 0, 1);
                unityObject.position = new Vector3(unityObject.position.x, (float) output[0] , unityObject.position.z);
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
			outputs[j] = data.position.y;
			j++;
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
