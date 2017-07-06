using UnityEngine;
using System.Linq;
using System.Collections;
using System.Collections.Generic;

public class AStarAlgo
{
	/// <summary>
	/// Une classe que nous allons utiliser pour stocker pour chaque noeud
	/// les informations n�cessaires � l'algorithme de Djikstra
	/// </summary>
	protected class NodeInformations
	{
		public Node CurrentNode { get; set; }
		public NodeInformations FromNode { get; set; }
		public float ComputedCost { get; set; }
	}

	/// <summary>
	/// Convertit une grille bool�enne (case vide et obtacles) en graphe orienté pondér�é
	/// Ex�cute l'algorithme de djikstra sur le graphe orient� pond�r� g�n�r�
	/// Convertit le chemin trouv� en succession de coordonn�es sur la grille
	/// </summary>
	/// <param name="grid">La grille bool�enne repr�sentant l'environnement</param>
	/// <param name="startPosX">La coordonn�e X de la position de d�part</param>
	/// <param name="startPosY">La coordonn�e Y de la position de d�part</param>
	/// <param name="endPosX">La coordonn�e X de la position d'arriv�e</param>
	/// <param name="endPosY">La coordonn�e Y de la position d'arriv�e</param>
	/// <returns></returns>
	static public IEnumerable<Vector2> RunOn2DGrid(bool[][] grid, int startPosX, int startPosY, int endPosX, int endPosY)
	{
		// Pour plus d'efficacit�, nous allons utiliser deux dictionnaires 
		// pour avoir une association parfaite entre un Noeud et une position
		var graphToGridMapping = new Dictionary<Node, Vector2>();
		var gridToGraphMapping = new Dictionary<Vector2, Node>();

		// Cr�ation de l'ensemble des noeuds (un noeud par case)
		for (int i = 0; i < grid.Length; i++)
		{
			var line = grid[i];

			for (int j = 0; j < line.Length; j++)
			{
				// Cr�ation du noeud associ� � la position (i, j)
				var node = new Node()
				{
					ID = i * grid.Length + j,
				};

				// Cr�ation de la position (i, j)
				var pos = new Vector2(i, j);

				// ajout du couple cr�� dans les deux structures
				graphToGridMapping.Add(node, pos);
				gridToGraphMapping.Add(pos, node);
			}
		}

		// Cr�ation des arcs pour chaque noeuds en fonction de la position de
		// la case sur la grille et selon les obstacles
		foreach (var pos in gridToGraphMapping.Keys)
		{
			// Cr�ation de la structure stockant les liens (au maximum de 4 si l'on ne peut)
			// se d�placer dans la grille qu'horizontalement et verticalement
			var linkList = new List<Link>(4);

			// Doit-on ajouter un arc vers la case sup�rieure ?
			if (pos.y < grid.Length - 1 && !grid[(int)pos.x][(int)pos.y + 1])
			{
				var link = new Link()
				{
					Cost = Mathf.Abs (endPosY-pos.y) + Mathf.Abs(endPosX-pos.x),
					Origin = gridToGraphMapping[pos],
					Target = gridToGraphMapping[new Vector2(pos.x, pos.y + 1)]
				};
				linkList.Add(link);
			}

			// Doit-on ajouter un arc vers la case inf�rieure ?
			if (pos.y > 0 && !grid[(int)pos.x][(int)pos.y - 1])
			{
				var link = new Link()
				{
					Cost = Mathf.Abs (endPosY-pos.y) + Mathf.Abs(endPosX-pos.x),
					Origin = gridToGraphMapping[pos],
					Target = gridToGraphMapping[new Vector2(pos.x, pos.y - 1)]
				};
				linkList.Add(link);
			}

			// Doit-on ajouter un arc vers la case de gauche ?
			if (pos.x < grid[(int)pos.y].Length - 1 && !grid[(int)pos.x + 1][(int)pos.y])
			{
				var link = new Link()
				{
					Cost = Mathf.Abs (endPosY-pos.y) + Mathf.Abs(endPosX-pos.x),
					Origin = gridToGraphMapping[pos],
					Target = gridToGraphMapping[new Vector2(pos.x + 1, pos.y)]
				};
				linkList.Add(link);
			}

			// Doit-on ajouter un arc vers la case de droite ?
			if (pos.x > 0 && !grid[(int)pos.x - 1][(int)pos.y])
			{
				var link = new Link()
				{
					Cost = Mathf.Abs (endPosY-pos.y) + Mathf.Abs(endPosX-pos.x),
					Origin = gridToGraphMapping[pos],
					Target = gridToGraphMapping[new Vector2(pos.x - 1, pos.y)]
				};
				linkList.Add(link);
			}

			// R�cup�ration du neoud
			var node = gridToGraphMapping[pos];

			// Initialisation des liens sous forme de tableau pour plus d'efficacit�
			node.Links = linkList.ToArray();
		}

		// Lancement de l'algorithme de Djiikstra sur le graphe pond�r� orient� associ�
		var result = Run(graphToGridMapping

			// On n'ajoute que les noeuds correspondant � des cases qui ne sont pas des obstacles
			.Where((kv1) => !grid[(int)kv1.Value.x][(int)kv1.Value.y])
			.Select((kv2) => kv2.Key)
			,
			gridToGraphMapping[new Vector2(startPosX, startPosY)],
			gridToGraphMapping[new Vector2(endPosX, endPosY)]);


		// On renvoie l'ensemble des positions du chemins optimal ou null si aucun n'a �t� trouv�
		return (result != null) ? result.Select((node2) => graphToGridMapping[node2]) : null;
	}
		

