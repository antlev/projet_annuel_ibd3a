using UnityEngine;
using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Threading;
using System.Diagnostics;


/// <summary>
/// Classe principale à utiliser pour implémenter vos algorithmes
/// Si vous souhaitez utiliser plusieurs scripts (1 par algorithme), 
/// vous le pouvez aussi.
/// </summary>
public class MainScript : MonoBehaviour
{
	/// <summary>
	/// Indique si un algorithme est en cours d'exécution
	/// </summary>
	private bool _isRunning = false;

	/// <summary>
	/// Indique si une evaluation de solution est en cours
	/// </summary>
	private bool _inSimulation = false;

	/// <summary>
	/// Méthode utilisée pour gérer les informations et 
	/// boutons de l'interface utilisateur
	/// </summary>
	public void OnGUI()
	{
		// Démarrage d'une liste de composants visuels verticale
		GUILayout.BeginVertical();

		// Affiche un bouton permettant le lancement de la recherche locale naéve
		if (GUILayout.Button("DEMARRAGE RECHERCHE LOCALE NAIVE"))
		{
			// Le bouton est inactif si un algorithme est en cours d'exécution
			if (!_isRunning)
			{
				// Démarrage de la recherche locale naéve en pseudo asynchrone
				StartCoroutine("NaiveLocalSearch");
			}
		}
		// Affiche un bouton permettant le lancement du recuit simulé
		if (GUILayout.Button("DEMARRAGE RECUIT SIMULE"))
		{
			// Le bouton est inactif si un algorithme est en cours d'exécution
			if (!_isRunning)
			{
				// Démarrage du recuit simulé en pseudo asynchrone
				StartCoroutine("SimulatedAnnealing");
			}
		}
		// Affiche un bouton permettant le lancement du recuit simulé 2
		if (GUILayout.Button("DEMARRAGE RECUIT SIMULE 2"))
		{
			// Le bouton est inactif si un algorithme est en cours d'exécution
			if (!_isRunning)
			{
				// Démarrage du recuit simulé en pseudo asynchrone
				StartCoroutine("SimulatedAnnealing2");
			}
		}
		// Affiche un bouton permettant le lancement de l'algorithme génétique
		if (GUILayout.Button("DEMARRAGE ALGORITHME GENETIQUE"))
		{
			// Le bouton est inactif si un algorithme est en cours d'exécution
			if (!_isRunning)
			{
				// Démarrage de l'algorithme génétique en pseudo asynchrone
				StartCoroutine("GeneticAlgorithm");
			}
		}

		// Affiche un bouton permettant le lancement de l'algorithme de Djikstra
		if (GUILayout.Button("DEMARRAGE DJIKSTRA"))
		{
			// Le bouton est inactif si un algorithme est en cours d'exécution
			if (!_isRunning) {
				// Démarrage de l'algorithme de Djikstra en pseudo asynchrone
				StartCoroutine ("Djikstra");
			}
			Thread.Sleep (100);
		}

		// Affiche un bouton permettant le lancement de l'algorithme A*
		if (GUILayout.Button("DEMARRAGE A*"))
		{
			// Le bouton est inactif si un algorithme est en cours d'exécution
			if (!_isRunning)
			{
				// Démarrage de l'algorithme A* en pseudo asynchrone
				StartCoroutine("AStar");
			}
		}
		// Fin de la liste de composants visuels verticale
		GUILayout.EndVertical();
	}

	/// <summary>
	/// Initialisation du script
	/// </summary>
	void Start()
	{
		// Pour faire en sorte que l'algorithme puisse continuer d'étre actif méme
		// en téche de fond.
		Application.runInBackground = true;
	}

	/// <summary>
	/// Implémentation possible de la recherche locale naive
	/// sous forme de coroutine pour le mode pseudo asynchone
	/// </summary>
	/// <returns></returns>
	public IEnumerator NaiveLocalSearch()
	{
		// Permet de mesurer le temps
		Stopwatch sw = Stopwatch.StartNew ();

		// Indique que l'algorithme est en cours d'exécution
		_isRunning = true;
		//-------------------------------------------------
		//------------------ PARAMETRES -------------------
		//-------------------------------------------------
		const int nbMoveInSolution = 5;
		//-------------------------------------------------
		// Nous récupérons l'erreur minimum atteignable
		// Ceci est optionnel et dépendant de la fonction
		// d'erreur
		// valable uniquement pour les problèmes sans obstacles

		// 1) ARRETER L'ALGO AVEC L'ERREUR MINIMUM
		//var minError = GetMinError();

		// 2) RECHERCHE CONTINUELLEMENT UNE MEILLEURE SOLUTION
		var minError = 5;

		// 3) ARRETER L'ALGO AU BOUT D'UN CERTAIN NOMBRE D'ITERATIONS
		// Nombre max d'itérations  
		const int iterationsMax = 100;
		//-------------------------------------------------
			
		// Génére une solution initiale au hazard (ici une séquence
		// de nbMoveInSolution mouvements)
		var currentSolution = new PathSolutionScript(nbMoveInSolution);

		// Récupére le score de la solution initiale
		// Sachant que l'évaluation peut nécessiter une 
		// simulation, pour pouvoir la visualiser nous
		// avons recours à une coroutine
		var scoreEnumerator = GetError(currentSolution);
		yield return StartCoroutine(scoreEnumerator);
		float currentError = scoreEnumerator.Current;

		// Affichage de l'erreur initiale
		UnityEngine.Debug.Log("Lancement de l'algo recherche locale naive : currentError >" + currentError + "< - minimumError >" + minError + "<");

		// Initialisation du nombre d'itérations
		int iterations = 0;

		while (currentError != minError)
//		while (iterations < iterationsMax)
		{
			// On obtient une copie de la solution courante
			// pour ne pas la modifier dans le cas ou la modification
			// ne soit pas conservée.
			var newsolution = CopySolution(currentSolution);

			// On procéde à une petite modification de la solution
			// courante.
			RandomChangeInSolution(newsolution);

			// Deuxième possibilité
			///Inversion de deux positions
//			SwapActionsInSolution(newsolution, nbMoveInSolution);

			// Récupére le score de la nouvelle solution
			// Sachant que l'évaluation peut nécessiter une 
			// simulation, pour pouvoir la visualiser nous
			// avons recours à une coroutine
			var newscoreEnumerator = GetError(newsolution);
			yield return StartCoroutine(newscoreEnumerator);
			float newError = newscoreEnumerator.Current;

			// On affiche l'erreur actuelle , la nouvelle erreur et si on change de solution
			UnityEngine.Debug.Log("[" + iterations + "] currentError >" + currentError + "< - newError >" + newError + "<- change Solution >" + (newError <= currentError) + "< - time elapsed >"+sw.ElapsedMilliseconds+"< (ms)");

			// Si la solution a été améliorée
			if (newError <= currentError)
			{
				// On met à jour la solution courante
				currentSolution = newsolution;

				// On met à jour l'erreur courante
				currentError = newError;
			}

			// On incrémente le nombre d'itérations
			iterations++;

			// On rend la main au moteur Unity3D
			yield return 0;
		}
		// On stop le chronomètre
		sw.Stop();

		// Fin de l'algorithme, on indique que son exécution est stoppée
		_isRunning = false;

		// On affiche le nombre d'itérations nécessaire à l'algorithme pour trouver la solution
		UnityEngine.Debug.Log("CONGRATULATIONS !!! Solution Found : iterations >" + iterations + "< - error >" + currentError + "< - Elapsed time >"+sw.ElapsedMilliseconds+"< (ms)");
	}

