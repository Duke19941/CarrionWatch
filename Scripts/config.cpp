class CfgPatches
{
	class CarrionWatch_Scripts
	{
		units[] = {};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Data", "DZ_Scripts" };
	};
};

class CfgMods
{
	class CarrionWatch
	{
		dir = "CarrionWatch";
		name = "Carrion Watch";
		author = "Dead Air Studio";
		version = "0.1.0";
		extra = 0;
		type = "mod";
		dependencies[] = { "Game", "World", "Mission" };

		class defs
		{
			class gameScriptModule
			{
				value = "";
				files[] = { "CarrionWatch/Scripts/3_Game" };
			};
			class worldScriptModule
			{
				value = "";
				files[] = { "CarrionWatch/Scripts/4_World" };
			};
			class missionScriptModule
			{
				value = "";
				files[] = { "CarrionWatch/Scripts/5_Mission" };
			};
		};
	};
};
