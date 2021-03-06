#ifndef __ROSE_SERVERS__
#define __ROSE_SERVERS__

#include "datatypes.h"
#include "../common/sockets.h"
#include "worldmonster.h"
#include "worldmap.h"
#include "player.h"
#include "character.h"
#include "party.h"

#include "stbhandler.h"

//STL handler.
#include "stlhandler.h"

#include "quest/strhashing.h"
#include "quest/CRoseArray.hpp"
#include "quest/CRoseFile.hpp"
#include "quest/CStrStb.hpp"
#include "quest/CIntStb.hpp"
#include "quest/QuestTrigger.h"

//LMA: LTB:
#include "ltbhandler.h"

#define MAXVISUALRANGE 100  // Original Value is 100
#define MINVISUALRANGE 60   // Original Value is 60
#define ClearItem(i) { i.appraised=false; i.count=0; i.durability=0; i.itemnum=0; i.itemtype=0; i.lifespan=0; i.refine=0; i.socketed=false; i.stats=0; i.gem=0; }

#define WORLD_THREAD 0
#define VISUALITY_THREAD 1
#define SHUTDOWN_THREAD 2

//LMA: Grids
#define NB_MAPS 154        //Nb maps (120 maps, including bad and non existant).
#define NB_GRIDS 154        //nb active grids (active maps)

//LMA: LTB.
#define LTBINDEX 0
#define LTBKOREAN 1
#define LTBENGLISH 2

#define REFINE_GRADES 16

#define REFINE_TYPES 2

//LMA: AIP
#include "aip/Aip.h"

//LMA TESTLOOP
extern UINT lma_loop;
extern UINT fmmonstertype;
extern UINT ftypeskill;
extern UINT fskill;


//LMA: test
	struct SQuestDatum1 {
		int size;
		int opcode;
		byte* data;
	};
//LMA: test.


// Main loginserver server class
class CWorldServer : public CServerSocket
{
    public:
        //LMA: static client ID and party ID.
        #ifdef STATICID
        static unsigned last_cid;
        static unsigned last_pid;
        static unsigned last_gid;
        #endif
    	//------------------ MAIN (worldserver.cpp)
    	CWorldServer ( string );
        CClientSocket* CreateClientSocket( );
    	~CWorldServer( );
    	bool OnReceivePacket( CClientSocket* thisclient, CPacket* P );
    	void OnClientDisconnect( CClientSocket* thisclient );
    	void DeleteClientSocket( CClientSocket* thisclient );
    	bool OnServerReady( );
    	void SpawnMonsters( );
    	void LoadConfigurations( const char* file);
        void LoadCommandLevels( void );
        void ServerLoop( );
        bool isBanned( sockaddr_in* ClientInfo );

        //LMA
        //Used for Grids
        CListMap allmaps[NB_MAPS];   //all maps (even those not used)
        CGridMap gridmaps[NB_GRIDS];  //only active maps
        //LMA End

      	//------------------ WORLD PROCESS (worldprocess.cpp)
      	//bool GiveExp( CMonster* thismon );
      	//LMA BEGIN
      	//20070621-211100
      	//FOR CF
      	bool GiveExp( CMonster* thismon, UINT special_lvl, UINT special_exp);
      	//LMA END

        //LMA BEGIN
        //20070623-232300
        bool Ping();        //MySQL Ping
        //LMA END

        //------------------ QUEST DATA (quest.cpp)
    	bool pakQuestReq( CPlayer* thisclient, CPacket* P );

    	//------------------ BUFFS (buff.cpp)
        CBValue GetBuffValue( CSkills* thisskill, CCharacter* character, int intvalue, UINT i, UINT up, UINT down, UINT CurrentValue, bool Buff=true, bool Status=false );
        //CBValue GetBuffValue( CSkills* thisskill, CCharacter* character, UINT Evalue, UINT i, UINT up, UINT down, UINT CurrentValue, bool Buff=true );
        bool CheckABuffs( CSkills* thisskill, CCharacter* character, int intvalue, int i);
        //bool CheckDBuffs( CSkills* thisskill, CCharacter* character, int Evalue ,int i);
        bool AddBuffs( CSkills* thisskill, CCharacter* character, int intvalue, bool flag);
        //bool AddDeBuffs( CSkills* thisskill, CCharacter* character, int Evalue );
        bool AddBuffs( CSkills* thisskill, CCharacter* character, int intvalue );
        bool InstantBuff( CSkills* thisskill, CCharacter* character, int intvalue, int i );
        UINT BuildUpBuffs( CCharacter* player );
        UINT BuildDeBuffs( CCharacter* player );
        UINT BuildUpBuffs2( CCharacter* player );
        UINT BuildDeBuffs2( CCharacter* player );
        UINT GetBUFFPosition( int status );
        UINT GetBUFFPosition2( int status );
        bool AddUseItemStatus( CUseInfo* thisuse, CPlayer* character );


    	//------------------ SERVER EXTRAS (extrafunctions.cpp)
        bool IsValidItem(UINT type, UINT id );
        unsigned BuildItemShow(CItem thisitem);
        UINT GetUIntValue( const char* s , void* var=NULL );
        int GetIntValue( const char* s , void* var=NULL );
        char* GetStrValue( const char* s , void* var=NULL );
    	bool SendSysMsg( CPlayer* thisclient, string message );
    	UINT RandNumber( UINT init, UINT range, UINT seed=0 );
        UINT ReturnItemType( CPlayer* thisclient, int slot );   //LMA: returns a "type" from an item in a slot.
    	UINT GetColorExp( UINT playerlevel,UINT moblevel, UINT exp );
    	bool CheckInventorySlot( CPlayer* thisclient, int slot );
    	bool pakGMClass( CPlayer* thisclient, char* classid );
        bool pakGMKillInRange( CPlayer* thisclient, int range );
        bool pakGMnpcshout( CPlayer* thisclient, char* shan, char* aipqsd, int npctype, int ltbid );    //LMA: LTB use.
    	bool pakGMHide( CPlayer* thisclient, int mode );
    	void SendToVisible( CPacket* pak, CPlayer* thisclient, bool dothisclient=true );
    	void SendToVisible( CPacket* pak, CPlayer* thisclient, CPlayer* xotherclient );
    	void SendToVisibleAIP( CPacket* pak, CMonster* thismon, CDrop* thisdrop );  //LMA: used for AIP.

