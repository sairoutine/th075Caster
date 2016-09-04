#include "conf.h"
#include "boosterDatClass.h"

//OpenProcessできるか、baseはあるか,名前は取得できたか、
//AIオープンできたか、キャラ継続か、変更してたときの処理 完了したら0を返す
//boosterDatInitFlgはRefleshDat()で1にする
//playerSideを変えたらbodyIniFlgを1にすること
int boosterDatClass::boosterDatInit(){
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".boosterDatInit() " << endl;
	#endif
	
	hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
	if(hWnd == 0) return 0xF;
	
	//メモリを確保できなかったとき
	if(AI == 0) return 0xF;
	
	//ボタン入力初期化
	if(bodyIniFlg==0){
		for(Counter=1;Counter<9;Counter++){
			if(gameInfoIni[Counter][5]){	//押されている
				Input.type = INPUT_KEYBOARD;
				Input.ki.wVk = gameInfoIni[Counter][2];
				Input.ki.wScan = MapVirtualKey(gameInfoIni[Counter][2], 0);
				Input.ki.dwFlags = KEYEVENTF_KEYUP;
				Input.ki.dwExtraInfo = 0;
				Input.ki.time = 0;
				
				SendInput(1, &Input, sizeof(INPUT));
				gameInfoIni[Counter][4]=0;
				gameInfoIni[Counter][5]=0;
			}
		}
	}
	
	baseAddress = 0x671418;	//baseAddress
	gameMode = 0;
	
	
	GetWindowThreadProcessId( hWnd , &PID );
	if( PID == 0 ) return 1;
	
	hProcess  = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
	if(playerSide == 0xA){
		ReadProcessMemory( hProcess , (void *)(baseAddress + 0x00) , &myBase , 4, NULL );
		myIniOffset = 0x28;
	}
	if(playerSide == 0xB){
		ReadProcessMemory( hProcess , (void *)(baseAddress + 0x04) , &myBase , 4, NULL );
		myIniOffset = 0x50;
	}
	
	if(myBase==0){
		CloseHandle( hProcess );
		return 1;
	}
	ReadProcessMemory( hProcess , (void *)myBase , &myName , 4, NULL );
	ReadProcessMemory( hProcess , (void *)(baseAddress + 0x218) , &gameMode , 1, NULL );
	CloseHandle( hProcess );
	
	if(gameMode ==5){
		gameTimeAddress = baseAddress + 0x498;		//gameTimeAddress	//リプレイモードのときの時間
	}else{
		gameTimeAddress = baseAddress + 0x49C;		//gameTimeAddress
	}
	
	AIMode = 0;
	if(gameMode==0) AIMode = 0;					//story	//stop			//AIModeの設定
	if(gameMode==1) AIMode = 0;					//arcade	//stop
	if(gameMode==2){
		if( listeningMode ){
			AIMode = 1;						//human	//listeningMode
		}else{
			if( playerSide == 0xB ) AIMode = 2;		//human	//active
			if( playerSide == 0xA ) AIMode = 1;		//human	//listening
		}
	}
	if(gameMode==3) AIMode = 0;					//cpu		//stop
	if(gameMode==4) AIMode = 0;					//practice	//stop
//	if(gameMode==4 && playerSide == 0xB) AIMode = 0;	//practice	//stop
//	if(gameMode==4 && playerSide == 0xA) AIMode = 1;	//practice	//listening
	if(gameMode==5) AIMode = 1;					//replay	//listening
	
	Flg = GetName();
	if(Flg) return 1;
	
	if(boosterDatInitFlg){
		if(strcmp(NameTemp, Name) !=0){
			if( strcmp(NameTemp, "init") && strcmp(NameTemp, "error") && strcmp(NameTemp, "second")){	//init,error,secondのとき保存しない
				if( CloseAI()|| CloseSpellAI() || CloseIndividualAI() || CloseBackAI() ) return 1;
				
				if( eigenValueLocal[0][0] ){
					if( CloseLocalAI() ) return 1;
				}
			}
		}
	}
	
	if(boosterDatInitFlg){
		boosterDatInitFlg = 0;
		return 1;
	}
	if( enDat->boosterDatInitFlg == 1 ) return 1;		//相手が終了していないと進まない
	strcpy( enName, enDat->Name );
	
	//同じキャラ同士なときはAI[]を共有する。→自分のAI[]を持たない:secondaryMode
	if(myID == enDat->myID){				//キャラIDを参照	//同じキャラ同士の対戦
		if( strcmp(NameTemp,"second") == 0 ){
			secondaryModeFlg = 1;			//前回secondaryModeだったら継続
		}else{
			if( enDat->secondaryModeFlg == 0 ){			//テキがsecodaryModeではなかったとき
				if( strcmp(Name,NameTemp) == 0 ){
					secondaryModeFlg = 0;	//自分側がキャラ継続しているときは相手側がsecondaryModeになることを要求
				}else{
					secondaryModeFlg = 1;
				}
			}else{
				secondaryModeFlg = 0;
			}
		}
		if( !secondaryModeFlg && !enDat->secondaryModeFlg ){
			return 1;
		}
	}else{
		secondaryModeFlg = 0;				//同じキャラ同士の対戦ではない
	}
	
	#if debug_mode
		if(secondaryModeFlg) cout << "debug : " << hex << playerSide << ".secondaryMode" << endl;
	#endif
	
	
	//ファイルを開く
	if(secondaryModeFlg){
		strcpy(Name,"second\0");
	}else{
		if( !( !strcmp(NameTemp, "init") || !strcmp(NameTemp, "error") || !strcmp(NameTemp, "second") ) && !strcmp(NameTemp, Name) ){
			//前回と同じキャラ
			if(strcmp(enNameTemp, enName) !=0){
				if( CloseIndividualAI() ) return 1;
				
				Flg = OpenIndividualAI(Name,enName);
				if( Flg == 0xF ) return 0xF;
				if( Flg ) memset(AI + AIsizeArray[0] + AIsizeArray[1], 0, AIsizeArray[2]);
				
			}
		}else{
			//前回がinit,error,secondのとき or 前回と違うキャラ
			Flg = OpenAI(Name);
			if( Flg == 0xF ) return 0xF;
			if( Flg ) memset(AI, 0, AIsizeArray[0]);
			
			Flg = OpenSpellAI(Name);
			if( Flg == 0xF ) return 0xF;
			if( Flg ) memset(AI + AIsizeArray[0], 0, AIsizeArray[1]);
			
			Flg = OpenIndividualAI(Name,enName);
			if( Flg == 0xF ) return 0xF;
			if( Flg ) memset(AI + AIsizeArray[0] + AIsizeArray[1], 0, AIsizeArray[2]);
			
			
			if(eigenValueLocal[0][0]){
				Flg = OpenLocalAI(Name);
				if( Flg == 0xF ) return 0xF;
				if( Flg ) memset(AI + AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2], 0, AIsizeArray[3]);
				
			}
			
			Flg = OpenBackAI(Name);
			if( Flg == 0xF ) return 0xF;
			if( Flg ) memset(AI + AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3], 0, AIsizeArray[4]);
			
		}
	}
	
	strcpy(enName,enDat->Name);
	strcpy(NameTemp,Name);
	strcpy(enNameTemp,enName);
	
	//クラスの在り方を設定するものはここまで
	//初期化・計算はここから
	
	CalcAddress();
	Flg = ConvertIni();	//必ずCalcAddress()の後で行うこと
	if(Flg) return 1;
	statusInit();
	
	intervalFlg = 0;
	gameTime = 0;
	gameTimeTemp = 0;
	gameTimeTemp2 = 0;
	inputTime = 0;
	inputResetTime = 0;
	decodeTime = 0;
	statusID = 0;
	
	memset( myGameInfo, 0, sizeof( myGameInfo ) );
	memset( commandInput, 0, sizeof( commandInput ) );
	for(Counter=0; Counter<10; Counter++){
		gameInfoIni[Counter][4] = 0;
	}
	memset( LNAIBuf, 0, sizeof( LNAIBuf ) );
	memset( SNAIBuf, 0, sizeof( SNAIBuf ) );
	memset( SWAIBuf, 0, sizeof( SWAIBuf ) );
	memset( SpellAIBuf, 0, sizeof( SpellAIBuf ) );
	memset( RecoverAIBuf, 0, sizeof( RecoverAIBuf ) );
	memset( BackAIBuf, 0, sizeof( BackAIBuf ) );
	
	if( secondaryModeFlg ){
		AIbase = (BYTE*)(enDat->AI);
	}else{
		AIbase = (BYTE*)AI;
	}
	
	//baseAi
	SNAIbase	= AIbase;
	SWAIbase	= AIbase + eigenValueSN[0][1];
	
	//spellAI
	SpellAIbase	= AIbase + AIsizeArray[0];
	
	//individualAI
	LNAIbase		= AIbase + AIsizeArray[0] + AIsizeArray[1];
	RecoverAIbase	= AIbase + AIsizeArray[0] + AIsizeArray[1] + eigenValueLN[0][1];
	
	//localAL
	LocalAIbase	= AIbase + AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2];
	
	//backAI
	BackAIbase	= AIbase + AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3];
	
	return 0;
}
