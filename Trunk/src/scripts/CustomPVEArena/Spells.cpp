#include "StdAfx.h"
#include "Setup.h"

void OnAddRemovePlayerItem(Player * pPlayer, Item *item, bool Add)
{
	if( pPlayer != NULL && item != NULL && item->GetProto() && item->GetProto()->InventoryType == INVTYPE_CUSTOM_CHARM )
	{
		ItemPrototype *ip = item->GetProto();
		for( uint32 i=0;i<5;i++)
			if( ip->Spells.Id[i] != 0 && ip->Spells.Trigger[i] == LEARNING )
			{
				if( Add == true )
					pPlayer->CastSpell( pPlayer, ip->Spells.Id[i], true );
				else
					pPlayer->RemoveAura( ip->Spells.Id[i] );
			}
		for( uint32 i = 0; i < 10; i++ )
		{
			int32 val = ip->Stats.Value[i];
			if( val == 0 )
				continue;
			pPlayer->ModifyBonuses( ip->Stats.Type[i], val, Add );
		}
		pPlayer->UpdateStats();
	}
}

/*
//item ids : 348,349,350,351,352,353,354,355,356,357,358,359,360,361,362,363,364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397
//item ids : 364,365,366,367,368,369,370,371,372,373,374,375,376,377,378,379,380,381,382,383,384,385,386,387,388,389,390,391,392,393,394,395,396,397
//spell ids : 2343,2344,2345,2346,2347,2348,2349,2350,2351,2352,2353,2354,2355,2356,2357,2358,2359,2360,2361,2362,2363,2364,2365,2375,2376,2377,2384,2388,2390,2391,2398,2404,2405,2407,2408,2409,2410,2411,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,2422,2423,2424,2427,2428,2429,2430,2431,2432,2433,2434,2435,2436,2437,2438,2439,2440,2441,2445,2447,2448,2449,2450,2451,2452,2453,2454,2455,2456,2459,2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470,2471,2472,2473,2474,2475,2476,2477,2478,2480,2482,2483,2485,2486,2487,2488,2489,2490,2491,2492,2493,2494,2495,2496,2497,2498,2499,2500,2501,2502,2503,2504,2505,2506,2507,2508,2509,2510,2511,2512,2513,2514,2515,2516,2517,2518,2519,2520,2521,2522,2523,2524,2525,2526,2527,2528,2529,2530,2531,2532,2533,2534,2535,2536,2537,2553,2554,2555,2556,2557,2558,2559,2560,2561,2562,2563,2564,2566,2568,2569,2570,2571,2572,2573,2574,2577,2578,2579,2586,2587,2588,2589,2590,2591,2592,2593,2594,2595,2603,2604,2611,2612,2613,2614,2615,2616,2617,2618,2619,2620,2621,2622,
//spell ids : 2357,2358,2359,2360,2361,2362,2363,2364,2365,2375,2376,2377,2384,2388,2390,2391,2398,2404,2405,2407,2408,2409,2410,2411,2412,2413,2414,2415,2416,2417,2418,2419,2420,2421,2422,2423,2424,2427,2428,2429,2430,2431,2432,2433,2434,2435,2436,2437,2438,2439,2440,2441,2445,2447,2448,2449,2450,2451,2452,2453,2454,2455,2456,2459,2460,2461,2462,2463,2464,2465,2466,2467,2468,2469,2470,2471,2472,2473,2474,2475,2476,2477,2478,2480,2482,2483,2485,2486,2487,2488,2489,2490,2491,2492,2493,2494,2495,2496,2497,2498,2499,2500,2501,2502,2503,2504,2505,2506,2507,2508,2509,2510,2511,2512,2513,2514,2515,2516,2517,2518,2519,2520,2521,2522,2523,2524,2525,2526,2527,2528,2529,2530,2531,2532,2533,2534,2535,2536,2537,2553,2554,2555,2556,2557,2558,2559,2560,2561,2562,2563,2564,2566,2568,2569,2570,2571,2572,2573,2574,2577,2578,2579,2586,2587,2588,2589,2590,2591,2592,2593,2594,2595,2603,2604,2611,2612,2613,2614,2615,2616,2617,2618,2619,2620,2621,2622,

damage value is : DPS over 100 seconds
defense value is : counter DPS

#1 dmg stackable
#value : 100 * ( 1 / 2 ) = 50
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('348','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base Dmg','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');
update items set stat_type1=38,stat_value1=14 where entry=348; #1 dmg

#200 life stackable
#value : 200 / 0.5 / 8 = 50
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('351','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base life','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');
update items set stat_type1=1,stat_value1=200 where entry=351; #200 life

#haste stackable
#value : 10% dps, over 100 seconds with 1 dmg i do 50 dmg, with haste i will do 55 dmg
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('352','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base haste','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');
update items set stat_type1=36,stat_value1=4 where entry=352; #10% haste <= 10 / 0.384615004063

#move speed stackable
#value : movetime over 30 yards : 8 yards / sec => 4 sec move, 4000 * 10% = 0.4 sec
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('353','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2344','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base move speed 10%','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#stealth stackable
#value : +10% visibility range
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('354','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base 10% to detection','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#stealth stackable
#value : -15% enemy stealth range
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('355','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base -15% to enemy detection','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#Attack range stackable
#value : 3 yards ranged attack reach
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('357','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2350','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Base 3 yards to ranged attack','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#lightning shield
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('349','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2343','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Bonus : Lightning Shield : zaps nearby enemy. Static DPS','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#lightning shield 2
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('350','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2345','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Bonus : Lightning Shield : zaps 2 nearby enemy. Favor your target. Static DPS','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#heal wounds : You recover 75% of dmg received over 2 minutes time
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('356','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2349','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Heal Wounds : You recover 75% of dmg received over 2 minutes time. Recover stronger at the beggining','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#slow death : direct damage received will be reduced by x% and will build up as a DOT over fixed period
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('358','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2351','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Slow Death : Damage received will be reduced by 75% and will build up as a DOT over 1 minute. Damage is stronger at the beggining','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#blend in : standing still for 2 seconds puts you in stealth. Breaks on anything
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('359','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2352','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Blend In : Standing still for 2 seconds puts you in stealth. Breaks on anything','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#multidimensional 1 : blink in and out of existance every 10 seconds
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('360','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2353','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Multidimensional : blink in and out of existance every 10 seconds. Unique','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#multidimensional 2 : 10 seconds become immune to dmg, then for 10 seconds take double dmg
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('361','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2354','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Multidimensional : 10 seconds become immune to dmg, then for 10 seconds take double dmg. Unique','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#baited 1 : when below 30% hp you take 1 less dmg from all sources
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('362','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2355','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Baited : When below 30% hp you take 1 less dmg from all sources. Unique','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');

#sponge : soak damage for X seconds then AOE explode x% of it
replace into `items` (`entry`, `quality`, `flags`, `flags2`, `buyprice`, `sellprice`, `inventorytype`, `allowableclass`, `allowablerace`, `itemlevel`, `requiredlevel`, `requiredskill`, `requiredskillrank`, `requiredspell`, `RequiredPlayerRank1`, `RequiredPlayerRank2`, `RequiredFaction`, `RequiredFactionStanding`, `maxcount_overall`, `maxcount`, `containerslots`, `stat_type1`, `stat_type2`, `stat_type3`, `stat_type4`, `stat_type5`, `stat_type6`, `stat_type7`, `stat_type8`, `stat_type9`, `stat_type10`, `stat_value1`, `stat_value2`, `stat_value3`, `stat_value4`, `stat_value5`, `stat_value6`, `stat_value7`, `stat_value8`, `stat_value9`, `stat_value10`, `stat_unk1_1`, `stat_unk1_2`, `stat_unk1_3`, `stat_unk1_4`, `stat_unk1_5`, `stat_unk1_6`, `stat_unk1_7`, `stat_unk1_8`, `stat_unk1_9`, `stat_unk1_10`, `stat_unk2_1`, `stat_unk2_2`, `stat_unk2_3`, `stat_unk2_4`, `stat_unk2_5`, `stat_unk2_6`, `stat_unk2_7`, `stat_unk2_8`, `stat_unk2_9`, `stat_unk2_10`, `ScaledStatsDistributionId`, `dmg_type1`, `delay`, `range`, `spellid_1`, `spellid_2`, `spellid_3`, `spellid_4`, `spellid_5`, `spelltrigger_1`, `spelltrigger_2`, `spelltrigger_3`, `spelltrigger_4`, `spelltrigger_5`, `spellcharges_1`, `spellcharges_2`, `spellcharges_3`, `spellcharges_4`, `spellcharges_5`, `spellcooldown_1`, `spellcooldown_2`, `spellcooldown_3`, `spellcooldown_4`, `spellcooldown_5`, `spellcategory_1`, `spellcategory_2`, `spellcategory_3`, `spellcategory_4`, `spellcategory_5`, `spellcategorycooldown_1`, `spellcategorycooldown_2`, `spellcategorycooldown_3`, `spellcategorycooldown_4`, `spellcategorycooldown_5`, `bonding`, `name1`, `name2`, `name3`, `name4`, `description`, `page_id`, `page_language`, `page_material`, `quest_id`, `lock_id`, `lock_material`, `sheathID`, `randomprop`, `randomsuffix`, `itemset`, `maxdurability`, `ZoneNameID`, `mapid`, `bagfamily`, `totemcategory`, `socket_color_1`, `socket_color_2`, `socket_color_3`, `socket_content_1`, `socket_content_2`, `socket_content_3`, `socket_bonus`, `gemproperties`, `armordamagemodifier`, `existingduration`, `ItemLimitCategoryId`, `holidayid`, `statscalingfactor`, `field_130`, `field_131`, `class`, `subclass`, `DisplayId`) 
values('363','1','65536','8192','500000','0','112','-1','-1','1','0','0','0','0','0','0','0','0','1','1','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','2356','0','0','0','0','6','0','0','0','0','0','0','0','0','0','-1','-1','-1','-1','-1','0','0','0','0','0','-1','-1','-1','-1','-1','1','Sponge : soak damage for 10 seconds then AOE explode 10% of it.Unique','0','0','0','','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','0','240','0','0','0','0','0','0','0','47356');
*/