	/// <summary>
	/// Ex�cute l'algorithme de Djikstra sur un graphe quelconque compos� de noeuds (Node) et d'arcs (Link)
	/// </summary>
	/// <param name="graph">L'ensemble des noeuds du graphe</param>
	/// <param name="startNode">Le noeud de d�part</param>
	/// <param name="endNode">Le noeud d'arriv�e</param>
	/// <returns></returns>
	static public IEnumerable<Node> Run(IEnumerable<Node> graph, Node startNode, Node endNode)
	{
		// Initialisation de la liste des noeuds � explorer 
		// (on prend le choix ici de la construire progressivement et de ne pas rajouter tous les noeuds d'un coup)
		var nodesToExploreList = new Dictionary<Node, NodeInformations>(graph.Count());

		// Ajout � la liste des noeuds � explorer du noeud de d�part (et �tiquetage � z�ro de ce dernier)
		nodesToExploreList.Add(startNode, new NodeInformations()
			{
				CurrentNode = startNode,
				FromNode = null,
				ComputedCost = 0
			});

		// Initialisation de la liste des noeuds explor�s
		var exploredNodesList = new Dictionary<Node, NodeInformations>(graph.Count());

		// It�ration successives de l'algorithme
		while (true)
		{
			// Condition de sortie 1 : si il n'y a plus de noeuds � explorer
			if (nodesToExploreList.Count == 0)
			{
				nodesToExploreList = null;
				return null;
			}

			// S�lection du noeud de co�t minimum parmi ceux � explorer
			var orderedNodesInfos = nodesToExploreList.Values.OrderBy((kv) => kv.ComputedCost);
			var selectedNodeInfos = orderedNodesInfos.First();

			// Condition de sortie 2 : si le noeud � explorer est le noeud d'arrivée
			if (selectedNodeInfos.CurrentNode == endNode)
			{
				break;
			}

			// Ajout du noeud s�lectionn� � la liste des noeuds explor�s
			exploredNodesList.Add(selectedNodeInfos.CurrentNode, selectedNodeInfos);

			// Mise � jour des poids des noeuds voisins au noeud s�lectionn� et ajout de ces derniers
			// � la liste des noeuds � explorer s'ils n'y �taient pas d�j�.
			UpdateNeighbours(selectedNodeInfos, nodesToExploreList, exploredNodesList, endNode);

			// On retire de la liste des noeuds � explorer le noeud s�lectionn� et explor�
			nodesToExploreList.Remove(selectedNodeInfos.CurrentNode);
		}

		// Si le noeud d'arriv�e a �t� trouv�
		if (nodesToExploreList.ContainsKey(endNode))
		{
			// R�cup�ration des informations du noeud d'arriv�e
			var endNodeInfo = nodesToExploreList[endNode];

			// Pour faciliter la vie du GC
			nodesToExploreList = null;
			exploredNodesList = null;

			// On renvoie le chemin trouv� (du noeud de d�part au noeud d'arriv�e)
			return GetInvertedPath(endNodeInfo).Reverse();
		}
		else
		{
			// Aucun chemin n'a �t� trouv�
			return null;
		}
	}

	/// <summary>
	/// R�cup�re le chemin de co�t minimum � partir d'un noeud
	/// vers le noeud de d�part
	/// </summary>
	/// <param name="nodeInfos"></param>
	/// <returns></returns>
	static protected IEnumerable<Node> GetInvertedPath(NodeInformations nodeInfos)
	{
		do
		{
			yield return nodeInfos.CurrentNode;
			nodeInfos = nodeInfos.FromNode;
		}
		while (nodeInfos != null);
	}

	/// <summary>
	/// Mets � jour les voisins du noeud pass� en param�tre 
	/// </summary>
	/// <param name="nodeInfos"></param>
	/// <param name="nodesToExplore"></param>
	/// <param name="exploredNodes"></param>
	static protected void UpdateNeighbours(NodeInformations nodeInfos,
		Dictionary<Node, NodeInformations> nodesToExplore,
		Dictionary<Node, NodeInformations> exploredNodes,
		Node endNode
	)
	{
		// Pour chacun des arcs sortant du noeud
		foreach (var link in nodeInfos.CurrentNode.Links)
		{
			// On exclut les noeuds d�j� explor�s
			if (!exploredNodes.ContainsKey(link.Target))
			{
				// On calcule le co�t pour atteindre le noeud voisin 
				// en passant par le noeud s�lectionn�
				var updatedCost = (nodeInfos.ComputedCost + link.Cost);
				NodeInformations targetNodeInfos = null;
				if (nodesToExplore.ContainsKey(link.Target))
					targetNodeInfos = nodesToExplore[link.Target];

				// Si le noeud voisin n'a pas �t� encore 
				// ajout� � la liste des noeuds � explorer
				else
				{
					targetNodeInfos =
						new NodeInformations()
					{
						CurrentNode = link.Target,
						FromNode = null,
						ComputedCost = int.MaxValue 

					};
								nodesToExplore.Add(targetNodeInfos.CurrentNode, targetNodeInfos);
				}

				// Si le nouveau co�t est inf�rieur au co�t courant du noeud voisin, on
				// met � jour.
				if (updatedCost < targetNodeInfos.ComputedCost)
				{
					targetNodeInfos.ComputedCost = updatedCost;
					targetNodeInfos.FromNode = nodeInfos;
				}
			}
		}
	}
}
