#include "mainDatClass.h"
using namespace std;


int mainDatClass::mainRoop(){
	int Res;
	
	Res = WaitForIni();
	while( Res != 0xF ){
		Res = WaitForRoopFlg();
		if( Res == 0 ){
			if( charInit() == 0 ){
				if( SetCode() == 0 ){
					while( Res == 0 ){
						Res = WaitForMenu();
						if( Res == 0 ){
							Res = ManipMenu();
							if( Res == 0 ){
								Res = WaitForStart();
								if( Res == 0 ){
									Res = mainStep();
								}
							}
						}
					}
					RemoveCode();
				}else{
					cout << "ERROR : SetCode() failed." << endl;
					roopFlg = 0;
				}
				charEnd();
			}else{
				cout << "ERROR : charInit() failed." << endl;
				roopFlg = 0;
			}
		}
		if( th075QuitFlg && Res != 0xF ){
			Res = th075Quit();
		}
	}
	roopFlg = 0;
	
	return 0;
}

int mainDatClass::WaitForIni(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : WaitForIni()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	//�K�v�Ȃ��H
	DWORD deInfo;
	DWORD Counter = 0;
	for(;;){
		if( th075Roop( &deInfo ) ) return 0xF;
		if( deInfo == de_body ){
			Counter++;
			if( Counter > 10 ) break;
		}
	}
	
	return 0;
}

int mainDatClass::th075Quit(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : th075Quit()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	myInfo.A.phase = 0;
	myInfo.B.phase = 0;
	
	dataInfo.A.phase = 0;
	dataInfo.B.phase = 0;
	
	DWORD deInfo;
	
	BYTE menuFlg;
	BYTE gameMode;
	DWORD FlgA,FlgB;
	BYTE HPCountA,HPCountB;
	HWND hWnd;
	
	WORD Flg = 1;
	for(;;){
		if( th075Roop( &deInfo ) ) break;
		if( deInfo == de_body ){
			if( Flg ){
				ReadProcessMemory( hProcess, (void *)((DWORD)pStackBase - 420) , &menuFlg , 1 , NULL );	//12FE5C
				ReadProcessMemory( hProcess , (void*)(0x671418 + 0x218) , &gameMode , 1 , NULL );	//0x671418 + 0x218	//BYTE
				ReadMemory( (void*)0x671418 , &FlgA , 4 );
				ReadMemory( (void*)0x67141C , &FlgB , 4 );
				
				hWnd = FindWindow( NULL , "�������z �` Immaterial and Missing Power. ver1.11" );
				if( hWnd ){
					if( menuFlg == 3 && gameMode == 2 ){
						//�L�����N�^�[�I��
						PostMessage(hWnd, WM_CLOSE, 0, 0);
						Flg = 0;
						
					}else if( menuFlg == 0xD ){
						//���[�r�[
						PostMessage(hWnd, WM_CLOSE, 0, 0);
						Flg = 0;
						
					}else if( menuFlg == 2 ){
						//���j���[
						PostMessage(hWnd, WM_CLOSE, 0, 0);
						Flg = 0;
						
					}else if( FlgA && FlgB ){
						//�퓬���ŁA����HP������Ƃ�
						ReadMemory( (void*)(FlgA + 0x39D) , &HPCountA , 1 );
						ReadMemory( (void*)(FlgB + 0x39D) , &HPCountB , 1 );
						if( HPCountA && HPCountB || autoNextFlg ){
							PostMessage(hWnd, WM_CLOSE, 0, 0);
							Flg = 0;
						}
					}
				}
			}
		}
	}
	return 0xF;
}

int mainDatClass::WaitForRoopFlg(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : WaitForRoopFlg()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	myInfo.phase = phase_default;
	myInfo.A.phase = 0;
	myInfo.B.phase = 0;
	
	dataInfo.A.phase = 0;
	dataInfo.B.phase = 0;
	
	
	DWORD deInfo;
	if( !roopFlg ){
		UnRockTime();
		for(;;){
			if( th075Roop( &deInfo ) ) return 0xF;
			if( roopFlg ) break;
		}
	}
	
	return 0;
}



