using System.Runtime.InteropServices;

public class LibWrapperMachineLearning
{

#if UNITY_EDITOR_WIN || UNITY_STANDALONE_WIN
	[DllImport("test")] 
	public static extern int test(System.IntPtr test, int testSize);

	[DllImport("test")] 
	public static extern int toto();

	[DllImport("Cours1ML-280317-FirstDll")] 
    public static extern int return42();

    [DllImport("Cours1ML-280317-FirstDll")] 
    public static extern System.IntPtr linear_create_model(int inputDimension);

    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern void linear_remove_model(System.IntPtr model);

    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int linear_fit_regression(System.IntPtr model, System.IntPtr inputs, int inputsSize, int inputSize, System.IntPtr outputs, int outputsSize);

    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int linear_fit_classification_hebb(System.IntPtr model, System.IntPtr inputs, int inputsSize, int inputSize, int iterationNumber, double step);

    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int linear_fit_classification_rosenblatt(System.IntPtr model, System.IntPtr inputs, int inputsSize, int inputSize, System.IntPtr outputs, int outputsSize, int iterationNumber, double step);

    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern double linear_classify(System.IntPtr model, System.IntPtr input, int inputSize);
    
    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern double linear_predict(System.IntPtr model, System.IntPtr input, int inputSize);

    [DllImport("Cours1ML-280317-FirstDll")]
    public static extern int test();
#endif

    //lib pour par recoder l'inversion de matrice eigen

    //inputDimension -> pointeur sur un tableau de x paramétre plus 1 (x, y, neurones de biais)
    //inputSize -> taille du tableau nombre de caractere passée 
    //outpsSize -> taille du tableau de sortie 
    //iterationNumber -> le nombre d'iiteration a effectuér
    //step -> 
}