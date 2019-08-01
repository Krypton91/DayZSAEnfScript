class WeaponManager
{
	protected PlayerBase 					m_player;
	
	protected int							m_LastAcknowledgmentID;
	
	protected int 							m_PendingWeaponActionAcknowledgmentID;
	protected Magazine 						m_PendingTargetMagazine;
	protected ref InventoryLocation			m_TargetInventoryLocation;
	protected int 							m_PendingWeaponAction; 
	protected ref InventoryLocation			m_PendingInventoryLocation;
	
	protected bool							m_canEnd;
	protected bool							m_justStart;
	protected bool							m_InProgress;
	protected bool							m_IsEventSended;
	protected bool							m_WantContinue;
	protected bool							m_InIronSight;
	protected bool							m_InOptic;
	protected bool 							m_readyToStart;
	protected Weapon_Base					m_WeaponInHand;
	protected MagazineStorage				m_MagazineInHand;
	protected ActionBase					m_ControlAction;
	protected int 							m_ForceEjectBulletTimestamp;
	
	protected const int 					FORCE_EJECT_BULLET_TIMEOUT = 2000;
	
	//Reload
	protected ref array<Magazine>			m_MagazinePilesInInventory;
	protected ref array<MagazineStorage>	m_MagazineStorageInInventory;
	protected ref array<Magazine>			m_SuitableMagazines;
	protected Magazine						m_PreparedMagazine;
	
	//Jamming
	protected float m_NewJamChance;
	protected bool m_WaitToSyncJamChance;
	
	void WeaponManager(PlayerBase player)
	{
		m_ForceEjectBulletTimestamp = -1;
		m_player = player;
		m_PendingWeaponActionAcknowledgmentID = -1;
		m_PendingTargetMagazine = NULL;
		m_PendingInventoryLocation = NULL;
		m_WeaponInHand = NULL;
		m_MagazineInHand = NULL;
		m_ControlAction = NULL;
		m_PendingWeaponAction = -1;
		m_LastAcknowledgmentID = 1;
		m_InProgress = false;
		m_WantContinue = true;
		m_IsEventSended = false;
		m_canEnd = false;
		m_readyToStart = false;
		
		m_NewJamChance = -1;
		m_WaitToSyncJamChance = false;
		
		m_MagazinePilesInInventory = new array<Magazine>;
		m_MagazineStorageInInventory = new array<MagazineStorage>;
		m_SuitableMagazines = new array<Magazine>;
		m_PreparedMagazine = null;
	}
//----------------------------------------------------------------------------	
// Weapon Action conditions
//----------------------------------------------------------------------------	
	bool CanFire(Weapon_Base wpn)
	{
		if( m_player.GetHumanInventory().GetEntityInHands() != wpn )
			return false;
		
		if( m_player.IsLiftWeapon() || !m_player.IsRaised() || wpn.IsDamageDestroyed() || m_player.GetDayZPlayerInventory().IsProcessing() || !m_player.IsWeaponRaiseCompleted() || m_player.IsFighting() ) 
			return false;
		
		return true;
		
	}
	
	
	bool CanAttachMagazine(Weapon_Base wpn, Magazine mag, bool reservationCheck = true )
	{	
		if ( !wpn || !mag )
			return false;
		
		if ( m_player.GetHumanInventory().GetEntityInHands() != wpn )
			return false;
		
		if ( wpn.IsDamageDestroyed())
			return false;
		
		if ( mag.GetHierarchyRootPlayer() && mag.GetHierarchyRootPlayer() != m_player )
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn, null) || m_player.GetInventory().HasInventoryReservation(mag, null)))
			return false;
		
		InventoryLocation invLoc;
		invLoc = new InventoryLocation;
		
		mag.GetInventory().GetCurrentInventoryLocation(invLoc);
		Weapon_Base wnp2;
		

		//magazine is already in weapon
		if ( Class.CastTo(wnp2,  invLoc.GetParent()) )
			return false;
			
		int muzzleIndex = wpn.GetCurrentMuzzle();
		
		if (wpn.CanAttachMagazine(muzzleIndex, mag))
			return true;

		return false;
		
	}
