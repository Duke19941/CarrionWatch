modded class PlayerBase
{
	override void EEKilled(Object killer)
	{
		super.EEKilled(killer);

		if (GetGame() && GetGame().IsServer())
			CW_Manager.Get().OnPlayerDied(this);
	}

	override void EEDelete(EntityAI parent)
	{
		if (GetGame() && GetGame().IsServer() && !IsAlive())
			CW_Manager.Get().OnCorpseGone(this);

		super.EEDelete(parent);
	}
};
