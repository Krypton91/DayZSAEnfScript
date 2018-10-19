class ActionMineTreeFenceCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_CRAFT);
	}
};

class ActionMineTreeFence: ActionContinuousBase
{
	void ActionMineTreeFence()
	{
		m_CallbackClass = ActionMineTreeFenceCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_HACKTREE;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_HIGH;
	}
	
	override void CreateConditionComponents()  
	{	
		m_ConditionTarget 	= new CCTTree(UAMaxDistances.DEFAULT);
		m_ConditionItem 	= new CCINonRuined;
	}

	override int GetType()
	{
		return AT_MINE_TREE_FENCE;
	}
		
	override string GetText()
	{
		return "#craft_fence_kit";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		ActionMineTreeActionSwitch switch_action = ActionMineTreeActionSwitch.Cast( player.GetActionManager().GetAction( AT_MINE_TREE_ACTION_SWITCH ) );
		
		if ( switch_action.GetActualMineTreeAction() == MineTreeActions.MINE_TREE_FENCE && targetObject.IsTree() )
		{ 
			return true;
		}
		
		return false;
	}
	
	override void OnCompleteServer( ActionData action_data )
	{			
		Object targetObject = action_data.m_Target.GetObject();
		
		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
		
		//put the tree down
		targetObject.DecreaseHealth( "", "", targetObject.GetHealth() );
		
		//spawn fence
		GetGame().CreateObject( "Fence", action_data.m_Player.GetPosition(), false );
	}

};