//---------------------------------------------------------------------------	
	
	bool CanSwapMagazine(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if ( !wpn || !mag )
			return false;
		
		if ( m_player.GetHumanInventory().GetEntityInHands() != wpn )
			return false;
		
		if ( mag.IsDamageDestroyed() || wpn.IsDamageDestroyed())
			return false;
		
		if ( mag.GetHierarchyRootPlayer() && mag.GetHierarchyRootPlayer() != m_player )
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn, null) || m_player.GetInventory().HasInventoryReservation(mag, null)))
			return false;
		

		InventoryLocation invLoc;
		invLoc = new InventoryLocation;
		
		mag.GetInventory().GetCurrentInventoryLocation(invLoc);
		Weapon_Base wnp2;
		
		//second magazine is already in weapon
		if( Class.CastTo(wnp2,  invLoc.GetParent()) )
			return false;		
		
		int muzzleIndex = wpn.GetCurrentMuzzle();	
					
		Magazine mag2;
		if( !Class.CastTo(mag2, wpn.GetMagazine(muzzleIndex)) || !GameInventory.CanSwapEntities( mag, mag2 ) )
			return false;
		
		return true;
	}
//----------------------------------------------------------------------------			
	bool CanDetachMagazine(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if ( !wpn || !mag )
			return false;
		
		if ( m_player.GetHumanInventory().GetEntityInHands() != wpn )
			return false;
		
		if ( mag.GetHierarchyParent() != wpn )
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn, null) || m_player.GetInventory().HasInventoryReservation(mag, null)))
			return false;
		
		return true;
	}
//---------------------------------------------------------------------------
	bool CanLoadBullet(Weapon_Base wpn, Magazine mag, bool reservationCheck = true)
	{
		if ( !wpn || !mag )
			return false;
		
		if ( m_player.GetHumanInventory().GetEntityInHands() != wpn )
			return false;
		
		if( mag.IsDamageDestroyed() || wpn.IsDamageDestroyed())
			return false;
		
		if ( mag.GetHierarchyRootPlayer() && mag.GetHierarchyRootPlayer() != m_player )
			return false;
		
		if( wpn.IsJammed(/*wpn.GetCurrentMuzzle()*/) )
			return false;
		
		if ( reservationCheck && (m_player.GetInventory().HasInventoryReservation(wpn,null) || m_player.GetInventory().HasInventoryReservation(mag,null)))
			return false;
		
		int muzzleIndex = wpn.GetCurrentMuzzle();
		
		if( !wpn.CanChamberBullet( muzzleIndex, mag ) )
			return false;
		
		return true;
	}
//---------------------------------------------------------------------------	
	bool CanUnjam(Weapon_Base wpn, bool reservationCheck = true)
	{
		if( !wpn )
			return false;
		 
		if( m_player.GetHumanInventory().GetEntityInHands() != wpn)
			return false;
		
		if( wpn.IsDamageDestroyed())
			return false;
		
		if ( reservationCheck && m_player.GetInventory().HasInventoryReservation(wpn, null))
			return false;

		
		if( !wpn.IsJammed(/*wpn.GetCurrentMuzzle()*/) )
			return false;
		 
		return true;
	}
	
	bool CanEjectBullet(Weapon_Base wpn, bool reservationCheck = true)
	{
		if( !wpn )
			return false;
		 
		if( m_player.GetHumanInventory().GetEntityInHands() != wpn)
			return false;
		
		if( reservationCheck && m_player.GetInventory().HasInventoryReservation(wpn, null))
			return false;
		
		if( wpn.IsJammed(/*wpn.GetCurrentMuzzle()*/) )
			return false;
		 
		return true;
	}
