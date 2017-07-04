using UnityEngine;
using System.Linq;
using System.Collections.Generic;
using System.Collections;

public class PathSolutionScript {
    ActionSolutionScript[] _actions;

    public ActionSolutionScript[] Actions
    {
        get { return _actions; }
        set { _actions = value; }
    }

    public PathSolutionScript(int numActions)
    {
        _actions = new ActionSolutionScript[numActions];

        for (int i = 0; i < numActions; i++)
            _actions[i] = new ActionSolutionScript();
    }
}
