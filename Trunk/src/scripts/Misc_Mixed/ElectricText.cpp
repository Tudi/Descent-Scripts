#include "StdAfx.h"
#include "Setup.h"
#include "ElectricText.h"

//crap some spell spawns this guy
//#define CENTER_MOB_ENTRY					15214		//should be custom not yet scripted
#define CENTER_MOB_ENTRY					19198		//should be custom not yet scripted
#define TEXT_CONTROL_POINT_STALKER_ENTRY	12999		//can be whatever non scripted mob
#define LETTER_COLOR_GREEN					689			//- channeled spell id
#define LETTER_COLOR_GREEN2					34036		//- channeled spell id
#define LETTER_COLOR_VIOLET					1120		//- channeled spell id
#define LETTER_COLOR_RED					41238		//- channeled spell id
#define LETTER_COLOR_BLUE					31611		//- channeled spell id
#define INVISIBLE_CREATURE_DISPLAY			11686		//or maybe 1126 ?
//whatever not visible for debug
//#define INVISIBLE_CREATURE_DISPLAY			987		//or maybe 1126 ?

Creature *SpawnControlPoint(Creature *spawner,float x, float y, float z, uint32 color)
{
	CreatureProto * proto = CreatureProtoStorage.LookupEntry(TEXT_CONTROL_POINT_STALKER_ENTRY);
	CreatureInfo * info = CreatureNameStorage.LookupEntry(TEXT_CONTROL_POINT_STALKER_ENTRY);
	if(!proto || !info)
	{
		sLog.outDetail("Warning : Missing summon creature template %u for electric text!",TEXT_CONTROL_POINT_STALKER_ENTRY);
		return NULL;
	}
	Creature * p = spawner->GetMapMgr()->CreateCreature(TEXT_CONTROL_POINT_STALKER_ENTRY);
	if( p == NULL )
		return NULL;
	p->Load(proto, x, y, z);
//	p->SetZoneId( spawner->GetZoneId() );
	p->m_faction = dbcFactionTemplate.LookupEntry(proto->Faction);
	if(p->m_faction)
		p->m_factionDBC = dbcFaction.LookupEntry(p->m_faction->Faction);
	p->SetFaction( 35 );
	p->GetAIInterface()->m_canCallForHelp = false;
	p->GetAIInterface()->m_canFlee = false;
	p->GetAIInterface()->m_canRangedAttack = false;
	p->GetAIInterface()->disable_combat = true;
	p->GetAIInterface()->disable_melee = true;
	p->GetAIInterface()->disable_spell = true;
	p->GetAIInterface()->disable_targeting = true;
	p->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_SELECTABLE);
	p->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 9000000 );
	p->SetUInt32Value( UNIT_FIELD_HEALTH, 9000000 );
	p->SetUInt32Value( UNIT_FIELD_DISPLAYID, INVISIBLE_CREATURE_DISPLAY );
	p->SetUInt32Value( UNIT_CHANNEL_SPELL, color );
	p->GetAIInterface()->m_moveFly = true;
	p->PushToWorld(spawner->GetMapMgr());
	return p;
}

