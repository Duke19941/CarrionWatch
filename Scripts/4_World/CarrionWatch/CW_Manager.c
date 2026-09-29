class CW_Pending
{
	PlayerBase m_Corpse;
	vector m_Pos;
	float m_DueAt;
};

class CW_Manager
{
	protected static ref CW_Manager s_Instance;

	protected ref array<ref CW_Pending> m_Pending;
	protected ref array<CW_CarrionFlock> m_Flocks;
	protected float m_Clock;

	static CW_Manager Get()
	{
		if (!s_Instance)
			s_Instance = new CW_Manager();
		return s_Instance;
	}

	void CW_Manager()
	{
		m_Pending = new array<ref CW_Pending>;
		m_Flocks = new array<CW_CarrionFlock>;
		m_Clock = 0;
	}

	void StartTicking()
	{
		GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(Tick, 1000, true);
	}

	void OnPlayerDied(PlayerBase player)
	{
		CW_Settings settings = CW_Config.Get();
		if (!settings || !settings.enabled || !player)
			return;

		if (settings.playersOnly && player.IsInherited(PlayerBase) == false)
			return;

		CW_Pending pending = new CW_Pending();
		pending.m_Corpse = player;
		pending.m_Pos = player.GetPosition();
		pending.m_DueAt = m_Clock + settings.delaySeconds;
		m_Pending.Insert(pending);

		Print(CW_Constants.LOG_PREFIX + "Death queued. Flock in " + settings.delaySeconds.ToString() + "s");
	}

	void OnCorpseGone(PlayerBase player)
	{
		if (!player)
			return;

		for (int i = m_Flocks.Count() - 1; i >= 0; i--)
		{
			CW_CarrionFlock flock = m_Flocks.Get(i);
			if (flock && flock.GetCorpse() == player)
			{
				flock.Dismiss();
			}
		}

		for (int j = m_Pending.Count() - 1; j >= 0; j--)
		{
			if (m_Pending.Get(j) && m_Pending.Get(j).m_Corpse == player)
				m_Pending.Remove(j);
		}
	}

	void UnregisterFlock(CW_CarrionFlock flock)
	{
		int idx = m_Flocks.Find(flock);
		if (idx >= 0)
			m_Flocks.Remove(idx);
	}

	protected void Tick()
	{
		if (!GetGame() || !GetGame().IsServer())
			return;

		m_Clock = m_Clock + 1.0;

		CW_Settings settings = CW_Config.Get();
		if (!settings || !settings.enabled)
			return;

		PromoteDue();
		UpdateFlocks(settings);
	}

	protected void PromoteDue()
	{
		for (int i = m_Pending.Count() - 1; i >= 0; i--)
		{
			CW_Pending pending = m_Pending.Get(i);
			if (!pending)
			{
				m_Pending.Remove(i);
				continue;
			}

			if (m_Clock < pending.m_DueAt)
				continue;

			m_Pending.Remove(i);

			PlayerBase corpse = pending.m_Corpse;
			vector pos = pending.m_Pos;

			if (corpse)
			{
				pos = corpse.GetPosition();
			}

			SpawnFlock(corpse, pos);
		}
	}

	protected void SpawnFlock(PlayerBase corpse, vector pos)
	{
		if (pos == "0 0 0")
			return;

		Object obj = GetGame().CreateObjectEx(CW_Constants.FLOCK_CLASS, pos, ECE_PLACE_ON_SURFACE | ECE_NOLIFETIME);
		CW_CarrionFlock flock = CW_CarrionFlock.Cast(obj);
		if (!flock)
		{
			Print(CW_Constants.LOG_PREFIX + "Failed to spawn flock entity");
			return;
		}

		flock.Arm(corpse, CW_Config.Get().lifetimeSeconds);
		m_Flocks.Insert(flock);
		Print(CW_Constants.LOG_PREFIX + "Flock spawned at " + pos.ToString());
	}

	protected void UpdateFlocks(CW_Settings settings)
	{
		for (int i = m_Flocks.Count() - 1; i >= 0; i--)
		{
			CW_CarrionFlock flock = m_Flocks.Get(i);
			if (!flock)
			{
				m_Flocks.Remove(i);
				continue;
			}

			flock.ServerPulse(settings);
		}
	}
};
