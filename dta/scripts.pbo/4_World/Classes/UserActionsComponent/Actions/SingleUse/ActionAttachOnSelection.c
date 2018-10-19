class ActionAttachOnSelection: ActionSingleUseBase
{
	int m_attSlot;
	
	void ActionAttachOnSelection()
	{
		m_MessageSuccess = "I've attached the object.";
		m_attSlot = -1;
	}

	override void CreateConditionComponents() 
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}
	
	override int GetType()
	{
		return AT_ATTACH_SELECTION;
	}
		
	override string GetText()
	{
		//return "attach on Selection";
		return "#attach";
	}
	
	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_entity = EntityAI.Cast( target.GetObject() );
		EntityAI item_entity = EntityAI.Cast( item );
		
		if ( target_entity && item_entity )
		{
			if ( !target_entity.GetInventory() ) return false;
			if ( !target_entity.GetInventory().CanAddAttachment( item_entity ) ) return false;

			array<string> selections = new array<string>;
			target_entity.GetActionComponentNameList(target.GetComponentIndex(), selections);

			//if ( IsInReach(player, target, UAMaxDistances.DEFAULT )) return false;

			for (int s = 0; s < selections.Count(); s++)
			{

				int carId = InventorySlots.GetSlotIdFromString( selections[s] );
				int slotsCnt = item_entity.GetInventory().GetSlotIdCount();

				for (int i=0; i < slotsCnt; i++ )
				{
					int itmSlotId = item_entity.GetInventory().GetSlotId(i);

					if ( carId == itmSlotId )
					{
						m_attSlot = itmSlotId;
						return true;
					}
				}
			}

		}	
		return false;
	}

	override void OnCompleteServer( ActionData action_data )
	{
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		EntityAI item_entity = EntityAI.Cast( action_data.m_MainItem );
		
		action_data.m_Player.PredictiveTakeEntityToTargetAttachmentEx(target_entity, item_entity, m_attSlot );
		//target_entity.PredictiveTakeEntityAsAttachmentEx( item_entity, m_attSlot );

	}
/*
	override void OnCompleteClient( ActionData action_data )
	{
		EntityAI target_entity = EntityAI.Cast( action_data.m_Target.GetObject() );
		EntityAI item_entity = EntityAI.Cast( action_data.m_MainItem );
		
		target_entity.LocalTakeEntityAsAttachmentEx( item_entity, m_attSlot );
	}
*/
}