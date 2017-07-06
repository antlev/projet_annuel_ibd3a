using UnityEngine;
using System.Collections;

/// <summary>
/// R�pr�sente un arc entre deux noeuds
/// </summary>
public class Link {

    /// <summary>
    /// La pond�ration associ� � l'arc
    /// </summary>
    public float Cost { get; set; }

    /// <summary>
    /// Le noeud d'origine
    /// </summary>
    public Node Origin { get; set; }

    /// <summary>
    /// Le noeud d'arriv�e
    /// </summary>
    public Node Target { get; set; }
}