///////////////////////////////////////////////////////////
// Dummy Tick Handlers
///////////////////////////////////////////////////////////

bool ADTH_2343( uint32 i, Aura *aur, bool apply )
{
	if( aur->GetTarget()->IsPlayer() == false )
		return false;
	if( aur->GetTarget()->GetMapMgr() == NULL )
		return false;
	int32 MaxZapCount = 1;
	if( aur->GetSpellId() == 2345 )
	{
		//try to zap our attack target
		Unit *target = aur->GetTarget()->GetMapMgr()->GetUnit( SafePlayerCast( aur->GetTarget() )->GetSelection() );
		if( target && IsInrange( aur->GetTarget(), target, 30 * 30 ) && isAttackable( aur->GetTarget(), target ) )
		{
			dealdamage tdmg;
			tdmg.base_dmg = MAX( 1, aur->GetTarget()->GetMinDamage() );
			tdmg.pct_mod_final_dmg = 100;
			tdmg.DisableProc = false;
			tdmg.StaticDamage = true;
			tdmg.FirstTick = true;
			aur->GetTarget()->SpellNonMeleeDamageLog( target, aur->GetSpellProto(), &tdmg, 1, 0 );
			aur->GetTarget()->CastSpell( target, 2346, true );
		}
		else
			MaxZapCount++;
	}
	//get nearby enemy player if possible
//	InRangePlayerSetRecProt::iterator itr,itr2;
	InRangeSetRecProt::iterator itr,itr2;
	aur->GetTarget()->AquireInrangeLock(); //make sure to release lock before exit function !
	InrangeLoopExitAutoCallback AutoLock;
//	for( itr2 = aur->GetTarget()->GetInRangePlayerSetBegin( AutoLock ); itr2 != aur->GetTarget()->GetInRangePlayerSetEnd();)
	for( itr2 = aur->GetTarget()->GetInRangeSetBegin( AutoLock ); itr2 != aur->GetTarget()->GetInRangeSetEnd();)
	{
		itr = itr2;
		itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
		if( (*itr)->IsUnit() == false )
			continue;
		Unit *u = SafeUnitCast( *itr );
		if( u->isAlive() == false )
			continue;
		if( IsInrange( aur->GetTarget(), u, 30 * 30 ) == false )
			continue;
		if( isAttackable( aur->GetTarget(), u ) == false )
			continue;
		dealdamage tdmg;
		tdmg.base_dmg = MAX( 1, aur->GetTarget()->GetMinDamage() );
		tdmg.pct_mod_final_dmg = 100;
		tdmg.DisableProc = false;
		tdmg.StaticDamage = true;
		tdmg.FirstTick = true;
		aur->GetTarget()->SpellNonMeleeDamageLog( u, aur->GetSpellProto(), &tdmg, 1, 0 );
		aur->GetTarget()->CastSpell( u, 2346, true );
		MaxZapCount--;
		if( MaxZapCount == 0 )
			break;
	}
	aur->GetTarget()->ReleaseInrangeLock();
	return true;
}

