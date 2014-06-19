#include "StdAfx.h"
#include "Setup.h"
#include "EAS/EasyFunctions.h"

class ScourgeGryphon : public GossipScript
{
public:
	void GossipHello(Object* pObject, Player * plr, bool AutoSend)
	{
		TaxiPath * path = sTaxiMgr.GetTaxiPath( 1053 );
		plr->TaxiStart( path, 26308, 0 );
		plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
	}
};

class ScourgeGryphon2 : public GossipScript
{
public:
   void GossipHello(Object* pObject, Player * plr, bool AutoSend)
   {
      TaxiPath * path = sTaxiMgr.GetTaxiPath( 1054 );
      plr->TaxiStart( path, 26308, 0 );
      plr->RemoveFlag(UNIT_FIELD_FLAGS, UNIT_FLAG_MOUNTED_TAXI);
   }
};

void SetupDeathKnight(ScriptMgr * mgr)
{
	GossipScript * SG = (GossipScript*) new ScourgeGryphon();
	mgr->register_gossip_script(29488, SG);
	GossipScript * SG2 = (GossipScript*) new ScourgeGryphon2();
    mgr->register_gossip_script(29501, SG2);
}