    	void SendToVisible( CPacket* pak, class CCharacter* character, CDrop* thisdrop=NULL );

    	bool IsMonInCircle( CPlayer* thisclient, CMonster* thismon, float radius );
        bool IsPlayerInCircle( CPlayer* thisclient, CPlayer* otherclient, float radius );
        CUseInfo* GetUseItemInfo(  CPlayer* thisclient, UINT slot );
    	bool IsVisible( CPlayer* thisclient, CPlayer* otherclient );
    	bool IsMonInCircle( fPoint center, fPoint position, float radius );
    	bool IsVisible( CPlayer* thisclient, CMonster* thismon );
    	bool IsVisible( CPlayer* thisclient, CDrop* thisdrop );
        CItem GetItemByHeadAndData( unsigned head, unsigned data );
    	bool IsVisible( CPlayer* thisclient, CNPC* thisnpc );
    	bool IsVisibleNPCType( CPlayer* thisclient, UINT npc_type );    //LMA: Used for AIP.
    	void SendToVisible( CPacket* pak, CMonster* thismon, CDrop* thisdrop=NULL );
    	void SendToVisible( CPacket* pak, CDrop* thisdrop );
    	void SetVisibleEquipment(CPlayer* thisclient, UINT slot);
        CPlayer* GetClientByUserName( char *username );
    	UINT BuildBuffs( CCharacter* player );
    	UINT BuildBuffs2( CCharacter* player );
    	void CheckForLevelUp ( CPlayer* thisclient );
        bool isSkillTargetFriendly( CSkills* thisskill );
    	CRespawnPoint* GetRespawnByID( UINT id );
    	CPlayer* GetClientByCID( DWORD id, UINT map=0 );
    	CPlayer* GetClientByCharName( char* name );
    	CPlayer* GetClientByCharNameCI( char* name ); //LMA: search but case insensitive
    	CPlayer* GetClientByID( UINT id, UINT map=0 );
    	//CTeleGate* GetTeleGateByID( UINT id );
    	CTeleGate* GetTeleGateByID( UINT id, UINT offset=0);  //LMA: Telegates, with extra gate.
    	UINT BuildItemHead( CItem thisitem );
    	UINT BuildItemData( CItem thisitem );
    	CMonster* GetMonsterByID( UINT id, UINT map );
    	CSpawnArea* GetSpawnArea( UINT id, UINT map=0 );
        CMobGroup* GetMobGroup( UINT id, UINT map=0 );
        bool DeleteSpawn( CSpawnArea* spawn );
        CNPCData* GetNPCDataByID( UINT id );
    	void SendToMap( CPacket* pak, int mapid );
    	bool IsUnionAlly( int union_player, int union_other_player);    //LMA: check if two players are in the same alliance.
    	void SendToUnionInMap( CPacket* pak, int mapid, int union_player);  //LMA: Send to ally (union) in a map.
        unsigned BuildItemRefine(CItem thisitem );
        CMDrops* GetDropData( UINT id );
    	CRespawnPoint* GetRespawnByMap( int id );
        CSkills* GetSkillByID( UINT id );
        CStatus* GetStatusByID( unsigned int id );  //getting status.
    	void ClearClientID( UINT id );
    	void ClearGAid( UINT id );
    	CDrop* GetDropByID( UINT id, UINT map=0 );
        CChest* GetChestByID( UINT id );
    	CNPC* GetNPCByID( UINT id, UINT map=0 );
    	char* GetNPCNameByType(UINT id);    //LMA: getting the npc name.
    	char* GetSTLObjNameByID(UINT family, UINT idorg,bool comment=false);   //LMA: getting the STL name for objects (1-14)
    	char* GetSTLMonsterNameByID(UINT idorg); //LMA: getting the STL name of a monster / NPC by ID.
        char* GetSTLItemPrefix(int family, UINT idorg);  //LMA: to get an item prefix.
        char* GetSTLQuestByID(UINT idorg);  //LMA: getting quest name by STL.
        char* GetSTLSkillByID(UINT idorg);  //LMA: to get a skill name :)
        char* GetSTLZoneNameByID(UINT idorg);   //LMA: get a zone name :)
        CNPC* GetNPCByType( UINT npctype );
        //CDrop* GetDrop( CMonster* thismon );
        CDrop* GetPYDrop( CMonster* thismon, UINT droptype );   //hidden
        CDrop* GetPYDropAnd( CMonster* thismon, UINT droptype );   //LMA: And system.
    	void SendToAll( CPacket* pak );
    	void SendToAllInMap( CPacket* pak, int mapid);     //LMA: Send a message to all people in a given map
        UINT gi(UINT itemvalue, short type);    //LMA: to get a clean itemtype and itemnum.
        UINT getClanPoints(int clanid);   //LMA: We get the exact Clan Points amount through mysql database
        UINT getClanGrade(int clanid);    //LMA: We get the exact Clan Grade through mysql database
        UINT SummonFormula(CPlayer* thisclient,CMonster* thismonster);    //LMA: Formulas for summons.
        string EscapeMe(char* texte);  //LMA: Escaping.
        UINT GetTimerFromAIP(dword script_id, bool is_npc); //LMA: used to get a timer from a script.
        UINT GetBreakID(UINT itemnum,UINT itemtype);  //LMA: Getting the breakid from an item.
        int GetNbUserID( UINT userid ); //LMA: Used to count all avatars from a single userid.
        bool ForceDiscClient( unsigned int userid );  //LMA: Disc a client.
    	UINT GetNewClientID( );
    	UINT GetNewGAId( );
    	unsigned GetNewPartyID( );  //LMA: Getting a Party ID.
    	unsigned GetNewGAGroup( );  //LMA: Getting a Party ID.
    	void DisconnectAll();
    	CPlayer* GetClientByUserID( UINT userid );
        UINT GetLevelGhost( UINT map, UINT level );
        UINT GetFairyRange( UINT part );
        UINT GetGridNumber(int mapid, int posx, int posy);    //LMA: maps
        UINT GetGridNumber(int mapid, int posx, int posy, CMonster* thismonster);    //LMA: maps for monsters.
        bool SaveAllStorage( CPlayer* thisclient);    //LMA: Saving Storage (all).
        bool GetAllStorage( CPlayer* thisclient);     //LMA: Getting Storage (all).
        bool GetSlotStorage( CPlayer* thisclient,UINT slotnum);   //LMA: refreshing one slot from MySQL storage.
        bool SaveSlotStorage( CPlayer* thisclient,UINT slotnum);  //LMA: Saving one slot into MySQL storage.
        bool SaveSlotMall( CPlayer* thisclient,UINT slotnum);  //LMA: Saving one slot into MySQL Item Mall.
        bool SaveItemMallPoints(CPlayer* thisclient);
        bool GetZulyStorage( CPlayer* thisclient);     //LMA: Get Zuly from Storage (from MySQL)
        bool GetWishlist( CPlayer* thisclient);   //LMA: Getting Wishlist.
        bool SaveZulyStorage( CPlayer* thisclient);    //LMA: Save Zuly to Storage (to MySQL)
        CMonster* LookAOEMonster( CPlayer* thisclient);       //LMA: Trying to get a monster from a location (for AOE_TARGET paket mainly).
        CMonster* LookAOEMonster( class CCharacter* character);       //LMA: Character version.
        void ReturnItemMallList(CPlayer* thisclient);   //LMA: Return ItemMall List
        void TakeItemMallList(CPlayer* thisclient,int qty,int slot); //LMA: takes an item from Item Mall to player's inventory
        void RefreshFairy( );

