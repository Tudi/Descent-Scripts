/* 
Event  : Hallow'end Despawn
Author: Angelis
Team   : Sun++
*/

/* DELETE QUERY */
/*=============== GAMEOBJECT ===============*/
DELETE FROM `gameobject_spawns` WHERE `entry` IN
	('180405','180406','180407','180408','180409','180410','180411','180412','180415','180425','180426','180427','180428','180429','180471','180472','180523','180431','180433','180700','186234','186614','186615','186720');


/*=============== CREATURE ===============*/
DELETE FROM `creature_spawns` WHERE `entry` IN
	('24034');


/*=============== END OF FILE ===============*/