bool ADTH_2349( uint32 i, Aura *aur, bool apply )
{
	if( aur->GetTarget()->IsPlayer() == false )
		return false;

	//no damage since last fight
	int64 *DamageTakenStore = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_DAMAGE_TAKEN_HEAL_WOUNDS );
	if( *DamageTakenStore == 0 )
		return true;

	int64 NeedToHealTotal = *DamageTakenStore * 75 / 100;

	//we healed all
	int64 *DamageHealedStore = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_DAMAGE_HEALED_HEAL_WOUNDS );
	if( *DamageHealedStore >= NeedToHealTotal )
		return true;

	int64 NeedToHealTick = ( NeedToHealTotal - *DamageHealedStore ) / ( 120000 / aur->GetSpellProto()->eff[0].EffectAmplitude );
//	if( NeedToHealTick < 1 )
//		return true;

	NeedToHealTick = MAX( 1, NeedToHealTick );

	aur->GetTarget()->ModHealth( NeedToHealTick );
	*DamageHealedStore += NeedToHealTick;

	return true;
}

bool ADTH_2352( uint32 i, Aura *aur, bool apply )
{
	if( aur->GetTarget()->IsPlayer() == false )
		return false;

	int64 *posx = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_POSITION_X_AUTO_STEALTH );
	int64 *posy = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_POSITION_Y_AUTO_STEALTH );
	int64 *comb = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_COMBATSTATUS_AUTO_STEALTH );

	int64 posxprev = *posx;
	int64 posyprev = *posy;

	*posx = aur->GetTarget()->GetPositionX();
	*posy = aur->GetTarget()->GetPositionY();

	if( aur->GetTarget()->CombatStatus.IsInCombat() == true )
	{
		*comb = 1;
		return false;
	}
	*comb = 0;

	if( *posx != posxprev || *posy != posyprev )
	{
		return false;
	}

	aur->GetTarget()->CastSpell( aur->GetTarget(), 58984, true ); //shadowmeld

	return true;
}

