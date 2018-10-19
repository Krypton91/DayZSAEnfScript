class ActionBuildPartSwitch: ActionSingleUseBase
{
	ref array<string> m_BuildParts;
	ref Construction m_Construction;
	int m_PartIndex;
	string m_MainPartName;
	bool m_ACTriggeredOnServer;			//Was Action Condition Triggered on Server
	
	void ActionBuildPartSwitch()
	{
		m_BuildParts = new array<string>;
		m_PartIndex = 0;
		m_ACTriggeredOnServer = false;
	}
	
	override void CreateConditionComponents()  
	{		
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.BASEBUILDING);
	}
	
	override int GetType()
	{
		return AT_BUILD_PART_SWITCH;
	}
	
	override string GetText()
	{
		return "#switch_to_the_next_part";
	}
	
	//TODO - refactor (obtaining data on server if action condition is not triggered - no action switch)
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		if ( GetGame().IsServer() )
		{
			m_ACTriggeredOnServer = true;
		}
		
		Object targetObject = target.GetObject();
		if ( targetObject && targetObject.CanUseConstruction() )
		{
			BaseBuildingBase base_building = BaseBuildingBase.Cast( targetObject );
			
			m_MainPartName = targetObject.GetActionComponentName( target.GetComponentIndex() );
			m_Construction = base_building.GetConstruction();
			
			m_Construction.GetConstructionPartsToBuild( m_MainPartName, m_BuildParts );		//get all parts (of main part) that can be build
			
			if ( m_BuildParts.Count() > 1 && base_building.IsFacingBack( player ) )
			{
				return true;
			}
		}
		
		return false;
	}
	
	override void Start( ActionData action_data )
	{
		SetNextIndex();
	}	
		
	override bool IsInstant()
	{
		return true;
	}
	
	override bool RemoveForceTargetAfterUse()
	{
		return false;
	}
	
	protected void SetNextIndex()
	{
		if ( m_BuildParts.Count() > 1 )
		{
			if ( m_PartIndex <= m_BuildParts.Count() - 2 )
			{
				m_PartIndex++;
			}
			else if ( m_PartIndex >= m_BuildParts.Count() >  - 1 )
			{
				m_PartIndex = 0;
			}
		}
		else
		{
			m_PartIndex = 0;
		}
	}
	
	string GetActualBuildPart()
	{
		if ( m_Construction )
		{
			m_Construction.GetConstructionPartsToBuild( m_MainPartName, m_BuildParts );		//refresh all parts that can be build
			
			if ( m_BuildParts.Count() > 0 ) 
			{
				m_PartIndex = Math.Clamp( m_PartIndex, 0, m_BuildParts.Count() - 1 );
				
				return m_BuildParts.Get( m_PartIndex );
			}
		}
		
		return "";
	}
}