	// Coroutine à utiliser pour implémenter l'algorithme de Djikstra
	public IEnumerator Djikstra()
	{
		// Permet de mesurer le temps
		Stopwatch sw = Stopwatch.StartNew ();

		// Récupération de l'environnement sous forme de matrice
		var matrix = MatrixFromRaycast.CreateMatrixFromRayCast();

		bool[][] booleanGrid = new bool[matrix.Length][];

		// Conversion de la grille proposée par le probléme en grille booléenne (case vide / obstacle)
		for (int i = 0; i < matrix.Length; i++)
		{
			booleanGrid[i] = new bool[matrix[i].Length];
			for (int j = 0; j < matrix[i].Length; j++)
			{
				booleanGrid[i][j] = (matrix[i][j] == LayerMask.NameToLayer("Obstacle")) ? true : false;
			}
		}

		// Récupération des positions de départ et d'arrivée
		var startPosX = PlayerScript.StartXPositionInMatrix;
		var startPosY = PlayerScript.StartYPositionInMatrix;
		var endPosX = PlayerScript.GoalXPositionInMatrix;
		var endPosY = PlayerScript.GoalYPositionInMatrix;

		// Lancement de l'algorithme de Djikstra
		var path = DjikstraAlgorithm.RunOn2DGrid(booleanGrid, startPosX, startPosY, endPosX, endPosY);

		// Si l'algorithme de Djikstra n'a pas trouvé de chemin possible.
		if (path == null || path.Count() < 1)
		{
			UnityEngine.Debug.Log("Lancement de l'algo de Djikstra : Aucune solution trouvée - Temps d'éxecution >"+sw.ElapsedMilliseconds+"< (ms)");
		}
		else
		{
			var patharray = path.ToArray();

			// Conversion du chemin trouvé en ensemble d'actions
			var solution = new PathSolutionScript(patharray.Length - 1);
			for (int i = 0; i < patharray.Length - 1; i++)
			{
				// Conversion d'un mouvement entre deux case en action
				solution.Actions[i] =
					new ActionSolutionScript()
				{
					Action = new Vector3(
						(float)(patharray[i + 1].x - patharray[i].x),
						0f,
						(float)(patharray[i + 1].y - patharray[i].y)
					)
				};
			}

			// Simulation de la solution trouvée
			var scoreEnumerator = GetError(solution);
			yield return StartCoroutine(scoreEnumerator);
			float currentError = scoreEnumerator.Current;
			UnityEngine.Debug.Log("Lancement de l'algo de Djikstra : Meilleure solution trouvée>" + currentError + "< - Temps d'éxecution >"+sw.ElapsedMilliseconds+"< (ms)");
		}
		// On stop le chronomètre
		sw.Stop();
		yield return null;
	}

	// Coroutine à utiliser pour implémenter l'algorithme d' A*
	public IEnumerator AStar()
	{
		// Permet de mesurer le temps
		Stopwatch sw = Stopwatch.StartNew ();

		// Récupération de l'environnement sous forme de matrice
		var matrix = MatrixFromRaycast.CreateMatrixFromRayCast();

		bool[][] booleanGrid = new bool[matrix.Length][];

		// Conversion de la grille proposée par le probléme en grille booléenne (case vide / obstacle)
		for (int i = 0; i < matrix.Length; i++)
		{
			booleanGrid[i] = new bool[matrix[i].Length];
			for (int j = 0; j < matrix[i].Length; j++)
			{
				booleanGrid[i][j] = (matrix[i][j] == LayerMask.NameToLayer("Obstacle")) ? true : false;
			}
		}

		// Récupération des positions de départ et d'arrivée
		var startPosX = PlayerScript.StartXPositionInMatrix;
		var startPosY = PlayerScript.StartYPositionInMatrix;
		var endPosX = PlayerScript.GoalXPositionInMatrix;
		var endPosY = PlayerScript.GoalYPositionInMatrix;

		// Lancement de l'algorithme AStar
		var path = AStarAlgo.RunOn2DGrid(booleanGrid, startPosX, startPosY, endPosX, endPosY);

		// Si l'algorithme AStar n'a pas trouvé de chemin possible.
		if (path == null || path.Count() < 1)
		{
			UnityEngine.Debug.Log("Lancement de l'algo AStar : Aucune solution trouvée - Temps d'éxecution >"+sw.ElapsedMilliseconds+"< (ms)");
		}
		else
		{
			var patharray = path.ToArray();

			// Conversion du chemin trouvé en ensemble d'actions
			var solution = new PathSolutionScript(patharray.Length - 1);
			for (int i = 0; i < patharray.Length - 1; i++)
			{
				// Conversion d'un mouvement entre deux case en action
				solution.Actions[i] =
					new ActionSolutionScript()
				{
					Action = new Vector3(
						(float)(patharray[i + 1].x - patharray[i].x),
						0f,
						(float)(patharray[i + 1].y - patharray[i].y)
					)
				};
			}

			// Simulation de la solution trouvée
			var scoreEnumerator = GetError(solution);
			yield return StartCoroutine(scoreEnumerator);
			float currentError = scoreEnumerator.Current;
			UnityEngine.Debug.Log("Lancement de l'algo AStar : Meilleure solution trouvée>" + currentError + "< - Temps d'éxecution >"+sw.ElapsedMilliseconds+"< (ms)");
		}
		// On stop le chronomètre
		sw.Stop();
		yield return null;
	}

