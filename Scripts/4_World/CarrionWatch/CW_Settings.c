class CW_Settings
{
	bool enabled = true;
	bool playersOnly = true;
	bool enableSmoke = true;
	bool enableFlies = true;
	bool enableCall = true;
	bool enableScatter = true;

	float delaySeconds = 90.0;
	float lifetimeSeconds = 720.0;
	float scatterSeconds = 120.0;
	float gunshotRadius = 35.0;
	float smokeHeight = 1.6;

	void ResetToDefaults()
	{
		enabled = true;
		playersOnly = true;
		enableSmoke = true;
		enableFlies = true;
		enableCall = true;
		enableScatter = true;
		delaySeconds = CW_Constants.DEFAULT_DELAY_SEC;
		lifetimeSeconds = CW_Constants.DEFAULT_LIFETIME_SEC;
		scatterSeconds = CW_Constants.DEFAULT_SCATTER_SEC;
		gunshotRadius = CW_Constants.DEFAULT_GUNSHOT_RADIUS;
		smokeHeight = CW_Constants.DEFAULT_SMOKE_HEIGHT;
	}
};

class CW_Config
{
	protected static ref CW_Settings s_Settings;

	static CW_Settings Get()
	{
		if (!s_Settings)
			s_Settings = new CW_Settings();
		return s_Settings;
	}

	static void Load()
	{
		s_Settings = new CW_Settings();

		if (!FileExist(CW_Constants.PROFILE_DIR))
			MakeDirectory(CW_Constants.PROFILE_DIR);

		if (FileExist(CW_Constants.SETTINGS_PATH))
		{
			JsonFileLoader<CW_Settings>.JsonLoadFile(CW_Constants.SETTINGS_PATH, s_Settings);
			Print(CW_Constants.LOG_PREFIX + "Loaded " + CW_Constants.SETTINGS_PATH);
		}
		else
		{
			s_Settings.ResetToDefaults();
			Save();
			Print(CW_Constants.LOG_PREFIX + "Wrote default settings");
		}
	}

	static void Save()
	{
		if (!s_Settings)
			s_Settings = new CW_Settings();
		if (!FileExist(CW_Constants.PROFILE_DIR))
			MakeDirectory(CW_Constants.PROFILE_DIR);
		JsonFileLoader<CW_Settings>.JsonSaveFile(CW_Constants.SETTINGS_PATH, s_Settings);
	}
};