        bool CheckValidName(CPlayer* thisclient, const char* my_name);    //LMA: Checking avatar name to avoid GM tags and not valid chars.
        bool EscapeMySQL(const char* data,string & mystring,int nb_car_max,bool check_same);   //LMA: escaping.
        bool CheckEscapeMySQL(const char* data,int nb_car_max,bool check_same);    //LMA: escaping (this version only checks escaped and unescaped versions of a string are the same).

        // PY extra stats lookup
        UINT GetExtraStats( UINT modifier );
        // PY end

        //------------------ Fairies ---------------------
        void DoFairyStuff( CPlayer* targetclient, int action );
        void DoFairyFree( int fairy );

    	//------------------ MATH (extrafunctions.cpp)
    	fPoint RandInCircle(fPoint center, float radius);
    	fPoint RandInPoly(fPoint p[], int pcount);
    	fPoint RandInTriangle(fPoint p1, fPoint p2, fPoint p3);
    	float AreaOfTriangle(fPoint p1, fPoint p2, fPoint p3);
    	float distance( fPoint pos1, fPoint pos2 );

    	//------------------ PACKETS (worldpackets.cpp)
    	bool pakWeight( CPlayer* thisclient, CPacket* P );
    	bool pakPrintscreen( CPlayer* thisclient, CPacket* P );
    	bool pakSpawnChar( CPlayer* thisclient, CPlayer* otherclient );
        bool pakModifiedItemDone( CPlayer* thisclient, CPacket* P );
        bool pakItemMallList( CPlayer* thisclient, CPacket* P );
    	bool pakSpawnDrop( CPlayer* thisclient, CDrop* thisdrop );
        bool pakChangeStorage( CPlayer* thisclient, CPacket* P );
        bool pakChangeItemMallPoints(CPlayer* thisclient, CPacket* P);
        bool pakUpdateIMpoints(CPlayer* thisclient, CPacket* P);
        bool pakModifiedItem( CPlayer* thisclient, CPacket* P );
        bool pakPartyManager( CPlayer* thisclient, CPacket* P );
        bool pakLearnSkill( CPlayer *thisclient, CPacket* P );
        bool pakLevelUpSkill( CPlayer *thisclient, CPacket* P );
    	bool pakChangeStance( CPlayer* thisclient, CPacket* P );
        bool pakCharDSClient( CPlayer* thisclient, CPacket* P );
    	bool pakChangeEquip( CPlayer* thisclient, CPacket* P );
    	bool pakChangeCostume( CPlayer* thisclient, CPacket* P );
    	void pakSetVisibleEquipment(CPlayer* thisclient, BYTE slot);
    	bool pakStartAttack( CPlayer* thisclient, CPacket* P );
       	bool pakSpawnNPC( CPlayer* thisclient, CNPC* thisnpc );
        bool pakSpawnIfoObject( CPlayer* thisclient, UINT npctype,bool forcerefresh=false);    //LMA: Spawning IfoObjects.
        bool pakRideRequest( CPlayer* thisclient, CPacket* P );
        bool pakPartyActions( CPlayer* thisclient, CPacket* P );
        bool pakPartyOption( CPlayer* thisclient, CPacket* P );
    	bool pakTradeAction( CPlayer* thisclient, CPacket* P );
    	bool pakChangeCart( CPlayer* thisclient, CPacket* P );
        bool pakSkillSelf ( CPlayer* thisclient, CPacket* P );
    	bool pakDoIdentify( CPlayer *thisclient, CPacket *P );
    	bool pakNormalChat( CPlayer* thisclient, CPacket* P );
    	bool pakCharSelect( CPlayer* thisclient, CPacket* P );
    	bool pakChatUnion ( CPlayer* thisclient, CPacket* P );  //LMA: chat union.
        bool pakChatTrade ( CPlayer* thisclient, CPacket* P );  //LMA: chat trade.
        int CheckPatList(CPlayer* client);
        int GetMileageFreeSlotOnline(CPlayer* client);
        int GetMileageFreeSlotOffline(char* name);
        CItem CreateItem(int itemID, int itemType, int amount);
        bool GetIfCharExists(const char * name);
        bool GiftOffline( CPlayer* thisclient, char* name, int fromslot, int toslot);
        bool GiftOnline( CPlayer* fromclient, CPlayer* toclient, int fromslot, int toslot);
        bool GetIfOwnName(CPlayer* thisclient, char* name);
    //quest trigger
        bool pakQuestTrigger( CPlayer* thisclient);

