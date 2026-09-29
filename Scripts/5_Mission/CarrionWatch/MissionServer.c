modded class MissionServer
{
	override void OnInit()
	{
		super.OnInit();
		CW_Config.Load();
		CW_Manager.Get().StartTicking();
		Print(CW_Constants.LOG_PREFIX + "Server ready. enabled=" + CW_Config.Get().enabled.ToString());
	}
};

modded class MissionGameplay
{
	override void OnInit()
	{
		super.OnInit();
		Print(CW_Constants.LOG_PREFIX + "Client ready.");
	}
};
