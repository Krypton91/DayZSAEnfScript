class RestrainingToolLocked extends ItemBase
{
	void ~RestrainingToolLocked()
	{
		PlayerBase player = PlayerBase.Cast(GetHierarchyRootPlayer());
		if( player && player.IsRestrained() )
		{
			player.SetRestrained(false);
		}
	}
	
	override void OnItemLocationChanged(EntityAI old_owner, EntityAI new_owner) 
	{ 
		if( GetGame().IsServer() )
		{
			if( old_owner )
			{
				PlayerBase old_player = PlayerBase.Cast(old_owner);
				if(old_player && old_player.IsRestrained())
				{
					Print("old_owner restrain");
					old_player.SetRestrained(false);
				}
			}
			
			if( !new_owner && old_owner )
			{
				Print("restrain item dropped - deleting");
				this.Delete();
			}
		}
	}
}

class HandcuffsLocked extends RestrainingToolLocked
{
}

class RopeLocked extends RestrainingToolLocked
{
}

class DuctTapeLocked extends RestrainingToolLocked
{
}

class MetalWireLocked extends RestrainingToolLocked
{
}

class BarbedWireLocked extends RestrainingToolLocked
{
}