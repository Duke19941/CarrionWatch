class CfgPatches
{
	class CarrionWatch_Data
	{
		units[] = { "CW_CarrionFlock" };
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = { "DZ_Data", "DZ_Gear_Camping", "DZ_Sounds_Effects" };
	};
};

class CfgVehicles
{
	class Inventory_Base;

	class CW_CarrionFlock : Inventory_Base
	{
		scope = 2;
		displayName = "";
		descriptionShort = "";
		model = "\\dz\\gear\\consumables\\stone.p3d";
		weight = 1;
		itemSize[] = { 1, 1 };
		rotationFlags = 16;
		canBeDigged = 0;
		forceFarBubble = 1;
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 1;
					healthLevels[] = { {1.0, {}}, {0.7, {}}, {0.5, {}}, {0.3, {}}, {0.0, {}} };
				};
			};
		};
	};
};

class CfgSoundShaders
{
	class CW_CarrionCall_SoundShader
	{
		samples[] =
		{
			{ "DZ\\sounds\\environment\\animals\\birds\\crow\\crow_01", 1 },
			{ "DZ\\sounds\\environment\\animals\\birds\\crow\\crow_02", 1 }
		};
		volume = 1.8;
		range = 420;
		rangeCurve = "LinearCurve";
		limitation = 0;
	};

	class CW_CarrionScatter_SoundShader
	{
		samples[] =
		{
			{ "DZ\\sounds\\environment\\animals\\birds\\wingflap\\wingflap_01", 1 },
			{ "DZ\\sounds\\environment\\animals\\birds\\crow\\crow_01", 1 }
		};
		volume = 2.2;
		range = 280;
		rangeCurve = "LinearCurve";
		limitation = 0;
	};
};

class CfgSoundSets
{
	class CW_CarrionCall_SoundSet
	{
		soundShaders[] = { "CW_CarrionCall_SoundShader" };
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 1;
		distanceFilter = "defaultDistanceFilter";
	};

	class CW_CarrionScatter_SoundSet
	{
		soundShaders[] = { "CW_CarrionScatter_SoundShader" };
		sound3DProcessingType = "character3DProcessingType";
		volumeCurve = "characterAttenuationCurve";
		spatial = 1;
		doppler = 0;
		loop = 0;
		distanceFilter = "defaultDistanceFilter";
	};
};