    // Observe: LuckySpin
        bool pakLuckySpin           ( CPlayer* thisclient, CPacket* P );  //Observe: Lucky Spin.
        bool pakLuckySpinList       ( CPlayer* thisclient, CPacket* P );  //Observe: Lucky Spin List.
        bool pakLuckySpinShuffle    ( CPlayer* thisclient, CPacket* P );  //Observe: Lucky Spin Shuffle.
        bool pakLuckySpinBuy        ( CPlayer* thisclient, CPacket* P );  //Observe: Lucky Spin Buy Spins.
        bool pakSpinWheel           ( CPlayer* thisclient, CPacket* P );  //Observe: Spin Wheel.
        bool pakLuckySpinCoins      ( CPlayer* thisclient, CPacket* P );  //Observe: Buy spins with coins

    // Observe: Premium
        bool pakPremiumRedeem       ( CPlayer* thisclient, CPacket* P ); // Observe: Redeem free item a day

    // Observe: Itemmall
        bool pakItemmallBuy         ( CPlayer* thisclient, CPacket* P ); // Observe: Buy Itemmall Item
        bool pakItemMall            ( CPlayer* thisclient, CPacket* P );

    // Observe: Mount
        bool pakMount               ( CPlayer* thisclient, CPacket* P ); // Mount / Dismount
    // Vetyst: GA
        bool pakGameListReq              ( CPlayer* thisclient, CPacket* P );
        bool pakGroup               ( CPlayer* thisclient, CPacket* P );
        bool Recv_pakGroupMemberList               ( CPlayer* thisclient, CPacket* P );
        void Send_pakGroupMemberList(GAGroup* thisgroup, BYTE btCommand, CPlayer* player=NULL);
    // Observe: Game Arena
        bool RefreshGroups          ( CPlayer* thisclient );
        bool pakGroupActions        ( CPlayer* thisclient, CPacket* P ); // ToxiN: Invite / Leave / Kick
        bool pakGroupChat           ( CPlayer* thisclient, CPacket* P ); // ToxiN: Group Chat
        bool pakGroupManager        ( CPlayer* thisclient, CPacket* P ); // ToxiN: Group managing
        bool pakGroupOption         ( CPlayer* thisclient, CPacket* P ); // ToxiN: Group Options
        bool pakQueueJoin           ( CPlayer* thisclient, CPacket* P );
        bool pakQueueLeave          ( CPlayer* thisclient, CPacket* P );
        bool pakSendGroupID         ( CPlayer* thisclient, CPacket* P );
        bool pakJoinArena           ( CPlayer* thisclient, CPacket* P );
        bool SeaOfDawn              ( GAGroup* thisgroup, int map );
        bool SeaOfDawnSpawn         ( GAGroup* thisgroup , int map );