//rotation is around Z axis
void SpawnLetter_W(Creature *spawner,Creature **StoreList,uint32 &index, float cx,float cy, float cz, float scale, float dist, float rot, uint32 color)
{
	if( spawner == NULL || spawner->GetMapMgr() == NULL )
		return;
	float control_points[5][3]={{-2.0f,0.0f,1.0f},{-1.0f,0.0f,-1.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,-1.0f},{2.0f,0.0f,1.0f}};
	int i;
	for(i=0;i<5;i++)
	{
		// rotation around z
		float ldist = dist+control_points[i][0]*scale;
		float lx = cx + ldist*cos(rot);
		float ly = cy + ldist*sin(rot);
		float lz = cz + control_points[i][2]*scale;
		StoreList[index+i] = SpawnControlPoint( spawner,lx,ly,lz,color);
		if( i > 0 )
			StoreList[index+i-1]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+i]->GetGUID() );
	}
	index += i-1;
}
//rotation is around Z axis
void SpawnLetter_O(Creature *spawner,Creature **StoreList,uint32 &index, float cx,float cy, float cz, float scale, float dist, float rot, uint32 color)
{
	if( spawner == NULL || spawner->GetMapMgr() == NULL )
		return;
	float control_points[4][3]={{-1.0f,0.0f,1.0f},{-1.0f,0.0f,-1.0f},{1.0f,0.0f,-1.0f},{1.0f,0.0f,1.0f}};
	int i;
	for(i=0;i<4;i++)
	{
		float ldist = dist+control_points[i][0]*scale;
		float lx = cx + ldist*cos(rot);
		float ly = cy + ldist*sin(rot);
		float lz = cz + control_points[i][2]*scale;
		StoreList[index+i] = SpawnControlPoint( spawner,lx,ly,lz,color);
		if( i > 0 )
			StoreList[index+i-1]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+i]->GetGUID() );
	}
	StoreList[index+i-1]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+0]->GetGUID() );
	index += i-1;
}
void SpawnLetter_B(Creature *spawner,Creature **StoreList,uint32 &index, float cx,float cy, float cz, float scale, float dist, float rot, uint32 color)
{
	if( spawner == NULL || spawner->GetMapMgr() == NULL )
		return;
	float control_points[7][3]={{1.0f,0.0f,0.7f},{-1.0f,0.0f,1.2f},{-1.0f,0.0f,0.0f},{0.5f,0.0f,0.0f},{-1.0f,0.0f,-1.2f},{1.0f,0.0f,-0.7f},{0.5f,0.0f,0.0f}};
	int i;
	for(i=0;i<7;i++)
	{
		float ldist = dist+control_points[i][0]*scale;
		float lx = cx + ldist*cos(rot);
		float ly = cy + ldist*sin(rot);
		float lz = cz + control_points[i][2]*scale;
		StoreList[index+i] = SpawnControlPoint( spawner,lx,ly,lz,color);
	}
	StoreList[index+0]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+1]->GetGUID() );
	StoreList[index+1]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+2]->GetGUID() );
	StoreList[index+2]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+3]->GetGUID() );
	StoreList[index+3]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+0]->GetGUID() );
	StoreList[index+4]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+2]->GetGUID() );
	StoreList[index+5]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+4]->GetGUID() );
	StoreList[index+6]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+5]->GetGUID() );
	index += i-1;
}
void SpawnLetter_E(Creature *spawner,Creature **StoreList,uint32 &index, float cx,float cy, float cz, float scale, float dist, float rot, uint32 color)
{
	if( spawner == NULL || spawner->GetMapMgr() == NULL )
		return;
	float control_points[6][3]={{1.0f,0.0f,1.0f},{-1.0f,0.0f,1.0f},{-1.0f,0.0f,0.0f},{1.0f,0.0f,0.0f},{-1.0f,0.0f,-1.0f},{1.0f,0.0f,-1.0f}};
	int i;
	for(i=0;i<6;i++)
	{
		float ldist = dist+control_points[i][0]*scale;
		float lx = cx + ldist*cos(rot);
		float ly = cy + ldist*sin(rot);
		float lz = cz + control_points[i][2]*scale;
		StoreList[index+i] = SpawnControlPoint( spawner,lx,ly,lz,color);
	}
	StoreList[index+0]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+1]->GetGUID() );
	StoreList[index+1]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+2]->GetGUID() );
	StoreList[index+2]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+3]->GetGUID() );
	StoreList[index+4]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+2]->GetGUID() );
	StoreList[index+5]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+4]->GetGUID() );
	index += i-1;
}
void SpawnLetter_Z(Creature *spawner,Creature **StoreList,uint32 &index, float cx,float cy, float cz, float scale, float dist, float rot, uint32 color)
{
	if( spawner == NULL || spawner->GetMapMgr() == NULL )
		return;
	float control_points[4][3]={{-1.0f,0.0f,1.0f},{1.0f,0.0f,1.0f},{-1.0f,0.0f,-1.0f},{1.0f,0.0f,-1.0f}};
	int i;
	for(i=0;i<4;i++)
	{
		float ldist = dist+control_points[i][0]*scale;
		float lx = cx + ldist*cos(rot);
		float ly = cy + ldist*sin(rot);
		float lz = cz + control_points[i][2]*scale;
		StoreList[index+i] = SpawnControlPoint( spawner,lx,ly,lz,color);
		if( i > 0 )
			StoreList[index+i-1]->SetUInt64Value( UNIT_FIELD_CHANNEL_OBJECT, StoreList[index+i]->GetGUID() );
	}
	index += i-1;
}
//	float control_points[4][3]={{-2.0f,0.0f,1.0f},{-1.0f,0.0f,0.0f},{0.0f,0.0f,1.0f},{1.0f,0.0f,0.0f},{2.0f,0.0f,1.0f}};
class ElectricTextCenter : public CreatureAIScript
{
public:
	ADD_CREATURE_FACTORY_FUNCTION(ElectricTextCenter);

