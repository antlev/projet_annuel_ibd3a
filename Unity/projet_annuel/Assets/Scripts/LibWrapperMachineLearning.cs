using System.Runtime.InteropServices;

public class LibWrapperMachineLearning
{

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
    // Linear Perceptron
    [DllImport("Dll-Machine-Learning")]
    public static extern System.IntPtr createLinearModelClassif(int inputDimension, int outputDimension);
    [DllImport("Dll-Machine-Learning")]
    public static extern void eraseLinearModel(System.IntPtr pmodel);
        // Classification
    [DllImport("Dll-Machine-Learning")]
    public static extern int linearFitClassificationRosenblatt(System.IntPtr pmodel, double[] inputs, int inputsSize, double[] expectedOutputs, int iterationMax, double step);
    [DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr linearClassify(System.IntPtr pmodel, double[] input);
        // Regression
    [DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr linearCreateAndFitRegression(double[] inputs, int inputsSize, int inputSize, double[] expectedOutputs, int outputSize);
    [DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr linearPredict(System.IntPtr pmodel, double[] input);

    // MLP
    [DllImport("Dll-Machine-Learning")]
    public static extern System.IntPtr createMlp(int[] structure, int nbLayer);
    [DllImport("Dll-Machine-Learning")]
    public static extern void eraseMlp(System.IntPtr pMLP);
        // Classification
    [DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr classify(System.IntPtr pMLP, double[] oneInput);
    [DllImport("Dll-Machine-Learning")]
    public static extern void fitClassification(System.IntPtr pMLP, double[] inputs, int inputsSize, double[] expectedOutputs);
        // Regression
    [DllImport("Dll-Machine-Learning")]
    public static extern void fitRegression(System.IntPtr pMLP, double[] inputs, int inputsSize, double[] expectedOutputs);
    [DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr predict(System.IntPtr pMLP, double[] oneInput);

    // NAIVE RBF
	[DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr createNaiveRbfModel(int nbExamples, double[] gamma, double[] inputs, int inputSize, double[] expectedOutputs);
	[DllImport("Dll-Machine-Learning")]
	public static extern double getNaiveRbfResponseClassif(System.IntPtr pNaiveRBF, double[] input);	
	[DllImport("Dll-Machine-Learning")]
	public static extern double getNaiveRbfResponseRegression(System.IntPtr pNaiveRBF, double[] input);

	//  RBF
	[DllImport("Dll-Machine-Learning")]
	public static extern System.IntPtr createRbfModel(int nbExamples, double[] gamma, double[] inputs, int inputSize, double[] expectedOutputs, int nbRepresentatives);
    [DllImport("Dll-Machine-Learning")]
	public static extern double getRbfResponseClassif(System.IntPtr pRBF, double[] input);
    [DllImport("Dll-Machine-Learning")]
    public static extern double getRbfResponseRegression(System.IntPtr pRBF, double[] input);

    // TEST
    [DllImport("Dll-Machine-Learning")]
    public static extern int return42();
    [DllImport("Dll-Machine-Learning")]
    public static extern System.IntPtr createToto();
    [DllImport("Dll-Machine-Learning")]
    public static extern int getTiti(System.IntPtr toto);
#endif

}