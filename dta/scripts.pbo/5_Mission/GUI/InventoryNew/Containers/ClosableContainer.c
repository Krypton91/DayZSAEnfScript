class ClosableContainer: Container
{
	protected bool m_Closed;
	protected EntityAI m_Entity;

	void ClosableContainer( LayoutHolder parent )
	{
		m_Body = new array<ref LayoutHolder>;
		m_Body.Insert( new ClosableHeader( this, "CloseButtonOnMouseButtonDown" ) );
	}

	void Open()
	{
		ItemManager.GetInstance().SetDefaultOpenState( m_Entity.GetType(), true );
		m_Closed = false;
		OnShow();
		m_Parent.m_Parent.Refresh();
	}
	
	void SetOpenState( bool state )
	{
		ItemManager.GetInstance().SetDefaultOpenState( m_Entity.GetType(), state );
		m_Closed = !state;
		if( !m_Closed )
		{
			OnShow();
		}
		else
		{
			OnHide();
		}
	}

	void Close()
	{
		ItemManager.GetInstance().SetDefaultOpenState( m_Entity.GetType(), false );
		m_Closed = true;
		this.OnHide();
	}

	bool IsOpened()
	{
		return !m_Closed;
	}

	override void SetLayoutName()
	{
		m_LayoutName = WidgetLayoutName.ClosableContainer;
	}

	override void OnShow()
	{
		if( !m_Closed )
		{
			super.OnShow();
		}
	}

	override void Insert( LayoutHolder container )
	{
		m_Body.Insert( container );
	}

	override LayoutHolder Get( int x )
	{
		if( m_Body && x < m_Body.Count() && x >= 0 )
			return m_Body.Get( x );
		return null;
	}

	override void Refresh()
	{
		if( !m_Closed )
		{
			super.Refresh();
		}
	}

	void CloseButtonOnMouseButtonDown()
	{
		m_Closed = true;
		this.OnHide();
		m_Parent.m_Parent.Refresh();
	}
}