bool ADTH_2351( uint32 i, Aura *aur, bool apply )
{
	if( aur->GetTarget()->IsPlayer() == false )
		return false;

	//no damage since last fight
	int64 *DamageTakenStore = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_DAMAGE_TAKEN_HEAL_WOUNDS );
	if( *DamageTakenStore == 0 )
		return true;

	int64 NeedToHealTotal = *DamageTakenStore * 75 / 100;

	//we healed all
	int64 *DamageHealedStore = aur->GetTarget()->GetCreateIn64Extension( EXTENSION_ID_DAMAGE_HEALED_HEAL_WOUNDS );
	if( *DamageHealedStore >= NeedToHealTotal )
		return true;

	int64 NeedToDamageTick = ( NeedToHealTotal - *DamageHealedStore ) / ( 120000 / aur->GetSpellProto()->eff[0].EffectAmplitude );
//	if( NeedToDamageTick < 1 )
//		return true;

	NeedToDamageTick = MAX( 1, NeedToDamageTick );

	dealdamage tdmg;
	tdmg.base_dmg = MAX( 1, aur->GetTarget()->GetMinDamage() );
	tdmg.pct_mod_final_dmg = 100;
	tdmg.DisableProc = false;
	tdmg.StaticDamage = true;
	tdmg.FirstTick = true;
	aur->GetTarget()->SpellNonMeleeDamageLog( aur->GetTarget(), aur->GetSpellProto(), &tdmg );

	*DamageHealedStore += NeedToDamageTick;

	return true;
}

