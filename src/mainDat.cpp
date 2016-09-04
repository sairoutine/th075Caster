#include "mainDatClass.h"
#include <shlwapi.h>
using namespace std;

int mainDatClass::mainDatInit(int argc, char** argv){
	cout << "Command version : " << cmd_version << endl;
	startTime = time( NULL );
	
	//nowDir
	memset( nowDir, 0, sizeof(nowDir) );
	strcpy( nowDir, "fail\0" );
	{
		char dir[200];
		
		if( GetModuleFileName( NULL, dir, 200 ) ){
			if( PathRemoveFileSpec( dir ) ){
				dir[199] = 0;
				strcpy( nowDir, dir );
				
				//作業フォルダ設定
				SetCurrentDirectory( nowDir );
			}
		}
		
		dir[199] = 0;
	}
	
	//iniPath
	strcpy( iniPath, "fail\0" );
	{
		char dir[200];
		if( strcmp( nowDir, "fail" ) && strlen( nowDir ) < 180 ){
			//ok
			strcpy( iniPath ,nowDir );
			strcat( iniPath, "\\config_caster.ini\0" );
		}
	}
	
	//arg
	mainFlg = main_default;
	memset( &argData, 0, sizeof( argData ) );
	strcpy( argData.targetIP, "default\0" );
	{
		int Counter;
		char* arg;
		
		for(Counter = 0; Counter<argc; Counter++){
			arg = argv[Counter];
			if( *arg=='-' ){
				arg++;
				switch( *arg ){
				case 'f':
					if( strcmp( iniPath, "fail" ) ){
						if( GetPrivateProfileInt( "MAIN", "allowFileMode", 0, iniPath ) ){;
							if( mainFlg == main_default ){
								mainFlg = main_file;
							}
						}
					}
					break;
				case 'i':
					if( strcmp( iniPath, "fail" ) ){
						if( GetPrivateProfileInt( "MAIN", "allowArgMode", 1, iniPath ) ){;
							
							mainFlg = main_arg;
							argData.argMode = 0;
							
							//IP
							strncpy( argData.targetIP, arg + 1, sizeof( argData.targetIP ) );
						}
					}else{
						mainFlg = main_arg;
						argData.argMode = 0;
						
						//IP
						strncpy( argData.targetIP, arg + 1, sizeof( argData.targetIP ) );
					}
					break;
				case 'p':
					//port
					argData.targetPort = (WORD)atoi( arg + 1 );
					break;
				case 'w' :
					mainFlg = main_arg;
					argData.argMode = 2;
					break;
				}
			}
		}
	}
	if( mainFlg == main_arg ){
		//arg mode
		cout << "debug : arg Mode" << endl;
		
		if( strcmp( argData.targetIP, "default" ) && argData.argMode == 2 ){
			//特定相手待ち
			argData.argMode = 1;
		}
		
	}else if( mainFlg == main_file ){
		//file mode
		cout << "debug : file Mode" << endl;
	}else if( mainFlg != main_file ){
		if( strcmp( iniPath, "fail" ) ){
			if( GetPrivateProfileInt( "MAIN", "fileModeStart", 0, iniPath ) ){
				mainFlg = main_file;
				cout << "debug : file Mode" << endl;
			}
		}
	}
	
	datA.playerSide = 0xA;
	datB.playerSide = 0xB;
	myInfo.terminalMode = 0;
	continueFlg = 1;
	th075Flg = 0;
	roopFlg = 0;
	rockFlg = 0;
	toughModeFlg = 0;
	boosterFlg = 0;
	portSeekFlg = 0;
	newCWflg = 0;
	
	hRecvTh = NULL;
	hSendTh = NULL;
	hTh075Th = NULL;
	hBoosterTh = NULL;
	
	memset( &myInfo, 0, sizeof(myInfo) );
	memset( &enInfo, 0, sizeof(enInfo) );
	memset( &dataInfo, 0, sizeof(dataInfo) );
	
	memset( &Here , 0, sizeof(SOCKADDR_IN) );
	memset( &Away , 0, sizeof(SOCKADDR_IN) );
	memset( &Root , 0, sizeof(SOCKADDR_IN) );
	memset( &Branch , 0, sizeof(SOCKADDR_IN) );
	memset( &subBranch , 0, sizeof(SOCKADDR_IN) );
	memset( &Access , 0, sizeof(SOCKADDR_IN) );
	
	memset( &Leaf[0] , 0, sizeof(SOCKADDR_IN) );
	memset( &Leaf[1] , 0, sizeof(SOCKADDR_IN) );
	memset( &Leaf[2] , 0, sizeof(SOCKADDR_IN) );
	memset( &Leaf[3] , 0, sizeof(SOCKADDR_IN) );
	
	memset( &Ready , 0, sizeof(SOCKADDR_IN) );
	memset( &Standby[0] , 0, sizeof(SOCKADDR_IN) );
	memset( &Standby[1] , 0, sizeof(SOCKADDR_IN) );
	
	ZeroMemory(&pi, sizeof(pi));
	
	//uMsg
	uMsg = RegisterWindowMessage( umsg_string );
	
	
	//read ini
	myPort = 0;
	enPort = 7500;
	beepFlg = 0;
	waveFlg = 1;
	endTimeFlg = 0;
	escSelectFlg = 0;
	lessCastFlg = 0;
	th075QuitFlg = 0;
	zlibFlg = 0;
	perfectFreezeFlg = 0;
	obsCountFlg = 0;
	autoNextOnFlg = 0;
	boosterOnFlg = 0;
	autoWaitOnFlg = 0;
	autoWaitFlg = 0;
	replaySaveFlg = 0;
	playerSideFlg = 0;
	allowObsFlg = 1;
	windowTopFlg = 0;
	deSyncSoundFlg = 0;
	floatControlFlg = 1;
	if( strcmp( iniPath, "fail" ) ){
		int Res;
		
		if( GetPrivateProfileInt( "PORT", "onoff", 0, iniPath ) ){
			//myPort
			Res = GetPrivateProfileInt( "PORT", "myPort", 0, iniPath );
			if( Res ){
				myPort = Res;
			}
			
			//enPort
			Res = GetPrivateProfileInt( "PORT", "enPort", 0, iniPath );
			if( Res ){
				enPort = Res;
			}
		}
		
		//beep
		Res = GetPrivateProfileInt( "SOUND", "beep", 0, iniPath );
		if( Res ){
			beepFlg = 1;
		}
		
		//sound
		Res = GetPrivateProfileInt( "SOUND", "wave", 0, iniPath );
		if( Res ){
			waveFlg = 1;
		}else{
			waveFlg = 0;
		}
		
		//endTime
		if( GetPrivateProfileInt( "MAIN", "time", 0, iniPath ) ){
			endTimeFlg = 1;
		}
		
		//escSelect
		if( GetPrivateProfileInt( "MAIN", "escSelect", 0, iniPath ) ){
			escSelectFlg = 1;
		}
		
		//lessCast
		if( GetPrivateProfileInt( "MAIN", "lessCast", 0, iniPath ) ){
			lessCastFlg = 1;
		}
		
		//th075Quit
		if( GetPrivateProfileInt( "MAIN", "th075Quit", 0, iniPath ) ){
			th075QuitFlg = 1;
		}
		
		//zlib
		if( GetPrivateProfileInt( "MAIN", "zlib", 0, iniPath ) ){
			zlibFlg = 1;
		}
		
		//perfectFreeze
		perfectFreezeFlg = GetPrivateProfileInt( "MAIN", "perfectFreeze", 0, iniPath );
		
		//obsCount
		if( GetPrivateProfileInt( "MAIN", "obsCount", 0, iniPath ) ){
			obsCountFlg = 1;
		}
		
		//autoNextOn
		if( GetPrivateProfileInt( "MAIN", "autoNextOn", 0, iniPath ) ){
			autoNextOnFlg = 1;
		}
		
		//th075booster
		boosterOnFlg = GetPrivateProfileInt( "MAIN", "th075Booster", 0, iniPath );
		
		
		//autoWaitOn
		if( GetPrivateProfileInt( "MAIN", "autoWaitOn", 0, iniPath ) ){
			autoWaitOnFlg = 1;
//			autoWaitFlg = 1;
		}
		
		//replaySave
		if( GetPrivateProfileInt( "MAIN", "replaySave", 0, iniPath ) ){
			replaySaveFlg = 1;
		}
		
		//playerSide
		playerSideFlg = GetPrivateProfileInt( "MAIN", "playerSide", 0, iniPath );
		if( playerSideFlg > 3 ) playerSideFlg = 0;
		
		//allowObs
		if( GetPrivateProfileInt( "MAIN", "allowObs", 1, iniPath ) ){
			allowObsFlg = 1;
		}else{
			allowObsFlg = 0;
		}
		
		//deSyncSound
		deSyncSoundFlg = (WORD)GetPrivateProfileInt( "SOUND", "deSyncSound", 0, iniPath );
		
		//floatControl
		floatControlFlg = (WORD)GetPrivateProfileInt( "MAIN", "floatControl", 1, iniPath );
		
	}
	
	
	
	s = INVALID_SOCKET;
	
	Here.sin_family = AF_INET;
	Here.sin_addr.s_addr = htonl( INADDR_ANY );
	
	
	if( !myPort ){
		WORD Temp;
		myPort = 7500;
		cout	<< endl
			<< "<StartUp>" << endl
			<< "0 : Exit" << endl
			<< "1 : UDP.7500" << endl
			<< "2 : UDP.0" << endl
			<< "3 : Specific Port" << endl
			<< "Input >";
		cin >> Temp;
		if( cin.fail() ){
			cin.clear();
			cin.ignore(1024,'\n');
			Temp = 0;
		}
		switch( Temp ){
		case 0 :
			return 1;
		case 1 :
			break;
		case 2 :
			myPort = 0;
			break;
		case 3 :
			cout << "Input Port Number >";
			cin >> myPort;
			if( cin.fail() ){
				cin.clear();
				cin.ignore(1024,'\n');
				myPort = 7500;
			}
			break;
		default :
			return 1;
		}
	}
	/*
	//myPortはWORD
	if( myPort < 0 || myPort > 0xFFFF ){
		myPort = 7500;
	}
	*/
	
	Here.sin_port = htons( myPort );
	
	
	WORD wVersion = MAKEWORD( 2, 0);
	if( !WSAStartup(wVersion , &wsaData) ){
		if( wVersion == wsaData.wVersion) {
			hPrintMutex = CreateMutex(NULL, FALSE, NULL);
			if( hPrintMutex ){
				hMutex = CreateMutex(NULL, FALSE, NULL);
				if( hMutex ){
					hInputRecvEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
					if( hInputRecvEvent ){
						hSendEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
						if( hSendEvent ){
							hManageTh = (HANDLE)_beginthreadex(NULL, 0, manageThread, this, 0, NULL);
							if( hManageTh ){
								SetThreadPriority( hManageTh, THREAD_PRIORITY_TIME_CRITICAL );
								s = socket(AF_INET , SOCK_DGRAM , 0);
								if( s != INVALID_SOCKET ){
									if( bind( s, (SOCKADDR*)&Here, sizeof(Here)) >= 0 ){
										//要検討
										GetMyPort();
										hSendTh = (HANDLE)_beginthreadex(NULL, 0, sendThread, this, 0, NULL);
										if( hSendTh ){
										 	if( SetThreadPriority( hSendTh, THREAD_PRIORITY_TIME_CRITICAL ) ){
												hRecvTh = (HANDLE)_beginthreadex(NULL, 0, recvThread, this, 0, NULL);
												if( hRecvTh ){
												 	SetThreadPriority( hRecvTh, THREAD_PRIORITY_TIME_CRITICAL );
												 	
												 	hBoosterTh = (HANDLE)_beginthreadex(NULL, 0, boosterThread, this, 0, NULL);
												 	if( hBoosterTh ){
												 		SetThreadPriority( hBoosterTh, THREAD_PRIORITY_LOWEST );
												 	}
												 	
												 	goto noError;
												}
												continueFlg = 0;
												SetEvent(hSendEvent);
												WaitForSingleObject(hSendTh, 1000);
												CloseHandle(hSendTh);
											}
										}
									}else{
										cout << "debug : Bind error." << endl;
										Sleep( 500 );
									}
									closesocket(s);
								}
								continueFlg = 0;
								WaitForSingleObject(hManageTh, 1000);
								CloseHandle(hManageTh);
							}
							CloseHandle( hSendEvent );
						}
						CloseHandle( hInputRecvEvent );
					}
					CloseHandle( hMutex );
				}
				CloseHandle( hPrintMutex );
			}
		}
		WSACleanup();
	}
	return 1;
	
noError :
	return 0;
}
	


