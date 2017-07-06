using UnityEngine;
using System.Collections;
using System.Linq;
using System.Collections.Generic;

/// <summary>
/// L'objet utilis� pour ex�cuter la s�quence d'action
/// </summary>
public class PlayerScript : MonoBehaviour
{

    #region StaticOperators

    public static PlayerScript CreatePlayer()
    {
        return (Instantiate((GameObject)Resources.Load("RandomPlayer", typeof(GameObject)),
            GameObject.Find("StartPosition").transform.position,
            Quaternion.identity) as GameObject).GetComponent<PlayerScript>();
    }

    #region Matrix Object Position Helper
    static public int GoalXPositionInMatrix
    {
        get
        {
            return Mathf.RoundToInt(GameObject.Find("GOAL").transform.position.x) + 25;
        }
    }

    static public int GoalYPositionInMatrix
    {
        get
        {
            return Mathf.RoundToInt(GameObject.Find("GOAL").transform.position.z) + 25;
        }
    }

    static public int StartXPositionInMatrix
    {
        get
        {
            return Mathf.RoundToInt(GameObject.Find("StartPosition").transform.position.x) + 25;
        }
    }

    static public int StartYPositionInMatrix
    {
        get
        {
            return Mathf.RoundToInt(GameObject.Find("StartPosition").transform.position.z) + 25;
        }
    }
    #endregion

    #endregion

    #region ExposedMembers

    private bool _inSimulation = false;
    public bool InSimulation
    {
        get { return _inSimulation; }
        private set { _inSimulation = value; }
    }

    private int _performedActionsNumber = 0;

    public int PerformedActionsNumber
    {
        get { return _performedActionsNumber; }
        private set { _performedActionsNumber = value; }
    }

    private int _exploredPuts = 0;

    public int ExploredPuts
    {
        get { return _exploredPuts; }
        private set { _exploredPuts = value; }
    }

    private bool _runWithoutSimulation = false;

    public bool RunWithoutSimulation
    {
        get { return _runWithoutSimulation; }
        set { _runWithoutSimulation = value; }
    }

    public int GetPutTypeAtCoordinates(int x, int y)
    {
        if (x > 49 || x < 0 || y < 0 || y > 49)
        {
            Debug.Log("La case demand�e doit �tre entre 0 et 49");
            return -42;
        }
        return Matrix[x][y];
    }

    public int PlayerXPositionInMatrix
    {
        get
        {
            return Mathf.RoundToInt(this.transform.position.x) + 25;
        }
    }

    public int PlayerYPositionInMatrix
    {
        get
        {
            return Mathf.RoundToInt(this.transform.position.z) + 25;
        }
    }

    public bool FoundGoal
    {
        get;
        set;
    }

    public bool FoundObstacle
    {
        get;
        set;
    }

    #endregion

    public float _totalAnimationTime = 42f;
    public GameObject _collisionParticleSystemPrefab;
    public int _totalSolutionSteps = 42;

    private Vector3 _initialPosition;


    private int[][] _matrix;

    public int[][] Matrix
    {
        get { return _matrix; }
        set { _matrix = value; }
    }


    PathSolutionScript _solution;

    // Use this for initialization
    void Start()
    {
        _initialPosition = transform.position;
        Matrix = MatrixFromRaycast.CreateMatrixFromRayCast();
        if (PlayerScript._trailColorIterator == null)
        {
            PlayerScript._trailColorIterator = _playerTrailColors().GetEnumerator();
        }
        PlayerScript._trailColorIterator.MoveNext();
        this.GetComponent<TrailRenderer>().time = _totalAnimationTime;
        this.GetComponent<TrailRenderer>().materials[0].SetColor("_TintColor", PlayerScript._trailColorIterator.Current);

    }

    public void LaunchSimulation(PathSolutionScript solution)
    {
        InSimulation = true;

        _initialPosition = transform.position;
        this.GetComponent<TrailRenderer>().time = _totalAnimationTime;
        _solution = solution;
        Initialize();
        StartCoroutine("PlaySolution", _totalAnimationTime);
    }

    void Initialize()
    {
        transform.position = _initialPosition;
        FoundGoal = false;
        FoundObstacle = false;
        PerformedActionsNumber = 0;
        ExploredPuts = 0;
        Matrix = MatrixFromRaycast.CreateMatrixFromRayCast();
        this.GetComponent<TrailRenderer>().time = _totalAnimationTime;
    }

