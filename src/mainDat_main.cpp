#include "mainDatClass.h"
using namespace std;

//要検討
//念のためinputData.init()を余計に行っている

typedef struct{
	WORD mode;
	WORD targetPort;
	WORD enPort;
	char targetIP[50];
	HANDLE hPrintMutex;
	WORD boosterOnFlg;
}menuDataStruct;

void Sound( mainDatClass* dat ){
	//sound
	if( dat->waveFlg ){
		if( strcmp( dat->nowDir, "fail" ) && strlen( dat->nowDir ) < 180 ){
			char path[200];
			strcpy( path ,dat->nowDir );
			strcat( path, "\\sound.wav\0" );
			if( !_access( path, 0 ) ){
				PlaySound( path, NULL, SND_ASYNC );
			}
		}
	}
	
	if( dat->beepFlg ){
		Beep(3000, 500);
	}
}


int menu( menuDataStruct* menuData ){
	if( !menuData ) return 0xF;
	
	memset( menuData->targetIP, 0, sizeof( menuData->targetIP ) );
	
	WaitForSingleObject( menuData->hPrintMutex, INFINITE );
	cout	<< endl
		<< "<Menu>" << endl
		<< "Escape key will take you back to here." << endl
		<< "0 : Exit" << endl
		<< "1 : Wait for access" << endl
		<< "2 : Try access to UDP." << dec << menuData->enPort << endl
		<< "3 : Try access to specific Port" << endl
		<< "4 : Broadcast" << endl
		<< "5 : Get Information" << endl
		<< "6 : Try access ( Tough )" << endl
		<< "7 : Standby" << endl
		<< "8 : Test myPort" << endl;
	switch( menuData->boosterOnFlg ){
	case 0:
		cout << "9 : vs th075Booster" << endl;
		break;
	case 1:
		cout << "9 : vs th075Booster (always on)" << endl;
		break;
	default:
		cout << "9 : debug( 127.0.0.1 )" << endl;
		break;
	}
	cout	<< "Input >";
	ReleaseMutex( menuData->hPrintMutex );
	cin >> setw(1) >> menuData->mode;
	if( cin.fail() ){
		cin.clear();
		cin.ignore(1024,'\n');
		menuData->mode = 0;
		return 0xF;
	}
	
	switch( menuData->mode ){
	case 0 :
		return 0xF;
	case 1 :
		break;
	case 2 :
		menuData->targetPort = menuData->enPort;
		
		WaitForSingleObject( menuData->hPrintMutex, INFINITE );
		cout << "Input Target IP >";
		ReleaseMutex( menuData->hPrintMutex );
		cin >> setw(50) >> menuData->targetIP;
		if( cin.fail() ){
			cin.clear();
			cin.ignore(1024,'\n');
			return 1;
		}
		break;
	case 3 :
	case 5 :
	case 6 :
	case 7 :
	case 8 :
		WaitForSingleObject( menuData->hPrintMutex, INFINITE );
		cout << "Input Target Port >";
		ReleaseMutex( menuData->hPrintMutex );
		cin >> menuData->targetPort;
		if( cin.fail() ){
			cin.clear();
			cin.ignore(1024,'\n');
			return 1;
		}
		if( menuData->targetPort == 0 ) return 1;
		
		//ポート番号の入力で間違ってIPを入力したときの対処
		cin.clear();
		cin.ignore(1024,'\n');
		
		WaitForSingleObject( menuData->hPrintMutex, INFINITE );
		cout << "Input Target IP >";
		ReleaseMutex( menuData->hPrintMutex );
		cin >> setw(50) >> menuData->targetIP;
		if( cin.fail() ){
			cin.clear();
			cin.ignore(1024,'\n');
			return 1;
		}
		break;
	case 4 :
		break;
	case 9 :
		break;
	case 55 :
		menuData->mode = 5;
		menuData->targetPort = menuData->enPort;
		
		WaitForSingleObject( menuData->hPrintMutex, INFINITE );
		cout << "Input Target IP >";
		ReleaseMutex( menuData->hPrintMutex );
		cin >> setw(50) >> menuData->targetIP;
		if( cin.fail() ){
			cin.clear();
			cin.ignore(1024,'\n');
			return 1;
		}
		break;
	default :
		return 0xF;
	}
	
	menuData->targetIP[49] = 0;
	return 0;
}