bool ADTH_2353( uint32 i, Aura *aur, bool apply )
{
	if( aur->GetTarget()->IsPlayer() == false )
		return false;

	uint32 Flipper = GetTickCount() / 10000;
	uint32 IsInvisible = Flipper & 1;

	if( IsInvisible )
		aur->GetTarget()->Phase( PHASE_SET, 2 );
	else
		aur->GetTarget()->Phase( PHASE_RESET );

	return true;
}

///////////////////////////////////////////////////////////
// Aura Apply Handlers
///////////////////////////////////////////////////////////


SPELL_EFFECT_OVERRIDE_RETURNS AH_2347( Aura *aur, bool apply, uint8 i )
{
	if( aur->GetSpellProto()->eff[i].EffectApplyAuraName == SPELL_AURA_DUMMY )
	{
		Unit *target = aur->GetTarget();
		if( target->IsPlayer() )
		{
			Player *p_target = SafePlayerCast( target );

			int16 TempSeeRange;
			if( p_target->GetMapMgr() && p_target->GetMapMgr()->GetMapInfo() )
				TempSeeRange = MIN( _cellSize, p_target->GetMapMgr()->GetMapInfo()->update_distance );
			else
				TempSeeRange = _cellSize;

			int32 PCTGain = 10;
			if( aur->GetSpellId() == 2348 )
				PCTGain = -15;

			if(apply)
				p_target->m_SeeRange += TempSeeRange * PCTGain / 100;
			else
				p_target->m_SeeRange -= TempSeeRange * PCTGain / 100;
		}
	}
	return SPELL_EFFECT_OVERRIDE_CONTINUE_EXECUTION;
}

///////////////////////////////////////////////////////////
// Proc Handlers
///////////////////////////////////////////////////////////

void PH_2349( ProcHandlerContextShare *context )
{
	if( context->in_dmg <= context->in_abs )
		return;
	int64 *DamageTakenStore = context->in_Caller->GetCreateIn64Extension( EXTENSION_ID_DAMAGE_TAKEN_HEAL_WOUNDS );
	*DamageTakenStore += ( context->in_dmg - context->in_abs );
}

void PH_2351( ProcHandlerContextShare *context )
{
	if( context->in_dmg <= context->in_abs )
		return;
	int64 *DamageTakenStore = context->in_Caller->GetCreateIn64Extension( EXTENSION_ID_DAMAGE_TAKEN_HEAL_WOUNDS );
	*DamageTakenStore += ( context->in_dmg - context->in_abs );
	//swallow all dmg
	context->out_dmg_absorb += context->in_dmg;
}

void PH_2354( ProcHandlerContextShare *context )
{
	if( context->in_dmg <= context->in_abs )
		return;
	uint32 Flipper = GetTickCount() / 10000;
	uint32 IsImmune = Flipper & 1;
	if( IsImmune )
		context->out_dmg_absorb += context->in_dmg;
	else
		*context->in_dmg_loc = (*context->in_dmg_loc) * 2;
}