    IEnumerator OnCollisionEnter(Collision col)
    {
        this.GetComponent<TrailRenderer>().time = 0;
        StopCoroutine("PlaySolution");
        StopCoroutine("PlayActionOnGameObject");
        StopCoroutine("PlaySolution");
        StopCoroutine("PlayActionOnGameObject");
        GetComponent<Animation>().Stop();

        InSimulation = false;

        yield return new WaitForSeconds(0);
    }

    public IEnumerator PlaySolution()
    {
        yield return StartCoroutine("PlayActionOnGameObject", _solution.Actions);

        InSimulation = false;
    }

    public void UpdateMatrix(Vector3 position)
    {
        PerformedActionsNumber++;
        if (Matrix[Mathf.RoundToInt(position.x + 25)][Mathf.RoundToInt(position.z + 25)] == 0)
        {
            Matrix[Mathf.RoundToInt(position.x + 25)][Mathf.RoundToInt(position.z + 25)] = 1;
            ExploredPuts++;
        }
    }

    public void RandomChangeInSolution(PathSolutionScript sol)
    {
        sol.Actions[Random.Range(0, sol.Actions.Length)] = new ActionSolutionScript();
    }

    public PathSolutionScript CopySolution(PathSolutionScript sol)
    {
        var newSol = new PathSolutionScript(_totalSolutionSteps);

        for (int i = 0; i < sol.Actions.Length; i++)
        {
            newSol.Actions[i].Action = sol.Actions[i].Action;
        }

        return newSol;
    }

    public IEnumerator PlayActionOnGameObject(ActionSolutionScript[] actions)
    {
        if (GetComponent<Animation>()["movementClip"] != null)
            GetComponent<Animation>().RemoveClip("movementClip");

        var clip = new AnimationClip();
        clip.legacy = true;

        var time = 0f;

        var xcurve = new AnimationCurve();
        var ycurve = new AnimationCurve();
        var zcurve = new AnimationCurve();

        var lastPosition = transform.position;

        xcurve.AddKey(new Keyframe(0f, lastPosition.x));
        ycurve.AddKey(new Keyframe(0f, lastPosition.y));
        zcurve.AddKey(new Keyframe(0f, lastPosition.z));


        foreach (var action in actions)
        {
            if (Matrix[Mathf.RoundToInt(lastPosition.x + 25)][Mathf.RoundToInt(lastPosition.z + 25)] == LayerMask.NameToLayer("Goal"))
            {
                FoundGoal = true;
                break;
            }
            lastPosition = lastPosition + action.Action;

            if (Matrix[Mathf.RoundToInt(lastPosition.x + 25)][Mathf.RoundToInt(lastPosition.z + 25)] == LayerMask.NameToLayer("Obstacle"))
            {
                FoundObstacle = true;
                break;
            }

            UpdateMatrix(lastPosition);

            if (RunWithoutSimulation)
                continue;

            time += _totalAnimationTime / (float)_totalSolutionSteps;

            xcurve.AddKey(new Keyframe(time, lastPosition.x));
            ycurve.AddKey(new Keyframe(time, lastPosition.y));
            zcurve.AddKey(new Keyframe(time, lastPosition.z));

            clip.SetCurve("", typeof(Transform), "localPosition.x", xcurve);
            clip.SetCurve("", typeof(Transform), "localPosition.y", ycurve);
            clip.SetCurve("", typeof(Transform), "localPosition.z", zcurve);

        }
        if (Matrix[Mathf.RoundToInt(lastPosition.x + 25)][Mathf.RoundToInt(lastPosition.z + 25)] == LayerMask.NameToLayer("Goal"))
        {
            FoundGoal = true;
        }
        if (RunWithoutSimulation)
        {
            this.transform.position = lastPosition;
            yield return 0;
        }
        else
        {
            GetComponent<Animation>().AddClip(clip, "movementClip");
            GetComponent<Animation>().Play("movementClip");

            yield return new WaitForSeconds(GetComponent<Animation>()["movementClip"].length);
        }
    }

    static IEnumerator<Color> _trailColorIterator;

    static IEnumerable<Color> _playerTrailColors()
    {
        while (true)
        {
            yield return Color.blue;
            yield return Color.red;
            yield return Color.green;
            yield return Color.yellow;
            yield return Color.magenta;
            yield return Color.cyan;
        }
    }
}
