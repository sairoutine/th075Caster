#include "mainDatClass.h"
using namespace std;

int mainDatClass::Communicate(){
	//アクセス相手がどんな状況か、など
	//myInfo.terminalMode = mode_root など
	
	accessFlg = status_default;
	
	for(;;){
		if( floatControlFlg ){
			//for FPU control flg
			BYTE data[10];	//dummy
			data[0] = 0;	//cowcaster_id[5]
			data[1] = 0;
			data[2] = 0;
			data[3] = 0;
			data[4] = 0;
			data[5] = 0;	//flg
			data[6] = floatControlFlg;
			SendCmd( dest_access, cmd_access, data, 7 );
		}else{
			SendCmd( dest_access, cmd_access );
		}
		
		Sleep(100);
		if( accessFlg == status_ok ){
			//アクセスOK
			myInfo.terminalMode = mode_root;
			lastTime.Away = nowTime;
			WaitForSingleObject( hMutex, INFINITE );
			Away = Access;
			memset( &Access, 0, sizeof(Access) );
			ReleaseMutex( hMutex );
			break;
			
		}else  if( accessFlg == status_bad ){
			//アクセスBAD
//			cout << "STATUS : BAD ( Communicate() )" << endl;
			myInfo.terminalMode = mode_branch;
			lastTime.Root = nowTime;
			WaitForSingleObject( hMutex, INFINITE );
			Root = Access;
			memset( &Access, 0, sizeof(Access) );
			ReleaseMutex( hMutex );
			break;
			
		}else if( accessFlg == status_error ){
			cout << "ERROR : ( version error? / float setting )" << endl;
			myInfo.terminalMode = mode_default;
			WaitForSingleObject( hMutex, INFINITE );
			memset( &Access, 0, sizeof(SOCKADDR_IN) );
			ReleaseMutex( hMutex );
			return 1;
			
		}
		if( !(Access.sin_addr.s_addr) ){
			cout << "ERROR : TIMEOUT? ( Access )" << endl;
			return 1;
		}
		if( GetEsc() ) return 1;
	}
	
	return 0;
}


