//========= Mapbase - https://github.com/mapbase-source/source-sdk-2013 =================
//
// Purpose: See vscript_singletons.cpp
//
// $NoKeywords: $
//=============================================================================

#ifndef VSCRIPT_SINGLETONS
#define VSCRIPT_SINGLETONS
#ifdef _WIN32
#pragma once
#endif

void RegisterScriptSingletons();



#ifdef CLIENT_DLL
// usercmd
#define SCRIPT_NETMSG_DATA_SIZE ( ( 1 << 11 ) - 1 )
#else
// usermsg
#define SCRIPT_NETMSG_DATA_SIZE MAX_USER_MSG_DATA
#endif

#ifdef CLIENT_DLL
class CNetMsgScriptHelper : public CAutoGameSystem
#else
class CNetMsgScriptHelper
#endif
{
private:
#ifdef GAME_DLL
	CRecipientFilter m_filter;
	bf_read *m_MsgIn;
#else
	bf_read m_MsgIn;
#endif
	bf_write m_MsgOut;
	byte m_MsgData[ PAD_NUMBER( SCRIPT_NETMSG_DATA_SIZE, 4 ) ];
	HSCRIPT m_Hooks;

public:
#ifdef CLIENT_DLL
	bool m_bWriteReady; // dt ready to send

	CNetMsgScriptHelper() : m_Hooks(NULL), m_bWriteReady(false) {}
#else
	CNetMsgScriptHelper() : m_Hooks(NULL) {}
#endif

public:
#ifdef CLIENT_DLL
	bool Init(); // IGameSystem
	static void __MsgFunc_ScriptMsg( bf_read &msg );
#endif
	void LevelShutdownPreVM(); // Executed in CVScriptGameSystem
	void InitPostVM();

#ifdef GAME_DLL
	void RecieveMessage( bf_read *msg, CBaseEntity *pPlayer );
#else
	void RecieveMessage( bf_read &msg );
#endif
	void WriteToBuffer( bf_write *bf );

public:
	inline void Reset();
	void Start( const char *msg );
#ifdef GAME_DLL
	void Send( HSCRIPT player, bool bReliable );
#else
	void Send();
#endif
	void Recieve( const char *msg, HSCRIPT func );

#ifdef GAME_DLL
	inline void DoSendUserMsg( CRecipientFilter *filter, int type );
	inline void DoSendEntityMsg( CBaseEntity *entity, bool reliable );

	void SendUserMessage( HSCRIPT hPlayer, const char *msg, bool bReliable );
	void SendEntityMessage( HSCRIPT hEnt, bool bReliable );
#else // CLIENT_DLL
	void DispatchUserMessage( const char *msg );
#endif

#ifdef GAME_DLL
public:
	void AddRecipient( HSCRIPT player );
	void AddRecipientsByPVS( const Vector &pos );
	void AddRecipientsByPAS( const Vector &pos );
	void AddAllPlayers();
#endif // GAME_DLL

public:
	void WriteInt( int iValue, int bits );
	void WriteUInt( int iValue, int bits );
	void WriteByte( int iValue );  // 8 bit unsigned char
	void WriteChar( int iValue );  // 8 bit char
	void WriteShort( int iValue ); // 16 bit short
	void WriteWord( int iValue );  // 16 bit unsigned short
	void WriteLong( int iValue );  // 32 bit long
	void WriteFloat( float flValue );
	void WriteNormal( float flValue ); // 12 bit
	void WriteAngle( float flValue ); // 8 bit unsigned char
	void WriteCoord( float flValue );
	void WriteVec3Coord( const Vector& rgflValue );
	void WriteVec3Normal( const Vector& rgflValue ); // 27 bit ( 3 + 2 * (1 + NORMAL_FRACTIONAL_BITS) )
	void WriteAngles( const QAngle& rgflValue );
	void WriteString( const char *sz ); // max 512 bytes at once
	void WriteBool( bool bValue );      // 1 bit
	void WriteEntity( HSCRIPT hEnt );   // 11 bit (entindex)
	void WriteEHandle( HSCRIPT hEnt );  // 32 bit long
	int           ReadInt( int bits );
	int           ReadUInt( int bits );
	int           ReadByte();
	int           ReadChar();
	int           ReadShort();
	int           ReadWord();
	int           ReadLong();
	float         ReadFloat();
	float         ReadNormal();
	float         ReadAngle();
	float         ReadCoord();
	const Vector& ReadVec3Coord();
	const Vector& ReadVec3Normal();
	const QAngle& ReadAngles();
	const char*   ReadString();
	bool          ReadBool();
	HSCRIPT       ReadEntity();
	HSCRIPT       ReadEHandle();
	//int           GetNumBitsLeft(); // unreliable on server because of usercmds. so just do away with it
	int           GetNumBitsWritten();

};

extern CNetMsgScriptHelper *g_ScriptNetMsg;

#ifdef CLIENT_DLL
void VScriptSaveRestoreUtil_OnVMRestore();
#endif

#endif
