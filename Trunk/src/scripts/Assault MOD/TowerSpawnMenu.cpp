#include "StdAfx.h"
#include "Setup.h"
#include "global.h"


PlayerResourceHolder *FetchHolder( Player* Plr )
{
	PlayerResourceHolder *myholder = (PlayerResourceHolder *)Plr->GetExtension( PLAYER_EXTENSION_ID );
	if( myholder == NULL )
	{
		myholder = new PlayerResourceHolder;
		memset( myholder, 0, sizeof( myholder ) );
		Plr->SetExtension( PLAYER_EXTENSION_ID, myholder );
		myholder->AssignedToInstanceID = 0;
	}
	if( myholder->AssignedToInstanceID != Plr->GetInstanceID() )
	{
		myholder->AssignedToInstanceID = Plr->GetInstanceID();
		myholder->ResourceNow = 0;
		myholder->ResourceToGather[TOWER_INDEX_TANK] = TOWER_INITIAL_RESOURCE_COST;
		myholder->ResourceToGather[TOWER_INDEX_SUPPORT] = TOWER_INITIAL_RESOURCE_COST;
		myholder->ResourceToGather[TOWER_INDEX_DPS] = TOWER_INITIAL_RESOURCE_COST;
		myholder->ResourceToGather[TOWER_INDEX_TANK_UPGRADE] = TOWER_INITIAL_RESOURCE_COST;
		myholder->ResourceToGather[TOWER_INDEX_SUPPORT_UPGRADE] = TOWER_INITIAL_RESOURCE_COST;
		myholder->ResourceToGather[TOWER_INDEX_DPS_UPGRADE] = TOWER_INITIAL_RESOURCE_COST;
		myholder->ResourceLastTime = Plr->m_dmg_made_since_login + Plr->m_heal_made_since_login + Plr->m_heal_received_since_login + Plr->m_dmg_received_since_login;
		myholder->TowerLevelBonuses[ TOWER_INDEX_TANK ] = 0;
		myholder->TowerLevelBonuses[ TOWER_INDEX_SUPPORT ] = 0;
		myholder->TowerLevelBonuses[ TOWER_INDEX_DPS ] = 0;
	}
	return myholder;
}

void UpdatePlayerResource( Player* Plr )
{
	//award player with some points
	PlayerResourceHolder *t = FetchHolder( Plr );
	//avoid values that would mess up values in the future
	if( Plr->m_dmg_made_since_login > CAP_RESOURCE_PER_UPGRADE || Plr->m_dmg_made_since_login < 0 )
		Plr->m_dmg_made_since_login = 0;
	if( Plr->m_heal_made_since_login > CAP_RESOURCE_PER_UPGRADE || Plr->m_heal_made_since_login < 0  )
		Plr->m_heal_made_since_login = 0;
	if( Plr->m_heal_received_since_login > CAP_RESOURCE_PER_UPGRADE || Plr->m_heal_received_since_login < 0  )
		Plr->m_heal_received_since_login = 0;
	if( Plr->m_dmg_received_since_login > CAP_RESOURCE_PER_UPGRADE || Plr->m_dmg_received_since_login < 0  )
		Plr->m_dmg_received_since_login = 0;

	//make a sum of the activity this player had until now
	uint32 player_values_now = Plr->m_dmg_made_since_login + Plr->m_heal_made_since_login + Plr->m_heal_received_since_login + Plr->m_dmg_received_since_login;
	player_values_now /= PLAYER_RESOURCE_SCALEDOWN;
	//protection
	if( player_values_now > CAP_RESOURCE_PER_UPGRADE )
		player_values_now = CAP_RESOURCE_PER_UPGRADE;
	//values got crapped out 
	if(  t->ResourceLastTime > player_values_now )
	{
		t->ResourceLastTime = 0;
		t->ResourceNow = 0;
		Plr->m_dmg_made_since_login = 0;
		Plr->m_heal_made_since_login = 0;
		Plr->m_heal_received_since_login = 0;
		Plr->m_dmg_received_since_login = 0;
		return;
	}
	//value is insanly large
	if( t->ResourceNow > CAP_RESOURCE_PER_UPGRADE )
		t->ResourceNow = 0;
	//update resource we have with value diff
	t->ResourceNow += player_values_now - t->ResourceLastTime;
	t->ResourceLastTime = player_values_now;
}