    	bool pakStartSkill( CPlayer* thisclient, CPacket* P );
        bool pakPvp796( CPlayer* thisclient, CPacket* P );   //LMA: test for packet 0x796
        bool pakRepairItem( CPlayer* thisclient, CPacket* P,int packet_type);
    	bool pakMoveSkill( CPlayer* thisclient, CPacket* P );
        bool pakCloseShop( CPlayer* thisclient, CPacket* P );
        bool pakStoreZuly( CPlayer* thisclient, CPacket* P );
        bool pakPartyChat( CPlayer* thisclient, CPacket* P );
    	bool pakGameGuard( CPlayer* thisclient, CPacket* P );
        bool pakOpenShop( CPlayer* thisclient, CPacket* P );
        bool pakShowShop( CPlayer* thisclient, CPacket* P );
        bool pakSellShop( CPlayer* thisclient, CPacket* P );
    	bool pakMoveChar( CPlayer* thisclient, CPacket* P );
    	bool pakStopChar( CPlayer* thisclient, CPacket* P );
    	bool pakPickDrop( CPlayer* thisclient, CPacket* P );
    	bool pakAddStats( CPlayer* thisclient, CPacket* P );
        bool pakShowHeal( CPlayer* thisclient, CPacket* P );
        bool pakSkillAOE( CPlayer* thisclient, CPacket* P );
        bool pakidentify( CPlayer* thisclient, CPacket* P );
    	bool pakUserDied( CPlayer* thisclient, CPacket* P );
    	bool pakTradeAddItem( CPlayer* thisclient, CPacket* P );
    	bool pakTradeAddZulie( CPlayer* thisclient, CPacket* P );
        bool pakUseItem ( CPlayer* thisclient, CPacket* P );
    	bool pakWhisper( CPlayer* thisclient, CPacket* P );
    	bool pakEquipABC(CPlayer* thisclient, CPacket *P );
        bool pakStorage( CPlayer* thisclient, CPacket* P );
        bool pakBuyShop( CPlayer* thisclient, CPacket* P );
    	bool pakDoEmote( CPlayer* thisclient, CPacket* P );
    	bool pakNPCBuy( CPlayer* thisclient, CPacket* P );
    	bool pakDoDrop( CPlayer* thisclient, CPacket* P );
    	bool pakShout( CPlayer* thisclient, CPacket* P );
    	bool pakCraft( CPlayer* thisclient, CPacket* P );
    	bool pakDoID( CPlayer* thisclient, CPacket* P );
    	bool pakGate( CPlayer* thisclient, CPacket* P );
    	bool pakChangeRespawn( CPlayer* thisclient, CPacket* P );
    	bool pakExpTC ( CPlayer* thisclient, CPacket* P );   //LMA: Bonus Exp Time Coupon
        bool pakExit( CPlayer* thisclient, CPacket* P );
    	bool pakPing( CPlayer* thisclient, CPacket* P );
      	bool SendLevelUPtoChar(CPlayer* thisclient );
        bool pakRepairTool( CPlayer* thisclient, CPacket* P );
        void pakPlayer( CPlayer *thisclient );
        void pak724( CPlayer *thisclient );
        void pak855( CPlayer *thisclient );
        void pakServerTime( CPlayer *thisclient );
        void pakInventory( CPlayer *thisclient );
        void pakCostumeEquipment( CPlayer *thisclient );
        void pakQuestData( CPlayer *thisclient );
    	void pakClearUser( CPlayer* thisclient );
//        bool LearnSkill( CPlayer* thisclient, UINT skill );
        bool LearnSkill( CPlayer* thisclient, UINT skill, bool takeSP = true );
        bool CheckCompatibleClass(UINT rclass, UINT player_job);
   		bool pakAddWishList( CPlayer* , CPacket* );
        bool GiveChestItems( CPlayer* thisclient,UINT chest_slot,CChest* thischest); //LMA: used for chests and slots.
        bool GiveDasmItems( CPlayer* thisclient,UINT src);    //LMA: Used for disassemble.
        bool GiveDefaultDasm( CPlayer* thisclient,UINT src, bool not_found, bool is_failed);

        //-------------------------- Clan functions
        bool pakClanManager ( CPlayer* thisclient, CPacket* P );
        bool pakCreateClan ( CPlayer* thisclient, CPacket* P );

    	//------------------ GM COMMANDS (gmcmds.cpp)
    	bool pakGMMoveTo( CPlayer* thisclient, fPoint position );
    	bool pakGMTeleAllHere( CPlayer* thisclient );
    	bool GMShowTargetInfo( CPlayer* thisclient );
    	bool pakGMServerInfo( CPlayer* thisclient );
    	bool pakGMHeal( CPlayer* thisclient );
    	bool pakGMStat( CPlayer* thisclient, const char* statname, int statvalue );
        bool pakGMItemtoplayer(CPlayer* thisclient, char* name , UINT itemid , UINT itemtype , UINT itemamount , UINT itemrefine , UINT itemls, UINT itemstats , UINT itemsocket) ;
        bool pakGMItem( CPlayer* thisclient, UINT itemid , UINT itemtype , UINT itemamount , UINT itemrefine , UINT itemls, UINT itemstats , UINT itemsocket );

