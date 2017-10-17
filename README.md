# SteamTools [![GitHub version](https://badge.fury.io/gh/Arkshine%2FSteamTools.svg)](http://badge.fury.io/gh/Arkshine%2FSteamTools) [![Build Status](https://travis-ci.org/Arkshine/SteamTools.svg)](https://travis-ci.org/Arkshine/SteamTools) [![Build status](https://ci.appveyor.com/api/projects/status/i4hdpkj1ghuts6t0/branch/master?svg=true)](https://ci.appveyor.com/project/Arkshine/steamtools/branch/master)
AMX Mod X Module

DESCRIPTION
-----------
This module gives access to a select few SteamWorks functions for AMX Mod X plugins.
➥ For people who don't know, SteamWorks is basically an API provided by Steam which developers can use to integrate many of Steam's functions.

Interface presently supported and exposed to Pawn is essentially `ISteamGameServer` for now.

Usually, the relevant interfaces from GoldSrc server point of view are: `ISteamGameServer`, `ISteamUtils`, `ISteamNetworking`, and `ISteamHTTP`.
Not all functions are going to work for GoldSrc, but let met know if they are some you would want to see implemented.

API
---
The fast way is to check directly  [`steamtools.inc`](https://github.com/Arkshine/SteamTools/blob/master/amxmodx/scripting/include/steamtools.inc) include.
All is documented, it should be easy to use them.

TODO
----
* [ ] Enabling hooks/forwards on-demand
* [ ] Maybe implementing the HTTP interface