//
// AMX Mod X SteamTools
// Copyright (C) Vincent Herbet (Arkshine)
//
// This software is licensed under the GNU General Public License, version 2.
// For full license details, see LICENSE file.
//

#ifndef _STEAM_INTERFACES_H_
#define _STEAM_INTERFACES_H_

#define NO_CSTEAMID_STL
#define INTERFACEOSW_H

#include <Steamworks.h>

class ISteamClient : public ISteamClient012 {};
#define STEAMCLIENT_INTERFACE_VERSION STEAMCLIENT_INTERFACE_VERSION_012

class ISteamGameServer : public ISteamGameServer011 {};
#define STEAMGAMESERVER_INTERFACE_VERSION STEAMGAMESERVER_INTERFACE_VERSION_011

class ISteamUtils : public ISteamUtils005 {};
#define STEAMUTILS_INTERFACE_VERSION STEAMUTILS_INTERFACE_VERSION_005

class ISteamGameServerStats : public ISteamGameServerStats001 {};
#define STEAMGAMESERVERSTATS_INTERFACE_VERSION STEAMGAMESERVERSTATS_INTERFACE_VERSION_001

class ISteamHTTP : public ISteamHTTP002 {};
#define STEAMHTTP_INTERFACE_VERSION STEAMHTTP_INTERFACE_VERSION_002

#endif // _STEAM_INTERFACES_H_
