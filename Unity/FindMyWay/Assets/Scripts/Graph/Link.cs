using UnityEngine;
using System.Collections;

/// <summary>
/// Réprésente un arc entre deux noeuds
/// </summary>
public class Link {

    /// <summary>
    /// La pondération associé à l'arc
    /// </summary>
    public float Cost { get; set; }

    /// <summary>
    /// Le noeud d'origine
    /// </summary>
    public Node Origin { get; set; }

    /// <summary>
    /// Le noeud d'arrivée
    /// </summary>
    public Node Target { get; set; }
}