        //LMA: for Quests
        bool pakGMItemQuest( CPlayer* thisclient, UINT itemid, UINT itemtype, UINT itemamount, UINT itemrefine, UINT itemls, UINT itemstats, UINT itemsocket, char buffer2[200] );
        bool pakGMTeleOtherPlayer( CPlayer *thisclient, char* name, int map, float x, float y );
        bool pakGMZuly( CPlayer* thisclient, int mode, int amount, char* charname );
        bool pakGMEventType(CPlayer* thisclient, int npctype, int dialog, long int type); //Event
        bool pakGMEventName(CPlayer* thisclient, char* eventname, int is_on); //Event (by its name)
        bool pakGMEventIFO(CPlayer* thisclient, int ifoType,int eventID);   //LMA: for Ifo Objects
    	bool pakGMTele( CPlayer* thisclient, int map, float x, float y, int no_qsd=0);
    	bool pakGMMon( CPlayer* thisclient, int montype, int moncount,int monteam=0, int lma_aip=-1);
        //bool pakGMZulygive(CPlayer* thisclient, char* name, int zuly);
        bool pakGMZulygive(CPlayer* thisclient, char* name, long long zuly);
        bool pakGMFairyto(CPlayer* thisclient, char* name, int mode);
        bool pakGMClanRewardPoints(CPlayer* thisclient, char* name, int points);    //reward points
        bool pakGMClanPoints(CPlayer* thisclient, char* name, int points);    //clan points
        bool pakGMRaiseCG(CPlayer* thisclient); //LMA: Raising Clan Grade
        bool pakGMManageFairy(CPlayer* thisclient, int mode);
        bool pakGMHurtHim(CPlayer* thisclient, char* name);   //LMA: Gm command.
        bool pakGMChangeFairyWait(CPlayer* thisclient, int newvalue);
        bool pakGMChangeFairyStay(CPlayer* thisclient, int newvalue);
        bool pakGMChangeFairyTestMode(CPlayer* thisclient, int mode);
        bool pakGMTelePlayerHere( CPlayer* thisclient, char* name );
        bool pakGMAllSkill ( CPlayer* thisclient, char* name); // by crashinside
        bool pakGMDelSkills ( CPlayer* thisclient, char* name); // by rl2171
        bool pakGMObjVar(CPlayer* thisclient, int npctype, int output=1); //LMA: ObjVar for a NPC.
        bool pakGMSetObjVar(CPlayer* thisclient, int npctype, int index, int value);    //LMA: set an ObjVar for a NPC.
        bool pakGMForceUW(CPlayer* thisclient, int time); //LMA: forcing Union Wars.
        bool pakGMForceGemQuest(CPlayer* thisclient, int time); //LMA: forcing gem quest.
        bool pakGMForceUWPlayers(CPlayer* thisclient, int nb_players);   //LMA: forcing Union Wars (nb players).
        bool pakGMExportSTBSTL(CPlayer* thisclient);    //LMA: export STB and STL to a .sql file.
        bool pakGMSpawnList(CPlayer* thisclient, int range); //LMA: New spawn handling
        bool pakGMSpawnDetail(CPlayer* thisclient, UINT id, UINT map); //LMA: New spawn handling
        bool pakGMSpawnForceRefresh(CPlayer* thisclient, UINT id, UINT map);   //LMA: New spawn handling
        bool pakGMReborn( CPlayer* thisclient); //Reborn by Core
        bool pakGMLevel( CPlayer* thisclient, int level , char* name);
        bool pakGMTeleToPlayer( CPlayer* thisclient, char* name );
        bool pakGMDoEmote( CPlayer* thisclient, int emotionid );
    	bool pakGMCommand( CPlayer* thisclient, CPacket* P );
        bool ReloadMobSpawn( CPlayer* thisclient, int id );
        bool pakGMKick( CPlayer* thisclient, char* name );
    	bool pakGMAnn( CPlayer* thisclient, CPacket *P );
        bool pakGMBan( CPlayer* thisclient, char* name );
        bool pakGMInfo(CPlayer* thisclient, char* name);
        bool pakGMNpc(CPlayer* thisclient, int npcid, int dialogid,int eventid);
        bool pakGMUnion(CPlayer* thisclient, char* name, int which_union);      //LMA: UW.
        bool pakGMUnionPoints(CPlayer* thisclient, char* name, int nb_points);    //LMA: giving faction points
      	bool pakGMGotomap( CPlayer* thisclient, int map, bool no_qsd_zone=false);
      	bool pakGMMute( CPlayer* thisclient, char* name, int time);
      	bool pakGMPartylvl( CPlayer* partyclient, int level );
      	bool pakGMChangePlayerDmg(CPlayer* thisclient, int rate);
      	bool pakGMChangeMonsterDmg(CPlayer* thisclient, int rate);
      	bool pakGMChangeCfmode(CPlayer* thisclient, int mode);
      	bool pakGMWhoAttacksMe(CPlayer* thisclient);
      	bool pakGMChangeAtkSpeedModif(CPlayer* thisclient, int modif);
      	bool pakGMChangeHitDelayModif(CPlayer* thisclient, int modif);
      	bool pakGMChangeMSpeedModif(CPlayer* thisclient, int modif);
        bool pakGMBuff(CPlayer* thisClient, int strength); // by Drakia
        bool pakGMDebuff(CPlayer* thisClient); // by Drakia
        bool pakGMGiveBuff(CPlayer* thisClient, CPlayer* targetClient, int skillID, int strength); // by Drakia
        bool pakGMMaxStats(CPlayer* thisClient);
        bool pakGMGMSkills ( CPlayer* thisclient, char* name);
        bool pakGMGMGear ( CPlayer* thisclient, char* name);
        bool pakGMWarning(CPlayer* thisclient, char* name, char* reason);
        bool pakWarningCount(CPlayer* thisclient);
        bool pakWarningReason(CPlayer* thisclient);
        bool pakGMSeeWarning(CPlayer *thisclient, char *name);
        void pakGMDelWarning(CPlayer *thisclient, char *name,int warning);
    	//------------------ CHAR SERVER COMMANDS (worldpackets.cpp)
    	bool TeleportTo ( CPlayer* thisclient, int map, fPoint position );
    	bool pakCSCharSelect( CPlayer* thisclient, CPacket* P );
    	bool pakCSReady( CPlayer* thisclient, CPacket* P );

    	//----------------- Server StartUp Functions (startup.cpp)
    	bool LoadSTBData( );

        bool LoadIfoObjects( ); //LMA: Loading Objects ID.

        //lma: initializing arrays default values...
        bool InitDefaultValues();

        bool LoadLTB( );    //LMA: LTB.
    	bool LoadZoneData( );
    	bool LoadGrids( );         //LMA: maps
    	bool LoadConsItem( );
    	bool LoadSellData( );
    	bool LoadGameArenaData();
    	bool LoadProductItem( );
    	bool LoadPatItem( );
    	bool LoadMountItem( );      // Observe: Load Mounts
    	bool LoadNaturalItem( );
    	bool LoadJemItem( );
    	bool LoadEquip( );
    	bool LoadItemStats( );
    	bool LoadGrades();  //LMA: loading refines (grades) bonuses.
    	bool LoadJobReq( ); //LMA: Job requirements.
    	bool LoadStatLookup( );
        bool LoadTeleGateData( );
        bool LoadIfoObjects2( );
        bool LoadArenaSpawns();

        //Voting Point shop
        bool voteShopInfo(CPlayer* thisclient, int type);
        bool voteShopBuy(CPlayer* thisclient, int number);
        int GetVotePoints(CPlayer* thisclient);

        #ifdef PYCUSTOM
        bool LoadCustomTeleGate( );
        bool LoadCustomEvents( );
        #endif

