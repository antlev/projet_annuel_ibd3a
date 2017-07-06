using System;

namespace AssemblyCSharp
{
	public class Result
	{
		/// <summary>
		/// Titre du test
		/// </summary>
		private string title; 


		/// <summary>
		/// Le nombre d'itérations éxecuté par le test
		/// </summary>
		private int iterations;

		/// <summary>
		/// Meilleure erreur trouvé par le test
		/// </summary>
		private int bestError;
		/// <summary>
		/// Structure de donnée créée pour pouvoir stocker les 
		/// résultats lors des simulations
		/// </summary>
		public Result()
		{

		}
		public string getTitle(){
			return title;
		}
		public void setTitle(string newTitle){
			this.title = newTitle;
		}
		public int getIterations(){
			return iterations;
		}
		public void setIterations(int newIterations){
			this.iterations = newIterations;
		}
		public int getBestError(){
			return bestError;
		}
		public void setBestError(int newBestError){
			this.bestError = newBestError;
		}


		public void logResults()
		{
			System.IO.StreamWriter sw = System.IO.File.AppendText(
				getTempPath() + title);

			//		Debug.Log (sw.ToString);
			try
			{
				string logLine = System.String.Format(
					"{0:G}: {1}.", System.DateTime.Now, bestError);
				sw.WriteLine(logLine);
			}
			finally
			{
				sw.Close();
			}
		}
		public string getTempPath()
		{
			string path = System.Environment.GetEnvironmentVariable("TEMP");
			if (!path.EndsWith("\\")) path += "\\";
			return path;
		}
	}
}



