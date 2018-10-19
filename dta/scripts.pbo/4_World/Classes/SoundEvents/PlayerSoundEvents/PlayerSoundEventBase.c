enum EPlayerSoundEventType
{
	GENERAL,
	STAMINA,
	DAMAGE,
}

class PlayerSoundEventBase extends SoundEventBase
{
	PlayerBase m_Player;
	
	void PlayerSoundEventBase()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
	}
	
	void ~PlayerSoundEventBase()
	{
		if(m_SoundSetCallback) m_SoundSetCallback.Stop();
	}
	
	int GetSoundVoiceAnimEventClassID()
	{
		return m_SoundVoiceAnimEventClassID;
	}
	
	bool ThisHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id)
	{
		return true;
	}
	
	void OnTick(float delta_time)
	{
		// !check for playback end
		if(!m_SoundSetCallback)
			delete this;
		
		// !update pos
		if(m_SoundSetCallback)
			m_SoundSetCallback.SetPosition(m_Player.GetPosition());
		//PrintString(m_Player.GetPosition().ToString());
		
	}
	
	bool CanPlay(PlayerBase player)
	{
		if( player.IsHoldingBreath() ) 
		{
			return false;
		}
		return true;
	}
	
	void Init(PlayerBase player)
	{
		m_Player = player;
	}
	
	override void Play()
	{
		m_SoundSetCallback = m_Player.ProcessVoiceEvent("","", m_SoundVoiceAnimEventClassID);
	}
}