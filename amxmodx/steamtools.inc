
#if defined _steamtools_included
	#endinput
#endif
#define _steamtools_included

#pragma reqlib steamtools
#if !defined AMXMODX_NOAUTOLOAD
	#pragma loadlib steamtools
#endif

#include <steamtools_const>

/**
 * Called after SteamTools has completely finished loading. 
 * No features are available before this point.
 */
forward Steam_FullyLoaded();

/**
 * Fired upon a successfull connection to Steam.
 */
forward Steam_SteamServersConnected();

/**
 * Fired upon a connection failure to Steam.
 * 
 * @note This will occur periodically if the Steam client is not connected
 *       and has failed in it's retry to establish a connection.
 *
 * @param result        General result code, see k_EResult* constants
 */
forward Steam_SteamServersConnectFailure(EResult:result);

/**
 * Fired upon disconnection from Steam.
 *
 * @param result        General result code, see k_EResult* constants
 */
forward Steam_SteamServersDisconnected(EResult:result);

/**
 * Called when a client has been approved to connect to this game server.
 *
 * @param ownerAccountID   Unique account identifier of original owner for game license
 * @apram AccountID        unique account identifier of approved player
 */
forward Steam_OnValidateClient(ownerAccountID, AccountID);

/**
 * Called if the master server has requested a restart.
 * Return PLUGIN_HANDLED to block request.
 */
forward Steam_RestartRequested();


/**
 * Requests a client's status in a Steam group.
 * 
 * @note Response is returned in Steam_GroupStatusResult forward.
 *
 * @param client         Client index
 * @param groupAccountID 32-bit account ID of group
 *
 * @return               A bool representing whether or not the request was sent to Steam.
 */
native bool:Steam_RequestGroupStatus(client, groupAccountID);

/**
 * Called when a response to a group status request is recieved.
 * 
 * @note This is called for all responses recieved, not just ones requested by 
 *       your plugin.
 *
 * @param client         Client index
 * @param groupAccountID 32-bit account ID of group. Make sure to check 
 *                       this agaist the ID you are expecting
 * @param groupMember    Whether or not the client is a member in the specified group
 * @param groupOfficer   Whether or not the client is an officer in the specified group
 */
forward Steam_GroupStatusResult(client, groupAccountID, bool:groupMember, bool:groupOfficer);


/**
 * Gets the current status of VAC on the server.
 *
 * @return              A bool representing the current VAC status.
 */
native bool:Steam_IsVACEnabled();

/**
 * Gets the server's external IP address, as reported by Steam.
 *
 * @note IP address is known only after server is connected to Steam.
 *
 * @param octets        Reference to an array to be filled with the octets of 
 *                      the IP address.
 * @noreturn
 */
native Steam_GetPublicIP(octets[4]);

/**
 * Returns whether SteamTools is properly loaded.
 *
 * @return              True if loaded, false otherwise 
 */
native bool:Steam_IsLoaded();

/**
 * Sets the description of the gamen, displayed in the steam server browser.
 *
 * @param description   Game description to set
 * @noreturn              
 */
native Steam_SetGameDescription(const description[]);

/**
 * Sets an entry in the server's list of rules. 
 *
 * @note This list is used to build the response to the A2S_RULES query 
 *       and is generally known as the list of public cvars.
 *
 * @param key           Name of the key to set, is created if it does not already exist
 * @param value         Value of the key to set, the named key is removed if this is blank
 *
 * @noreturn
 */
native Steam_SetRule(const key[], const value[]);

/**
 * Clears the server's list of rules. This list is used to build the 
 *
 * @note This list is used to build the response to the A2S_RULES query 
 *       and is generally known as the list of public cvars.
 *
 * @noreturn
 */
native Steam_ClearRules();

/**
 * Forces a heartbeat to steam at the next opportunity
 *
 * @noreturn
 */
native Steam_ForceHeartbeat();

/**
 * Converts a Steam group ID to Steam ID structure
 *
 * @param groupID           Steam group ID to convert
 * @param steamID           Buffer to copy value to
 * @param maxlength         Maximum size of the buffer
 *
 * @return                  Number of characters copied
 */
native Steam_GroupIDToCSteamID(groupID, steamID[], maxlength);

/**
 * Converts a Steam ID structure to a Steam group ID.
 *
 * @param steamID           Buffer to copy value to
 *
 * @return                  Group ID
 * @error                   Invalid Steam ID
 */
native Steam_CSteamIDToGroupID(const steamID[]);

/**
 * Gets a client's Steam ID structure.
 *
 * @param client            Client index
 * @param steamID           Buffer to copy value to
 * @param maxlength         Maximum size of the buffer
 *
 * @return                  Number of characters copied
 * @error                   Invalid Steam ID
 */
native Steam_GetCSteamIDForClient(client, steamID[], maxlength);

/**
 * Converts a rendered Steam ID to its structure.
 *
 * @param renderedID        Rendered Steam ID to convert
 * @param steamID           Buffer to copy value to
 * @param maxlength         Maximum size of the buffer
 *
 * @return                  Number of characters copied
 * @error                   Invalid rendered Steam ID
 */
native Steam_RenderedIDToCSteamID(const renderedID[], steamID[], maxlength);

/**
 * Converts a Steam ID structure to its rendered string.
 *
 * @param steamID           Steam ID structure to convert
 * @param renderedID        Buffer to copy value to
 * @param maxlength         Maximum size of the buffer
 *
 * @return                  Number of characters copied
 * @error                   Invalid Steam ID
 */
native Steam_CSteamIDToRenderedID(const steamID[], renderedID[], maxlength);

