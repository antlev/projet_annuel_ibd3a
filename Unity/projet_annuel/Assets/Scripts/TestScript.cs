using UnityEngine;
using System.Runtime.InteropServices;
using System.Threading;


public class TestScript : MonoBehaviour {

	public static System.IntPtr model;
	public static int inputSize = 2;

    public static int iterationNumber = 100000;
	public static double step = 0.3f;

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


        if (GUILayout.Button("Test all")){
            if (!_isRunning){
                executeAllTests();
            }
        }
        if (GUILayout.Button("Test pipe")){
            if (!_isRunning){
		        if(return42Test() == 0 && objectTest() == 0){
		            Debug.Log("Test pipe : OK");
		        }else{
		            Debug.Log("Test pipe : FAILED");
		        }    
		    }
        }
        if (GUILayout.Button("Test simple perceptron classification")){
            if (!_isRunning){
		        if(simplePerceptronClassificationTest(1) == 0){
		            Debug.Log("Test >simplePerceptronClassification< : OK");
		        }else{
		            Debug.Log("Test >simplePerceptronClassification< : FAILED");
		        }            
		    }
        }
        if (GUILayout.Button("Test MLP Classification")){
            if (!_isRunning){
		        if(mlpClassificationTest(1) == 0){
		            Debug.Log("Test >mlpClassification< : OK");
		        }else{
		            Debug.Log("Test >mlpClassification< : FAILED" );
		        }            
	    	}
        }
        if (GUILayout.Button("Test MLP Regression")){
            if (!_isRunning){
		        if((mlpRegressionTest(1)) == 0){
		            Debug.Log("Test >mlpRegression< : OK");
		        }else{
		            Debug.Log("Test >mlpRegression< : FAILED");
		        }       
	    	}
        }
        if (GUILayout.Button("Test naive RBF Classification"))
        {
            if (!_isRunning){
		        if(naiveRbfClassificationTest(1) == 0){
		            Debug.Log("Test >naiveRBFClassification< : OK");
		        }else{
		            Debug.Log("Test >naiveRBFClassification< : FAILED");
		        }       
          	}
        }
        if (GUILayout.Button("Test naive RBF Regression"))
        {
            if (!_isRunning){
		 		if(naiveRbfRegressionTest(1) == 0){
		            Debug.Log("Test >naiveRBFRegression< : OK");
		        }else{
		            Debug.Log("Test >naiveRBFRegression< : FAILED");
		        }            
		    }
        }
        if (GUILayout.Button("Test RBF Classification"))
        {
            if (!_isRunning){
		        if(rbfClassificationTest(1) == 0){
		            Debug.Log("Test >RBFClassification< : OK");
		        }else{
		            Debug.Log("Test >RBFClassification< : FAILED");
		        }  
	        }
        }    
		GUILayout.TextArea ("     step >" + step + "<");
		GUILayout.TextArea ("     iterations >" + iterationNumber + "<");
		GUILayout.TextArea ("     size of input >" + inputSize + "<");