int mainDatClass::Entrance(){
	#if debug_mode_main
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : Entrance()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	menuDataStruct menuData;
	menuData.hPrintMutex = hPrintMutex;
	menuData.enPort = enPort;
	menuData.boosterOnFlg = boosterOnFlg;
	
	roopFlg = 0;
	delayTime = 0;
	continueFlg = 1;
	
	if( boosterOnFlg == 1 ){
		boosterFlg = 1;
	}else{
		boosterFlg = 0;
	}
	newCWflg = 0;
	
	//SOCADDR_INの初期化
	WaitForSingleObject( hMutex, INFINITE );
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
	ReleaseMutex( hMutex );
	
//	Sleep(100);
	if( inputData.init() ) return 0xF;
	
	
	memset( &myInfo, 0, sizeof(myInfo) );
	memset( &enInfo, 0, sizeof(enInfo) );
	memset( &dataInfo, 0, sizeof(dataInfo) );
	myInfo.terminalMode = mode_default;
	myInitFlg = 0;
	enInitFlg = 0;
	memset( &echoFlg, 0, sizeof(echoFlg) );
	memset( &lastTime, 0, sizeof(lastTime) );
	myInfo.phase = phase_default;
	dataInfo.phase = phase_default;
	myRandNo = 0;
	enRandNo = 0;
	myRand = 0;
	enRand = 0;
	delay = 0;
	toughModeFlg = 0;
	autoNextFlg = 0;
	targetMode = 0;
	
	stdbyPort[0] = 0;
	stdbyPort[1] = 0;
	readyPort = 0;
	playerSideHostFlg = 0;
	portSeekFlg = 0;
	
	memset( obsIP, 0, sizeof( obsIP ) );
	
	
	//srand
//	srand( (unsigned)startTime );
//	srand( (unsigned)time( NULL ) );
	
	//wait
	if( mainFlg == main_default && autoWaitFlg ){
		mainFlg = main_wait;
	}
	
	//main
	switch( mainFlg ){
	case main_default :
		{
			int Res;
			for(;;){
				Res = menu( &menuData );
				if( Res == 0xF ){
					if( endTimeFlg ){
						//稼動時間を表示
						cout	<< endl
							<< "<End>" << endl
							<< "Time : ";
						double allTime = difftime( time( NULL ), startTime );
						if( allTime > 60 * 60 ) cout << (DWORD)(( allTime / 60 ) / 60) << "h ";
						allTime = allTime - (DWORD)((allTime / 60) / 60) * 60 * 60;
						cout << (DWORD)(allTime / 60)  << "min" << endl;
						Sleep(500);
					}
					return 0xF;
				}
				if( Res == 0 ) break;
			}
		}
		break;
	case main_file :
		//動作モードと相手のIPとポートの情報を読む
		{
			char path[200];
			if( GetCurrentDirectory( 180, path ) < 180 ){
				//ok
				path[180] = 0;
				strcat( path, "\\info.ini\0" );
				
				if( GetPrivateProfileInt( "MAIN", "onoff", 0, path ) ){
					//on
					int mode = GetPrivateProfileInt( "MAIN", "mode", 0, path );
					switch( mode ){
					case 0 :
						//通常
						//それ用のモードを追加
						menuData.mode = 20;
						
						//情報読み込み
						GetPrivateProfileString( "TARGET", "ip", '\0', menuData.targetIP, 49, path );
						menuData.targetIP[49] = '\0';
						menuData.targetPort = GetPrivateProfileInt( "TARGET", "port", 0, path );
						
						if( menuData.targetIP[0] != '\0' && menuData.targetPort != 0 ){
							//ok
						}else{
							return 0xF;
						}
						break;
					case 1 :
						//特定相手待ち
						menuData.mode = 21;
						GetPrivateProfileString( "TARGET", "ip", '\0', menuData.targetIP, 49, path );
						menuData.targetIP[49] = '\0';
						
						if( menuData.targetIP[0] != '\0' ){
							//ok
						}else{
							return 0xF;
						}
						cout	<<endl
							<< "<Menu>" << endl;
						break;
					case 2 :
						//待ち
						menuData.mode = 1;
						cout	<<endl
							<< "<Menu>" << endl;
						break;
					default :
						return 0xF;
					}
				}else{
					return 0xF;
				}
			}else{
				return 0xF;
			}
		}
		
		mainFlg = main_end;
		break;
	case main_arg :
		//引数指定
		//fileモードのを流用
		switch( argData.argMode ){
		case 0 :
			menuData.mode = 20;
			
			//IP
			strcpy( menuData.targetIP, argData.targetIP );
			menuData.targetIP[49] = '\0';
			
			//port
			if( argData.targetPort ){
				menuData.targetPort = argData.targetPort;
			}else{
				menuData.targetPort = 7500;
			}
			
			if( menuData.targetIP[0] != '\0' ){
				//ok
			}else{
				return 0xF;
			}
			
			break;
		case 1 :
			//特定相手待ち
			menuData.mode = 21;
			strcpy( menuData.targetIP, argData.targetIP );
			menuData.targetIP[49] = '\0';
			
			if( menuData.targetIP[0] != '\0' ){
				//ok
			}else{
				return 0xF;
			}
			cout	<<endl
				<< "<Menu>" << endl;
			break;
		case 2 :
			//待ち
			menuData.mode = 1;
			cout	<<endl
				<< "<Menu>" << endl;
			break;
		default :
			return 0xF;
		}
		
		mainFlg = main_end;
		break;
	case main_end :
		//終了を待つ
		for(;;){
			if( GetEsc() ) break;
			if( !FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" ) ) break;
			Sleep(200);
		}
		return 0xF;
	case main_wait :
		cout	<<endl
			<< "<Menu>" << endl;
		menuData.mode = 1;
		break;
	default :
		return 0xF;
	}
	
	nowTime = timeGetTime();
	
	myInfo.terminalMode = mode_access;
	switch( menuData.mode ){
	case 1 :
		//Wait for access
		cout << "Now Waiting for access." << endl;
		
		if( autoWaitOnFlg ){
			autoWaitFlg = 1;
		}
		
		myInfo.terminalMode = mode_wait;
		accessFlg = status_default;
		for(;;){
			if( accessFlg == status_ok ) break;
			if( accessFlg == status_error ) return 1;
			if( accessFlg == status_bad ) return 1;
			if( GetEsc() ){
				autoWaitFlg = 0;
				if( mainFlg == main_wait ){
					mainFlg = main_default;
				}
				
				return 1;
			}
			Sleep(100);
		}
		cout << "Access from " << inet_ntoa( Away.sin_addr ) << endl;
		myInfo.terminalMode = mode_root;
		
		Sound( this );
		
		//playerSideHost
		playerSideHostFlg = 1;
		
		break;
	case 6 :
		//Tough
		toughModeFlg = 1;
	case 2 :
	case 3 :
		//Try access
		
		//念のため（要検討）　
		lastTime.Access = nowTime;
		
		Access.sin_family = AF_INET;
		Access.sin_addr.s_addr = inet_addr( menuData.targetIP );
		if( !Access.sin_addr.s_addr || Access.sin_addr.s_addr == 0xFFFFFFFF ){
			return 1;
		}
		
		Access.sin_port = htons( menuData.targetPort );
		
		lastTime.Access = nowTime;
		echoFlg.Access = 0;
		for(;;){
			SendCmd( dest_access, cmd_echo );
			Sleep(100);
			if( echoFlg.Access ) break;
			if( !(Access.sin_addr.s_addr) ) return 1;
			if( GetEsc() ) return 1;
		}
		break;
	case 4 :
		//Broadcast
		myInfo.terminalMode = mode_broadcast;
		break;
	case 5 :
		//Get Information
		//念のため（要検討）　
		lastTime.Access = nowTime;
		
		Access.sin_family = AF_INET;
		Access.sin_addr.s_addr = inet_addr( menuData.targetIP );
		if( !Access.sin_addr.s_addr || Access.sin_addr.s_addr == 0xFFFFFFFF ){
			return 1;
		}
		
		Access.sin_port = htons( menuData.targetPort );
		
		lastTime.Access = nowTime;
		
		{
			dataInfo.phase = 0xFF;
			for(;;){
				SendCmd( dest_access, cmd_dataInfo );
				Sleep(100);
				if( dataInfo.phase != 0xFF ) break;
				if( !(Access.sin_addr.s_addr) ){
					return 1;
				}
				if( GetEsc() ) return 1;
			}
			
			float delayTemp = 0;
			if( GetDelay( dest_access, &delayTemp) ) return 1;
			
			
			cout.setf(ios::fixed, ios::floatfield);
			cout	<< endl
				<< "<Information>" << endl
				<< "Delay : About " << setprecision( 1 ) << delayTemp << "[ms]" << endl;
			
			switch( dataInfo.phase ){
			case phase_read :
			case phase_battle :
				cout << "Phase : Battle" << endl;
				{
					BYTE ID;
					WORD printFlg = 0;
					for(;;){
						if( printFlg == 0){
							ID = dataInfo.A.ID;
							printFlg = 1;
						}else{
							ID = dataInfo.B.ID;
							printFlg = 2;
						}
						switch( ID ){
						case 0 : cout << "Reimu";	break;
						case 1 : cout << "Marisa";	break;
						case 2 : cout << "Sakuya";	break;
						case 3 : cout << "Alice";	break;
						case 4 : cout << "Patchouli";	break;
						case 5 : cout << "Youmu";	break;
						case 6 : cout << "Remilia";	break;
						case 7 : cout << "Yuyuko";	break;
						case 8 : cout << "Yukari";	break;
						case 9 : cout << "Suika";	break;
						case 10 : cout << "Meiling";	break;
						}
						if( printFlg == 1 ) cout << " vs ";
						if( printFlg > 1 ) break;
					}
				}
				cout << " ( " << dataInfo.gameTime / 2 / 60 << "sec )" << endl;
				break;
			case phase_menu :
				cout << "Phase : Menu" << endl;
				break;
			case phase_default :
				if( targetMode == mode_wait ){
					cout << "Phase : Default ( Wait )" << endl;
				}else{
					cout << "Phase : Default" << endl;
				}
				break;
			default : 
				cout << "Phase : Default?" << endl;
				break;
			}
		}
		return 1;
	case 7 :
		//Standby
		lastTime.Access = nowTime;
		
		Access.sin_family = AF_INET;
		Access.sin_addr.s_addr = inet_addr( menuData.targetIP );
		if( !Access.sin_addr.s_addr || Access.sin_addr.s_addr == 0xFFFFFFFF ){
			return 1;
		}
		
		Access.sin_port = htons( menuData.targetPort );
		
		lastTime.Access = nowTime;
		
		cout << "Now Waiting for introduction." << endl;
		
		{
			DWORD roopCounter = 100;
			for(;;){
				if( roopCounter > 30 ){
					SendCmd( dest_access, cmd_standby, &myPort, 2 );
					roopCounter = 0;
				}
				roopCounter++;
				Sleep(10);
				if( Ready.sin_addr.s_addr ) break;
				if( !(Access.sin_addr.s_addr) ) return 1;
				if( GetEsc() ) return 1;
			}
		}
		
		lastTime.Access = nowTime;
		WaitForSingleObject( hMutex, INFINITE );
		Access = Ready;
		ReleaseMutex( hMutex );
		portSeekFlg = 1;
		
		//転送後
		cout << "Now Waiting for response." << endl;
		echoFlg.Access = 0;
		for(;;){
			{
				SendCmd( dest_access, cmd_echo );
				
				SOCKADDR_IN AccessTemp = Access;
				AccessTemp.sin_port = htons( readyPort );
				SendCmd( &AccessTemp, cmd_echoes, &myPort, 2 );
			}
			
			Sleep(100);
			if( echoFlg.Access ) break;
			if( !(Access.sin_addr.s_addr) ) return 1;
			if( GetEsc() ) return 1;
		}
		portSeekFlg = 0;
		
		
		Sound( this );
		
		//echoesを待つ
		Sleep(200);
		
		break;
	case 8 :
		//Receive Test signal
		{
			SOCKADDR_IN addrTemp;
			memset( &addrTemp, 0, sizeof( addrTemp ) );
			
			addrTemp.sin_family = AF_INET;
			addrTemp.sin_addr.s_addr = inet_addr( menuData.targetIP );
			if( !addrTemp.sin_addr.s_addr || addrTemp.sin_addr.s_addr == 0xFFFFFFFF ){
				return 1;
			}
			addrTemp.sin_port = htons( menuData.targetPort );
			
			
			int Res = TestPort( &addrTemp );
			
			cout	<< endl
				<< "<Report>" << endl;
			switch( Res ){
			case status_ok :
				cout << "Port : OK ( UDP." << myPort << " received signal )" << endl;
				break;
			case status_bad :
				cout << "Port : BAD ( UDP." << myPort << " received No signal )" << endl;
				break;
			case status_error :
				cout << "Port : ERROR or ESC" << endl;
				break;
			default :
				return 1;
			}
		}
		return 1;
	case 9 :
		//Debug ( 127.0.0.1 )
		WaitForSingleObject( hMutex, INFINITE );
		memset( &Away, 0, sizeof(Away));
		Away.sin_family = AF_INET;
		Away.sin_addr.s_addr = inet_addr( "127.0.0.1" );
		Away.sin_port = htons( myPort );
		ReleaseMutex( hMutex );
		
		myInfo.terminalMode = mode_debug;
		if( boosterOnFlg == 0xF ){
			boosterFlg = 0;
			newCWflg = 1;
		}else{
			boosterFlg = 1;
		}
		
		//sound
//		Sound( this );
		
		break;
	case 20 :
		//Try access
		
		/*
		//要検討
		//相手の準備を待つ時間（recvfromでのエラーなど）
		//後で
		if( argData.argMode ){
			//none
		}else{
			Sleep(1000);
		}
		*/
		
		//念のため（要検討）　
		lastTime.Access = nowTime;
		
		Access.sin_family = AF_INET;
		Access.sin_addr.s_addr = inet_addr( menuData.targetIP );
		if( !Access.sin_addr.s_addr || Access.sin_addr.s_addr == 0xFFFFFFFF ){
			return 1;
		}
		
		Access.sin_port = htons( menuData.targetPort );
		
		lastTime.Access = nowTime;
		echoFlg.Access = 0;
		for(;;){
			SendCmd( dest_access, cmd_echo );
			Sleep(100);
			if( echoFlg.Access ) break;
			if( !(Access.sin_addr.s_addr) ) return 1;
			if( GetEsc() ) return 1;
		}
		
//		Sound( this );
		break;
	case 21 :
		//Wait for access
		cout << "Now Waiting for access. " << endl;
		waitTargetIP = inet_addr( menuData.targetIP );
		if( !waitTargetIP || waitTargetIP == 0xFFFFFFFF ){
			cout << "debug : IP conv fail" << endl;
			Sleep(1000);
			return 0xF;
		}
		
		myInfo.terminalMode = mode_wait_target;
		accessFlg = status_default;
		for(;;){
			if( accessFlg == status_ok ) break;
			if( accessFlg == status_error ) return 1;
			if( accessFlg == status_bad ) return 1;
			if( GetEsc() ) return 1;
			Sleep(100);
		}
//		cout << "Access from " << inet_ntoa( Away.sin_addr ) << endl;
		myInfo.terminalMode = mode_root;
		
		//playerSideHost
		playerSideHostFlg = 1;
		
		Sound( this );
		break;
	default :
		return 1;
	}
	
	if( myInfo.terminalMode == mode_access ){
		//mode_root or mode_branch or mode_subbranch
		if( Communicate() ) return 1;
	}
	
	//test
	dataInfo.terminalMode = myInfo.terminalMode;
	
	
	if( myInfo.terminalMode != mode_debug ){
		if( boosterOnFlg == 1 ){
			boosterFlg = 1;
		}
	}
	
	
	if( myInfo.terminalMode == mode_debug ){
		myInfo.playerSide = 0xA;
		if( boosterFlg ){
			int delayTemp;
			cout << "Input delay 0-5 >";
			cin >> delayTemp;
			if( cin.fail() ){
				cin.clear();
				cin.ignore(1024,'\n');
				return 1;
			}
			if( delayTemp > 5 ) delayTemp = 5;
			delayTime = delayTemp * 2;
		}else{
			delayTime = 10;
		}
		myInfo.sessionNo = 1;
		myInfo.sessionID = 1 + rand()%255;
		myInfo.sessionIDNext = myInfo.sessionID;
		
		dataInfo.sessionNo = myInfo.sessionNo;
		dataInfo.sessionID = myInfo.sessionID;
		dataInfo.sessionIDNext = myInfo.sessionIDNext;
	}else if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
		//対戦キャラクター、gameTIme表示など
		//観戦するかどうか
		for(;;){
			SendCmd( dest_root, cmd_dataInfo );
			Sleep(100);
			if( dataInfo.sessionID ) break;
			if( !(Root.sin_addr.s_addr) ){
				cout << "ERROR : TIMEOUT ( Root )" << endl;
				return 1;
			}
			if( GetEsc() ) return 1;
		}
		
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout	<< endl
			<< "<Target Condition>" << endl;
		
		//test
		switch( dataInfo.terminalMode ){
		case mode_root :
			cout << "Mode : Root" << endl;
			break;
		case mode_broadcast :
			cout << "Mode : Broadcast" << endl;
			break;
		case mode_branch :
			cout << "Mode : Branch" << endl;
			break;
		case mode_subbranch :
			cout << "Mode : subBranch" << endl;
			break;
		}
		
		switch( dataInfo.phase ){
		case phase_read :
		case phase_battle :
			{
				BYTE ID;
				WORD printFlg = 0;
				for(;;){
					if( printFlg == 0){
						ID = dataInfo.A.ID;
						printFlg = 1;
					}else{
						ID = dataInfo.B.ID;
						printFlg = 2;
					}
					switch( ID ){
					case 0 : cout << "Reimu";	break;
					case 1 : cout << "Marisa";	break;
					case 2 : cout << "Sakuya";	break;
					case 3 : cout << "Alice";	break;
					case 4 : cout << "Patchouli";	break;
					case 5 : cout << "Youmu";	break;
					case 6 : cout << "Remilia";	break;
					case 7 : cout << "Yuyuko";	break;
					case 8 : cout << "Yukari";	break;
					case 9 : cout << "Suika";	break;
					case 10 : cout << "Meiling";	break;
					}
					if( printFlg == 1 ) cout << " vs ";
					if( printFlg > 1 ) break;
				}
			}
			cout << " ( " << dataInfo.gameTime / 2 / 60 << "sec )" << endl;
			break;
		case phase_default :
		case phase_menu :
		default : 
			cout << "Target is in Preparation." << endl;
			break;
		}
		
		WORD Temp;
		cout	<< "Decide within " << timeout_time / 1000 << "sec." << endl
			<< "0 : Stop" << endl
			<< "1 : Continue" << endl
			<< "Input >";
		ReleaseMutex( hPrintMutex );
		
		
		cin >> setw(1) >> Temp;
		if( cin.fail() ){
			cin.clear();
			cin.ignore(1024,'\n');
			return 1;
		}
		if( !(Root.sin_addr.s_addr) ){
			cout << "ERROR : TIMEOUT ( Root )" << endl;
			return 1;
		}
		
		
		switch( Temp ){
		case 0 :
			return 1;
		case 1 :
			//続行
			break;
		default :
			return 1;
		}
		joinRes = status_default;
		while( joinRes == status_default ){
			SendCmd( dest_root, cmd_join, &myPort, 2 );
			Sleep(100);
			if( !(Root.sin_addr.s_addr) ){
				cout << "ERROR : TIMEOUT ( Root )" << endl;
				return 1;
			}
			if( GetEsc() ) return 1;
		}
		if( joinRes == status_ok ){
			cout << "Status : OK" << endl;
		}else if( joinRes == status_bad ){
			cout << "Status : BAD" << endl;
			return 1;
		}
	}else if( myInfo.terminalMode == mode_broadcast ){
		//セッションID導入
		myInfo.sessionNo = 1;
		myInfo.sessionID = 1 + rand()%255;
		myInfo.sessionIDNext = myInfo.sessionID;
		
		dataInfo.sessionNo = myInfo.sessionNo;
		dataInfo.sessionID = myInfo.sessionID;
		dataInfo.sessionIDNext = myInfo.sessionIDNext;
		
		myInfo.playerSide = 0;
		enInfo.playerSide = 0;
		
		{
			WORD delayTimeTemp = 0;
			cout	<< endl
				<< "<delay>" << endl;
			
			//sideA
			cout << "Input delay A >";
			cin >> setw(1) >> delayTimeTemp;
			if( cin.fail() ){
				cin.clear();
				cin.ignore(1024,'\n');
				return 1;
			}
			if( delayTimeTemp > 10 ) delayTimeTemp = 10;
			delayTimeA = delayTimeTemp * 2;
			
			//sideB
			cout << "Input delay B >";
			cin >> setw(1) >> delayTimeTemp;
			if( cin.fail() ){
				cin.clear();
				cin.ignore(1024,'\n');
				return 1;
			}
			if( delayTimeTemp > 10 ) delayTimeTemp = 10;
			delayTimeB = delayTimeTemp * 2;
		}
	}else if( myInfo.terminalMode == mode_root ){
		
		//データをまとめておいたほうが楽
		//後ほど
		myInitFlg = 1;
		for(;;){
			if( enInitFlg ) break;
			SendCmd( dest_away, cmd_initflg );
			if( !(Away.sin_addr.s_addr) ) return 1;
			if( GetEsc() ) return 1;
			Sleep(100);
		}
		
		//通信してABを決める
		if( GetPlayerSide() ) return 1;
		
		if( myInfo.playerSide == 0xA ){
			DWORD delayTimeTemp;
			
			for(;;){
				if( GetDelay( dest_away, &delay ) ) return 1;
				cout.setf(ios::fixed, ios::floatfield);
				cout	<< endl
					<< "<Delay>" << endl
					<< "About " << setprecision( 1 ) << delay << "[ms] delay exist in a round." << endl
					<< "( DelayTime[ms] = 16[ms] x Input )" << endl
					<< "Input >";
				cin >> delayTimeTemp;
				if( cin.fail() ){
					cin.clear();
					cin.ignore(1024,'\n');
					return 1;
				}
				
				if( delayTimeTemp > 10 ) delayTimeTemp = 10;
//				if( delayTimeTemp == 0 ) delayTimeTemp = 1;
				if( delayTimeTemp == 1 ){
					/*
					if( delay > 4 ){
						delayTimeTemp = 2;
						cout << endl;
						cout << "Now 'Buffer Margin 1' is limited." << endl;
						cout << "The value is changed to 2." << endl;
						cout << "( 16ms Roop causes lag )" << endl;
					}
					*/
				}
				if( delayTimeTemp ) break;
			}
			delayTime = delayTimeTemp * 2;
		}else{
			cout	<< endl
				<< "<Delay>" << endl
				<< "Now waiting for buffer margin value." << endl;
			for(;;){
				SendCmd( dest_away, cmd_delay );
				if( delayTime ) break;
				if( GetEsc() ) return 1;
				Sleep(100);
			}
			cout.setf(ios::fixed, ios::floatfield);
			cout << "Buffer margin : " << delayTime / 2 << " ( Observed about " << setprecision( 1 ) << delay << "[ms] delay )" << endl;
		}
		
		if( myInfo.playerSide == 0xA ){
			//セッションID（ランダム）を導入
			myInfo.sessionNo = 1;
			myInfo.sessionID = 1 + rand()%255;
			myInfo.sessionIDNext = myInfo.sessionID;
		}else{
			for(;;){
				SendCmd( dest_away, cmd_session );
				if( enInfo.sessionID ) break;
				if( !(Away.sin_addr.s_addr) ) return 1;
				if( GetEsc() ) return 1;
				Sleep(100);
			}
			myInfo.sessionNo = enInfo.sessionNo;
			myInfo.sessionID = enInfo.sessionID;
			myInfo.sessionIDNext = enInfo.sessionIDNext;
		}
		
		myInitFlg = 2;
		for(;;){
			if( enInitFlg == 2) break;
			SendCmd( dest_away, cmd_initflg );
			if( !(Away.sin_addr.s_addr) ) return 1;
			if( GetEsc() ) return 1;
			Sleep(100);
		}
	}else{
		return 1;
	}
	
	if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
		if( autoNextOnFlg ){
			autoNextFlg = 1;
			cout << "debug : AutoNext ON" << endl;
		}
	}
	if( newCWflg ){
		cout << "debug : float CW set ON" << endl;
	}else{
		cout << "debug : float CW set OFF" << endl;
	}
	
	
	
	//念のため
	if( inputData.init() ) return 0xF;
	
	
	
	if( hTh075Th ){
		if( !WaitForSingleObject( hTh075Th, 0 ) ){
			CloseHandle( hTh075Th );
			hTh075Th = NULL;
			
			HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
			if( hWnd ){
				PostMessage(hWnd, WM_CLOSE, 0, 0);
				Sleep(3000);
			}
			//萃夢想のスレッドを開始する
			hCheckEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
			hProcess = NULL;
			hTh075Th = (HANDLE)_beginthreadex(NULL, 0, th075Thread, this, 0, NULL);
			if( !hTh075Th ) return 1;
			
			WaitForSingleObject(hCheckEvent, 5000);
			CloseHandle( hCheckEvent );
			hCheckEvent = NULL;
			
			if( !hProcess ){
				CloseHandle( hTh075Th );
				hTh075Th = NULL;
				return 1;
			}
		}else{
			if( myInfo.terminalMode == mode_debug ){
				cout << "debug : th075thread already running" << endl;
			}
			
			HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
			if( !hWnd ){
				//終わりかけ
				if( WaitForSingleObject(hTh075Th, 3000) ){
					CloseHandle( pi.hThread );
					CloseHandle( pi.hProcess );
				}
				CloseHandle( hTh075Th );
				hTh075Th = NULL;
				cout << "debug : th075thread restart" << endl;
				
				hCheckEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
				hProcess = NULL;
				hTh075Th = (HANDLE)_beginthreadex(NULL, 0, th075Thread, this, 0, NULL);
				if( !hTh075Th ) return 1;
				
				WaitForSingleObject(hCheckEvent, 5000);
				CloseHandle( hCheckEvent );
				hCheckEvent = NULL;
				
				if( !hProcess ){
					CloseHandle( hTh075Th );
					hTh075Th = NULL;
					return 1;
				}
			}
		}
	}else{
		HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
		if( hWnd ){
			DWORD PID;
			HANDLE hProcessTemp;
			GetWindowThreadProcessId( hWnd , &PID );
			hProcessTemp  = OpenProcess( PROCESS_ALL_ACCESS, FALSE, PID );
			
			PostMessage(hWnd, WM_CLOSE, 0, 0);
			WaitForSingleObject( hProcessTemp, 3000 );
			CloseHandle( hProcessTemp );
		}
		//萃夢想のスレッドを開始する
		hCheckEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		hProcess = NULL;
		hTh075Th = (HANDLE)_beginthreadex(NULL, 0, th075Thread, this, 0, NULL);
		if( !hTh075Th ) return 1;
		
		WaitForSingleObject(hCheckEvent, 5000);
		CloseHandle( hCheckEvent );
		hCheckEvent = NULL;
		
		if( !hProcess ){
			CloseHandle( hTh075Th );
			hTh075Th = NULL;
			return 1;
		}
	}
	
	
	return 0;
}

//バックグラウンド
int mainDatClass::backGroundRoop(){
	#if debug_mode
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : backGroundRoop()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	
	//萃夢想のウィンドウを待つ
	for(;;){
		if( GetEsc() ){
			return 1;
		}
		if( FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" ) ) break;
		Sleep(200);
	}
	
	//top
	if( windowTopFlg ){
		HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
		if( hWnd ){
			SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		}
	}
	
	//windowSize
	if( windowResizeFlg == 2 ){
		HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
		if( hWnd ){
			/*
			char path[200];
			strcpy( path ,nowDir );
			strcat( path, "\\d3d8.dll\0" );
			if( !_access( path, 0 ) ){
				SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, windowWidth, windowHeight, SWP_NOMOVE);
			}
			*/
			SetWindowPos(hWnd, NULL, 0, 0, windowWidth, windowHeight, SWP_NOMOVE);
		}
	}
	
	roopFlg = 1;
	for(;;){
		if( GetEsc() ) break;
		if( !FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" ) ) break;
		if( !roopFlg ) break;
		Sleep(200);
	}
	roopFlg = 0;
	return 0;
}


