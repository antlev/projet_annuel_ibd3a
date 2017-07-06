using UnityEngine;
using System.Collections;

/// <summary>
/// Représente un noeud dans un graphe
/// </summary>
public class Node {

    /// <summary>
    /// L'identifiant du noeud
    /// </summary>
    public int ID { get; set; }

    /// <summary>
    /// L'ensemble des arcs sortants du noeud
    /// </summary>
    public Link[] Links { get; set; }
}