//----------------------------------------------------------------------------
	bool InventoryReservation( Magazine mag, InventoryLocation invLoc)
	{
		
		Weapon_Base weapon;
		InventoryLocation ilWeapon = new InventoryLocation;
		if (Weapon_Base.CastTo(weapon, m_player.GetItemInHands()) )
		{
			weapon.GetInventory().GetCurrentInventoryLocation(ilWeapon); 
			if ( m_player.GetInventory().HasInventoryReservation(weapon, ilWeapon) )
			{
				return false;
			}
			else
			{
				m_player.GetInventory().AddInventoryReservation(weapon,ilWeapon,GameInventory.c_InventoryReservationTimeoutMS);
			}	
		}
		
		if( invLoc )
		{
			if ( m_player.GetInventory().HasInventoryReservation(invLoc.GetItem(),invLoc) )
			{
				m_player.GetInventory().ClearInventoryReservation(weapon, ilWeapon);
				return false;
			}
			else
			{
				m_player.GetInventory().AddInventoryReservation(invLoc.GetItem(),invLoc,GameInventory.c_InventoryReservationTimeoutMS);
			}
		}
		
		if( mag )
		{
			m_TargetInventoryLocation = new InventoryLocation;
			mag.GetInventory().GetCurrentInventoryLocation(m_TargetInventoryLocation);
			
			if ( m_player.GetInventory().HasInventoryReservation(mag, m_TargetInventoryLocation) )
			//if ( !m_player.GetInventory().AddInventoryReservation( mag, m_TargetInventoryLocation, GameInventory.c_InventoryReservationTimeoutMS) )
			{
				m_player.GetInventory().ClearInventoryReservation(weapon, ilWeapon);
				if (invLoc)
				{
					m_player.GetInventory().ClearInventoryReservation(invLoc.GetItem(), invLoc);
				}
				return false;
			}
			else
			{
				m_player.GetInventory().AddInventoryReservation(mag, m_TargetInventoryLocation, GameInventory.c_InventoryReservationTimeoutMS);
			}
		}
		m_PendingTargetMagazine = mag;
		m_PendingInventoryLocation = invLoc;
		
		return true;
	}	
	
//----------------------------------------------------------------------------	
// Weapon Actions	
//----------------------------------------------------------------------------
	bool AttachMagazine( Magazine mag , ActionBase control_action = NULL )
	{
		return StartAction(AT_WPN_ATTACH_MAGAZINE, mag, NULL, control_action);
	}
	
	bool DetachMagazine( InventoryLocation invLoc, ActionBase control_action = NULL)
	{
		return StartAction(AT_WPN_DETACH_MAGAZINE, NULL, invLoc, control_action);
	}
	
	bool SwapMagazine( Magazine mag, ActionBase control_action = NULL )
	{
		return StartAction(AT_WPN_SWAP_MAGAZINE, mag, NULL, control_action);
	}
	
	bool LoadBullet( Magazine mag, ActionBase control_action = NULL )
	{
		return StartAction(AT_WPN_LOAD_BULLET, mag, NULL, control_action);
	}
	
	bool LoadMultiBullet( Magazine mag, ActionBase control_action = NULL )
	{
		return StartAction(AT_WPN_LOAD_MULTI_BULLETS_START, mag, NULL, control_action);
	}
	
	void LoadMultiBulletStop( )
	{
		if(m_InProgress) m_WantContinue = false;
	}
	
	bool Unjam( ActionBase control_action = NULL )
	{
		return StartAction(AT_WPN_UNJAM, NULL, NULL, control_action);
	}

	bool EjectBullet( ActionBase control_action = NULL )
	{
		return StartAction(AT_WPN_EJECT_BULLET, NULL, NULL, control_action);
	}
	
	bool EjectBulletVerified( ActionBase control_action = NULL )
	{
		
		if ( m_WeaponInHand )
		{	
			int mi = m_WeaponInHand.GetCurrentMuzzle();
			if ( !m_WeaponInHand.IsChamberFiredOut(mi) && !m_WeaponInHand.IsChamberEmpty(mi) )
			{
				int actual_time = GetGame().GetTime();
				if ( actual_time - m_ForceEjectBulletTimestamp < FORCE_EJECT_BULLET_TIMEOUT )
				{
					m_ForceEjectBulletTimestamp = actual_time;
					return StartAction(AT_WPN_EJECT_BULLET, NULL, NULL, control_action);
				}
				m_ForceEjectBulletTimestamp = actual_time;
			}
			else
			{
				m_ForceEjectBulletTimestamp = -1;
				return StartAction(AT_WPN_EJECT_BULLET, NULL, NULL, control_action);
			}
		}	
		return false;
	}
	
	bool SetNextMuzzleMode ()
	{
		return StartAction(AT_WPN_SET_NEXT_MUZZLE_MODE, NULL, NULL, NULL);
	}
	
	void Fire(Weapon_Base wpn)
	{
		int mi = wpn.GetCurrentMuzzle();
		if( wpn.IsChamberFiredOut(mi) || wpn.IsJammed() || wpn.IsChamberEmpty(mi) )
		{
			wpn.ProcessWeaponEvent(new WeaponEventTrigger(m_player));
			return;
		}
		
		if(wpn.JamCheck(0))
		{
			wpn.ProcessWeaponEvent(new WeaponEventTriggerToJam(m_player));
		}
		else
		{
			wpn.ProcessWeaponEvent(new WeaponEventTrigger(m_player));
		}
	}
