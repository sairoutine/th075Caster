#include "conf.h"
#include "boosterDatClass.h"

//OpenProcess�ł��邩�Abase�͂��邩,���O�͎擾�ł������A
//AI�I�[�v���ł������A�L�����p�����A�ύX���Ă��Ƃ��̏��� ����������0��Ԃ�
//boosterDatInitFlg��RefleshDat()��1�ɂ���
//playerSide��ς�����bodyIniFlg��1�ɂ��邱��
int boosterDatClass::boosterDatInit(){
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".boosterDatInit() " << endl;
	#endif
	
	hWnd = FindWindow( NULL , "�������z �` Immaterial and Missing Power. ver1.11" );
	if(hWnd == 0) return 0xF;
	
	//���������m�ۂł��Ȃ������Ƃ�
	if(AI == 0) return 0xF;
	
	//�{�^�����͏�����
	if(bodyIniFlg==0){
		for(Counter=1;Counter<9;Counter++){
			if(gameInfoIni[Counter][5]){	//������Ă���
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
		gameTimeAddress = baseAddress + 0x498;		//gameTimeAddress	//���v���C���[�h�̂Ƃ��̎���
	}else{
		gameTimeAddress = baseAddress + 0x49C;		//gameTimeAddress
	}
	
	AIMode = 0;
	if(gameMode==0) AIMode = 0;					//story	//stop			//AIMode�̐ݒ�
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
			if( strcmp(NameTemp, "init") && strcmp(NameTemp, "error") && strcmp(NameTemp, "second")){	//init,error,second�̂Ƃ��ۑ����Ȃ�
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
	if( enDat->boosterDatInitFlg == 1 ) return 1;		//���肪�I�����Ă��Ȃ��Ɛi�܂Ȃ�
	strcpy( enName, enDat->Name );
	
	//�����L�������m�ȂƂ���AI[]�����L����B��������AI[]�������Ȃ�:secondaryMode
	if(myID == enDat->myID){				//�L����ID���Q��	//�����L�������m�̑ΐ�
		if( strcmp(NameTemp,"second") == 0 ){
			secondaryModeFlg = 1;			//�O��secondaryMode��������p��
		}else{
			if( enDat->secondaryModeFlg == 0 ){			//�e�L��secodaryMode�ł͂Ȃ������Ƃ�
				if( strcmp(Name,NameTemp) == 0 ){
					secondaryModeFlg = 0;	//���������L�����p�����Ă���Ƃ��͑��葤��secondaryMode�ɂȂ邱�Ƃ�v��
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
		secondaryModeFlg = 0;				//�����L�������m�̑ΐ�ł͂Ȃ�
	}
	
	#if debug_mode
		if(secondaryModeFlg) cout << "debug : " << hex << playerSide << ".secondaryMode" << endl;
	#endif
	
	
	//�t�@�C�����J��
	if(secondaryModeFlg){
		strcpy(Name,"second\0");
	}else{
		if( !( !strcmp(NameTemp, "init") || !strcmp(NameTemp, "error") || !strcmp(NameTemp, "second") ) && !strcmp(NameTemp, Name) ){
			//�O��Ɠ����L����
			if(strcmp(enNameTemp, enName) !=0){
				if( CloseIndividualAI() ) return 1;
				
				Flg = OpenIndividualAI(Name,enName);
				if( Flg == 0xF ) return 0xF;
				if( Flg ) memset(AI + AIsizeArray[0] + AIsizeArray[1], 0, AIsizeArray[2]);
				
			}
		}else{
			//�O��init,error,second�̂Ƃ� or �O��ƈႤ�L����
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
	
	//�N���X�݂̍����ݒ肷����̂͂����܂�
	//�������E�v�Z�͂�������
	
	CalcAddress();
	Flg = ConvertIni();	//�K��CalcAddress()�̌�ōs������
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
