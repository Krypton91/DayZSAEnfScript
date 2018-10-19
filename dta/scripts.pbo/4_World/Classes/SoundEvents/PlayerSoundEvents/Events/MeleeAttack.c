class MeleeAttackSoundEvents extends PlayerSoundEventBase
{
	override bool CanPlay()
	{
		return true;
	}
	
	override bool ThisHasPriority(PlayerBase player, EPlayerSoundEventID other_state_id)
	{
		if(PlayerSoundEventHandler.GetSoundEventType(other_state_id) != EPlayerSoundEventType.DAMAGE)
		{
			return true;
		}
		return false;
	}
}


class MeleeAttackLightEvent extends MeleeAttackSoundEvents
{
	void MeleeAttackLightEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.MELEE_ATTACK_LIGHT;
		m_SoundVoiceAnimEventClassID = 16;
	}
}

class MeleeAttackHeavyEvent extends MeleeAttackSoundEvents
{
	void MeleeAttackHeavyEvent()
	{
		m_Type = EPlayerSoundEventType.GENERAL;
		m_ID = EPlayerSoundEventID.MELEE_ATTACK_HEAVY;
		m_SoundVoiceAnimEventClassID = 17;
	}
	
}