	//constructor
	ElectricTextCenter(Creature* pCreature) : CreatureAIScript(pCreature)
	{
		//we create a mindless bot that cannot attack. Faster and cleaner then scripting when we need control guards
		pCreature->SetFaction( 35 );
		pCreature->GetAIInterface()->m_canCallForHelp = false;
		pCreature->GetAIInterface()->m_canFlee = false;
		pCreature->GetAIInterface()->m_canRangedAttack = false;
		pCreature->GetAIInterface()->disable_combat = true;
		pCreature->GetAIInterface()->disable_melee = true;
		pCreature->GetAIInterface()->disable_spell = true;
		pCreature->GetAIInterface()->disable_targeting = true;
		pCreature->SetUInt32Value( UNIT_FIELD_FLAGS, UNIT_FLAG_NOT_ATTACKABLE_2 | UNIT_FLAG_NOT_SELECTABLE);
		pCreature->SetUInt32Value( UNIT_FIELD_MAXHEALTH, 9000000 );
		pCreature->SetUInt32Value( UNIT_FIELD_HEALTH, 9000000 );
		pCreature->SetUInt32Value( UNIT_FIELD_DISPLAYID, INVISIBLE_CREATURE_DISPLAY );
		pCreature->GetAIInterface()->m_moveFly = true;

		for(uint32 i=0;i<7;i++)
			pCreature->SchoolImmunityAntiEnemy[i] = 999;
		for(uint32 i=0;i<31;i++)
			pCreature->MechanicsDispels[i] = 999;
		//we will spawn 35 mobs(at least) to write wowbeez
		Creature *points[50];	//not sure why i store these :P
		uint32		index = 0;
		//row 1 
		static const float scale = 3.0f;
		static const float row_1_height = scale + 1.0f;
		static const float row_2_height = -(scale + 1.0f);
		SpawnLetter_W( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_1_height, scale, -10.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_BLUE);
		SpawnLetter_O( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_1_height, scale, -0.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_GREEN);
		SpawnLetter_W( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_1_height, scale, +10.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_BLUE);
		//row 2
		SpawnLetter_B( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_2_height, scale, -15.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_VIOLET );
		SpawnLetter_E( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_2_height, scale, -5.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_RED );
		SpawnLetter_E( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_2_height, scale, +5.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_RED );
		SpawnLetter_Z( pCreature, &points[0],index, pCreature->GetPositionX(),pCreature->GetPositionY(),pCreature->GetPositionZ()+row_2_height, scale, +15.0f, pCreature->GetOrientation() + 3.14f/2.0f, LETTER_COLOR_VIOLET );

		guids_count = index;
		for(int i=0;i<guids_count;i++)
			point_guids[i] = points[i]->GetGUID();
	}
	void OnRemoveFromWorld()
	{
		if( _unit->GetMapMgr() == NULL )
			return;
		for(int i=0;i<guids_count;i++)
		{
			Creature *point = _unit->GetMapMgr()->GetCreature( point_guids[i] );
			if( point )
				point->Despawn( 1, 0 );
		}
	}
	uint64	 point_guids[50];	//not sure why i store these :P
	uint32	 guids_count;
};

void RegisterElectricTextHandlers(ScriptMgr * mgr)
{
	mgr->register_creature_script(CENTER_MOB_ENTRY, &ElectricTextCenter::Create);
}