	// Coroutine à utiliser pour implémenter l'algorithme du recuit simulé
	public IEnumerator SimulatedAnnealing()
	{
		// Permet de mesurer le temps
		Stopwatch sw = Stopwatch.StartNew ();

		// Indique que l'algorithme est en cours d'exécution
		_isRunning = true;
		//-------------------------------------------------
		//------------------ PARAMETRES -------------------
		//-------------------------------------------------
		// Nombre de mouvement contenu dans NotificationServices solutions
		const int nbMoveInSolution = 10;
		// Initialisation du nombre d'itérations maximum
		const int iterationsMax = 1000;
		// Pour la deuxième implémentation
		const float temperatureInit = 1f;
		//-------------------------------------------------
		// prob() And temp() functions
		//-------------------------------------------------
		//--------------- CONDITION D'ARRET ---------------
		// Nous récupérons l'erreur minimum atteignable
		// Ceci est optionnel et dépendant de la fonction
		// d'erreur
		// 1) ARRETER L'ALGO AVEC L'ERREUR MINIMUM
		//var minError = GetMinError();
		// 2) RECHERCHE CONTINUELLEMENT UNE MEILLEURE SOLUTION
		var minError = 0;
		// 3) ARRETER L'ALGO AU BOUT D'UN CERTAIN NOMBRE D'ITERATIONS
		// Nombre max d'itérations  
		//-------------------------------------------------

		// Initialisation du nombre d'itérations
		int iterations = 0;

		// Génére une solution initiale au hazard (ici une séquence
		// de nbSolutionMovesmouvements)
		var currentSolution = new PathSolutionScript(nbMoveInSolution);

		// Récupére le score de la solution initiale
		// Sachant que l'évaluation peut nécessiter une 
		// simulation, pour pouvoir la visualiser nous
		// avons recours à une coroutine
		var scoreEnumerator = GetError(currentSolution);
		yield return StartCoroutine(scoreEnumerator);
		float currentError = scoreEnumerator.Current;

		// Initialisation de la meilleure erreur obtenue à l'erreur initiale.
		// On garde en mémoire la meilleure solution obtenue
		float bestError = currentError;

		// Affichage de l'erreur initiale
		UnityEngine.Debug.Log("Lancement de l'algo recuit simulé : currentError >" + currentError + "< - minimumError >" + minError + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");

		// --------------- PREMIERE IMPLEMENTATION ---------------
		// On fait varier la température en fonction 
		// du nombre d'itération et itérations max

		// La condition d'arret est le nombre d'itérations max
		while (iterations <= iterationsMax)
		{
			// On obtient une copie de la solution courante
			// pour ne pas la modifier dans le cas ou la modification
			// ne soit pas conservée.
			var newsolution = CopySolution(currentSolution);

			// On procéde à une petite modification de la solution
			// courante.
			RandomChangeInSolution(newsolution);
			// Deuxième possibilité
			///Inversion de deux positions
//			SwapActionsInSolution(newsolution, nbMoveInSolution);

			// Récupére le score de la nouvelle solution
			// Sachant que l'évaluation peut nécessiter une 
			// simulation, pour pouvoir la visualiser nous
			// avons recours à une coroutine
			var newscoreEnumerator = GetError(newsolution);
			yield return StartCoroutine(newscoreEnumerator);
			float newError = newscoreEnumerator.Current;


			// Tirage d'un nombre aléatoire entre 0 et 1
			float rdm = UnityEngine.Random.Range (0f, 1f);

			// La fonction Prob() dépendant de la différence d'erreur entre 
			// l'erreur courrante et la nouvelle erreur ainsi que la température
			float prob = Prob(newError - currentError ,temp(iterations,iterationsMax));

			// On affiche pour des raisons de Debug et de suivi
			// la comparaison entre l'erreur courante et la
			// nouvelle erreur
			UnityEngine.Debug.Log("[" + iterations + "] currentError >" + currentError + "< - newError >" + newError + "< - rdm >" + rdm.ToString() + "< - prob>" + "< - Solution change >" + (newError <= currentError || rdm < prob) + "< - iterationsMax >" + iterationsMax + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");

			// Si la solution est meilleure
			// ou dans certain cas si la valeur aléatoire générée 
			// est inférieure à la variable renvoyé par la fonction prob
			if (newError <= currentError || rdm < prob)
			{
				// On met à jour la solution courante
				currentSolution = newsolution;
				// On met à jour l'erreur courante
				currentError = newError;
				// Si la solution est meilleure on met à jour la meilleure solution
				if (newError <= bestError) {
					bestError = newError;
					UnityEngine.Debug.Log("Meilleure solution trouvée >"+bestError+"< iteration >"+iterations+"< - Time >"+sw.ElapsedMilliseconds+"< (ms)");

				}

			}
			// On incrémente le nombre d'itérations
			iterations++;

			// On rend la main au moteur Unity3D
			yield return 0;
		}
			
		// Fin de l'algorithme, on indique que son exécution est stoppée
		_isRunning = false;

		if (bestError <= minError) {
			// On affiche le nombre d'itérations nécessaire à l'algorithme pour trouver la solution
			UnityEngine.Debug.Log ("CONGRATULATIONS !!! Solution Found : iterations >" + iterations + "< - bestError >" + bestError + "< - iterationsMax >" + iterationsMax + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");
		} else {
			UnityEngine.Debug.Log ("Sorry. no solution found - best solution : iterations >" + iterations + "< - bestError >" + bestError + "< - minimumError >" + minError + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");
		}
		// On stop le chronomètre
		sw.Stop();
	}

	// Coroutine à utiliser pour implémenter l'algorithme du recuit simulé
	public IEnumerator SimulatedAnnealing2()
	{
		// Permet de mesurer le temps
		Stopwatch sw = Stopwatch.StartNew ();

		// Indique que l'algorithme est en cours d'exécution
		_isRunning = true;
		//-------------------------------------------------
		//------------------ PARAMETRES -------------------
		//-------------------------------------------------
		// Nombre de mouvement contenu dans NotificationServices solutions
		const int nbMoveInSolution = 15;
		// Initialisation du nombre d'itérations maximum
		const int iterationsMax = 1000;
		// Pour la deuxième implémentation
		const float temperatureInit = 1f;
		//-------------------------------------------------
		// prob() And temp() functions
		//-------------------------------------------------
		//--------------- CONDITION D'ARRET ---------------
		// Nous récupérons l'erreur minimum atteignable
		// Ceci est optionnel et dépendant de la fonction
		// d'erreur
		// 1) ARRETER L'ALGO AVEC L'ERREUR MINIMUM
		//var minError = GetMinError();
		// 2) RECHERCHE CONTINUELLEMENT UNE MEILLEURE SOLUTION
		var minError = 5;
		// 3) ARRETER L'ALGO AU BOUT D'UN CERTAIN NOMBRE D'ITERATIONS
		// Nombre max d'itérations  
		//-------------------------------------------------

		// Initialisation du nombre d'itérations
		int iterations = 0;

		// Génére une solution initiale au hazard (ici une séquence
		// de nbSolutionMovesmouvements)
		var currentSolution = new PathSolutionScript(nbMoveInSolution);

		// Récupére le score de la solution initiale
		// Sachant que l'évaluation peut nécessiter une 
		// simulation, pour pouvoir la visualiser nous
		// avons recours à une coroutine
		var scoreEnumerator = GetError(currentSolution);
		yield return StartCoroutine(scoreEnumerator);
		float currentError = scoreEnumerator.Current;

		// Initialisation de la meilleure erreur obtenue à l'erreur initiale.
		// On garde en mémoire la meilleure solution obtenue
		float bestError = currentError;

		// Affichage de l'erreur initiale
		UnityEngine.Debug.Log("Lancement de l'algo recuit simulé 2 : currentError >" + currentError + "< - minimumError >" + minError + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");

		// --------------- DEUXIEME IMPLEMENTATION ---------------
		// On fait varier la température en fonction de la stagnation

		// Initialisation de la valeur de 'stagnation' qui si elle dépasse un
		// certain seuil provoquera l'augmentation de la température.
		float stagnationInitial = 0.01f;	// 0.001f
		float stagnation = stagnationInitial;
		// Initialisation de la température
		float temperature = temperatureInit;
		// Tant que l'erreur minimum n'est pas atteinte
		while (currentError > minError) {
			// On obtient une copie de la solution courante
			// pour ne pas la modifier dans le cas ou la modification
			// ne soit pas conservée.
			var newsolution = CopySolution (currentSolution);

			// On procède à une petite modification de la solution
			// courante.
			RandomChangeInSolution (newsolution);
			// Deuxième possibilité
			///Inversion de deux positions
			//SwapActionsInSolution(newsolution, nbMoveInSolution);

			// Récupère le score de la nouvelle solution
			// Sachant que l'évaluation peut nécessiter une 
			// simulation, pour pouvoir la visualiser nous
			// avons recours à une coroutine
			var newscoreEnumerator = GetError (newsolution);
			yield return StartCoroutine (newscoreEnumerator);
			float newError = newscoreEnumerator.Current;

			// Tirage d'un nombre aléatoire entre 0 et 1.
			float rdm = UnityEngine.Random.Range (0f, 1f);

			// Comparaison de ce nombre à la probabilité d'accepter un changement
			// déterminée par le critère de Boltzman.
			if (rdm < BoltzmanCriteria(temperature, currentError, newError)){
				
			UnityEngine.Debug.Log ("Changement de solution ancienne>" + currentError + "< nouvelle >" + newError + "> - Time >"+sw.ElapsedMilliseconds+"< (ms)");

				// Si le nombre est inférieur, on accepte la permutation
				// et l'on met à jour l'erreur courante.
				currentError = newError;

				// Met l'ancienne solution dans la solution courante.
				currentSolution = newsolution;
			}

			// Si l'erreur stagne
			if (bestError == currentError) {
				// On incrémente la stagnation
				stagnation *= 1.001f;
			} else {
				// Sinon on la réinitialise
				stagnation = stagnationInitial;
			}
			// Si l'erreur diminue, on met a jour la meilleure solution
			// et on reset la stagnation initiale
			if (currentError < bestError) {
				bestError = currentError;
				stagnation = stagnationInitial;
			UnityEngine.Debug.Log("Meilleure solution trouvée >"+bestError+"< iteration >"+iterations+"< - Time >"+sw.ElapsedMilliseconds+"< (ms)");

			}

			// On met à jour la temperature à chaque tour de boucle :
			//  - si la stagnation est suffisante la temperature va augmenter
			//  - sinon la temperature décroit de manière géométrique
			temperature *= 0.998f + stagnation;

			// Affichage dans la console de Debug du couple temperature stagnation
			// pour pouvoir être témoin de l'augmentation de la température lorsque
			// l'on se retrouve coincé trop longtemps dans un minimum local.
			//UnityEngine.Debug.Log(temperature + "  -  " + stagnation);

			// On incrémente le nombre d'itérations
			iterations++;

			// On rend la main au moteur Unity3D
			yield return 0;
		}
	// Fin de l'algorithme, on indique que son exécution est stoppée
	_isRunning = false;

	if (bestError <= minError) {
		// On affiche le nombre d'itérations nécessaire à l'algorithme pour trouver la solution
			UnityEngine.Debug.Log ("CONGRATULATIONS !!! Solution Found : iterations >" + iterations + "< - bestError >" + bestError + "< - iterationsMax >" + iterationsMax + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");
	} else {
			UnityEngine.Debug.Log ("Sorry. no solution found - best solution : iterations >" + iterations + "< - bestError >" + bestError + "< - minimumError >" + minError + "< - Time >"+sw.ElapsedMilliseconds+"< (ms)");
	}
	// On stop le chronomètre
	sw.Stop();
}



	// Fonction de probabilité
	// Renvoie un entier entre 0 et 1
	// Plus l'entier renvoyé est proche de 0, moins l'exploration sera permise 
	private float Prob(float diffError, float temperature){
		return (float)Mathf.Exp (-diffError / temperature);
	}
	// Fonction de température
	// définit la température en fonction
	// du nombre d'itération et du nombre d'itérations max
	private float temp(int iterations, int iterationsMax){
		if (iterationsMax > iterations) {
			return 0;
		} else {
			// Variations linéaires de la température
//			return (float)iterationsMax - iterations;
			//Variations exponentielles
			return (float)Mathf.Exp(iterationsMax-iterations);
		}
	}
//	<summary>
//	Proposition d'implémentation du critère de Bolztman représentant une
//	fonction seuil, renvoyant la probabilité d'accepter une permutation
//	selon la différence entre l'erreur courante et la nouvelle erreur et
//	ainsi qu'en fonction de la temperature courante.
//	</summary>
//  <param name="temperature"></param>
//  <param name="currentError"></param>
//	<param name="newError"></param>
//	<returns></returns>
	float BoltzmanCriteria(float temperature, float currentError, float newError)
	{
		///Si la temperature est nulle
		///cas particulier pour éviter une division par zéro
		if (temperature == 0)
		{
			return currentError - newError;
		}

		///Critère de Boltzman
		return Mathf.Exp(((float)(currentError - newError)) / temperature);
	}

	// Coroutine à utiliser pour implémenter un algorithme génétique
	public IEnumerator GeneticAlgorithm()
	{
		// Permet de mesurer le temps
		Stopwatch sw = Stopwatch.StartNew ();
		//-------------------------------------------------
		//------------------ PARAMETRES -------------------
		//-------------------------------------------------
		// Nombre d'individus dans la population
		const int popSize = 35;
		// Meilleure pourcentage à selectionner
		const float bestPercentage = 0.35f;
		// Taux de mutation
		const float mutationRate = 0.1f;
		// Nombre d'actions pour la solution de chaque individu
		const int nbMoveInSolution = 20;
		//-------------------------------------------------
		const int bestCount = (int)(popSize * bestPercentage);
		//--------------- CONDITION D'ARRET ---------------
		// Nous récupérons l'erreur minimum atteignable
		// Ceci est optionnel et dépendant de la fonction
		// d'erreur
		// 1) ARRETER L'ALGO AVEC L'ERREUR MINIMUM
		//var minError = GetMinError();
		// 2) RECHERCHE CONTINUELLEMENT UNE MEILLEURE SOLUTION
		//var minError = 0;
		// 3) ARRETER L'ALGO AU BOUT D'UN CERTAIN NOMBRE D'ITERATIONS
		// Nombre max d'itérations  
		//-------------------------------------------------

		// Indique que l'algorithme est en cours d'exécution
		_isRunning = true;

		// INITIALISATION DE LA POPULATION 
		// 1) Première possibilité : 
		// On produit un ensemble de solutions 
		// au hasard et indépendantes les unes des autres
//		PathSolutionScript[] population = new PathSolutionScript[popSize];
//		for(var i = 0; i < popSize; i++)
//		{
//			population[i] = new PathSolutionScript(nbMoveInSolution);
//		}
			
		// 2) Deuxième possibilité : 
		// On produit une solution au hasard et modifie 
		// légèrement cette solution pour obtenir les autres individus de la population 
		PathSolutionScript[] population = new PathSolutionScript[popSize];
		// Génère une solution initiale au hasard
		var individuInitial = new PathSolutionScript(nbMoveInSolution);

		// Pour stocker la moyenne d'erreur de la population
		float initMeanError = 0;

		///Initialisation du tableau destiné à contenir l'ensemble des
		///couples configuration/score une fois la population évaluée
		var initScoredPopulation = new ScoredIndividual[popSize];

		// Pour chaque individu on stocke une solution voisine
		for(var i = 0; i < popSize; i++)
		{
			// On copie la solution initiale
			var newsolution = CopySolution(individuInitial);

			// >>>>>>>>>> Première possibilité
			// On procède à une petite modification de la solution
			// courante.
			RandomChangeInSolution(newsolution);

			// >>>>>>>>>> Deuxième possibilité
			///Inversion de deux positions
//			SwapActionsInSolution(newsolution, nbMoveInSolution);

			// On stocke la nouvelle solution
			population[i] = newsolution;

			// On évalue la solution de chaque individu
			var initErrorEnumerator = GetError(population[i]);
			yield return StartCoroutine(initErrorEnumerator);
			float initError = initErrorEnumerator.Current;

			// On somme toutes les erreurs...
			initMeanError += initError;

			///Création d'un couple configuration/solution et stockage
			///du score obtenu pour la configuration évaluée.
			initScoredPopulation[i] = new ScoredIndividual()
			{
				Configuration = population[i],
				Score = initError
			};	
		}
		// On organise les solutions en récupérant le tableau des scores pour afficher les stats
		var initOrderedScoredPopulation = initScoredPopulation
			.OrderBy((scoredindi) => scoredindi.Score)
			.Select((scoredindi2) => scoredindi2.Score)
			.ToArray();

		// ... on divise la somme des scores par le nombre d'individu pour avoir la moyenne
		initMeanError /= popSize;
		// Récupération du score de la meilleure solution
		float initBestError = initOrderedScoredPopulation [0];
		// Récupération du score de la moins bonne solution
		float initLastError = initOrderedScoredPopulation[popSize-1];

		UnityEngine.Debug.Log ("Lancement de l'algo génétique : popSize>"+popSize+"< - bestPercentage>"+bestPercentage+"< - mutationRate"+ mutationRate+"< --Meilleure solution >" +initBestError+ "< --Moins bonne solution >"+ initLastError +" --Erreur moyenne >"+initMeanError+"< - Time >"+sw.ElapsedMilliseconds+"< (ms)");

		int iterations = 1;

		// Pour stocker l'erreur de la meilleure solution de la population
		float bestError = initBestError;
		while (true)
//		while (bestError <= minError)
		{

		// EVALUATION DE LA POPULATION

			///Initialisation du tableau destiné à contenir l'ensemble des
			///couples configuration/score une fois la population évaluée
			var scoredPopulation = new ScoredIndividual[popSize];

			// Pour stocker la moyenne d'erreur de la population
			float meanError = 0;
			for(var i = 0; i < popSize; i++)
			{
				// Récupére le score de chaque solution de la population
				var errorEnumerator = GetError(population[i]);
				yield return StartCoroutine(errorEnumerator);
				float error = errorEnumerator.Current;

				// On somme toutes les erreurs...
				meanError += error;

				///Création d'un couple configuration/solution et stockage
				///du score obtenu pour la configuration évaluée.
				scoredPopulation[i] = new ScoredIndividual()
				{
					Configuration = population[i],
					Score = error
				};	
			}
			// ... on divise la somme des scores par le nombre d'individu pour avoir la moyenne
			meanError /= popSize;

		// SELECTION DES REPRODUCTEURS
			var bests = scoredPopulation
				.OrderBy((scoredindi) => scoredindi.Score)
				.Take(bestCount)
				.Select((scoredindi2) => scoredindi2.Configuration)
				.ToArray();


			// On récupère les scores ordonnés dans un tableau pour les stats
			var orderedScoredPopulation = scoredPopulation
				.OrderBy((scoredindi) => scoredindi.Score)
				.Select((scoredindi2) => scoredindi2.Score)
				.ToArray();
			
			// Récupère le meilleur score de la population
			float newBestError = orderedScoredPopulation[0];

			// Récupère le moins bon score de la population
			float newLastError = orderedScoredPopulation[popSize-1];

			// Si la solution a été améliorée
			if (newBestError < bestError) {
				// On affiche le debug
				UnityEngine.Debug.Log ("!!!!! MEILLEURE SOLUTION TROUVEE !!!!!!>" + newBestError + "< (ancienne : "+bestError+") - iterations >" + iterations + "< - moins bonne solution >"+ newLastError+"< - Moyenne de la population >"+meanError+"< - Time >"+sw.ElapsedMilliseconds+"< (ms)");	

				// On met à jour l'erreur courante
				bestError = newBestError;
			} else {
				// On affiche le debug
				UnityEngine.Debug.Log ("iterations >" + iterations + "< - Meilleure solution >" + bestError + "< - moins bonne solution >"+ newLastError+"< - Moyenne de la population >"+meanError+"< - Time >"+sw.ElapsedMilliseconds+"< (ms)");	
			}

		// CROISEMENT DE LA POPULATION
			PathSolutionScript[] newPopulation = new PathSolutionScript[popSize];

			///Pour chaque enfant que l'on doit générer par croisement
			for (int i = 0; i < popSize; i++)
			{
				///Récupération de deux reproduteurs au hasard parmis les meilleurs sélectionnés
				var parent1 = bests[UnityEngine.Random.Range(0, bestCount)];
				var parent2 = bests[UnityEngine.Random.Range(0, bestCount)];

				///Création d'un individu à partir du croisement des deux parents
				newPopulation[i] = Crossover(parent1, parent2);
			}


			// première proposition abandonnée
//			// On sélectionne 2 solutions au hasard parmis les reproducteurs (solutions conservées)
//			var sol1 = bests[Random.Range(0, bestCount)];
//			var sol2 = bests[Random.Range(0, bestCount)];
//			// On croise les solutions (cad on échange une action entre les deux)
//			var random = Random.Range(0,nbMoveInSolution);
//			var tmp = sol1.Actions[random];
//			sol1.Actions[random] = sol2.Actions[random];
//			sol2.Actions[random] = tmp;
//
		// MUTATION
			for(var i = 0;i < popSize; i++)
			{
				// On tire un nombre au hasard entre 0 et 1 et 
				// s'il est inférieur au tauxx de mutation, on procède à la mutation
				var rdm = UnityEngine.Random.Range(0f, 1f);
				if(rdm < mutationRate)
				{

					/// 2 Mutations proposées :
					/// 
					///Inversion de deux positions
					// SwapActionsInSolution(newPopulation[i], nbMoveInSolution);

					// Modification d'une action dans la solution au hasard
					RandomChangeInSolution(newPopulation[i]);

				}
			}

			///Remplacement de l'ancienne population par la nouvelle
			population = newPopulation;

			++iterations;
//			UnityEngine.Debug.Log ("iteration >" + iterations + "< bestSolution >" + currentError + "<");

		}
		yield return null;
	}

	/// <summary>
	/// Structure de donnée créée pour pouvoir stocker les 
	/// associations configuration/score lors de l'étape
	/// d'évaluation de la population.
	/// </summary>
	class ScoredIndividual
	{
		/// <summary>
		/// La configuration de la solution
		/// </summary>
		public PathSolutionScript Configuration { get; set; }

		/// <summary>
		/// Le score de la configuration ci-dessus
		/// </summary>
		public float Score { get; set; }
	}

	/// <summary>
	/// Méthode proposant une méthode pour obtenir une nouvel
	/// individu par croisement de deux configurations parentes
	/// </summary>
	/// <param name="parent1">Le parent 1</param>
	/// <param name="parent2">Le parent 2</param>
	/// <returns>L'enfant généré par croisement</returns>
	PathSolutionScript Crossover(PathSolutionScript parent1, PathSolutionScript parent2)
	{
		///L'index pointant sur une position au sein des parents
		int iParents = 0;

		///Variable indiquant si l'on cherche à copier une position
		///du parent1 ou du parent 2 dans l'enfant.
		bool lookingAtParent1 = true;

		///Initialisation de l'enfant.
		var child = new PathSolutionScript(parent1.Actions.Length);

		///Pour chaque position à remplir dans l'enfant.
		for (int i = 0; i < child.Actions.Length; i++)
		{
			///Si l'on souhaite copier une position du premier parent
			///dans l'enfant.
			if (lookingAtParent1)
			{
				///Si la position du parent1 n'est pas déjà présente dans l'enfant.
				if (child.Actions.All((pos) => !pos.Equals(parent1.Actions[iParents])))
				{
					///On copie la position courante du parent1 dans l'enfant.
					child.Actions[i] = parent1.Actions[iParents];

					///On précise qu'au tour du boucle suivant nous chercherons
					///à copier une position provenant du deuxième parent.
					lookingAtParent1 = false;
				}
				///Sinon, si la position du parent2 n'est pas présente dans l'enfant.
				else if (child.Actions.All((pos) => !pos.Equals(parent2.Actions[iParents])))
				{
					///On copie la position du parent2 dans l'enfant.
					child.Actions[i] = parent2.Actions[iParents];

					///On précise qu'au tour du boucle suivant nous chercherons
					///à copier une position provenant du premier parent.
					lookingAtParent1 = true;
				}
				///Si les deux positions du parent1 et du parent2 sont déjà présentes
				///dans l'enfant.
				else
				{
					///On décrémente préventivement l'index de l'enfant.
					i--;
				}
				///On incrémente l'index des parents
				iParents++;

				///Tout en vérifiant que l'on ne dépasse pas la taille du tableau
				///des parents, si tel est le cas, on remet à zéro cet index.
				if (iParents >= parent1.Actions.Length)
					iParents = 0;
			}
			///Si l'on souhaite copier une position du parent2 en premier
			else
			{
				///Si la position du parent2 n'est pas déjà présente dans l'enfant.
				if (child.Actions.All((pos) => !pos.Equals(parent2.Actions[iParents])))
				{
					///On copie la position du parent2 dans l'enfant.
					child.Actions[i] = parent2.Actions[iParents];

					///On précise qu'au tour du boucle suivant nous chercherons
					///à copier une position provenant du premier parent.
					lookingAtParent1 = true;
				}
				///Sinon, si la position du parent1 n'est pas présente dans l'enfant.
				else if (child.Actions.All((pos) => !pos.Equals(parent1.Actions[iParents])))
				{
					///On copie la position courante du parent1 dans l'enfant.
					child.Actions[i] = parent1.Actions[iParents];

					///On précise qu'au tour du boucle suivant nous chercherons
					///à copier une position provenant du deuxième parent.
					lookingAtParent1 = false;
				}
				///Si les deux positions du parent1 et du parent2 sont déjà présentes
				///dans l'enfant.
				else
				{
					///On décrémente préventivement l'index de l'enfant.
					i--;
				}
				///On incrémente l'index des parents
				iParents++;

				///Tout en vérifiant que l'on ne dépasse pas la taille du tableau
				///des parents, si tel est le cas, on remet à zéro cet index.
				if (iParents >= parent1.Actions.Length)
					iParents = 0;
			}
		}

		///Une fois le croisement effectué, on renvoie l'enfant ainsi généré.
		return child;
	}
	/// <summary>
	/// Exemple d'erreur minimum (pas forcément toujours juste) renvoyant
	/// la distance de manhattan entre la case d'arrivée et la case de départ.
	/// </summary>
	/// <returns></returns>
	int GetMinError()
	{
		return (int)(Mathf.Abs(PlayerScript.GoalXPositionInMatrix - PlayerScript.StartXPositionInMatrix) +
			Mathf.Abs(PlayerScript.GoalYPositionInMatrix - PlayerScript.StartYPositionInMatrix));
	}

	/// <summary>
	/// Exemple d'oracle nous renvoyant un score que l'on essaye de minimiser
	/// Ici est utilisé la position de la case d'arrivée, la position finale
	/// atteinte par la solution. Il est recommandé d'essayer plusieurs oracles
	/// pour étudier le comportement des algorithmes selon la qualité de ces
	/// derniers
	/// 
	/// Parmi les paramétres pouvant étre utilisés pour calculer le score/erreur :
	/// 
	///  - position de la case d'arrivée    : PlayerScript.GoalXPositionInMatrix
	///                                       PlayerScript.GoalYPositionInMatrix
	///  - position du joueur               : player.PlayerXPositionInMatrix
	///                                       player.PlayerYPositionInMatrix
	///  - position de départ du joueur     : PlayerScript.StartXPositionInMatrix
	///                                       PlayerScript.StartYPositionInMatrix
	///  - nombre de cases explorées        : player.ExploredPuts
	///  - nombre d'actions exécutées       : player.PerformedActionsNumber
	///  - vrai si le la balle a touché la case d'arrivée : player.FoundGoal
	///  - vrai si le la balle a touché un obstacle : player.FoundObstacle
	///  - interrogation de la matrice      :
	///       - la case de coordonnée (i, j) est elle un obstacle (i et j entre 0 et 49) :
	///           player.GetPutTypeAtCoordinates(i, j) == LayerMask.NameToLayer("Obstacle")
	///       - la case de coordonnée (i, j) est elle explorée (i et j entre 0 et 49) :
	///           player.GetPutTypeAtCoordinates(i, j) == 1
	///       - la case de coordonnée (i, j) est elle inexplorée (i et j entre 0 et 49) :
	///           player.GetPutTypeAtCoordinates(i, j) == 0
	/// </summary>
	/// <param name="solution"></param>
	/// <returns></returns>
	IEnumerator<float> GetError(PathSolutionScript solution)
	{
		// On indique que l'on s'appréte à lancer la simulation
		_inSimulation = true;

		// On créé notre objet que va exécuter notre séquence d'action
		var player = PlayerScript.CreatePlayer();

		// Pour pouvoir visualiser la simulation (moins rapide)
		player.RunWithoutSimulation = true;
		// On lance la simulation en spécifiant
		// la séquence d'action à exécuter
		player.LaunchSimulation(solution);

		// Tout pendant que la simulation n'est pas terminée
		while (player.InSimulation)
		{
			// On rend la main au moteur Unity3D
			yield return -1f;
		}

		// METHODE GENERATION ERREUR 1
		// Calcule la distance de Manhattan entre la case d'arrivée et la case finale de
		// notre objet, la pondére (la multiplie par zéro si le but a été trouvé) 
		// et ajoute le nombre d'actions jouées
//	    var error = (Mathf.Abs(PlayerScript.GoalXPositionInMatrix - player.PlayerXPositionInMatrix)
//			+ Mathf.Abs(PlayerScript.GoalYPositionInMatrix - player.PlayerYPositionInMatrix))
//			* (player.FoundGoal ? 0 : 100) +
//			player.PerformedActionsNumber;

		// METHODE GENERATION ERREUR 2
		// Erreur prenant en compte les obstacles
		var error = (Mathf.Abs (PlayerScript.GoalXPositionInMatrix - player.PlayerXPositionInMatrix)
		            + Mathf.Abs (PlayerScript.GoalYPositionInMatrix - player.PlayerYPositionInMatrix))
		            * (player.FoundGoal ? 0 : 100)
		            + player.PerformedActionsNumber;
//					+ 200 / player.ExploredPuts
//					+ (player.FoundObstacle ? 1000 : 0);

//		UnityEngine.Debug.Log("play.FoundGoal >" + player.FoundGoal + "< - FoundObstacle >" + player.FoundObstacle + "< nb cases exploré >" + player.ExploredPuts + "< actions exe >" + player.PerformedActionsNumber + "<" );

		// Détruit  l'objet de la simulation
		Destroy(player.gameObject);

		// Renvoie l'erreur précédemment calculée
		yield return error;

		// Indique que la phase de simulation est terminée
		_inSimulation = false;
	}

	/// <summary>
	/// Execute un changement aléatoire sur une solution
	/// ici, une action de la séquence est tirée au hasard et remplacée
	/// par une nouvelle au hasard.
	/// </summary>
	/// <param name="sol"></param>
	public void RandomChangeInSolution(PathSolutionScript sol)
	{
		sol.Actions[UnityEngine.Random.Range(0, sol.Actions.Length)] = new ActionSolutionScript();
	}

	/// <summary>
	///	Inverse deux actions dans la solution au hasard
	/// </summary>
	/// <param name="sol"></param>
	public void SwapActionsInSolution(PathSolutionScript sol, int nbMoveInSolution)
	{
		
		var pos1 = UnityEngine.Random.Range (0, nbMoveInSolution);
		var pos2 = UnityEngine.Random.Range (0, nbMoveInSolution);
		var tmp = sol.Actions[pos1];
		sol.Actions [pos1] = sol.Actions [pos2];
		sol.Actions [pos2] = tmp;
	}

	/// <summary>
	/// Fonction utilitaire ayant pour but de copier
	/// dans un nouvel espace mémoire une solution
	/// </summary>
	/// <param name="sol">La solution à copier</param>
	/// <returns>Une copie de la solution</returns>
	public PathSolutionScript CopySolution(PathSolutionScript sol)
	{
		// Initialisation de la nouvelle séquence d'action
		// de la méme longueur que celle que l'on souhaite copier
		var newSol = new PathSolutionScript(sol.Actions.Length);

		// Pour chaque action de la séquence originale,
		// on copie le type d'action.
		for (int i = 0; i < sol.Actions.Length; i++)
		{
			newSol.Actions[i].Action = sol.Actions[i].Action;
		}
//		Result test = new Result (){
//			
//		};

		// Renvoi de la solution copiée
		return newSol;
	}

//	public string getTempPath()
//	{
//		string path = System.Environment.GetEnvironmentVariable("TEMP");
//		if (!path.EndsWith("\\")) path += "\\";
//		return path;
//	}
//
//	// Ecrit une ligne de log dans le fichier resultats
//	public void logResults(int iteration, float currentError, float bestError)
//	{
//		System.IO.StreamWriter sw = System.IO.File.AppendText(
//			getTempPath() + title);
//
////		UnityEngine.Debug.Log (sw.ToString);
//		try
//		{
//			string logLine = System.String.Format(
//				"{0:G}: {1}.", System.DateTime.Now, "");
//			sw.WriteLine(logLine);
//		}
//		finally
//		{
//			sw.Close();
//		}
//	}
}