        bool LoadMonsterSpawn( );       // PY new spawn version. Switch to LoadMobGroups to restore lmame version
        bool LoadMobGroups( );
        //bool LoadMobGroupsSpecial( );   //LMA: Special Spawns (Halloween for example)
        bool LoadRespawnData( );

        // QSD Fuctions
        void ReadQSD(strings path, dword index);
        //void ExportQSDData(byte* dataorg,int opcode,int size,CQuestTrigger::SQuestDatum* dataraw);
        void ExportQSDData(byte* dataorg,int size,int opcode);  //LMA: exporting QSD conditions.
        void ExportQSDDataA(byte* dataorg,int size,int opcode);  //LMA: exporting QSD actions.
        char* Operators(byte btOp,char* buffer); //LMA: operators.
        char* Abilities(int btOp,char* buffer); //LMA: abilities.
        void LoadQuestData( );
        bool LoadQuestSTB();
        bool LoadBreakChestBlueList( );
        void LoadAnimationTimes( );



        fpQuestCond qstCondFunc[31];
        fpQuestRewd qstRewdFunc[35];

        //LMA: for AIP:
        fpQuestCondC qstCondFuncC[31];
        fpQuestRewdC qstRewdFuncC[35];

        CSTBData                STB_QUEST;

        //LMA: AIP:
        inline int round(double x) {return int(x > 0.0 ? x + 0.5 : x - 0.5);};
        //vector<CAip*> AipList;
        map<dword,CAip*> AipListMap;    //LMA: testing maps...
        map<dword,UINT> AipListTimer;    //LMA: Timer for AIP...

        //map<UINT,UINT> NPC_AIP;       //LMA: NPC AIP.
        map<UINT,vector<UINT> > NPC_AIP;       //LMA: NPC AIP.
        map<UINT,vector<CMDrops*> > DropsAnd;       //LMA: Drops And system.

        //LMA: test for quest hack (stackable).
        #ifdef QHACK
        map<UINT,int> MapStackQuest;       //LMA: map of stackable quests.
        #endif
        //LMA: end

        fpAiCond aiCondFunc[37];
        fpAiAct aiActFunc[40];

        //int ObjVar[4000][20];  //NPC variables used in AI
        int ObjVar[MAX_NPC][20];  //NPC variables used in AI

        // AIP Functions
        void ReadAIP(strings path, dword index);
        void LoadAipData();

        //LMA: exporting.
        void ExportAipData(byte* dataorg,int size,int opcode);  //LMA: exporting AIP condition data
        void ExportAipDataA(byte* dataorg,int size,int opcode);  //LMA: exporting AIP actions data
        //LMA: END exporting.

        bool LoadDropsData( );
        bool LoadPYDropsData( );    //hidden
        bool LoadPYDropsDataAnd( );    //hidden
        bool LoadSkillBookDropsData( ); //hidden
        bool LoadLuckyItems( ); // Observe: Loading luckyspin items
        bool LoadConfig( );
        //bool LoadLTB( );
        bool LoadChestData( );
        bool LoadSkillDataOld( );  //LMA: Old version, for debug
        bool TimerForNPC( );    //LMA: loading timer for NPC and monsters
        bool LoadStatusData( ); //loading status.
        bool LMACheckSkills( );  //LMA: for skill debug
        bool LMACheckStuff();   //LMA: for different tests.
        bool LoadSkillData( );
        bool LoadMonsters( );           //PY: Not used in this build.
        bool LoadMonstersFromIFO( );    //PY: New version of monster loading using IFO data
        bool LoadNPCData( );
        bool LoadQuestItemData( );
        bool LoadUpgrade( );
        bool LoadNPCs( );
        bool LoadNPCsSpecial( );
        bool LoadNPCsEvents( ); //LMA: loading events.
        bool LoadWayPoints( );  //PY Waypoints for TD game
        bool CleanConnectedList( );

        CQuest* GetQuestByID( unsigned long int id );
        CQuest* GetQuestByQuestID( unsigned long int questid );
        CQuest* GetQuestByFinalID( unsigned long int questid );
        bool DoQuestScript( CPlayer* thisclient, CQuest* thisquest );
        QUESTS* GetQuestByItemID( unsigned long int itemid );

        // Server Functions
        //bool SendPM( CPlayer* thisclient, char msg[200] );
        bool SendPM( CPlayer* thisclient, const char* Format, ... );
        bool SendGlobalMSG( CPlayer* thisclient, char msg[200] );

        //LMA: AIP and custom events.
        //bool NPCShout( CMonster* thismon, char msg[200], char npc[50], int mapid=0);
        bool NPCShout( CMonster* thismon, char* msg, char* npc, int mapid=0);
        //bool NPCAnnounce( char msg[200], char npc[50] );
        bool NPCAnnounce( char* msg, char* npc, int mapid=0 );
        bool NPCShout2( CMonster* thismon, char* msg, char* npc );
        bool NPCWhisper( CPlayer* thisclient, CMonster* thismon, char msg[200], char npc[50] );
        bool NPCMessage( CPlayer* thisclient, char msg[200], char npc[50] );

        UINT GetMaxPartyExp( UINT partylevel );
        bool DoSkillScript( CCharacter* character, CSkills* thisskill );

        bool AddParty( CParty* );
        bool ADDGAGroup( GAGroup* );
        bool RemoveParty( CParty* );
        bool RemoveGroup( GAGroup* );

