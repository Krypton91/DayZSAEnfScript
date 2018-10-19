class ActionBuildPartCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_CONSTRUCT);
	}
};

class ActionBuildPart: ActionContinuousBase
{
	float m_DamageAmount;
	
	string m_PartName;						//base part name
	
	void ActionBuildPart()
	{
		m_CallbackClass = ActionBuildPartCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_CRAFTING;
		m_FullBody = true;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		
		m_MessageStartFail = "I cannot build a construction part.";
		m_MessageStart = "I have build a construction part.";
		m_MessageSuccess = "I have build a construction part.";
		m_MessageFail = "I have failed to build a construction part.";
		
		m_DamageAmount = 2;
		m_SpecialtyWeight = UASoftSkillsWeight.ROUGH_HIGH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.BASEBUILDING);
	}

	override int GetType()
	{
		return AT_BUILD_PART;
	}
		
	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ActionBuildPartSwitch switch_action = ActionBuildPartSwitch.Cast( player.GetActionManager().GetAction( AT_BUILD_PART_SWITCH ) );
		string part_name_text;
		
		if ( switch_action )
		{
			if ( switch_action.m_Construction ) 
			{
				ConstructionPart construction_part = switch_action.m_Construction.GetConstructionPart( switch_action.GetActualBuildPart() );
				
				if ( construction_part )
				{
					part_name_text = construction_part.GetName();
				}				
			}
		}
		
		return "#build" + " " + part_name_text;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		string part_name;
		
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
			ActionBuildPartSwitch switch_action = ActionBuildPartSwitch.Cast( player.GetActionManager().GetAction( AT_BUILD_PART_SWITCH ) );
			
			if ( switch_action ) 
			{
				//Trigger action condition if not already triggered on Server (data setup)
				if ( GetGame().IsServer() && !switch_action.m_ACTriggeredOnServer )
				{
					switch_action.ActionCondition( player, target, item );
				}
				
				part_name = switch_action.GetActualBuildPart();
			}
			
			//Debug
			//base_building.GetConstruction().IsColliding( part_name );
			
			if ( part_name.Length() > 0 && base_building.IsFacingBack( player ) )
			{
				m_PartName = part_name;
				return true;
			}				
		}
		
		return false;
	}
		
	override void OnCompleteServer( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		
		if ( !construction.IsColliding( m_PartName ) && construction.CanBuildPart( m_PartName ) )
		{
			//build
			construction.BuildPart( m_PartName, true, true );
			
			//add damage to tool
			action_data.m_MainItem.DecreaseHealth ( m_DamageAmount );
		}
		else
		{
			SendMessageToClient( action_data.m_Player, base_building.MESSAGE_CANNOT_BE_CONSTRUCTED );
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
}