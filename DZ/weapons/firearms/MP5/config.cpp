////////////////////////////////////////////////////////////////////
//DeRap: weapons_firearms\MP5\config.bin
//Produced from mikero's Dos Tools Dll version 7.27
//https://bytex.market/products/item/weodpphdknnzm70o0h8q/Mikero%27s%20Dos%20Tools
//'now' is Tue Aug 20 16:37:18 2019 : 'file' last modified on Tue Aug 13 08:22:33 2019
////////////////////////////////////////////////////////////////////

#define _ARMA_

//(8 Enums)
enum {
	destructengine = 2,
	destructdefault = 6,
	destructwreck = 7,
	destructtree = 3,
	destructtent = 4,
	destructno = 0,
	destructman = 5,
	destructbuilding = 1
};

class CfgPatches
{
	class DZ_Weapons_Firearms_MP5
	{
		units[] = {"MP5K"};
		weapons[] = {};
		requiredVersion = 0.1;
		requiredAddons[] = {"DZ_Data","DZ_Weapons_Firearms"};
	};
};
class Mode_Safe;
class Mode_SemiAuto;
class Mode_Burst;
class Mode_FullAuto;
class OpticsInfoRifle;
class cfgWeapons
{
	class Rifle_Base;
	class MP5K_Base: Rifle_Base
	{
		scope = 0;
		weight = 1840;
		absorbency = 0.0;
		repairableWithKits[] = {5,1};
		repairCosts[] = {30.0,25.0};
		PPDOFProperties[] = {1,0.75,40,180,4,10};
		ironsightsExcludingOptics[] = {"M4_CarryHandleOptic","BUISOptic","M68Optic","M4_T3NRDSOptic","ReflexOptic"};
		WeaponLength = 0.583269;
		chamberSize = 1;
		chamberedRound = "";
		chamberableFrom[] = {"Ammo_9x19"};
		magazines[] = {"Mag_MP5_15Rnd","Mag_MP5_30Rnd"};
		magazineSwitchTime = 0.5;
		ejectType = 1;
		recoilModifier[] = {1,1,1};
		simpleHiddenSelections[] = {"hide_barrel"};
		drySound[] = {"dz\sounds\weapons\firearms\m4a1\m4_dry",0.5,1,20};
		reloadMagazineSound[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_reload",0.8,1,20};
		reloadAction = "ReloadMP5";
		hiddenSelections[] = {"camo"};
		modes[] = {"FullAuto","SemiAuto"};
		class SemiAuto: Mode_SemiAuto
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			begin1[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_single_0",1,1,900};
			begin2[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_single_1",1,1,900};
			begin3[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_single_2",1,1,900};
			soundBegin[] = {"begin1",0.33333,"begin2",0.33333,"begin3",0.33333};
			reloadTime = 0.125;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
			beginSilenced_Pro[] = {"dz\sounds\weapons\firearms\mp5k\mp5Silenced",1,1,75};
			beginSilenced_HomeMade[] = {"dz\sounds\weapons\firearms\mp5k\mp5Silenced",1,1,100};
			soundBeginExt[] = {{"beginSilenced_Pro",1},{"beginSilenced_HomeMade",1}};
		};
		class FullAuto: Mode_FullAuto
		{
			soundSetShot[] = {"MP5K_Shot_SoundSet","MP5K_Tail_SoundSet","MP5K_InteriorTail_SoundSet"};
			soundSetShotExt[] = {{"MP5K_silencerPro_SoundSet","MP5K_silencerTail_SoundSet","MP5K_silencerInteriorTail_SoundSet"}};
			begin1[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_single_0",1,1,900};
			begin2[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_single_1",1,1,900};
			begin3[] = {"dz\sounds\weapons\firearms\mp5k\mp5k_single_2",1,1,900};
			soundBegin[] = {"begin1",0.33333,"begin2",0.33333,"begin3",0.33333};
			reloadTime = 0.067;
			recoil = "recoil_mp5";
			recoilProne = "recoil_mp5_prone";
			dispersion = 0.003;
			magazineSlot = "magazine";
			beginSilenced_Pro[] = {"dz\sounds\weapons\firearms\mp5k\mp5Silenced",1,1,75};
			beginSilenced_HomeMade[] = {"dz\sounds\weapons\firearms\mp5k\mp5Silenced",1,1,100};
			soundBeginExt[] = {{"beginSilenced_Pro",1},{"beginSilenced_HomeMade",1}};
		};
		class Particles
		{
			class OnFire
			{
				class MuzzleFlash
				{
					ignoreIfSuppressed = 1;
					illuminateWorld = 1;
				};
				class SmokeCloud
				{
					overrideParticle = "weapon_shot_winded_smoke_small";
				};
				class ChamberSmokeBurst
				{
					overrideParticle = "weapon_shot_chamber_smoke";
					overridePoint = "Nabojnicestart";
					overrideDirectionPoint = "Nabojniceend";
				};
			};
			class OnOverheating
			{
				shotsToStartOverheating = 3;
				maxOverheatingValue = 10;
				overheatingDecayInterval = 1;
				class SmokingBarrel
				{
					overrideParticle = "smoking_barrel_small";
					onlyWithinOverheatLimits[] = {0,0.8};
					onlyWithinRainLimits[] = {0.0,0.2};
				};
				class SmokingBarrelHotSteam
				{
					overrideParticle = "smoking_barrel_steam_small";
					positionOffset[] = {0.1,0.02,0};
					onlyWithinOverheatLimits[] = {0,1};
					onlyWithinRainLimits[] = {0.2,1};
				};
				class OpenChamberSmoke
				{
					onlyIfBoltIsOpen = 1;
					overrideParticle = "smoking_barrel_small";
					overridePoint = "Nabojnicestart";
				};
			};
			class OnBulletCasingEject
			{
				class ChamberSmokeRaise
				{
					overrideParticle = "weapon_shot_chamber_smoke_raise";
					overridePoint = "Nabojnicestart";
				};
			};
		};
		class OpticsInfo: OpticsInfoRifle
		{
			memoryPointCamera = "eye";
			modelOptics = "-";
			distanceZoomMin = 100;
			distanceZoomMax = 100;
			discreteDistance[] = {25};
			discreteDistanceInitIndex = 0;
		};
		class AnimEvents
		{
			class SoundWeapon
			{
				class walkErc_L
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 1;
				};
				class walkErc_R
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_R_SoundSet";
					id = 2;
				};
				class runErc_L
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 3;
				};
				class runErc_R
				{
					soundSet = "Weapon_Movement_Rifle_runErc_R_SoundSet";
					id = 4;
				};
				class sprintErc_L
				{
					soundSet = "Weapon_Movement_Rifle_sprintErc_SoundSet";
					id = 5;
				};
				class sprintErc_R
				{
					soundSet = "Weapon_Movement_Rifle_sprintErc_SoundSet";
					id = 6;
				};
				class walkCro_L
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 21;
				};
				class walkCro_R
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 22;
				};
				class runCro_L
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 23;
				};
				class runCro_R
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 24;
				};
				class walkProne_L
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 31;
				};
				class walkProne_R
				{
					soundSet = "Weapon_Movement_Rifle_walkCro_SoundSet";
					id = 32;
				};
				class runProne_L
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 33;
				};
				class runProne_R
				{
					soundSet = "Weapon_Movement_Rifle_runCro_SoundSet";
					id = 34;
				};
				class jumpErc_L
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 35;
				};
				class jumpErc_R
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 36;
				};
				class landFootErc_L
				{
					soundSet = "Weapon_Movement_Rifle_landFootErc_SoundSet";
					id = 37;
				};
				class landFootErc_R
				{
					soundSet = "Weapon_Movement_Rifle_landFootErc_SoundSet";
					id = 38;
				};
				class walkRasErc_L
				{
					soundSet = "Weapon_Movement_Rifle_walkRasErc_SoundSet";
					id = 51;
				};
				class walkRasErc_R
				{
					soundSet = "Weapon_Movement_Rifle_walkRasErc_SoundSet";
					id = 52;
				};
				class runRasErc_L
				{
					soundSet = "Weapon_Movement_Rifle_runRasErc_SoundSet";
					id = 53;
				};
				class runRasErc_R
				{
					soundSet = "Weapon_Movement_Rifle_runRasErc_SoundSet";
					id = 54;
				};
				class HandStep_L
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 61;
				};
				class HandStep_R
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 62;
				};
				class HandStep_Hard_L
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 63;
				};
				class HandStep_Hard_R
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 64;
				};
				class landFeetErc
				{
					soundSet = "Weapon_Movement_Rifle_landFeetErc_SoundSet";
					id = 100;
				};
				class Weapon_Movement_Rifle_Walk
				{
					soundSet = "Weapon_Movement_Rifle_walkErc_L_SoundSet";
					id = 101;
				};
				class Weapon_Movement_Rifle_Run
				{
					soundSet = "Weapon_Movement_Rifle_runErc_L_SoundSet";
					id = 102;
				};
				class Weapon_Movement_Rifle_Sprint
				{
					soundSet = "Weapon_Movement_Rifle_sprintErc_SoundSet";
					id = 103;
				};
				class Weapon_Movement_Rifle_Land
				{
					soundSet = "Weapon_Movement_Rifle_landFeetErc_SoundSet";
					id = 104;
				};
				class Char_Gestures_Hand_Grab_Rifle
				{
					soundSet = "Char_Gestures_Hand_Grab_FabricRifle_SoundSet";
					id = 892;
				};
				class drop
				{
					soundset = "rifle_drop_SoundSet";
					id = 898;
				};
				class bodyfall_rifle_light
				{
					soundSet = "pickUpRifleLight_SoundSet";
					id = 13400;
				};
				class bodyfall_rifle
				{
					soundset = "pickUpRifle_SoundSet";
					id = 13401;
				};
			};
		};
	};
	class MP5K: MP5K_Base
	{
		scope = 2;
		displayName = "$STR_cfgWeapons_MP5K0";
		descriptionShort = "$STR_cfgWeapons_MP5K1";
		model = "\dz\weapons\firearms\MP5\MP5K.p3d";
		attachments[] = {"weaponOptics","weaponButtstockMP5","weaponMuzzleMP5","WeaponHandguardMP5","weaponFlashlight","pistolMuzzle"};
		itemSize[] = {5,3};
		hiddenSelectionsTextures[] = {"dz\weapons\firearms\MP5\data\mp5k_body_CO.paa"};
		hiddenSelectionsMaterials[] = {"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"};
		class DamageSystem
		{
			class GlobalHealth
			{
				class Health
				{
					hitpoints = 100;
					healthLevels[] = {{1.0,{"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"}},{0.7,{"DZ\weapons\firearms\MP5\data\mp5k_body.rvmat"}},{0.5,{"DZ\weapons\firearms\MP5\data\mp5k_body_damage.rvmat"}},{0.3,{"DZ\weapons\firearms\MP5\data\mp5k_body_damage.rvmat"}},{0.0,{"DZ\weapons\firearms\MP5\data\mp5k_body_destruct.rvmat"}}};
				};
			};
		};
	};
};
