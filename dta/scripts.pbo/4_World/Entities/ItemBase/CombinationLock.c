class CombinationLock extends ItemBase
{
	static const int COMBINATION_LENGTH = 3;			//how many digits will the combination contain
	int m_Combination;									//actual combination that is dialed on lock
	int m_CombinationLocked;							//combination that was dialed on lock before the shuffle
	
	void CombinationLock()
	{
		int combination_length = Math.Pow( 10, COMBINATION_LENGTH );
		
		//synchronized variables
		RegisterNetSyncVariableInt( "m_Combination", 		0, combination_length - 1 );
		RegisterNetSyncVariableInt( "m_CombinationLocked", 	0, combination_length - 1 );
	}
	
	// --- EVENTS
	override void OnStoreSave( ParamsWriteContext ctx )
	{   
		super.OnStoreSave( ctx );
		
		//write data
		ctx.Write( m_Combination );
		ctx.Write( m_CombinationLocked );
	}
	
	override void OnStoreLoad( ParamsReadContext ctx )
	{
		super.OnStoreLoad( ctx );
		
		//read data
		ctx.Read( m_Combination );
		ctx.Read( m_CombinationLocked );
	}
	
	// --- SYNCHRONIZATION
	void Synchronize()
	{
		if ( GetGame().IsServer() )
		{
			SetSynchDirty();
		}
	}
	
	void SetCombination( int combination )
	{
		m_Combination = combination;
		
		Synchronize();
	}
	
	void SetCombinationLocked( int combination )
	{
		m_CombinationLocked = combination;
		
		Synchronize();
	}
	
	// --- ACTIONS
	int GetCombination()
	{
		return m_Combination;
	}
	
	void DialNextNumber( int dial_index )
	{
		string combination_text = m_Combination.ToString();
		string dialed_text;
		
		//insert zeros to dials with 0 value
		int length_diff = COMBINATION_LENGTH - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		//assemble the whole combination with increased part
		for ( int j = 0; j < combination_text.Length(); ++j )
		{
			if ( j == dial_index )
			{
				int next_dialed_number = combination_text.Get( j ).ToInt() + 1;
				if ( next_dialed_number > 9 )
				{
					next_dialed_number = 0;
				}
				
				dialed_text += next_dialed_number.ToString();
			}
			else
			{
				dialed_text += combination_text.Get( j );
			}
		}
		
		SetCombination( dialed_text.ToInt() );
	}
	
	//Lock lock
	void Lock( EntityAI parent )
	{
		SetLockCombination();
		ShuffleLock();
		
		//set slot lock
		int slot_id = GetInventory().GetSlotId( 0 );
		parent.GetInventory().SetSlotLock( slot_id, true );		
	}
	
	void Unlock( EntityAI parent )
	{
		//set slot unlock
		int slot_id = GetInventory().GetSlotId( 0 );
		parent.GetInventory().SetSlotLock( slot_id, false );			
		
		//drop entity from attachment slot
		if ( GetGame().IsMultiplayer() )
		{
			parent.GetInventory().DropEntity( InventoryMode.PREDICTIVE, parent, this );
		}
		else
		{
			parent.GetInventory().DropEntity( InventoryMode.LOCAL, parent, this );
		}
	}
	
	//Shuffle lock
	void ShuffleLock()
	{
		string combination_text = m_Combination.ToString();
		string shuffled_text;
		
		//insert zeros to dials with 0 value
		int length_diff = COMBINATION_LENGTH - combination_text.Length();
		for ( int i = 0; i < length_diff; ++i )
		{
			combination_text = "0" + combination_text;
		}
		
		//assemble the whole combination with increased part
		for ( int j = 0; j < combination_text.Length(); ++j )
		{
			int dial_number = combination_text.Get( j ).ToInt();
			dial_number = ( dial_number + Math.RandomInt( 1, 9 ) ) % 10;
			shuffled_text = shuffled_text + dial_number.ToString();
		}
		
		SetCombination( shuffled_text.ToInt() );
	}
	
	void SetLockCombination()
	{
		SetCombinationLocked( m_Combination );
	}
	
	bool IsLocked()
	{
		if ( m_Combination != m_CombinationLocked )
		{
			return true;
		}
		
		return false;
	}
}