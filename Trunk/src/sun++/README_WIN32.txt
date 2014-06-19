***************************
*      .-.                *
*      `-.. ..-. + +      *
*      `-''-'' '          *
*  OpenSource Scripting   *
*          Team           *
* http://sunplusplus.info *
*                         *
***************************
This software is provided as free and open source by the
staff of the Sun++ Scripts Project, in accordance with 
the GPL license. This means we provide the software we have 
created freely and it has been thoroughly tested to work for 
the developers, but NO GUARANTEE is made it will work for you 
as well. Please give credit where credit is due, if modifying,
redistributing and/or using this software. Thank you.

Sun++ Team, July 2008
~~End of License Agreement
--------------------------------------------------------------------------------

How to install Sun++ On WIN32 as of (08/21/2008)

Ok, assuming you have already followed my guide on how to compile ArcEmu listed here: http://arcemu.org/forums/index.php?showtopic=1009 I think it is time we move on how to compile Sun++ scripts for ArcEmu. If you followed my Install Giude on building ArcEmu then you already have all the necessary tools in your tool box to proceed to making Sun++ scripts.

--===[OBTAINING THE SUN++ SCRIPT FILES]===--

Using TortoiseSVN, you will need to do a Checkout of the Sun++ SVN repository just like you did with the ArcEmu SVN. I shall link that below.

SUN++: http://svn.assembla.com/svn/sunplusplus

This, like ArcEmu comes in raw code format that will need to be compiled, and we will get to that soon.

--===[PROPER PLACEMENT OF THE SUN++ SCRIPT FILES]===--

Once you have obtained all of the Sun++ files we will need to create the proper folder where you compile ArcEmu. So, go to your ArcEmu development folder and navigate to, we shall use my default folder as an example. c:\Documents and Settings\Darknation\Desktop\Warcraft Development\Core\ArcEmu Clean\trunk\src and once in the SRC folder, make a new folder called Sun++ this will be the working folder that will contain all of the Sun++ files.

Now, open your folder you did the Sun++ checkout to and go into the TRUNK folder, there you will see the contents of the scripts and their solution files. Select all of those files and folders and copy them into your c:\Documents and Settings\Darknation\Desktop\Warcraft Development\Core\ArcEmu Clean\trunk\src\Sun++ folder. So now you should have your Sun++ folder populated.

--===[PREPARING SUN++ FOR COMPILING]===--

One thing that needs to be realized is that before you can compile your Sun++ scripts, you need to have compiled ArcEmu first. Everytime you update and recompile ArcEmu you must Clean and recompile Sun++, this is due to the ArcEmu revision checker. It was created by Ascent to help reduce server crashes caused by the scripts.

--===[COMPILING SUN++ SCRIPTS FOR ARCEMU]===--

So, before you compile Sun++, make sure to do a "Clean" on your ArcEmu solution and rebuild it. After that, close the ArcEmu solution and open your ArcEmu>TRUNK>SRC>SUN++ folder and open scripts2008, when you do this, go to the upper left of the Visual Studio window and select the Drop down with the green arrow next to it, if it says Debug, set it to Release. Now, to the left hand side you will see a "Solution Explorer" window. Right click on "Solution 'scripts2008' (7 projects) and select "Clean" this will ensure we are working with a clean copy.

Next, right click on "Solution 'scripts2008' (7 projects) and select build. If all goes well you should get something like this:

2>Linking...
2> Creating library ..\..\..\bin\release\script_bin/SunQuestScripts.lib and object ..\..\..\bin\release\script_bin/SunQuestScripts.exp
2>Embedding manifest...
2>Build log was saved at "file://c:\Documents and Settings\Darknation\Desktop\Warcraft Development\Core\ArcEmu\trunk\src\sun++\projects\2003_int_release_Sunquestscripts\BuildLog.htm"
2>SunQuestScripts - 0 error(s), 0 warning(s)
========== Build: 7 succeeded, 0 failed, 0 up-to-date, 0 skipped ==========

As I stated in my ArcEmu install guide, if you get warnings don't worry about it too much, it's errors that will cause builds to fail. Once all is said and done navigate to your ArcEmu>Bin>Release and in there should be a new folder called Script_Bin, copy that folder into your ArcEmu servers folder so you should have a new folder called Script_Bin. If all went well when you load ArcEmu at the very end you should see something like this:


20:24 N Server: Loading External Script Libraries...

LUAScripting.dll : 0x11730000 : v0.44 : delayed load.
ServerStatusPlugin.dll : 0x117D0000 : v0.44 : loaded.
SunGossipScripts.dll : 0x11A90000 : v0.44 : loaded.
SunInstanceScripts.dll : 0x11B00000 : v0.44 : loaded.
SunQuestScripts.dll : 0x12F70000 : v0.44 : loaded.
SunSpellHandlers.dll : 0x12FF0000 : v0.44 : loaded.

20:24 N Server: Loaded 6 external libraries.

20:24 N Server: Loading optional scripting engines...
20:24 N Server: Done loading script engines...
20:24 N Server: Ready for connections. Startup time: 11219ms

20:24 N RemoteConsole: Starting...
20:24 N LogonCommClient: Loading forced permission strings...
20:24 N LogonCommClient: Attempting to connect to logon server...
20:24 N LogonCommClient: Connecting to logonserver on `x.x.x.x:8093`...
20:24 N LogonCommClient: Authenticating...
20:24 N LogonCommClient: Authentication succeeded.
20:24 N LogonCommClient: Registering Realms...
20:24 N LogonCommClient: Realm `RubyWeapon` registered as realm 3.
20:24 N LogonCommClient: Build character mapping in 15ms. (1555)
20:24 N LogonCommClient: Logonserver latency is 0ms.
