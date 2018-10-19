enum MineTreeActions
{
	MINE_TREE 				= 0,
	MINE_TREE_FENCE 		= 1,
	MINE_TREE_WATCHTOWER 	= 2,
	
	COUNT					= 3
}

class ActionMineTreeActionSwitch: ActionSingleUseBase
{
	protected MineTreeActions m_MineTreeAction = MineTreeActions.MINE_TREE;
	
	void ActionMineTreeActionSwitch()
	{
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionTarget 	= new CCTTree(UAMaxDistances.DEFAULT);
		m_ConditionItem 	= new CCINonRuined;
	}
	
	override int GetType()
	{
		return AT_MINE_TREE_ACTION_SWITCH;
	}
	
	override string GetText()
	{
		return "#switch_action";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		if ( targetObject.IsTree() )
		{ 
			return true;
		}
		return false;
	}
	
	override void Start( ActionData action_data )
	{
		SetNextMineTreeAction();
	}	
	
	override bool IsInstant()
	{
		return true;
	}
	
	override bool RemoveForceTargetAfterUse()
	{
		return false;
	}
	
	protected void SetNextMineTreeAction()
	{
		if ( m_MineTreeAction < MineTreeActions.COUNT - 1 )
		{
			m_MineTreeAction++;
		}
		else
		{
			m_MineTreeAction = 0;
		}
	}
	
	MineTreeActions GetActualMineTreeAction()
	{
		return m_MineTreeAction;
	}
}