//-------------------------------------------------------------------------------------	
// Synchronize - initialize from client side
//-------------------------------------------------------------------------------------

	//Client
	private void Synchronize( )
	{
		if( GetGame().IsClient() )
		{
			m_PendingWeaponActionAcknowledgmentID = ++m_LastAcknowledgmentID;
			ScriptInputUserData ctx = new ScriptInputUserData;
			
			ctx.Write(INPUT_UDT_WEAPON_ACTION);
			ctx.Write(m_PendingWeaponAction);
			ctx.Write(m_PendingWeaponActionAcknowledgmentID);
			
			
			switch (m_PendingWeaponAction)
			{
				case AT_WPN_ATTACH_MAGAZINE:
				{	
					ctx.Write(m_PendingTargetMagazine);
					break;
				}
				case AT_WPN_SWAP_MAGAZINE:
				{
					ctx.Write(m_PendingTargetMagazine);
					break;
				}
				case AT_WPN_DETACH_MAGAZINE:
				{
					m_PendingInventoryLocation.WriteToContext(ctx);
					break;
				}
				case AT_WPN_LOAD_BULLET:
				{
					ctx.Write(m_PendingTargetMagazine);
					break;
				}
				case AT_WPN_LOAD_MULTI_BULLETS_START:
				{
					ctx.Write(m_PendingTargetMagazine);
					break;
				}
				case AT_WPN_UNJAM:
				{
					break;
				}
				case AT_WPN_EJECT_BULLET:
				{
					break;
				}
				default: 
				break;
			}
			ctx.Send();
			//if( !m_player.GetDayZPlayerInventory().HasLockedHands() )
			//	m_player.GetDayZPlayerInventory().LockHands();
		}
	}
	

	
	void OnSyncJuncture(int pJunctureID, ParamsReadContext pCtx)
	{
		if (pJunctureID == DayZPlayerSyncJunctures.SJ_WEAPON_SET_JAMMING_CHANCE )
		{
			pCtx.Read(m_NewJamChance);
		}
		else
		{
			int AcknowledgmentID;
			pCtx.Read(AcknowledgmentID);
			if ( AcknowledgmentID == m_PendingWeaponActionAcknowledgmentID)
			{
				if (pJunctureID == DayZPlayerSyncJunctures.SJ_WEAPON_ACTION_ACK_ACCEPT)
				{ 
					m_readyToStart = true;
				}
				else if (pJunctureID == DayZPlayerSyncJunctures.SJ_WEAPON_ACTION_ACK_REJECT)
				{
					if(m_PendingWeaponAction >= 0 )
					{
						if(!(GetGame().IsServer() && GetGame().IsMultiplayer()))
						{
							InventoryLocation ilWeapon = new InventoryLocation;
							ItemBase weapon = m_player.GetItemInHands();
							weapon.GetInventory().GetCurrentInventoryLocation(ilWeapon);
							m_player.GetInventory().ClearInventoryReservation(m_player.GetItemInHands(),ilWeapon);
							
							if( m_PendingTargetMagazine )
							{
								m_PendingTargetMagazine.GetInventory().ClearInventoryReservation(m_PendingTargetMagazine, m_TargetInventoryLocation );
							}
							
							if( m_PendingInventoryLocation )
							{
								m_player.GetInventory().ClearInventoryReservation( NULL, m_PendingInventoryLocation );
							}
						}
						m_PendingWeaponActionAcknowledgmentID = -1;
						m_PendingTargetMagazine = NULL;
						m_PendingWeaponAction = -1;
						m_PendingInventoryLocation = NULL;
						m_InProgress = false;
					}
				}
			}
		}
	}
	

		//Server
	bool OnInputUserDataProcess(int userDataType, ParamsReadContext ctx)
	{
		Weapon_Base wpn;
		InventoryLocation il;
		int slotID;
		bool accepted = false;
		if( userDataType == INPUT_UDT_WEAPON_ACTION)
		{
			if (!ctx.Read(m_PendingWeaponAction))
				return false;
			
			if (!ctx.Read(m_PendingWeaponActionAcknowledgmentID))
				return false;
			
			if(m_PendingTargetMagazine)
			{
				GetGame().ClearJuncture(m_player, m_PendingTargetMagazine);
				m_PendingTargetMagazine = NULL;
			}
			m_InProgress = true;
			m_IsEventSended = false;
			Magazine mag = NULL;
			switch (m_PendingWeaponAction)
			{
				case AT_WPN_ATTACH_MAGAZINE:
				{
					ctx.Read(mag);
					
					
					if ( !Weapon_Base.CastTo(wpn, m_player.GetItemInHands()) )
					{
						accepted = false;
					}
					else
					{
						slotID = wpn.GetSlotFromMuzzleIndex( wpn.GetCurrentMuzzle() );
						il = new InventoryLocation;
						il.SetAttachment(wpn,mag,slotID);
						if( GetGame().AddInventoryJuncture(m_player,mag,il, false, 10000) )
							accepted = true;
					}
					m_PendingTargetMagazine = mag;
					
					//AttachMagazine(mag);
					break;
				}
				case AT_WPN_SWAP_MAGAZINE:
				{
					ctx.Read(mag);
					if( GetGame().AddActionJuncture(m_player,mag,10000) )
						accepted = true;
					m_PendingTargetMagazine = mag;
					//SwapMagazine(mag);
					break;
				}
				case AT_WPN_DETACH_MAGAZINE:
				{
					il = new InventoryLocation;
					il.ReadFromContext(ctx);
					if( GetGame().AddInventoryJuncture(m_player, il.GetItem(), il, true, 10000))
						accepted = true;
					m_PendingInventoryLocation = il;
					m_PendingTargetMagazine = Magazine.Cast(il.GetItem());
					//DetachMagazine(il);
					break;
				}
				case AT_WPN_LOAD_BULLET:
				{
					ctx.Read(mag);
					if( GetGame().AddActionJuncture(m_player,mag,10000) )
						accepted = true;
					m_PendingTargetMagazine = mag;
					//LoadBullet(mag);
					break;
				}
				case AT_WPN_LOAD_MULTI_BULLETS_START:
				{
					ctx.Read(mag);
					if( GetGame().AddActionJuncture(m_player,mag,10000) )
						accepted = true;
					m_PendingTargetMagazine = mag;
					break;
				}
				case AT_WPN_UNJAM:
				{
					accepted = true;
					//Unjam();
					break;
				}
				case AT_WPN_EJECT_BULLET:
				{
					accepted = true;
					break;
				}
				case AT_WPN_SET_NEXT_MUZZLE_MODE:
				{
					accepted = true;
					break;
				}
				default:
					Error("unknown actionID=" + m_PendingWeaponAction);
					return false;
			}
			DayZPlayerSyncJunctures.SendWeaponActionAcknowledgment(m_player, m_PendingWeaponActionAcknowledgmentID, accepted);
		}
		
		return accepted;
	}
	
	bool StartAction(int action, Magazine mag, InventoryLocation il, ActionBase control_action = NULL)
	{
		//if it is controled by action inventory reservation and synchronization provide action itself
		if(control_action)
		{
			m_ControlAction = ActionBase.Cast(control_action);
			m_PendingWeaponAction = action;
			m_InProgress = true;
			m_IsEventSended = false;
			m_PendingTargetMagazine = mag;
			m_PendingInventoryLocation = il;
			StartPendingAction();
			
			return true;
		}
		
		
		if (GetGame().IsMultiplayer() && GetGame().IsServer())
			return false;
		
		if ( !ScriptInputUserData.CanStoreInputUserData() )
			return false;
		if ( !InventoryReservation(mag, il) )
			return false;

		m_PendingWeaponAction = action;
		m_InProgress = true;
		m_IsEventSended = false;
		
		if ( !GetGame().IsMultiplayer() )
			m_readyToStart = true;
		else
			Synchronize();
		
		return true;
	}
	
	void StartPendingAction()
	{		
		m_WeaponInHand = Weapon_Base.Cast(m_player.GetItemInHands());
		if(!m_WeaponInHand)
		{
			OnWeaponActionEnd();
			return;
		}
		switch (m_PendingWeaponAction)
		{
			case AT_WPN_ATTACH_MAGAZINE:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventAttachMagazine(m_player, m_PendingTargetMagazine) );
				break;
			}
			case AT_WPN_SWAP_MAGAZINE:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventSwapMagazine(m_player, m_PendingTargetMagazine) );
				break;
			}
			case AT_WPN_DETACH_MAGAZINE:
			{
				Magazine mag = Magazine.Cast(m_PendingInventoryLocation.GetItem());
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventDetachMagazine(m_player, mag, m_PendingInventoryLocation) );
				break;
			}
			case AT_WPN_LOAD_BULLET:
			{
				m_WantContinue = false;
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventLoad1Bullet(m_player, m_PendingTargetMagazine) );
				break;
			}
			case AT_WPN_LOAD_MULTI_BULLETS_START:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventLoad1Bullet(m_player, m_PendingTargetMagazine) );
				break;
			}
			case AT_WPN_LOAD_MULTI_BULLETS_END:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventContinuousLoadBulletEnd(m_player) );
				break;
			}
			case AT_WPN_UNJAM:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventUnjam(m_player, NULL) );
				break;
			}
			case AT_WPN_EJECT_BULLET:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventMechanism(m_player, NULL) );
				break;
			}
			case AT_WPN_SET_NEXT_MUZZLE_MODE:
			{
				m_player.GetDayZPlayerInventory().PostWeaponEvent( new WeaponEventSetNextMuzzleMode(m_player, NULL) );
				break;
			}
			default:
				m_InProgress = false;
				Error("unknown actionID=" + m_PendingWeaponAction);
		}	
		m_IsEventSended = true;
		m_canEnd = false;
	}
	
	bool IsRunning()
	{
		return m_InProgress;
	}
	
	void SetRunning( bool value)
	{
		m_InProgress = value;
	}
	
	void Refresh()
	{
		OnWeaponActionEnd();
	}
	
	void Update( float deltaT )
	{

		if (m_WeaponInHand != m_player.GetItemInHands())
		{
			if( m_WeaponInHand )
			{
				m_SuitableMagazines.Clear();
				OnWeaponActionEnd();
			}
			m_WeaponInHand = Weapon_Base.Cast(m_player.GetItemInHands());
			if ( m_WeaponInHand )
			{
				m_MagazineInHand = null;
				//SET new magazine
				SetSutableMagazines();
				m_WeaponInHand.SetSyncJammingChance(0);
			}
		}
		
		if (m_WeaponInHand)
		{
		
			if (!GetGame().IsMultiplayer())
			{
				m_WeaponInHand.SetSyncJammingChance(m_WeaponInHand.GetChanceToJam());
			}
			else
			{
				if ( m_NewJamChance >= 0)
				{
					m_WeaponInHand.SetSyncJammingChance(m_NewJamChance);
					m_NewJamChance = -1;
					m_WaitToSyncJamChance = false;
				}
				if (GetGame().IsServer() && !m_WaitToSyncJamChance )
				{
					float actual_chance_to_jam;
					actual_chance_to_jam = m_WeaponInHand.GetChanceToJam();
					if ( Math.AbsFloat(m_WeaponInHand.GetSyncChanceToJam() - m_WeaponInHand.GetChanceToJam()) > 0.001 )
					{
						DayZPlayerSyncJunctures.SendWeaponJamChance(m_player, m_WeaponInHand.GetChanceToJam());
						m_WaitToSyncJamChance = true;
					}
				}
			}
			
			if(m_readyToStart)
			{
				StartPendingAction();
				m_readyToStart = false;
				return;
			}
		
			if( !m_InProgress || !m_IsEventSended )
				return;
		
			if(m_canEnd)
			{
			
				if(m_WeaponInHand.IsIdle())
				{
					OnWeaponActionEnd();
				}
				else if(m_justStart)
				{
					m_InIronSight = m_player.IsInIronsights();
					m_InOptic = m_player.IsInOptics();
		
					if(m_InIronSight || m_InOptic)
					{
						m_player.GetInputController().ResetADS();
						m_player.ExitSights();
						//Print("exitsights");
					}
				
					m_justStart = false;
				}
			
			}
			else
			{
				m_canEnd = true;
				m_justStart = true;
			}
		}
		else
		{
			if ( m_MagazineInHand != m_player.GetItemInHands() )
			{
				m_MagazineInHand = MagazineStorage.Cast(m_player.GetItemInHands());
				if ( m_MagazineInHand )
				{
					SetSutableMagazines();
				}
			}
		
		
		}
	}

	void OnWeaponActionEnd()
	{
		if ( !m_InProgress )
			return;
		
		if (!m_ControlAction)
		{
			if(GetGame().IsServer() && GetGame().IsMultiplayer())
			{
				if(m_PendingTargetMagazine)
				{
					GetGame().ClearJuncture(m_player,m_PendingTargetMagazine);
				}
			}
			else
			{
				InventoryLocation il = new InventoryLocation;
				il.SetHands(m_player,m_player.GetItemInHands());
				m_player.GetInventory().ClearInventoryReservation(m_player.GetItemInHands(),il);
						
				if( m_PendingTargetMagazine )
				{
					m_player.GetInventory().ClearInventoryReservation(m_PendingTargetMagazine, m_TargetInventoryLocation );
				}
						
				if( m_PendingInventoryLocation )
				{
					m_player.GetInventory().ClearInventoryReservation( m_PendingInventoryLocation.GetItem(), m_PendingInventoryLocation );
				}
			}
		}
		
		if(m_WeaponInHand && !m_justStart && m_player.IsRaised())
		{
			if(m_InIronSight)
				m_player.SetIronsights(m_InIronSight);
			if(m_InOptic)
			{
				ItemOptics optic = m_WeaponInHand.GetAttachedOptics();
				if(optic)
					m_player.SwitchOptics(optic,true);
			}
		}
		m_ControlAction = NULL;
		m_PendingWeaponAction = -1;
		m_PendingTargetMagazine = NULL;
		m_PendingInventoryLocation = NULL;
		m_TargetInventoryLocation = NULL;
		m_PendingWeaponActionAcknowledgmentID = -1;
		//m_WeaponInHand = NULL;
		m_InProgress = false;
		m_readyToStart = false;
		m_WantContinue = true;
		
	}
	
	bool WantContinue()
	{
		return m_WantContinue;
	}
	
	Magazine GetPreparedMagazine ()
	{
		for(int i = 0; i < m_SuitableMagazines.Count(); i++)
			if(m_SuitableMagazines.Get(i).GetAmmoCount() > 0)
				return m_SuitableMagazines.Get(i);
		
		return null;
	}
	
	void OnMagazineInventoryEnter(Magazine mag)
	{
		int i;
		MagazineStorage sMag = MagazineStorage.Cast(mag);
		if(sMag)
		{
			for(i = 0; i < m_MagazineStorageInInventory.Count(); i++ ) 
			{
				MagazineStorage s_mag_i = m_MagazineStorageInInventory[i];
				if(!s_mag_i)
				{
					m_MagazineStorageInInventory.RemoveOrdered(i);
					i--;
					continue;
				}
				
				if(CompareMagazinesSuitability(s_mag_i,sMag)<0)
					break;
			}
			m_MagazineStorageInInventory.InsertAt(sMag,i);
			
			SetSutableMagazines(); //TODO optimalize
			return;
		}
		
		if(mag)
		{

			for(i = 0; i < m_MagazinePilesInInventory.Count(); i++ ) 
			{
				Magazine mag_i = m_MagazinePilesInInventory[i];
				if(!mag_i)
				{
					m_MagazinePilesInInventory.RemoveOrdered(i);
					i--;
					continue;
				}
				
				
				if(CompareMagazinesSuitability(mag_i,mag)<0)
				{
					break;
				}
			}
			m_MagazinePilesInInventory.InsertAt(mag,i);
			SetSutableMagazines(); //TODO optimalize
		}
		
	}
	
	void OnMagazineInventoryExit(Magazine mag)
	{
		m_SuitableMagazines.RemoveItem(mag);

		MagazineStorage sMag = MagazineStorage.Cast(mag);
		if(sMag)
		{
			m_MagazineStorageInInventory.RemoveItem(sMag);
			return;
		}
		
		if(mag)
		{
			m_MagazinePilesInInventory.RemoveItem(mag);
		}
	}
	
	void OnMagazineAttach(Magazine mag)
	{
		OnMagazineInventoryExit(mag);
	}
	
	void OnMagazineDetach(Magazine mag)
	{
		OnMagazineInventoryEnter(mag);
	}
	
	int CompareMagazinesSuitability( Magazine mag1, Magazine mag2 )
	{
		return mag1.GetAmmoCount() - mag2.GetAmmoCount();
	}
	
	void SetSutableMagazines()
	{
		m_SuitableMagazines.Clear();
		int i;
		
		if(m_WeaponInHand)
		{
			int mi = m_WeaponInHand.GetCurrentMuzzle();
			
			for(i = 0; i < m_MagazineStorageInInventory.Count(); i++ )
			{
				MagazineStorage s_mag = m_MagazineStorageInInventory[i];
				
				if(!s_mag)	
				{
					m_MagazineStorageInInventory.RemoveOrdered(i);
					i--;
					continue;
				}
				
				if( m_WeaponInHand.TestAttachMagazine(mi, s_mag, false, true))
					m_SuitableMagazines.Insert(s_mag);
			}
			
			for(i = 0; i < m_MagazinePilesInInventory.Count(); i++ )
			{
				Magazine mag = m_MagazinePilesInInventory[i];
				if(!mag)	
				{
					m_MagazinePilesInInventory.RemoveOrdered(i);
					i--;
					continue;
				}
				
				if(m_WeaponInHand.CanChamberFromMag(mi, mag))
					m_SuitableMagazines.Insert(mag);	
			}
//TODO m_MagazineStorageInInventory and m_MagazinePilesInInventory always sort
		}
		else if(m_MagazineInHand)
		{
			for(i = 0; i < m_MagazinePilesInInventory.Count(); i++ )
			{
				Magazine m_mag = m_MagazinePilesInInventory[i];
				if(!m_mag)	
				{
					m_MagazinePilesInInventory.RemoveOrdered(i);
					i--;
					continue;
				}
				if(m_MagazineInHand.IsCompatiableAmmo( m_mag ))
					m_SuitableMagazines.Insert(m_mag);
			}
		}
		else
		{
			m_PreparedMagazine = null;
		}
	
	}
}