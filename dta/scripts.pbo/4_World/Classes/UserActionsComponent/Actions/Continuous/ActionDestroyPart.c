class ActionDestroyPartCB : ActionContinuousBaseCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_DESTROY);
	}
};

class ActionDestroyPart: ActionContinuousBase
{
	float m_DamageAmount;
	
	string m_PartName;						//base part name
	string m_PartNameText;					//custom name of the part that will be displayed as action prompt
	
	void ActionDestroyPart()
	{
		m_CallbackClass = ActionDestroyPartCB;
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
		return AT_DESTROY_PART;
	}
		
	override string GetText()
	{
		return "#destroy" + " " +  m_PartNameText;		//add name of the construction part if possible
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		Object targetObject = target.GetObject();
		
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			string part_name = targetObject.GetActionComponentName(target.GetComponentIndex());
			
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
			Construction construction = base_building.GetConstruction();		
			
			ConstructionPart construction_part = construction.GetConstructionPartToDestroy( part_name );
			if ( construction_part && base_building.IsFacingFront( player ) )
			{
				//if part is base but more attachments are present
				if ( construction_part.IsBase() && base_building.GetInventory().AttachmentCount() > 1 )
				{
					return false;
				}
				
				m_PartName = construction_part.GetPartName();
				m_PartNameText = construction_part.GetName();
			
				return true;
			}
		}
		
		return false;
	}
	
	override void OnCompleteServer( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		
		if ( construction.CanDestroyPart( m_PartName ) )
		{
			//build
			construction.DestroyPart( m_PartName, false );
			
			//add damage to tool
			action_data.m_MainItem.DecreaseHealth ( "", "", m_DamageAmount, true );
		}
		else
		{
			SendMessageToClient( action_data.m_Player, base_building.MESSAGE_CANNOT_BE_DECONSTRUCTED );
		}

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	override void OnCompleteClient( ActionData action_data )
	{	
		BaseBuildingBase base_building = BaseBuildingBase.Cast( action_data.m_Target.GetObject() );
		Construction construction = base_building.GetConstruction();
		
		if ( construction.CanDestroyPart( m_PartName ) )
		{
			//dismantle
			construction.DestroyPart( m_PartName, false );
		}
	}
}