void PH_2355( ProcHandlerContextShare *context )
{
	if( context->in_dmg <= context->in_abs )
		return;
	if( context->in_Caller->GetHealthPct() <= 30 )
		context->out_dmg_absorb += 1;
}

void PH_2356( ProcHandlerContextShare *context )
{
	if( context->in_dmg <= context->in_abs )
		return;
	context->in_event->created_with_value += ( context->in_dmg - context->in_abs );
	//time to AOE Explode ?
	uint32 TimeNow = GetTickCount();
	if( context->in_event->custom_holder == 0 )
		context->in_event->custom_holder = (void*)TimeNow;
	else if( (uint32)context->in_event->custom_holder < TimeNow )
	{
		context->in_event->custom_holder = (void*)( TimeNow + 10000 );
		int32 DamageToDo = MAX( 1, context->in_event->created_with_value * 10 / 100 );
		context->in_event->created_with_value = 0;

		InRangeSetRecProt::iterator itr,itr2;
		context->in_Caller->AquireInrangeLock(); //make sure to release lock before exit function !
		InrangeLoopExitAutoCallback AutoLock;
		for( itr2 = context->in_Caller->GetInRangeSetBegin( AutoLock ); itr2 != context->in_Caller->GetInRangeSetEnd();)
		{
			itr = itr2;
			itr2++; //maybe scripts can change list. Should use lock instead of this to prevent multiple changes. This protects to 1 deletion only
			if( (*itr)->IsUnit() == false )
				continue;
			Unit *u = SafeUnitCast( *itr );
			if( u->isAlive() == false )
				continue;
			if( IsInrange( context->in_Caller, u, 30 * 30 ) == false )
				continue;
			if( isAttackable( context->in_Caller, u ) == false )
				continue;
			dealdamage tdmg;
			tdmg.base_dmg = DamageToDo;
			tdmg.pct_mod_final_dmg = 100;
			tdmg.DisableProc = false;
			tdmg.StaticDamage = true;
			tdmg.FirstTick = true;
			context->in_Caller->SpellNonMeleeDamageLog( u, context->out_CastSpell, &tdmg, 1, 0 );
//			context->in_Caller->CastSpell( u, 2346, true );
		}
		context->in_Caller->ReleaseInrangeLock();
	}
}

