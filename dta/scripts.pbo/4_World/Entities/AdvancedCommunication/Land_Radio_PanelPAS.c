class Land_Radio_PanelPAS extends PASBroadcaster
{
	//--- BASE
	override bool IsStaticTransmitter()
	{
		return true;
	}

	//--- POWER EVENTS
	override void OnSwitchOn()
	{
		if ( !GetCompEM().CanWork() )
		{
			GetCompEM().SwitchOff();
		}
	}
	
	override void OnWorkStart()
	{
		//turn off device
		SwitchOn ( true ); // start send/receive voice
	}
	
	override void OnWorkStop()
	{
		//auto switch off (EM)
		GetCompEM().SwitchOff();
		
		//turn off device
		SwitchOn ( false ); // stop send/receive voice
	}
}
