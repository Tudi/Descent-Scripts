#include "StdAfx.h"
#include "Setup.h"
#include "ElectricText.h"

void SetupGlobalInits(ScriptMgr * mgr)
{
	RegisterElectricTextHandlers( mgr );
	//example of possible event registers 
    //mgr->register_creature_script( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_gameobject_script( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_gossip_script( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_go_gossip_script( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_dummy_aura( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_dummy_spell( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_hook( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_item_gossip_script( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_quest_script( 123461, &AIDummyTowerReplacer::Create );
	//mgr->register_instance_script( 123461, &AIDummyTowerReplacer::Create );
	//For sub event types read in ScriptMgr.h
	//example for creature_script 
/*	virtual void OnCombatStart(Unit* mTarget) {}
	virtual void OnCombatStop(Unit* mTarget) {}
	virtual void OnDamageTaken(Unit* mAttacker, float fAmount) {}
	virtual void OnCastSpell(uint32 iSpellId) {}
	virtual void OnTargetParried(Unit* mTarget) {}
	virtual void OnTargetDodged(Unit* mTarget) {}
	virtual void OnTargetBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnTargetCritHit(Unit* mTarget, float fAmount) {}
	virtual void OnTargetDied(Unit* mTarget) {}
	virtual void OnParried(Unit* mTarget) {}
	virtual void OnDodged(Unit* mTarget) {}
	virtual void OnBlocked(Unit* mTarget, int32 iAmount) {}
	virtual void OnCritHit(Unit* mTarget, float fAmount) {}
	virtual void OnHit(Unit* mTarget, float fAmount) {}
	virtual void OnDied(Unit *mKiller) {}
	virtual void OnAssistTargetDied(Unit* mAssistTarget) {}
	virtual void OnFear(Unit* mFeared, uint32 iSpellId) {}
	virtual void OnFlee(Unit* mFlee) {}
	virtual void OnCallForHelp() {}
	virtual void OnLoad() {}
	virtual void OnReachWP(uint32 iWaypointId, bool bForwards) {}
	virtual void OnLootTaken(Player* pPlayer, ItemPrototype *pItemPrototype) {}
	virtual void AIUpdate() {}
	virtual void OnEmote(Player * pPlayer, EmoteType Emote) {}
	virtual void StringFunctionCall(const char * pFunc) {}
	void RegisterAIUpdateEvent(uint32 frequency);
	void ModifyAIUpdateEvent(uint32 newfrequency);
	void RemoveAIUpdateEvent();
	*/
}