		// Fin de la liste de composants visuels verticale
		GUILayout.EndVertical();
	}

    // TEST FUNCTION
    private void executeAllTests(){
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
        if(simplePerceptronClassificationTest(0) == 0){
            Debug.Log("Test >simplePerceptronClassification< : OK");
        }else{
            Debug.Log("Test >simplePerceptronClassification< : FAILED");
        }
        if(mlpClassificationTest(0) == 0){
            Debug.Log("Test >mlpClassification< : OK");
        }else{
            Debug.Log("Test >mlpClassification< : FAILED");
        }
        if((mlpRegressionTest(0)) == 0){
            Debug.Log("Test >mlpRegression< : OK");
        }else{
            Debug.Log("Test >mlpRegression< : FAILED");
        }
        if(naiveRbfClassificationTest(0) == 0){
            Debug.Log("Test >naiveRBFClassification< : OK");
        }else{
            Debug.Log("Test >naiveRBFClassification< : FAILED");
        }
        if(naiveRbfRegressionTest(0) == 0){
            Debug.Log("Test >naiveRBFRegression< : OK");
        }else{
            Debug.Log("Test >naiveRBFRegression< : FAILED");
        }
        if(rbfClassificationTest(0) == 0){
            Debug.Log("Test >RBFClassification< : OK");
        }else{
            Debug.Log("Test >RBFClassification< : FAILED");
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
        if(toto != System.IntPtr.Zero && titi == 42){
            return 0;
        }
        return 1;
    }
    private int simplePerceptronClassificationTest(int debug){
        int linear_inputSize = 2;
        int linear_outputSize = 1;
        int linear_nbData = 4;
        double[] linear_inputs = new double[linear_inputSize * linear_nbData];
        double[] linear_input = new double[linear_inputSize];
        double[] linear_outputs = new double[linear_outputSize * linear_nbData];

        int linear_maxIterations = 10000;
        double linear_step = 0.01;
        int debugFailure = 0;

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
        if(res[0] != -1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+linear_input[0]+";"+linear_input[1]+"] = "+res[0] +"  expected -1");
	            debugFailure=1;
    		}else{
    			return 1;
    		}
    	}
        linear_input[0] = 0;
        linear_input[1] = 1;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != -1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+linear_input[0]+";"+linear_input[1]+"] = "+res[0] +"  expected -1");
	            debugFailure=1;		
    		}else{
    			return 1;
    		}
    	}        
    	linear_input[0] = 1;
        linear_input[1] = 1;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != 1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+linear_input[0]+";"+linear_input[1]+"] = "+res[0] +"  expected 1");
	            debugFailure=1;		
			}else{
    			return 1;
    		}
    	}        
    	linear_input[0] = 1;
        linear_input[1] = 0;
        pRes = LibWrapperMachineLearning.linearClassify(linearModel, linear_input);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        Marshal.Copy (pRes, res, 0, linear_outputSize);
        if(res[0] != 1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+linear_input[0]+";"+linear_input[1]+"] = "+res[0] +"  expected 1");
	            debugFailure=1;		
    		}else{
    			return 1;
    		}
    	}  
    	if(	debugFailure == 1){
    		return 1;
    	}else{
    		return 0;
    	}
    }


    private int mlpClassificationTest(int debug){
        int testClassifMLP_nbLayer = 3;
        int[] testClassifMLP_modelStruct = new int[3] { 2, 2, 1 };
        int testClassifMLP_inputSize = 2;
        int testClassifMLP_outputSize = 1;
        int testClassifMLP_nbData = 4;
        double[] testClassifMLP_output = new double[testClassifMLP_outputSize];

        double[] testClassifMLP_inputs = new double[testClassifMLP_inputSize * testClassifMLP_nbData];
        double[] testClassifMLP_expectedOutputs = new double[testClassifMLP_outputSize * testClassifMLP_nbData];
        double[] testClassifMLP_oneInput = new double[testClassifMLP_inputSize];
        System.IntPtr pRes;
        int debugFailure = 0;

        System.IntPtr mlpModel = LibWrapperMachineLearning.createMlp(testClassifMLP_modelStruct, testClassifMLP_nbLayer);

        testClassifMLP_inputs[0] = 0;
        testClassifMLP_inputs[1] = 0;

        testClassifMLP_inputs[2] = 0;
        testClassifMLP_inputs[3] = 1;

        testClassifMLP_inputs[4] = 1;
        testClassifMLP_inputs[5] = 1;

        testClassifMLP_inputs[6] = 1;
        testClassifMLP_inputs[7] = 0;

        testClassifMLP_expectedOutputs[0] = 1;
        testClassifMLP_expectedOutputs[1] = 1;
        testClassifMLP_expectedOutputs[2] = -1;
        testClassifMLP_expectedOutputs[3] = -1;

        LibWrapperMachineLearning.fitClassification(mlpModel, testClassifMLP_inputs, testClassifMLP_inputSize * testClassifMLP_nbData,
                                          testClassifMLP_expectedOutputs);
        testClassifMLP_oneInput[0] = 0;
        testClassifMLP_oneInput[1] = 0;
        pRes = LibWrapperMachineLearning.classify(mlpModel, testClassifMLP_oneInput);
        Marshal.Copy (pRes, testClassifMLP_output, 0, testClassifMLP_outputSize);
        if(testClassifMLP_output[0] != 1){
        	if(debug == 1){
	            Debug.Log("response for input ["+testClassifMLP_oneInput[0]+";"+testClassifMLP_oneInput[1]+"] = "+testClassifMLP_output[0] +"  expected 1");
	            debugFailure=1;
        	} else{
	        	return 1; 
        	}
        }
        testClassifMLP_oneInput[0] = 0;
        testClassifMLP_oneInput[1] = 1;
        pRes = LibWrapperMachineLearning.classify(mlpModel, testClassifMLP_oneInput);
        Marshal.Copy (pRes, testClassifMLP_output, 0, testClassifMLP_outputSize);
        if(testClassifMLP_output[0] != 1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+testClassifMLP_oneInput[0]+";"+testClassifMLP_oneInput[1]+"] = "+testClassifMLP_output[0] +"  expected 1");
	            debugFailure=1;
        	} else{
	        	return 1; 
        	}
        }
        testClassifMLP_oneInput[0] = 1;
        testClassifMLP_oneInput[1] = 1;
        pRes = LibWrapperMachineLearning.classify(mlpModel, testClassifMLP_oneInput);
        Marshal.Copy (pRes, testClassifMLP_output, 0, testClassifMLP_outputSize);
        if(testClassifMLP_output[0] != -1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+testClassifMLP_oneInput[0]+";"+testClassifMLP_oneInput[1]+"] = "+testClassifMLP_output[0] +"  expected -1");
	            debugFailure=1;
        	} else{
	        	return 1; 
        	}
        }
        testClassifMLP_oneInput[0] = 1;
        testClassifMLP_oneInput[1] = 0;
        pRes = LibWrapperMachineLearning.classify(mlpModel, testClassifMLP_oneInput);
        Marshal.Copy (pRes, testClassifMLP_output, 0, testClassifMLP_outputSize);
        if(testClassifMLP_output[0] != -1){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+testClassifMLP_oneInput[0]+";"+testClassifMLP_oneInput[1]+"] = "+testClassifMLP_output[0] +"  expected -1");
	            debugFailure=1;
        	} else{
	        	return 1; 
        	}
        }
        return debugFailure;
    }

    private double mlpRegressionTest(int debug){
        int testRegressionMLP_nbLayer = 3;
        int[] testRegressionMLP_modelStruct = new int[3] { 2, 2, 1 };
        int testRegressionMLP_inputSize = 2;
        int testRegressionMLP_outputSize = 1;
        int testRegressionMLP_nbData = 3;

        double[] testRegressionMLP_inputs = new double[testRegressionMLP_inputSize * testRegressionMLP_nbData];
        double[] testRegressionMLP_expectedOutputs = new double[testRegressionMLP_outputSize * testRegressionMLP_nbData];
        double[] testRegressionMLP_oneInput = new double[testRegressionMLP_inputSize];
        double[] testRegressionMLP_oneOutput = new double[testRegressionMLP_outputSize];
        System.IntPtr pRes;
        int debugFailure=0;

        System.IntPtr testRegressionMLP = LibWrapperMachineLearning.createMlp(testRegressionMLP_modelStruct, testRegressionMLP_nbLayer);

        testRegressionMLP_inputs[0] = 0;
        testRegressionMLP_inputs[1] = 0;

        testRegressionMLP_inputs[2] = 0;
        testRegressionMLP_inputs[3] = 1;

        testRegressionMLP_inputs[4] = 1;
        testRegressionMLP_inputs[5] = 1;

        testRegressionMLP_expectedOutputs[0] = 0;
        testRegressionMLP_expectedOutputs[1] = 0;
        testRegressionMLP_expectedOutputs[2] = 0.5;

        LibWrapperMachineLearning.fitRegression(testRegressionMLP, testRegressionMLP_inputs, testRegressionMLP_nbData,
                                         testRegressionMLP_expectedOutputs);

        testRegressionMLP_oneInput[0] = 0;
        testRegressionMLP_oneInput[1] = 0;
        pRes = LibWrapperMachineLearning.predict(testRegressionMLP, testRegressionMLP_oneInput);
        Marshal.Copy (pRes, testRegressionMLP_oneOutput, 0, testRegressionMLP_outputSize);
        if(testRegressionMLP_oneOutput[0] < -0.001 || testRegressionMLP_oneOutput[0] > 0.001){ 
        	if(debug == 1){
        		Debug.Log("response for input ["+testRegressionMLP_oneInput[0]+";"+testRegressionMLP_oneInput[1]+"] = "+testRegressionMLP_oneOutput[0] +"  expected 0");
     			debugFailure=1;
    		}else{
           		   return 1; 
        		}
        }

        testRegressionMLP_oneInput[0] = 0;
        testRegressionMLP_oneInput[1] = 1;
        pRes = LibWrapperMachineLearning.predict(testRegressionMLP, testRegressionMLP_oneInput);
        Marshal.Copy (pRes, testRegressionMLP_oneOutput, 0, testRegressionMLP_outputSize);
        if(testRegressionMLP_oneOutput[0] < -0.001 || testRegressionMLP_oneOutput[0] > 0.001){ 
        	if(debug == 1){
        		Debug.Log("response for input ["+testRegressionMLP_oneInput[0]+";"+testRegressionMLP_oneInput[1]+"] = "+testRegressionMLP_oneOutput[0] +"  expected 0");
     			debugFailure=1;
    		}else{
            		return 2; 
        		}
        }

        
        testRegressionMLP_oneInput[0] = 1;
        testRegressionMLP_oneInput[1] = 1;
        pRes = LibWrapperMachineLearning.predict(testRegressionMLP, testRegressionMLP_oneInput);
        Marshal.Copy (pRes, testRegressionMLP_oneOutput, 0, testRegressionMLP_outputSize);
        if((testRegressionMLP_oneOutput[0]-0.5) > 0.1 ){ 
        	if(debug == 1){
	            Debug.Log("response for input ["+testRegressionMLP_oneInput[0]+";"+testRegressionMLP_oneInput[1]+"] = "+testRegressionMLP_oneOutput[0] +"  expected 0.5");
	            debugFailure=1;
            }else{
				return 3; 
            }
        }
        return debugFailure;
    }
    private int naiveRbfClassificationTest(int debug){
        int naiveRbfClassifTest_nbExamples = 4;
        double[] naiveRbfClassifTest_gamma = new double[naiveRbfClassifTest_nbExamples];
        int naiveRbfClassifTest_inputSize = 2;

        double[] naiveRbfClassifTest_inputs = new double[naiveRbfClassifTest_nbExamples * naiveRbfClassifTest_inputSize];
        double[] naiveRbfClassifTest_input = new double[naiveRbfClassifTest_inputSize];
        double naiveRbfOutput;
        double[] naiveRbfOutputs = new double[naiveRbfClassifTest_nbExamples];
        int debugFailure=0;

        for(int gamma=0;gamma<naiveRbfClassifTest_nbExamples;gamma++){
            naiveRbfClassifTest_gamma[gamma] = 0.1;
        }

        naiveRbfClassifTest_inputs[0] = 0;
        naiveRbfClassifTest_inputs[1] = 0;
        naiveRbfOutputs[0] = -1;

        naiveRbfClassifTest_inputs[2] = 0;
        naiveRbfClassifTest_inputs[3] = 1;
        naiveRbfOutputs[1] = -1;

        naiveRbfClassifTest_inputs[4] = 1;
        naiveRbfClassifTest_inputs[5] = 1;
        naiveRbfOutputs[2] = 1;

        naiveRbfClassifTest_inputs[6] = 1;
        naiveRbfClassifTest_inputs[7] = 0;
        naiveRbfOutputs[3] = 1;

        System.IntPtr naiveRbfModel = LibWrapperMachineLearning.createNaiveRbfModel(naiveRbfClassifTest_nbExamples, naiveRbfClassifTest_gamma, naiveRbfClassifTest_inputs, naiveRbfClassifTest_inputSize, naiveRbfOutputs);

        naiveRbfClassifTest_input[0] = 0;
        naiveRbfClassifTest_input[1] = 0;
        naiveRbfOutput = LibWrapperMachineLearning.getNaiveRbfResponseClassif(naiveRbfModel, naiveRbfClassifTest_input);
        if(naiveRbfOutput != -1){ 
            if(debug == 1){
            	Debug.Log("Response for input (" + naiveRbfClassifTest_input[0] + ";" + naiveRbfClassifTest_input[1] + ") : " + naiveRbfOutput + " - expected -1" );
            	debugFailure=1;
            }else{
            	return 1;
            }
         }
        naiveRbfClassifTest_input[0] = 0;
        naiveRbfClassifTest_input[1] = 1;
        naiveRbfOutput = LibWrapperMachineLearning.getNaiveRbfResponseClassif(naiveRbfModel, naiveRbfClassifTest_input);
        if(naiveRbfOutput != -1){ 
            if(debug == 1){
            	Debug.Log("Response for input (" + naiveRbfClassifTest_input[0] + ";" + naiveRbfClassifTest_input[1] + ") : " + naiveRbfOutput + " - expected -1" );
            	debugFailure=1;
            }else{
            	return 1;
            }
         }        
        naiveRbfClassifTest_input[0] = 1;
        naiveRbfClassifTest_input[1] = 1;
        naiveRbfOutput = LibWrapperMachineLearning.getNaiveRbfResponseClassif(naiveRbfModel, naiveRbfClassifTest_input);
        if(naiveRbfOutput != 1){ 
            if(debug == 1){
            	Debug.Log("Response for input (" + naiveRbfClassifTest_input[0] + ";" + naiveRbfClassifTest_input[1] + ") : " + naiveRbfOutput + " - expected 1" );
            	debugFailure=1;
            }else{
            	return 1;
            }
         }        
        naiveRbfClassifTest_input[0] = 1;
        naiveRbfClassifTest_input[1] = 0;
        naiveRbfOutput = LibWrapperMachineLearning.getNaiveRbfResponseClassif(naiveRbfModel, naiveRbfClassifTest_input);
        if(naiveRbfOutput != 1){ 
            if(debug == 1){
            	Debug.Log("Response for input (" + naiveRbfClassifTest_input[0] + ";" + naiveRbfClassifTest_input[1] + ") : " + naiveRbfOutput + " - expected 1" );
            	debugFailure=1;
            }else{
            	return 1;
            }
         }
        return debugFailure;
    }
    private int naiveRbfRegressionTest(int debug){
        int naiveRbfRegressionTest_nbExamples = 4;
        double[] naiveRbfRegressionTest_gamma = new double[naiveRbfRegressionTest_nbExamples];
        int naiveRbfRegressionTest_inputSize = 2;

        double[] naiveRbfRegressionTest_inputs = new double[naiveRbfRegressionTest_nbExamples * naiveRbfRegressionTest_inputSize];
        double[] naiveRbfRegressionTest_input = new double[naiveRbfRegressionTest_inputSize];
        double naiveRbfRegressionTest_output;
        double[] naiveRbfRegression_outputs = new double[naiveRbfRegressionTest_nbExamples];
        int debugFailure=0;

        for(int gamma=0;gamma<naiveRbfRegressionTest_nbExamples;gamma++){
            naiveRbfRegressionTest_gamma[gamma] = 0.1;
        }

        naiveRbfRegressionTest_inputs[0] = 0;
        naiveRbfRegressionTest_inputs[1] = 0;
        naiveRbfRegression_outputs[0] = 1;

        naiveRbfRegressionTest_inputs[2] = 0;
        naiveRbfRegressionTest_inputs[3] = 1;
        naiveRbfRegression_outputs[1] = 0.5;

        naiveRbfRegressionTest_inputs[4] = 1;
        naiveRbfRegressionTest_inputs[5] = 1;
        naiveRbfRegression_outputs[2] = -1;

        naiveRbfRegressionTest_inputs[6] = 1;
        naiveRbfRegressionTest_inputs[7] = 0;
        naiveRbfRegression_outputs[3] = -0.5;

        System.IntPtr naiveRbfModelRegression = LibWrapperMachineLearning.createNaiveRbfModel(naiveRbfRegressionTest_nbExamples, naiveRbfRegressionTest_gamma, naiveRbfRegressionTest_inputs, naiveRbfRegressionTest_inputSize, naiveRbfRegression_outputs);

        naiveRbfRegressionTest_input[0] = 0;
        naiveRbfRegressionTest_input[1] = 0;
        naiveRbfRegressionTest_output = LibWrapperMachineLearning.getNaiveRbfResponseRegression(naiveRbfModelRegression, naiveRbfRegressionTest_input);
        if((0.99 - naiveRbfRegressionTest_output) > 0.1){ 
            if(debug == 1){
            	Debug.Log("response for input ["+naiveRbfRegressionTest_input[0]+";"+naiveRbfRegressionTest_input[1]+"] = "+naiveRbfRegressionTest_output +"  expected 0.99...");
            	debugFailure=1;
            }else{
            	return 1; 
            }
        }
        naiveRbfRegressionTest_input[0] = 0;
        naiveRbfRegressionTest_input[1] = 1;
        naiveRbfRegressionTest_output = LibWrapperMachineLearning.getNaiveRbfResponseRegression(naiveRbfModelRegression, naiveRbfRegressionTest_input);
        if((0.49 - naiveRbfRegressionTest_output) > 0.1){ 
            if(debug == 1){
            	Debug.Log("response for input ["+naiveRbfRegressionTest_input[0]+";"+naiveRbfRegressionTest_input[1]+"] = "+naiveRbfRegressionTest_output +"  expected 0.49...");
            	debugFailure=1;
            }else{
            	return 2; 
            }
        }    
        naiveRbfRegressionTest_input[0] = 1;
        naiveRbfRegressionTest_input[1] = 1;
        naiveRbfRegressionTest_output = LibWrapperMachineLearning.getNaiveRbfResponseRegression(naiveRbfModelRegression, naiveRbfRegressionTest_input);
        if((-0.99 - naiveRbfRegressionTest_output) > 0.1){ 
            if(debug == 1){
            	Debug.Log("response for input ["+naiveRbfRegressionTest_input[0]+";"+naiveRbfRegressionTest_input[1]+"] = "+naiveRbfRegressionTest_output +"  expected -0.99...");
            	debugFailure=1;
            }else{
            	return 3; 
            }
        }               
        naiveRbfRegressionTest_input[0] = 1;
        naiveRbfRegressionTest_input[1] = 0;
        naiveRbfRegressionTest_output = LibWrapperMachineLearning.getNaiveRbfResponseRegression(naiveRbfModelRegression, naiveRbfRegressionTest_input);
        if((-0.49 - naiveRbfRegressionTest_output) > 0.1){ 
            if(debug == 1){
            	Debug.Log("response for input ["+naiveRbfRegressionTest_input[0]+";"+naiveRbfRegressionTest_input[1]+"] = "+naiveRbfRegressionTest_output +"  expected -0.49...");
            	debugFailure=1;
            }else{
            	return 4; 
            }
        }            
        return debugFailure;
    }

    private int rbfClassificationTest(int debug){
    	Debug.Log("Test rbfClassificationTest TODO");
        int rbfClassifTest_nbExamples = 6;
        int rbfClassifTest_nbRepresentatives = 2;
        double[] rbfClassifTest_gamma = new double[rbfClassifTest_nbRepresentatives];
        // for(int gamma=0;gamma<rbfClassifTest_nbRepresentatives;gamma++){
        //     rbfClassifTest_gamma[gamma] = 0.1;
        // }

        rbfClassifTest_gamma[0] = 0.1;
        rbfClassifTest_gamma[1] = 0.1;

        int rbfClassifTest_inputSize = 2;
        double[] rbfClassifTest_inputs = new double[rbfClassifTest_nbExamples * rbfClassifTest_inputSize];
        double[] rbfClassifTest_input = new double[rbfClassifTest_inputSize];
        double[] rbfClassifTest_outputs = new double[rbfClassifTest_nbExamples];
        double rbfClassifTest_output;
        int debugFailure=0;

        rbfClassifTest_inputs[0] = 0.15;
        rbfClassifTest_inputs[1] = 0.75;
        rbfClassifTest_outputs[0] = 1;

        rbfClassifTest_inputs[2] = 0.2;
        rbfClassifTest_inputs[3] = 0.8;
        rbfClassifTest_outputs[1] = 1;

        rbfClassifTest_inputs[4] = 0.25;
        rbfClassifTest_inputs[5] = 0.9;
        rbfClassifTest_outputs[2] = 1;

        rbfClassifTest_inputs[6] = 0.5;
        rbfClassifTest_inputs[7] = 0.2;
        rbfClassifTest_outputs[3] = -1;

        rbfClassifTest_inputs[8] = 0.7;
        rbfClassifTest_inputs[9] = 0.25;
        rbfClassifTest_outputs[4] = -1;

        rbfClassifTest_inputs[10] = 0.6;
        rbfClassifTest_inputs[11] = 0.3;
        rbfClassifTest_outputs[5] = -1;

        Debug.Log("rbfClassifTest_nbExamples >" + rbfClassifTest_nbExamples);
        Debug.Log("rbfClassifTest_gamma[0] >" + rbfClassifTest_gamma[0]);
        Debug.Log("rbfClassifTest_gamma[1] >" + rbfClassifTest_gamma[1]);
        Debug.Log("rbfClassifTest_inputs[0] >" + rbfClassifTest_inputs[0]);
        Debug.Log("rbfClassifTest_inputSize >" + rbfClassifTest_inputSize);
        Debug.Log("rbfClassifTest_nbRepresentatives >" + rbfClassifTest_nbRepresentatives);
        System.IntPtr rbfClassifTest_Mode = LibWrapperMachineLearning.createRbfModel(rbfClassifTest_nbExamples, rbfClassifTest_gamma, rbfClassifTest_inputs, rbfClassifTest_inputSize, rbfClassifTest_outputs, rbfClassifTest_nbRepresentatives);
        
//             rbfClassifTest_input[0] = 0.15;
//             rbfClassifTest_input[1] = 0.75;
        // rbfClassifTest_Output = LibWrapperMachineLearning.getRbfResponseClassif(rbfClassifTest_Model, rbfClassifTest_input);
//             Debug.Log("Response for input = [" + rbfClassifTest_input[0] + "][" + rbfClassifTest_input[1] + "]" + rbfClassifTest_Output + "< expected : 0.2");
//             rbfClassifTest_input[0] = 0.2;
//             rbfClassifTest_input[1] = 0.8;
        // rbfClassifTest_Output = LibWrapperMachineLearning.getRbfResponseClassif(rbfClassifTest_Model, rbfClassifTest_input);
//             Debug.Log("Response for input = [" + rbfClassifTest_input[0] + "][" + rbfClassifTest_input[1] + "]" + rbfClassifTest_Output + "< expected : 0.5"); 
//             rbfClassifTest_input[0] = 0.5;
//             rbfClassifTest_input[1] = 0.2;
        // rbfClassifTest_Output = LibWrapperMachineLearning.getRbfResponseClassif(rbfClassifTest_Model, rbfClassifTest_input);
//             Debug.Log("Response for input = [" + rbfClassifTest_input[0] + "][" + rbfClassifTest_input[1] + "]" + rbfClassifTest_Output + "< expected : 1"); 
//             rbfClassifTest_input[0] = 0.7;
//             rbfClassifTest_input[1] = 0.25;
        // rbfClassifTest_Output = LibWrapperMachineLearning.getRbfResponseClassif(rbfClassifTest_Model, rbfClassifTest_input);
//             Debug.Log("Response for input = [" + rbfClassifTest_input[0] + "][" + rbfClassifTest_input[1] + "]" + rbfClassifTest_Output + "< expected : -0.5");
        return debugFailure;
    }

}