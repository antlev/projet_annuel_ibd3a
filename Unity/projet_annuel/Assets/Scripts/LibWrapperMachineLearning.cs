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
    public static extern void classify(System.IntPtr pMLP, double[] oneInput, int inputSize);
    [DllImport("Dll-Machine-Learning")]
    public static extern void fitClassification(System.IntPtr pMLP, double[] inputs, int inputSize, int inputsSize, double[] expectedOutputs, int outputSize);
    [DllImport("Dll-Machine-Learning")]
    public static extern double getOutputsforClassif(System.IntPtr pMLP);
        // Regression
    [DllImport("Dll-Machine-Learning")]
    public static extern void fitRegression(System.IntPtr pMLP, double[] inputs, int inputSize, int inputsSize, double[] expectedOutputs, int outputSize);
    [DllImport("Dll-Machine-Learning")]
    public static extern void predict(System.IntPtr pMLP, double[] oneInput, int inputSize);
    [DllImport("Dll-Machine-Learning")]
    public static extern double getOutputsforRegression(System.IntPtr pMLP);

    // RBF
    [DllImport("Dll-Machine-Learning")]
    public static extern System.IntPtr createRbfModel(int nbRepresentatives);
    [DllImport("Dll-Machine-Learning")]
    public static extern void naiveLearnModel(System.IntPtr pRBF, int nbExamples, double gamma, double[] X, int inputSize, double[] Y);
    [DllImport("Dll-Machine-Learning")]
    public static extern void rbfLearnModel(System.IntPtr pRBF, int nbExamples, double gamma, double[] X, int inputSize, double[]  Y);
    [DllImport("Dll-Machine-Learning")]
    public static extern void getRbfResponse(System.IntPtr pRBF, double gamma, double[] input, int inputSize, double[] output, double[] X, int nbExamples);



    // OLD LIB
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern System.IntPtr linear_create_model(int inputDimension, int outputDimension);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern void linear_remove_model(System.IntPtr model);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int linear_fit_regression(System.IntPtr model, double[] inputs, int inputsSize, int inputSize, double[] outputs, int nb_iterations_max, double learning_rate);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int linear_fit_classification_hebb(System.IntPtr model, double[] inputs, int inputsSize, int inputSize, int iterationNumber, double step);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int linear_fit_classification_rosenblatt(System.IntPtr model, double[] inputs, int inputsSize, int inputSize, double[] outputs, int outputSize, int iterationNumber, double step);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern double linear_classify(System.IntPtr model, double[] input, int inputSize, double[] expectedOutputs, int outputDimension);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern double linear_predict(System.IntPtr model, double[] input, int inputSize);
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int test();
    [DllImport("Dll-Machine-Learning")]
    public static extern int return42();
    [DllImport("Dll-Machine-Learning")]
    public static extern System.IntPtr createToto();
    [DllImport("Dll-Machine-Learning")]
    public static extern int getTiti(System.IntPtr toto);
#endif

}