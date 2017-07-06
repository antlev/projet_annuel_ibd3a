using UnityEngine;
using System.Collections;
using System.Linq;
using System;

public class ActionSolutionScript {
    static readonly public Vector3[] _possibleActions =
    {
        Vector3.left,
        Vector3.right,
        Vector3.forward,
        Vector3.back
    };

    private Vector3 _action;

    public Vector3 Action
    {
        get { return _action; }
        set { _action = value; }
    }

    public ActionSolutionScript()
    {
        Action = _possibleActions[UnityEngine.Random.Range(0, 4)];
    }
}