    	UINT				    ClientIDList[0x10000];	// Clients List
    	UINT				    GAIDList[0x10000];	// Clients List
    	bool				    PartyIDList[0x1000];     //LMA: Party IDs
    	bool				    GAGroupIDList[0x1000];     //LMA: Party IDs
    	SOCKET					csock;					// Socket for accessing the char server
    	char*					cct;					// Encryption table for char server
    	map<int,char*>          NpcNameList;            //LMA: npc name map.
    	map<UINT,char*>         STLNameList;            //LMA: Name for all things taken from STL.
        map<UINT,char*>         STLCommentList;            //LMA: Comment for all things taken from STL.
    	IfoObject               WarpGate;               //LMA: Warp Gate for Union.
    	int                     LastTempleAccess[2];    //LMA: For temple Access.
    	word                    UWForceFrom;            //LMA: Forcing Union Wars (test)
    	word                    GemQuestForce;          //LMA: Forcing gem quest.
    	int                     GemQuestReset;          //LMA: Forcing gem quest to reset spawn counter.
    	int                     UWNbPlayers;            //LMA: Forcing Nb players required for UW.


        map<int,vector<int> >    ListAllNpc;             //LMA: we store all the NPC in a single map.


        UINT                    upgrade[16];         //LMA: new way (after 2010/05)
        UINT                    refine_grade[31];    //lma: refining rules.

        CEquipList              EquipList[10];
        CJemList                JemList;
        CNaturalList            NaturalList;
        CPatList                PatList;
        CMountList              MountList;              // Observe: MountList
        CProductList            ProductList;
        CSellList               SellList;
        CListArena              ArenaList;
        CUseList                UseList;
        CMapList                MapList;
        LTBData                 MyLTB;                  //LMA: LTB
        LTBData                 MyLTBQSD;               //LMA: LTB for QSD

        vector<CQuestTrigger*> TriggerList;
        vector<CTeleGate*>		TeleGateList;			// Telegates List
        vector<CIfoObject*>		IfoObjectList;
        vector<CCustomGate*>    CustomGateList;         // Custom Telegate list
        vector<CCustomEvent*>   CustomEventList;        //Custom events list
        vector<CCArenaSpawn*>    ArenaSpawnList;
        vector<CQuest*>         QuestList;              // Quest List
        vector<CSkills*>        SkillListOld;           // Skills List (LMA: for debug)
        vector<CMDrops*>        MDropList;              // Drops List
        vector<CMDrops*>        SkillbookList;          // Skillbook drop list (hidden)
        vector<CParty*>         PartyList;              // Party List
        vector<CFairy*>         FairyList;              // Fairy List
        vector<CChest*>         ChestList;              // Chest List
        vector<GAGroup*>        GroupList;              // Party List

        CExtraStats             StatLookup[MAX_EXTRA_STATS];
        vector<CStatus*>                      StatusList;
        vector<CSkills*>                        SkillList;
        vector<CGrade*>                       GradeList;
        vector<CNPCData*>                   NPCData;
        vector<CQuestItemData*>         QuestItemData;
        vector<CItemStas*>                 StatsList;
        vector<CLTBstring*>                 Ltbstring;
        vector<CLTBstring*>                 LtbstringQSD;
        vector<CBreakList*>                  BreakList;

        vector<UINT>            **ClassList;
        int                     maxClass;

        /*int                     maxStats;
        int                     maxStatus;
        int                     maxSkills;
        int                     maxGrades;
        int                     maxNPC;
        int                     maxQuestItems;
        int                     maxltbaip;              //LMA: max aip ltb
        int                     maxltbqsd;              //LMA: max qsd ltb
        int                     maxBreak;*/
        int                     maxZone;                //LMA: max map amount

        UINT BreakListSize;
        //bool LoadBreakList();

        //LMA: test for quest hack (stackable).
        #ifdef QHACK
        CSTBDataChar			STB_NPC;				// NPC data
        #else
        CSTBData				STB_NPC;				// NPC data
        #endif

        CSTBData				STB_ITEM[15];			// Item data
        CSTBData                STB_SKILL;              // Skill data
        CSTBData                STB_STATUS;             // Status Data
        CSTBData                STB_DROP;               // Drop list
        CSTBData                STB_PRODUCT;            // Crafting data
        CSTBData                STB_SELL;               // npc sell index
        CSTBData                BreakData;              // Break - Used for Break list, chests and blue crafts.
        CSTBDataChar            ZoneData;               // LMA: zone Data.
        CSTBData                ListClass;              // LMA: used to store the class list, actually to check equip requirements.
        CSTBData                ListGrade;              // LMA: used to store the refine bonuses (% and +).
        CSTBData                ListArena;              // LMA: used to store the refine bonuses (% and +).
    	clock_t				   lastServerStep;			// Last Update
    	clock_t                LastUpdateTime;          // Store the last update World Time
    	bool                   ServerOnline;
        string filename;

        pthread_t WorldThread[3];
        pthread_t MapThread[200];
        pthread_attr_t at;
        pthread_mutex_t MapMutex,SQLMutex,PlayerMutex;

        CDatabase* DB;

        //Test thingies
        unsigned ATTK_SPEED_MODIF;
        unsigned HIT_DELAY_MODIF;
        unsigned MOVE_SPEED_MODIF;
       // geo edit for disassemble // 22 oct 07

        // console
        bool handleCommand( char* );
        bool pakConsoleAnn( char* from, char* message );
		bool pakConsoleKick(char* name);

		//PY
		TowerDef WPList[100][100];      //Way point list for tower def. [mapid][wp id]
		bool ServerDebug;               //universal debug mode boolean. Activate/deactivate with GM command /serverdebug
};

extern class CWorldServer* GServer;

PVOID VisibilityProcess( PVOID TS );
PVOID MapProcess( PVOID TS );
PVOID WorldProcess( PVOID TS );
PVOID ShutdownServer( PVOID TS );

void StartSignal( );
void StopSignal( );
void HandleSignal( int num );
void PacketTime(CPlayer* thisclient);
// -----------------------------------------------------------------------------------------

#endif
