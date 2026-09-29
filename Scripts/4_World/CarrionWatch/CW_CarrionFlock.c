class CW_CarrionFlock extends Inventory_Base
{
	protected bool m_Active;
	protected bool m_Scattered;
	protected bool m_FxSmoke;
	protected bool m_FxFlies;
	protected bool m_FxCall;
	protected float m_LifeLeft;
	protected float m_ScatterLeft;
	protected float m_SmokeHeight;
	protected PlayerBase m_Corpse;

	protected Particle m_Smoke;
	protected Particle m_Flies;
	protected EffectSound m_Call;

	void CW_CarrionFlock()
	{
		RegisterNetSyncVariableBool("m_Active");
		RegisterNetSyncVariableBool("m_Scattered");
		RegisterNetSyncVariableBool("m_FxSmoke");
		RegisterNetSyncVariableBool("m_FxFlies");
		RegisterNetSyncVariableBool("m_FxCall");
		RegisterNetSyncVariableFloat("m_SmokeHeight");
		SetEventMask(EntityEvent.INIT);
	}

	override void EEInit()
	{
		super.EEInit();
		SetTakeable(false);
	}

	override bool CanPutInCargo(EntityAI parent)
	{
		return false;
	}

	override bool CanPutIntoHands(EntityAI parent)
	{
		return false;
	}

	override bool CanReceiveItemIntoCargo(EntityAI item)
	{
		return false;
	}

	override bool IsInventoryVisible()
	{
		return false;
	}

	PlayerBase GetCorpse()
	{
		return m_Corpse;
	}

	void Arm(PlayerBase corpse, float lifetime)
	{
		CW_Settings settings = CW_Config.Get();
		m_Corpse = corpse;
		m_LifeLeft = lifetime;
		m_ScatterLeft = 0;
		m_Scattered = false;
		m_Active = true;
		m_FxSmoke = settings.enableSmoke;
		m_FxFlies = settings.enableFlies;
		m_FxCall = settings.enableCall;
		m_SmokeHeight = settings.smokeHeight;
		SetSynchDirty();
	}

	void Dismiss()
	{
		m_Active = false;
		m_Scattered = false;
		SetSynchDirty();

		if (GetGame() && GetGame().IsServer())
		{
			CW_Manager.Get().UnregisterFlock(this);
			GetGame().GetCallQueue(CALL_CATEGORY_SYSTEM).CallLater(DeleteSafe, 400, false);
		}
	}

	void ServerPulse(CW_Settings settings)
	{
		if (!GetGame().IsServer())
			return;

		if (m_Corpse)
		{
			vector pos = m_Corpse.GetPosition();
			if (pos != "0 0 0")
				SetPosition(pos);
		}
		else
		{
			Dismiss();
			return;
		}

		if (m_Scattered)
		{
			m_ScatterLeft = m_ScatterLeft - 1.0;
			if (m_ScatterLeft <= 0)
			{
				m_Scattered = false;
				m_Active = true;
				SetSynchDirty();
			}
		}
		else
		{
			m_LifeLeft = m_LifeLeft - 1.0;
			if (m_LifeLeft <= 0)
			{
				Dismiss();
				return;
			}

			if (settings.enableScatter)
				MaybeScatterFromGunfire(settings);
		}
	}

	protected void MaybeScatterFromGunfire(CW_Settings settings)
	{
		array<Man> players = new array<Man>;
		GetGame().GetPlayers(players);

		vector origin = GetPosition();
		float radiusSq = settings.gunshotRadius * settings.gunshotRadius;

		for (int i = 0; i < players.Count(); i++)
		{
			PlayerBase pb = PlayerBase.Cast(players.Get(i));
			if (!pb || !pb.IsAlive())
				continue;

			if (vector.DistanceSq(origin, pb.GetPosition()) > radiusSq)
				continue;

			WeaponManager wm = pb.GetWeaponManager();
			if (wm && wm.IsShooting())
			{
				Scatter(settings.scatterSeconds);
				return;
			}
		}
	}

	void Scatter(float duration)
	{
		if (m_Scattered)
			return;

		m_Scattered = true;
		m_Active = false;
		m_ScatterLeft = duration;
		SetSynchDirty();
		Print(CW_Constants.LOG_PREFIX + "Flock scattered");
	}

	override void EEHitBy(TotalDamageResult damageResult, int damageType, EntityAI source, int component, string dmgZone, string ammo, vector modelPos, float speedCoef)
	{
		super.EEHitBy(damageResult, damageType, source, component, dmgZone, ammo, modelPos, speedCoef);

		if (GetGame().IsServer() && CW_Config.Get().enableScatter)
			Scatter(CW_Config.Get().scatterSeconds);
	}

	override void OnVariablesSynchronized()
	{
		super.OnVariablesSynchronized();
		ApplyClientFx();
	}

	override void EEDelete(EntityAI parent)
	{
		StopClientFx(true);
		super.EEDelete(parent);
	}

	protected void ApplyClientFx()
	{
		if (GetGame().IsDedicatedServer())
			return;

		if (m_Active && !m_Scattered)
		{
			StartClientFx();
		}
		else
		{
			bool playScatterPop = m_Scattered && m_FxCall;
			StopClientFx(playScatterPop);
		}
	}

	protected void StartClientFx()
	{
		if (GetGame().IsDedicatedServer())
			return;

		float height = m_SmokeHeight;
		if (height <= 0)
			height = 1.6;
		vector smokeOff = Vector(0, height, 0);

		if (m_FxSmoke && !m_Smoke)
		{
			m_Smoke = Particle.PlayOnObject(ParticleList.CAMP_SMALL_SMOKE, this, smokeOff);
		}

		if (m_FxFlies && !m_Flies)
		{
			m_Flies = Particle.PlayOnObject(ParticleList.ENV_SWARMING_FLIES, this, "0 0.4 0");
		}

		if (m_FxCall && (!m_Call || !m_Call.IsSoundPlaying()))
		{
			m_Call = SEffectManager.PlaySoundOnObject(CW_Constants.SOUND_CALL, this, 0.4, 0.4, true);
		}
	}

	protected void StopClientFx(bool playScatter)
	{
		if (GetGame().IsDedicatedServer())
			return;

		if (m_Smoke)
		{
			m_Smoke.Stop();
			m_Smoke = null;
		}
		if (m_Flies)
		{
			m_Flies.Stop();
			m_Flies = null;
		}
		if (m_Call)
		{
			m_Call.SoundStop();
			m_Call = null;
		}

		if (playScatter)
		{
			EffectSound scatter = SEffectManager.PlaySoundOnObject(CW_Constants.SOUND_SCATTER, this);
			if (scatter)
				scatter.SetAutodestroy(true);
		}
	}
};