int mainDatClass::TerminateTh075(){
	if( hTh075Th ){
		if( WaitForSingleObject( hTh075Th, 0) ){
			HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
			if( hWnd ){
				for(;;){
					hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
					if( !hWnd ){
						if( WaitForSingleObject(hProcess, 10000) ){
							CloseHandle( pi.hThread );
							pi.hThread = NULL;
							
							CloseHandle( pi.hProcess );
							pi.hProcess = NULL;
						}
						break;
					}else{
						PostMessage(hWnd, WM_CLOSE, 0, 0);
						if( !WaitForSingleObject(hProcess, 10000) ) break;
					}
				}
			}else{
				if( WaitForSingleObject(hProcess, 10000) ){
					CloseHandle( pi.hThread );
					pi.hThread = NULL;
					
					CloseHandle( pi.hProcess );
					pi.hProcess = NULL;
				}
			}
			WaitForSingleObject( hTh075Th, 10000);
		}
		CloseHandle( hTh075Th );
		hTh075Th = NULL;
	}
	return 0;
}


int mainDatClass::mainDatEnd(){
	//スレッドを閉じる
	continueFlg = 0;
	
	if( hSendTh ){
		SetEvent(hSendEvent);
		WaitForSingleObject(hSendTh, 100);
		CloseHandle(hSendTh);
		hSendTh = NULL;
	}
	CloseHandle(hSendEvent);
	hSendEvent = NULL;
	
	CloseHandle( hInputRecvEvent );
	hInputRecvEvent = NULL;
	
	if( s != INVALID_SOCKET ) closesocket(s);
	
	if( hRecvTh ){
		DWORD Flg = WAIT_TIMEOUT;
		while(Flg != WAIT_OBJECT_0 && Flg != WAIT_FAILED){
			Flg = WaitForSingleObject(hRecvTh, 100);
		}
		CloseHandle(hRecvTh);
		hRecvTh = NULL;
	}
	
	WSACleanup();
	
	//萃夢想のスレッドが開いていたら閉じる
	TerminateTh075();
	
	
	WaitForSingleObject(hManageTh, 1000);
	CloseHandle(hManageTh);
	hManageTh = NULL;
	
	if( hBoosterTh ){
		WaitForSingleObject(hBoosterTh, 1000);
		CloseHandle(hBoosterTh);
		hBoosterTh = NULL;
	}
	
	CloseHandle( hMutex );
	hMutex = NULL;
	
	CloseHandle( hPrintMutex );
	hPrintMutex = NULL;
	
	return 0;
}

sTaskClass::sTaskClass(){
	//debug
//	cout << "debug : sTask " << hex << (DWORD)&data[0] << " : " << dec << (DWORD)&data[0] << endl;
	
	Flg = 0;
}

int mainDatClass::charInit(){
	datA.hProcess = &hProcess;
	datB.hProcess = &hProcess;
	
	datA.th075Flg = &th075Flg;
	datB.th075Flg = &th075Flg;
	
	if (myInfo.terminalMode == mode_broadcast) {
		if( datA.init() || datB.init() ) return 1;
	} else if (myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug) {
		if( datA.init() ) return 1;
		datB.init();
	} else {
		if ( datA.init() ) {
			if (datA.init_simple_kbd()) return 1;
			cout << "Spectating and player 1 pad not found, using default keyboard inputs." << endl;
		}
		//datB.init();
	}
	
	return 0;
}

int mainDatClass::charEnd(){
	datA.end();
	datB.end();
	return 0;
}