uint32 GetSoulVendorRewardAmount(Player* Plr )
{
	PlayerResourceHolder *t = FetchHolder( Plr );
	uint32 max = 0;
	for(int i=0;i<TOWER_INDEX_TOTAL2;i++)
		if( t->ResourceToGather[i] > max )
			max = t->ResourceToGather[i];
	return max * PLAYER_SOUL_VENDOR_REWARD_PCT / 100;
}

uint32 GetTowerEntry(uint32 TeamID, uint32 TowerIndex )
{
	if( TeamID == 1 )
	{
		if( TowerIndex == TOWER_INDEX_TANK )
			return 123466;
		else if( TowerIndex == TOWER_INDEX_SUPPORT )
			return 123464;
		else if( TowerIndex == TOWER_INDEX_DPS )
			return 123462;
	}
	else
	{
		if( TowerIndex == TOWER_INDEX_TANK )
			return 123467;
		else if( TowerIndex == TOWER_INDEX_SUPPORT )
			return 123465;
		else if( TowerIndex == TOWER_INDEX_DPS )
			return 123463;
	}
	return 0;
}

void SpawnTowerForPlayer( Player* Plr, uint32 TowerType )
{
	if( TowerType > TOWER_INDEX_TOTAL )
	{
		Plr->BroadcastMessage("BUG : report to devs that error -1 occured");
		return;
	}
	//check for required resource
	PlayerResourceHolder *r = FetchHolder( Plr );
	if( r->ResourceToGather[ TowerType ] > r->ResourceNow )
	{
		Plr->BroadcastMessage("You need more resources to spawn this tower");
		return;
	}
	//check if close to enemy graveyard
	if( Plr->GetMapId() == 489 )	//warsong gulch
	{
		if( Plr->GetTeam() == 0 )
		{
			if( Distance2DSQ( Plr, 1032.644775f, 1388.316040f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 915.36f, 1433.78f ) < TOWER_MIN_DISTANCE_TO_GY_SQ
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
		else 	//horde cannot spawn in A
		{
			if( Distance2DSQ( Plr, 1423.218872f, 1554.663574f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1540.29f, 1481.78f ) < TOWER_MIN_DISTANCE_TO_GY_SQ
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
	}
	else if( Plr->GetMapId() == 529 )	//Arathi Basin
	{
		if( Plr->GetTeam() == 0 )	//horde cannot spawn in A
		{
			if( Distance2DSQ( Plr, 1201.869507f, 1163.130615f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 834.726379f, 784.978699f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1016.588318f, 955.184692f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1211.523682f, 781.556946f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 772.755676f, 1213.113770f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1354.699951f, 1270.270020f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
		else
		{
			if( Distance2DSQ( Plr, 1201.869507f, 1163.130615f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 834.726379f, 784.978699f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1016.588318f, 955.184692f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1211.523682f, 781.556946f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 772.755676f, 1213.113770f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 1354.699951f, 1270.270020f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
	}
	else if( Plr->GetMapId() == 30 )	//Alterac Valley
	{
		if( Plr->GetTeam() == 0 )	//horde cannot spawn in A
		{
			if( Distance2DSQ( Plr, 634.607971f, 47.408627f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 675.970947f, -373.707336f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 73.869370f, -495.597656f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, -161.472916f, 34.797512f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, -531.067627f, -405.459778f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, -1088.008545f, -248.774918f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
		else
		{
			if( Distance2DSQ( Plr, 634.607971f, 47.408627f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 675.970947f, -373.707336f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 73.869370f, -495.597656f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, -161.472916f, 34.797512f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, -531.067627f, -405.459778f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, -1088.008545f, -248.774918f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
	}
	else if( Plr->GetMapId() == 37 )	//Azshara Crate
	{
		if( Plr->GetTeam() == 0 )	//horde cannot spawn in A
		{
		}
		else
		{
		}
	}
	else if( Plr->GetMapId() == 566 )	//Eye of the Storm
	{
		if( Plr->GetTeam() == 0 )	//horde cannot spawn in A
		{
			if( Distance2DSQ( Plr, 2012.403442f, 1455.412354f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 2013.061890f, 1677.238037f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 2355.297852f, 1683.713989f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 2351.785400f, 1455.399048f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
		else
		{
			if( Distance2DSQ( Plr, 2012.403442f, 1455.412354f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 2013.061890f, 1677.238037f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 2355.297852f, 1683.713989f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				|| Distance2DSQ( Plr, 2351.785400f, 1455.399048f ) < TOWER_MIN_DISTANCE_TO_GY_SQ 
				)
			{
				Plr->BroadcastMessage("Too close to graveyard");
				return;
			}
		}
	}
	//check if close to enemy king
	//get type of the tower we need to spawn for this team
	uint32 entry_to_spawn = GetTowerEntry( Plr->GetTeam(), TowerType );
	//check for nearby spawned towers of same type and same team
	for( set<Object*>::iterator itr = Plr->GetInRangeSetBegin(); itr != Plr->GetInRangeSetEnd(); ++itr)
	{
		if( (*itr) == NULL 
			|| (*itr)->GetEntry() != entry_to_spawn 
			|| (*itr)->GetUInt32Value( UNIT_FIELD_HEALTH ) == 0
			)
			continue;

		float dist = Distance2DSQ( Plr, (*itr)->GetPositionX(), (*itr)->GetPositionY() );
		if( dist > TOWER_MIN_DISTANCE_TO_SPAWN_SQ )
			continue;

		dist = (int)sqrt( dist );
		char buff[500];
		sprintf(buff,"Tower from same type and team is too close. Found one at %u yards. Need %u",(int)dist,TOWER_MIN_DISTANCE_TO_SPAWN_SQ);
		Plr->BroadcastMessage(buff);
		return;
	}
	//try to spawn tower
	CreatureProto * proto = CreatureProtoStorage.LookupEntry( entry_to_spawn );
	CreatureInfo * info = CreatureNameStorage.LookupEntry( entry_to_spawn );
	Creature* p = Plr->GetMapMgr()->CreateCreature( entry_to_spawn );
	p->SetInstanceID( Plr->GetMapMgr()->GetInstanceID() );
	p->Load(proto, Plr->GetPositionX(), Plr->GetPositionY(), Plr->GetPositionZ());
	p->SetUInt64Value(UNIT_FIELD_SUMMONEDBY, Plr->GetGUID());
    p->SetUInt64Value(UNIT_FIELD_CREATEDBY, Plr->GetGUID());
    p->SetZoneId( Plr->GetZoneId() );
	p->SetUInt32Value(UNIT_FIELD_LEVEL, 70 + r->TowerLevelBonuses[ TowerType ] );
	p->PushToWorld( Plr->GetMapMgr() );
	//consume cost and advance cost to next level
	r->ResourceNow -= r->ResourceToGather[ TowerType ];
	r->ResourceToGather[ TowerType ] *= TOWER_RESOURCE_COST_INCREASE_MULT;
}

//check if we have resource and there are no nearby towers of same type
void UpgradeTowerForPlayer( Player* Plr, uint32 TowerType )
{
	uint32 upgrade_index = TowerType + TOWER_INDEX_TOTAL;
	//check cost
	PlayerResourceHolder *r = FetchHolder( Plr );
	if( r->ResourceToGather[ upgrade_index ] >= r->ResourceNow )
	{
		Plr->BroadcastMessage("You need more resources to upgrade this tower type");
		return;
	}
	//get entry that we need to upgrade
	uint32 entry_to_spawn = GetTowerEntry( Plr->GetTeam(), TowerType );
	//new level
	uint32 new_level = 70 + r->TowerLevelBonuses[ TowerType ] + 1;
	//scan map for our towers and upgrade them
	uint32 max_ind = Plr->GetMapMgr()->m_CreatureHighGuid;
	Creature **m_CreatureStorage = Plr->GetMapMgr()->m_CreatureStorage;
	for( uint32 i=0;i<=max_ind;i++)
		if( m_CreatureStorage[ i ] 
			&& m_CreatureStorage[ i ]->GetEntry() == entry_to_spawn 
			&& m_CreatureStorage[ i ]->GetUInt64Value( UNIT_FIELD_CREATEDBY ) == Plr->GetGUID() 
			)
		{
			m_CreatureStorage[ i ]->SetUInt32Value( UNIT_FIELD_LEVEL, new_level );
		}
	//update costs
	r->TowerLevelBonuses[ TowerType ]++;
	r->ResourceNow -= r->ResourceToGather[ upgrade_index ];
	r->ResourceToGather[ upgrade_index ] *= TOWER_RESOURCE_COST_INCREASE_MULT;
}

//this NPC needs to handle players that wish to join the Tournament as spectators
//only GMs can select and talk to him
class SCRIPT_DECL TowerSpawnGOSSIP : public GossipScript
{
public:
    void GossipHello(Object * pObject, Player* Plr, bool AutoSend);
    void GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code);
    void GossipEnd(Object * pObject, Player* Plr);
	void Destroy()   {      delete this;   }
	void send_MainPage(Object * pObject, Player* Plr)
	{
	  if( Plr == false )
		 return; 

	  GossipMenu *Menu;
	  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 100010, Plr);
	  if( 
		  Plr->m_bg && 
			Plr->isAlive() )
		Menu->AddItem( 0, "Open Tower Menu",2);
	  Menu->SendTo(Plr);
	}
	void send_TowerMenu(Object * pObject, Player* Plr)
	{
	  GossipMenu *Menu;
	  objmgr.CreateGossipMenuForPlayer(&Menu, pObject->GetGUID(), 100011, Plr);

	  //award player with some points
	  PlayerResourceHolder *t = FetchHolder( Plr );
	  UpdatePlayerResource( Plr );
	  char buff[500];
	  sprintf(buff,"Resource now |cffff0000%u|r.",t->ResourceNow);
	  Menu->AddItem( 1, buff,2);
	  sprintf(buff,"Spawn Tank Tower lvl %u = |cffffffff%u|r resource.",70+t->TowerLevelBonuses[TOWER_INDEX_TANK],t->ResourceToGather[TOWER_INDEX_TANK]);
	  Menu->AddItem( 0, buff,5);
	  sprintf(buff,"Spawn Support Tower lvl %u = |cffffffff%u|r resource.",70+t->TowerLevelBonuses[TOWER_INDEX_SUPPORT],t->ResourceToGather[TOWER_INDEX_SUPPORT]);
	  Menu->AddItem( 0, buff,6);
	  sprintf(buff,"Spawn DPS Tower lvl %u = |cffffffff%u|r resource.",70+t->TowerLevelBonuses[TOWER_INDEX_DPS],t->ResourceToGather[TOWER_INDEX_DPS]);
	  Menu->AddItem( 0, buff,7);

	  if( t->TowerLevelBonuses[TOWER_INDEX_TANK] < 10 )
	  {
		sprintf(buff,"Upgrade Tank Tower to lvl %u =  |cffffffff%u|r resource.",t->TowerLevelBonuses[TOWER_INDEX_TANK] + 2,t->ResourceToGather[TOWER_INDEX_TANK_UPGRADE]);
		Menu->AddItem( 0, buff,10);
	  }
	  if( t->TowerLevelBonuses[TOWER_INDEX_SUPPORT] < 10 )
	  {
		  sprintf(buff,"Upgrade Support Tower to lvl %u = |cffffffff%u|r resource.",t->TowerLevelBonuses[TOWER_INDEX_SUPPORT] + 2,t->ResourceToGather[TOWER_INDEX_SUPPORT_UPGRADE]);
		  Menu->AddItem( 0, buff,11);
	  }
	  if( t->TowerLevelBonuses[TOWER_INDEX_DPS] < 10 )
	  {
		sprintf(buff,"Upgrade DPS Tower to lvl %u = |cffffffff%u|r resource.",t->TowerLevelBonuses[TOWER_INDEX_DPS] + 2,t->ResourceToGather[TOWER_INDEX_DPS_UPGRADE]);
		Menu->AddItem( 0, buff,12);
	  }

	  if( Plr->HasNegativeAura( PLAYER_SOUL_VENDOR_AURA ) == false )
	  {
		 sprintf(buff,"Sell Soul for |cffffffff%u|r Resource.",GetSoulVendorRewardAmount( Plr ) );
		 Menu->AddItem( 0, buff,15);
	  }

	  Menu->AddItem( 0, "Repair Items",30);
		//special menu for GMs
		if( Plr->GetSession() && Plr->GetSession()->HasGMPermissions() )
		{
			Menu->AddItem( 0, "|cffff00ffAdd resource|r",20);
		}
	  Menu->SendTo(Plr);
	}
};

void TowerSpawnGOSSIP::GossipHello(Object * pObject, Player* Plr, bool AutoSend)
{
   if( AutoSend )
      send_MainPage(pObject,Plr);
}

void TowerSpawnGOSSIP::GossipSelectOption(Object * pObject, Player* Plr, uint32 Id, uint32 IntId, const char * Code)
{
   switch(IntId)
    {
      // info menu
       case 1:      send_MainPage(pObject,Plr);                                       break;
       case 2:      send_TowerMenu(pObject,Plr);									  break;
      //spawn a tower if possible
      case 5:      
      case 6:      
      case 7:      
		if( 
			Plr->m_bg && 
				Plr->isAlive() )
			SpawnTowerForPlayer( Plr, IntId - 5 );
         Plr->CloseGossip();
         break;
      //upgrade a tower if possible
      case 10:      
      case 11:      
      case 12:      
		if( 
			Plr->m_bg && 
			Plr->isAlive() )
	         UpgradeTowerForPlayer( Plr, IntId - 10 );
         Plr->CloseGossip();
         break;
      // sell soul for resource
      case 15:      
		 if( 
			 Plr->m_bg && 
			Plr->isAlive() && Plr->HasNegativeAura( PLAYER_SOUL_VENDOR_AURA ) == false )
		 {
			//cast nerf buff on player
			SpellCastTargets targets( Plr->GetGUID() );
			SpellEntry *spellInfo = dbcSpell.LookupEntry( PLAYER_SOUL_VENDOR_AURA );
			if (spellInfo)
			{
				Spell *spell = SpellPool.PooledNew();
				spell->Init( Plr, spellInfo ,true, NULL);
				spell->forced_duration = 5*60*1000;
				spell->prepare(&targets);
			}
			//award player with some points
			PlayerResourceHolder *t = FetchHolder( Plr );
			t->ResourceNow += GetSoulVendorRewardAmount( Plr );
		 }
         Plr->CloseGossip();
         break;
	 //GM menu
      case 20:      
		  {
			PlayerResourceHolder *t = FetchHolder( Plr );
			t->ResourceNow += GetSoulVendorRewardAmount( Plr ) * 50;
            Plr->CloseGossip();
		  }
         break;
		case 30://Repair Items
			{
			uint32 price = 20000;
			uint32 currentgold = Plr->GetUInt32Value(PLAYER_FIELD_COINAGE);
			if (currentgold>=price)
			{
				Player *p_caster = Plr;
				for( int i = 0; i < EQUIPMENT_SLOT_END; i++ )
				{
					Item *pItem = p_caster->GetItemInterface()->GetInventoryItem( i );
					if( pItem != NULL )
					{
						if( pItem->IsContainer() )
						{
							Container *pContainer = static_cast<Container*>( pItem );
							for( unsigned int j = 0; j < pContainer->GetProto()->ContainerSlots; ++j )
							{
								pItem = pContainer->GetItem( j );
								if( pItem != NULL )
								{
		//							int32 cost = 0;
		//							if( cost <= (int32)p_caster->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
									{
		//								p_caster->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
										pItem->SetDurabilityToMax();
										pItem->m_isDirty = true;
									}
								}
							}
						}
						else
						{
							if( pItem->GetProto()->MaxDurability > 0 && i < INVENTORY_SLOT_BAG_END && pItem->GetDurability() <= 0 )
							{
		//						int32 cost = 0;
		//						if( cost <= (int32)p_caster->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
								{
		//							p_caster->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
									pItem->SetDurabilityToMax();
									pItem->m_isDirty = true;
								}
								p_caster->ApplyItemMods( pItem, i, true );
							}
							else
							{
								int32 cost = 0;
								if( cost <= (int32)p_caster->GetUInt32Value( PLAYER_FIELD_COINAGE ) )
								{
									p_caster->ModUnsigned32Value( PLAYER_FIELD_COINAGE, -cost );
									pItem->SetDurabilityToMax();
									pItem->m_isDirty = true;
								}
							}
						}
					}
				}
				Plr->BroadcastMessage("Your items are repaired!");
			}
			else
			{
				Plr->BroadcastMessage("You need 2 gold to repair your items!");
			}
			Plr->CloseGossip();
			}
		break;

	  default:   send_MainPage(pObject,Plr);                                       break;
   }
}

void TowerSpawnGOSSIP::GossipEnd(Object * pObject, Player* Plr)
{
    GossipScript::GossipEnd(pObject, Plr);
}

void SetupTowerSpawnGossip(ScriptMgr * mgr)
{
	//make him have a gossip
	GossipScript * gs = (GossipScript*) new TowerSpawnGOSSIP();
	mgr->register_item_gossip_script(20946, gs);
	//adjust lost soul to last through death. Very bad move to do :p
	SpellEntry *sp = dbcSpell.LookupEntryForced( PLAYER_SOUL_VENDOR_AURA );
	if( sp )
	{
		sp->Effect[ 0 ] = SPELL_EFFECT_APPLY_AURA;
		sp->EffectApplyAuraName[ 0 ] = SPELL_AURA_MOD_ATTACKSPEED;
		sp->EffectBasePoints[ 0 ] = -99;
		sp->EffectImplicitTargetA[ 0 ] = EFF_TARGET_SELF;
		sp->Effect[ 1 ] = SPELL_EFFECT_APPLY_AURA;
		sp->EffectApplyAuraName[ 1 ] = SPELL_AURA_MOD_CASTING_SPEED;
		sp->EffectBasePoints[ 1 ] = -500;
		sp->EffectImplicitTargetA[ 1 ] = EFF_TARGET_SELF;
		sp->Effect[ 2 ] = SPELL_EFFECT_APPLY_AURA;
		sp->EffectApplyAuraName[ 2 ] = SPELL_AURA_MOD_TOTAL_STAT_PERCENTAGE;
		sp->EffectBasePoints[ 2 ] = -50;
		sp->EffectMiscValue[ 2 ] = -1;
		sp->EffectImplicitTargetA[ 2 ] = EFF_TARGET_SELF;
		sp->AttributesExC |= CAN_PERSIST_AND_CASTED_WHILE_DEAD;
		sp->c_is_flags |= SPELL_FLAG_IS_FORCEDDEBUFF;
	}

	//so that holder get initialized
	mgr->register_hook( SERVER_HOOK_EVENT_ON_ENTER_WORLD, UpdatePlayerResource );
}