int mainDatClass::WaitForMenu(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : WaitForMenu()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	
	DWORD deInfo;
	BYTE	menuFlg = 0;
	BYTE	gameMode = 0;
	DWORD	battleFlg = 0;
	
	myInfo.phase = phase_default;
	
	myInfo.A.phase = 0;
	myInfo.B.phase = 0;
	
	dataInfo.A.phase = 0;
	dataInfo.B.phase = 0;
	
	//sessionNo
	if( myInfo.sessionID != myInfo.sessionIDNext ){
		if( myInfo.sessionNo < 255 ){
			myInfo.sessionNo = myInfo.sessionNo + 1;
		}else{
			myInfo.sessionNo = 1;
		}
		myInfo.sessionID = myInfo.sessionIDNext;
	}
	
	
	WORD pushFlg = 0;
	BYTE InputA;
	BYTE InputB;
	DWORD menuAddress;
	BYTE menuIndex;
	for(;;){
		if( th075Roop( &deInfo ) ) return 0xF;
		if( !roopFlg ){
			UnRockTime();
			return 1;
		}
		if( deInfo == de_body ){
			//���j���[��҂�
//			InputA = 0;
			InputB = 0;
			ReadProcessMemory( hProcess, (void *)((DWORD)pStackBase - 420) , &menuFlg , 1 , NULL );	//12FE5C
			ReadProcessMemory( hProcess, (void *)((DWORD)pStackBase - 748) , &menuAddress , 4 , NULL );	//12FD14
			ReadProcessMemory( hProcess , (void*)(0x671418 + 0x218) , &gameMode , 1 , NULL );	//0x671418 + 0x218	//BYTE
			ReadProcessMemory( hProcess , (void*)0x671418 , &battleFlg , 4 , NULL );
			if( menuFlg == 2 ){
				//�X�^�b�N����
				ReadProcessMemory( hProcess , (void*)(menuAddress + 0x38), &menuIndex , 1 , NULL );
			}
			
			#if auto_menu
				if( menuFlg == 3 && gameMode == 2 ){
					datA.SetBodyInput( 0 );
					datB.SetBodyInput( 0 );
					break;
				}
//				cout << (WORD)menuFlg << " : " << (WORD)gameMode << endl;
				
				//���[�r�[�X�L�b�v�ƃ��j���[�����I��
				if( menuFlg == 0xD ){
					InputA = key_A;
				}else if( menuFlg == 2 ){
					if( menuIndex == 2 ){
						InputA = key_A;
					}else if( menuIndex < 2 ){
						InputA = key_down;
					}else{
						InputA = key_up;
					}
				}else{
					InputA = datA.GetInput();
					InputB = datB.GetInput();
				}
				
				//�v����
				//���v���C�ۑ��̌��menuFlg != 2�������Ə�肭�����Ȃ�
				if( battleFlg && menuFlg != 2){
					//auto
					if( autoNextFlg ){
						InputA = key_A;
						if(!pushFlg) InputA = 0;
					}
					datA.SetInput( InputA );
					datB.SetInput( InputB );
				}else{
					if(!pushFlg) InputA = 0;
					datA.SetBodyInput( InputA );
					datB.SetBodyInput( InputB );
				}
				if(!pushFlg){
					pushFlg = 10;
				}else{
					pushFlg--;
				}
			#else
				datA.SetBodyInput( datA.GetInput() );
				datB.SetBodyInput( datB.GetInput() );
				if( menuFlg == 3 && gameMode == 2 ){
					break;
				}
			#endif
		}
	}
	return 0;
}

int mainDatClass::WaitForStart(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : WaitForStart()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	DWORD deInfo;
	
	myInfo.phase = phase_read;
	dataInfo.phase = phase_read;
	
	for(;;){
		if( th075Roop( &deInfo ) ) return 0xF;
		if( !roopFlg ){
			UnRockTime();
			return 1;
		}
		if( deInfo == de_body ){
			//gameTime��҂�
			ReadMemory( (void*)0x6718B4, &myInfo.gameTime, 4 );
			if( myInfo.gameTime ) break;
		}
	}
	
	//�v����
	if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug || myInfo.terminalMode == mode_broadcast ){
		if( inputData.Start( &myInfo ) ) return 1;
	}else if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
		if( inputData.Start( &dataInfo ) ) return 1;
	}
	
	if( syncData.init() ) return 1;
	
	
	//�v����
	myInfo.phase = phase_battle;
	dataInfo.phase = phase_battle;
	{
		RockTime();
		
		if( myInfo.terminalMode == mode_root ){
			for(;;){
				if( th075Roop( &deInfo ) ) return 0xF;
				if( !roopFlg ){
					UnRockTime();
					return 1;
				}
				if( deInfo == de_body ){
					if( myInfo.playerSide == 0xA ){
						for(;;){
							myInfo.sessionIDNext = 1 + rand()%255;
							if( myInfo.sessionIDNext != myInfo.sessionID ) break;
						}
						break;
					}else{
						SendCmd( dest_away, cmd_session );
						if( enInfo.sessionID != enInfo.sessionIDNext ){
							myInfo.sessionIDNext = enInfo.sessionIDNext;
							break;
						}
					}
				}
			}
			dataInfo = myInfo;
		}
		if( myInfo.terminalMode == mode_broadcast || myInfo.terminalMode == mode_debug ){
			for(;;){
				myInfo.sessionIDNext = 1 + rand()%255;
				if( myInfo.sessionIDNext != myInfo.sessionID ) break;
			}
			dataInfo = myInfo;
		}
		if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch){
			for(;;){
				if( th075Roop( &deInfo ) ) return 0xF;
				if( !roopFlg ){
					UnRockTime();
					return 1;
				}
				if( deInfo == de_body ){
					SendCmd( dest_root, cmd_dataInfo );
					if( dataInfo.sessionID != dataInfo.sessionIDNext ){
						break;
					}
				}
			}
		}
		
		#if debug_mode_mainRoop
			WaitForSingleObject( hPrintMutex, INFINITE );
			cout << "debug : WaitForEnStart" << endl;
			ReleaseMutex( hPrintMutex );
		#endif
		if( myInfo.terminalMode == mode_root ){
			enInfo.gameTime = 0;
			myInfo.phase = phase_battle;
			for(;;){
				if( th075Roop( &deInfo ) ) return 0xF;
				if( !roopFlg ){
					UnRockTime();
					return 1;
				}
				if( deInfo == de_body ){
					//�����gameTime==1��҂�
					if( enInfo.gameTime ) break;
					
					//���M
					SendCmd( dest_away, cmd_gameInfo );
				}
			}
		}
		if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch){
			dataInfo.gameTime = 0;
			myInfo.phase = phase_battle;
			dataInfo.phase = phase_battle;
			for(;;){
				if( th075Roop( &deInfo ) ) return 0xF;
				if( !roopFlg ){
					UnRockTime();
					return 1;
				}
				if( deInfo == de_body ){
					if( dataInfo.gameTime > 10 ) break;
					
					if( !Root.sin_addr.s_addr ) roopFlg = 0;
				}
			}
		}
		UnRockTime();
	}
	return 0;
}