void SetupSpells( ScriptMgr* mgr )
{
	sScriptMgr.register_hook( SERVER_HOOK_EVENT_ON_ADD_REM_ITEM, &OnAddRemovePlayerItem );

	//need to remove these items before player joins the match
	uint32 UsedItemIds[] = { 348,349,350,351,352,353,354,355,356,357, 0};
	for( uint32 i=0;i<500 && UsedItemIds[i] != 0; i++ ) 
		RewardItemIdsAll.push_front( UsedItemIds[i] );

	SpellEntry *sp;

	//lightning shield
	CreateDummySpell( 2343 );	
	sp = dbcSpell.LookupEntryForced( 2343 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW; 
//		sp->eff[0].EffectTriggerSpell = 97709;		
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectAmplitude = 2000;
//		sp->procFlags = PROC_ON_SPELL_HIT_DOT | PROC_ON_ANY_DAMAGE_DONE;	
//		sp->procChance = 100;				
//		sp->proc_interval = 1000;	
//		sp->procFlags2 |= PROC2_TARGET_SELF;
		sp->AuraPeriodicDummyTickHook = &ADTH_2343;
		sp->spell_id_client = 421; //chain lightning 
//		DynamicCreateAssignSpellStackGroup( sp, 0, "Lightning Shield" );
	}

	//lightning shield 2
	CreateDummySpell( 2345 );	
	sp = dbcSpell.LookupEntryForced( 2345 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW; 
//		sp->eff[0].EffectTriggerSpell = 97709;		
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectAmplitude = 2000;
//		sp->procFlags = PROC_ON_SPELL_HIT_DOT | PROC_ON_ANY_DAMAGE_DONE;	
//		sp->procChance = 100;				
//		sp->proc_interval = 1000;	
//		sp->procFlags2 |= PROC2_TARGET_SELF;
		sp->AuraPeriodicDummyTickHook = &ADTH_2343;
		sp->spell_id_client = 421; //chain lightning 
	}

	//lightning shield - damage visual
	CreateDummySpell( 2346 );	
	sp = dbcSpell.LookupEntryForced( 2346 );
	//if( sp != NULL )
	{
		sp->eff[0].Effect = SPELL_EFFECT_DUMMY;					
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SINGLE_ENEMY;
		sp->eff[0].EffectBasePoints = 0;
		sp->spell_id_client = 421; //chain lightning 
		sp->rangeIndex = 4; //30 yards
		sp->SpellVisual[0] = 1;
	}

	//increase move speed non exclusive
	CreateDummySpell( 2344 );
	sp = dbcSpell.LookupEntryForced( 2344 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_MOD_INCREASE_SPEED_ALWAYS; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectBasePoints = 10;
		sp->eff[1].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[1].EffectApplyAuraName = SPELL_AURA_MOD_MOUNTED_SPEED_ALWAYS; 
		sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[1].EffectBasePoints = 10;
	}

	//increase detection range
	CreateDummySpell( 2347 );
	sp = dbcSpell.LookupEntryForced( 2347 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->AuraHandleHook = &AH_2347;
	}

	//decrease enemy detection range
	CreateDummySpell( 2348 );
	sp = dbcSpell.LookupEntryForced( 2348 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DUMMY; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->AuraHandleHook = &AH_2347;
	}

	//dynamic group incompatible defensive
	{
		//heal wounds : You recover 75% of dmg received over 2 minutes time
		CreateDummySpell( 2349 );	
		sp = dbcSpell.LookupEntryForced( 2349 );
		//if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_PASSIVE;
			sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
			sp->DurationIndex = 21;	//infinit
			sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
			sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW; 
			sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
			sp->eff[0].EffectAmplitude = 6000;
			sp->eff[1].Effect = SPELL_EFFECT_APPLY_AURA;					
			sp->eff[1].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL; 
			sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SELF;
			sp->eff[1].EffectTriggerSpell = 2349;		
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;	
			sp->c_is_flags |= SPELL_FLAG_IS_PROC_TRIGGER_PROC;	//register bonus dmg also
			sp->procChance = 100;				
			sp->proc_interval = 0;	
			sp->procFlags2 |= PROC2_TARGET_SELF;
			sp->AuraPeriodicDummyTickHook = &ADTH_2349;
			sp->ProcHandler = &PH_2349;
//			DynamicCreateAssignSpellStackGroup( sp, 0, "Defensive 1" );
		}

		//slow death : damage taken is applied over 1 minute
		CreateDummySpell( 2351 );	
		sp = dbcSpell.LookupEntryForced( 2351 );
		//if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_PASSIVE;
			sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
			sp->DurationIndex = 21;	//infinit
			sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
			sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW; 
			sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
			sp->eff[0].EffectAmplitude = 6000;
			sp->eff[1].Effect = SPELL_EFFECT_APPLY_AURA;					
			sp->eff[1].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL; 
			sp->eff[1].EffectImplicitTargetA = EFF_TARGET_SELF;
			sp->eff[1].EffectTriggerSpell = 2349;		
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;	
			sp->c_is_flags |= SPELL_FLAG_IS_PROC_TRIGGER_PROC;	//register bonus dmg also
			sp->procChance = 100;				
			sp->proc_interval = 0;	
			sp->procFlags2 |= PROC2_TARGET_SELF;
			sp->AuraPeriodicDummyTickHook = &ADTH_2351;
			sp->ProcHandler = &PH_2349;
//			DynamicCreateAssignSpellStackGroup( sp, 0, "Defensive 1" );
		}
	}

	//Attack range
	CreateDummySpell( 2350 );	
	sp = dbcSpell.LookupEntryForced( 2350 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_ADD_FLAT_MODIFIER; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectBasePoints = 3;	//x yards
		sp->eff[0].EffectSpellGroupRelation[0] = 1;	//auto shoot
	}

	//blend in : standing still for 2 seconds puts you in stealth. Breaks on movement
	CreateDummySpell( 2352 );	
	sp = dbcSpell.LookupEntryForced( 2352 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectAmplitude = 2000;
	}

	{
		//multidimensional 1 : blink in and out of existance every 10 seconds
		CreateDummySpell( 2353 );	
		sp = dbcSpell.LookupEntryForced( 2353 );
		//if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_PASSIVE;
			sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
			sp->DurationIndex = 21;	//infinit
			sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
			sp->eff[0].EffectApplyAuraName = SPELL_AURA_DRINK_NEW; 
			sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
			sp->eff[0].EffectAmplitude = 10000;
			sp->AuraPeriodicDummyTickHook = &ADTH_2353;
			DynamicCreateAssignSpellStackGroup( sp, 0, "multidimensional" );
		}

		//multidimensional 2 : 10 seconds become immune to dmg, then for 10 seconds take double dmg
		CreateDummySpell( 2354 );	
		sp = dbcSpell.LookupEntryForced( 2354 );
		//if( sp != NULL )
		{
			sp->Attributes |= ATTRIBUTES_PASSIVE;
			sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
			sp->DurationIndex = 21;	//infinit
			sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
			sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL; 
			sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
			sp->eff[0].EffectTriggerSpell = 2354;		
			sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;	
			sp->c_is_flags |= SPELL_FLAG_IS_PROC_TRIGGER_PROC;	//register bonus dmg also
			sp->procChance = 100;				
			sp->proc_interval = 0;	
			sp->procFlags2 |= PROC2_TARGET_SELF;
			sp->ProcHandler = &PH_2354;
			DynamicCreateAssignSpellStackGroup( sp, 0, "multidimensional" );
		}
	}

	//baited 1 : when below 30% hp you take X less dmg from all sources
	CreateDummySpell( 2355 );	
	sp = dbcSpell.LookupEntryForced( 2355 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectTriggerSpell = 2354;		
		sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;	
		sp->c_is_flags |= SPELL_FLAG_IS_PROC_TRIGGER_PROC;	//register bonus dmg also
		sp->procChance = 100;				
		sp->proc_interval = 0;	
		sp->procFlags2 |= PROC2_TARGET_SELF;
		sp->ProcHandler = &PH_2355;
		DynamicCreateAssignSpellStackGroup( sp, 0, "baited" );
	}

	//sponge : soak damage for X seconds then AOE explode x% of it
	CreateDummySpell( 2356 );	
	sp = dbcSpell.LookupEntryForced( 2356 );
	//if( sp != NULL )
	{
		sp->Attributes |= ATTRIBUTES_PASSIVE;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->DurationIndex = 21;	//infinit
		sp->eff[0].Effect = SPELL_EFFECT_APPLY_AURA;					
		sp->eff[0].EffectApplyAuraName = SPELL_AURA_PROC_TRIGGER_SPELL; 
		sp->eff[0].EffectImplicitTargetA = EFF_TARGET_SELF;
		sp->eff[0].EffectTriggerSpell = 2356;		
		sp->procFlags = PROC_ON_ANY_DAMAGE_VICTIM;	
		sp->c_is_flags |= SPELL_FLAG_IS_PROC_TRIGGER_PROC;	//register bonus dmg also
		sp->procChance = 100;				
		sp->proc_interval = 0;	
		sp->procFlags2 |= PROC2_TARGET_SELF;
		sp->ProcHandler = &PH_2356;
		DynamicCreateAssignSpellStackGroup( sp, 0, "sponge" );
	}
}