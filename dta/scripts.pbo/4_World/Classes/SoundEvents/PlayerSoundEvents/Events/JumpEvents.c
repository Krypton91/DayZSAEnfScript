class JumpEventsBase extends PlayerSoundEventBase
{
	override bool ThisHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id)
	{
		return true;
	}
	
	override bool CanPlay()
	{
		if( !super.CanPlay() )
		{
			return false;
		}
		return true;
	}
}

class JumpSoundEvent extends SymptomSoundEventBase
{
	void JumpSoundEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.JUMP;
		m_SoundVoiceAnimEventClassID = 18;
	}
}