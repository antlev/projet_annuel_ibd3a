using System.Runtime.InteropServices;

public class LibWrapperMachineLearning
{

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN

	[DllImport("Cours1ML-280317-FirstDll")] 
    public static extern int return42();

    [DllImport("Cours1ML-280317-FirstDll")] 
	public static extern System.IntPtr linear_create_model(int inputDimension, int outputDimension);

	[DllImport("Cours1ML-280317-FirstDll")]
    public static extern void linear_remove_model(System.IntPtr model);

    [DllImport("Cours1ML-280317-FirstDll")]
	public static extern System.IntPtr linear_fit_regression(double[] inputs, int inputsSize, int inputSize, double[] outputs, int outputSize);

    [DllImport("Cours1ML-280317-FirstDll")]
	public static extern int linear_fit_classification_hebb(System.IntPtr model, double[] inputs, int inputsSize, int inputSize,  int iterationNumber, double step);

    [DllImport("Cours1ML-280317-FirstDll")]
	public static extern int linear_fit_classification_rosenblatt(System.IntPtr model, double[] inputs, int inputsSize, int inputSize, double[] outputs, int outputSize, int iterationNumber, double step);

    [DllImport("Cours1ML-280317-FirstDll")]
	public static extern double linear_classify(System.IntPtr model, double[] input, int inputSize, double[] expectedOutputs, int outputDimension);
    
    [DllImport("Cours1ML-280317-FirstDll")]
	public static extern void linearPredict(System.IntPtr model, double[] input, int inputSize, double[] output, int outputSize);

	[DllImport("Cours1ML-280317-FirstDll")]
    public static extern int test();
#endif

}