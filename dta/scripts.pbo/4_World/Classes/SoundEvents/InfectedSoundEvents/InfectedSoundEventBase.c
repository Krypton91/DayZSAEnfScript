enum EInfectedSoundEventType
{
	GENERAL,
}

class InfectedSoundEventBase extends SoundEventBase
{
	ZombieBase m_Infected;
	ref EffectSound m_EffectSoundCallback;

	void InfectedSoundEventBase()
	{
		m_Type = EInfectedSoundEventType.GENERAL;
	}
	
	void ~InfectedSoundEventBase()
	{
		if(m_EffectSoundCallback != null)
		{
			SEffectManager.DestroySound(m_EffectSoundCallback);
			m_EffectSoundCallback = null;
		}
	}

	void Init(ZombieBase pInfected)
	{
		m_Infected = pInfected;
	}

	bool IsLooped()
	{
		return false;
	}

	override void Stop()
	{
		super.Stop();
		GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).RemoveByName(this, "PosUpdate");
	}
	
	void PosUpdate()
	{
		m_SoundSetCallback.SetPosition(m_Infected.GetPosition());
	}
	
	override void Play()
	{
		string soundset_name;
			
		soundset_name = m_Infected.ClassName() + "_" + m_SoundSetNameRoot + "_SoundSet";
		//Print(soundset_name);
		//m_EffectSoundCallback = m_Infected.PlayVoiceFX(soundset_name, IsLooped());
		m_SoundSetCallback = m_Infected.ProcessVoiceFX(soundset_name);
		if ( m_SoundSetCallback )
		{
			//m_SoundSetCallback.Loop(IsLooped());
			if (IsLooped())
			{
				GetGame().GetCallQueue(CALL_CATEGORY_GAMEPLAY).CallLaterByName(this, "PosUpdate", 0, true);
			}
		}
		//Print("Base::Playing: " + m_SoundSetCallback);
	}
}