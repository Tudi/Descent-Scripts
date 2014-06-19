!!!!only one tournament supervisor is allowed per realm !
edit globals.h to set static variables for this module

Tournament_spectator_NPC.cpp -> create gossip to port spectators near the arena
Tournament_spectator_supervisor.cpp -> NPC in the middle of the arena making sure no spectator enters the ring
Tournament_supervisor.cpp -> mostly comunicates with DB to prepare teams to be able to start the match. Also decides who won the match
	-> spawns a spectator supervisor in the middle of the arena
	-> should have gossip to reset match, force team to loose, force team to be disqualified, force player to get removed from team