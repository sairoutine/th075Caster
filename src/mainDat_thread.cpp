#include "mainDatClass.h"
#include "booster/boosterDatClass.h"
#include "zlib.h"
#include <float.h>
using namespace std;

unsigned __stdcall boosterThread(void* Address){
	if( !Address ) return 1;
	mainDatClass* dat = (mainDatClass*)Address;
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : boosterThread start" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	
	#if __magnification
		//none
	#else
		return 1;
	#endif
	
	DWORD	boosterDatInfoA;
	DWORD	boosterDatInfoB;
	WORD	stateFlgA;
	WORD	stateFlgB;
	DWORD	Counter;
	
	dat->boosterInput = 0;
	
	for(;;){
		Sleep(500);
		if( !(dat->continueFlg) ) break;
		if( dat->boosterFlg && FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" ) ){
			WaitForSingleObject( dat->hPrintMutex, INFINITE );
//			cout << "debug : booster find th075" << endl;
			ReleaseMutex( dat->hPrintMutex );
			
			WORD listeningMode = 0;
			dat->boosterInput = 0;
			
			boosterDatClass boosterDatA(0xA);
			boosterDatClass boosterDatB(0xB);
			
			boosterDatA.enDat = &boosterDatB;
			boosterDatB.enDat = &boosterDatA;
			
			boosterDatA.casterDat = dat;
			boosterDatB.casterDat = dat;
			
			if( dat->myInfo.terminalMode != mode_debug && dat->boosterFlg ){
				listeningMode = 1;
				boosterDatA.listeningMode = 1;
				boosterDatB.listeningMode = 1;
			}
			
			boosterDatInfoA = 0;
			boosterDatInfoB = 0;
			while(boosterDatInfoA != 0xF && boosterDatInfoB != 0xF && dat->continueFlg && dat->boosterFlg ){
				Sleep(200);
				boosterDatInfoA = boosterDatA.boosterDatInit();
				boosterDatInfoB = boosterDatB.boosterDatInit();
				
				if(boosterDatInfoA == 0 && boosterDatInfoB == 0){
					stateFlgA = 0;
					stateFlgB = 0;
					
					while(stateFlgA != 0xF && stateFlgB != 0xF && dat->continueFlg && dat->boosterFlg ){	//0xFで終わり
						if(Counter >= __magnification){
							stateFlgA = boosterDatA.RefleshDat();
							stateFlgB = boosterDatB.RefleshDat();
							if(stateFlgA == 0 && stateFlgB == 0){
								boosterDatA.ConvertDat();
								boosterDatB.ConvertDat();
								
								boosterDatA.mainRoop();	//ループ良好
								boosterDatB.mainRoop();	//ループ良好
							}
							Counter = 0;
						}
						boosterDatA.InputCommand();
						boosterDatB.InputCommand();
						
						
						if( listeningMode ){
							Sleep( 7 );
						}else{
							Sleep(sleep_time);	//1フレームで16msぐらい
						}
						Counter++;
					}
				}
			}
		}
	}
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : boosterThread end" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	return 0;
}


unsigned __stdcall manageThread(void* Address){
	if( !Address ) return 1;
	mainDatClass* dat = (mainDatClass*)Address;
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : manageThread start" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	
	//タイムアウト管理など
	lastTimeStruct* lastTime = &(dat->lastTime);
	DWORD* nowTime = &(dat->nowTime);
	int size = sizeof(SOCKADDR_IN);
	DWORD Counter;
	
	//cmd_echoではないほうがよい
	
	for(;;){
		*nowTime = timeGetTime();
		
		if( dat->Away.sin_addr.s_addr && lastTime->Away ){
			if( dat->toughModeFlg ){
				//TIMEOUTを無視
				if( lastTime->Away + 4000 < *nowTime ){
					dat->SendCmdM( dest_away, cmd_continue );
				}
			}else if( lastTime->Away + timeout_time < *nowTime ){
				WaitForSingleObject( dat->hMutex, INFINITE );
				memset( &(dat->Away), 0, size );
				ReleaseMutex( dat->hMutex );
				
				cout << "ERROR : TIMEOUT ( Away )" << endl;
				
				dat->roopFlg = 0;
			}else if( lastTime->Away + 4000 < *nowTime ){
				dat->SendCmdM( dest_away, cmd_continue );
			}
		}
		if( dat->Root.sin_addr.s_addr && lastTime->Root ){
			if( dat->toughModeFlg ){
				//TIMEOUTを無視
				if( lastTime->Root + 4000 < *nowTime ){
					dat->SendCmdM( dest_root, cmd_continue );
				}
			}else if( lastTime->Root + timeout_time < *nowTime ){
				WaitForSingleObject( dat->hMutex, INFINITE );
				memset( &(dat->Root), 0, size );
				ReleaseMutex( dat->hMutex );
				
				//要検討
				if( dat->roopFlg ){
					cout << "ERROR : TIMEOUT ( Root )" << endl;
				}
				
				//観戦してて、データが残っているときの対処
				if( dat->myInfo.phase == phase_battle && ( dat->myInfo.terminalMode == mode_branch || dat->myInfo.terminalMode == mode_subbranch ) ){
					//none
				}else{
					dat->roopFlg = 0;
				}
			}else if( lastTime->Root + 4000 < *nowTime ){
				dat->SendCmdM( dest_root, cmd_continue );
			}
		}
		for( Counter = 0; Counter<4; Counter++ ){
			if( dat->Leaf[ Counter ].sin_addr.s_addr && lastTime->Leaf[ Counter ] ){
				if( lastTime->Leaf[ Counter ] + timeout_time < *nowTime ){
					WaitForSingleObject( dat->hMutex, INFINITE );
					memset( &(dat->Leaf[ Counter ]), 0, size );
					ReleaseMutex( dat->hMutex );
				}else if( lastTime->Leaf[ Counter ] + 4000 < *nowTime ){
					dat->SendCmdM( &(dat->Leaf[ Counter ]), cmd_continue );
				}
			}
		}
		if( dat->Access.sin_addr.s_addr && lastTime->Access ){
			if( dat->toughModeFlg ){
				//TIMEOUTを無視
				/*
				if( lastTime->Access + 4000 < *nowTime ){
					dat->SendCmdM( dest_access, cmd_continue );
				}
				*/
			}else if( lastTime->Access + timeout_time < *nowTime ){
				WaitForSingleObject( dat->hMutex, INFINITE );
				memset( &(dat->Access), 0, size );
				ReleaseMutex( dat->hMutex );
				
				cout << "ERROR : TIMEOUT ( Access )" << endl;
				
			}else if( lastTime->Access + 4000 < *nowTime ){
				dat->SendCmdM( dest_access, cmd_continue );
			}
		}
		if( dat->Standby[0].sin_addr.s_addr ){
			if( lastTime->Standby[0] + 3000 < *nowTime ){
				WaitForSingleObject( dat->hMutex, INFINITE );
				memset( &(dat->Standby[0]), 0, size );
				memset( &(dat->Standby[1]), 0, size );
				ReleaseMutex( dat->hMutex );
			}
		}
		if( dat->Standby[1].sin_addr.s_addr ){
			if( lastTime->Standby[1] + 3000 < *nowTime ){
				WaitForSingleObject( dat->hMutex, INFINITE );
				memset( &(dat->Standby[0]), 0, size );
				memset( &(dat->Standby[1]), 0, size );
				ReleaseMutex( dat->hMutex );
			}
		}
		Sleep(500);
		if( !(dat->continueFlg) ) break;
	}
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : manageThread end" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	return 0;
}

unsigned __stdcall th075Thread(void* Address){
	mainDatClass* dat = (mainDatClass*)Address;
	if( !dat ) return 1;
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : th075th start" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	
	dat->deInitFlg = 0;
	{
		STARTUPINFO si;
		ZeroMemory( &si, sizeof(si) );
		si.cb = sizeof( si );
		
		ZeroMemory( &dat->pi, sizeof( dat->pi ) );
		
		dat->priorityFlg = 0;
		dat->windowModeFlg = 0;
		dat->stageLimitCancelFlg = 0;
		dat->windowResizeFlg = 0;
		dat->windowWidth = 640;
		dat->windowHeight = 480;
		dat->windowFilterState = 2;
		dat->windowDisableTransitionsFlg = 0;
		if( strcmp( dat->iniPath, "fail" ) ){
			//position
			if( GetPrivateProfileInt( "WINDOW", "setPosition", 0, dat->iniPath ) ){
				int x,y;
				
				x = GetPrivateProfileInt( "WINDOW", "x", 0xFFFFFFF, dat->iniPath );
				y = GetPrivateProfileInt( "WINDOW", "y", 0xFFFFFFF, dat->iniPath );
				if( x != 0xFFFFFFF && y != 0xFFFFFFF ){
					if( x >  2000 ) x = 0;
					if( x < -1000 ) x = 0;
					
					if( y >  2000 ) y = 0;
					if( y < -1000 ) y = 0;
					
					si.dwX = x;
					si.dwY = y;
					
					si.dwFlags |= STARTF_USEPOSITION;
				}
			}
			
			//priorityFlg
			dat->priorityFlg = (WORD)GetPrivateProfileInt( "MAIN", "priority", 0, dat->iniPath );
			
			//windowMode
			dat->windowModeFlg = (WORD)GetPrivateProfileInt( "MAIN", "windowMode", 0, dat->iniPath );
			
			//stageLimitCancel
			dat->stageLimitCancelFlg = (WORD)GetPrivateProfileInt( "MAIN", "stageLimitCancel", 0, dat->iniPath );
			
			//top
			dat->windowTopFlg = (WORD)GetPrivateProfileInt( "WINDOW", "setWindowTop", 0, dat->iniPath );
			
			//windowResize
			dat->windowResizeFlg = (WORD)GetPrivateProfileInt( "WINDOW", "windowResize", 0, dat->iniPath );
			
	 		dat->windowWidth = GetPrivateProfileInt( "WINDOW", "windowWidth", 640, dat->iniPath );
	 		if (dat->windowWidth < 320) {
	 			dat->windowWidth = 320;
				cout << "Error reading config file: POSITION::width must be at least 320" << endl;
	 		}
			
	 		dat->windowFilterState = GetPrivateProfileInt( "WINDOW", "bilinearFilterState", 2, dat->iniPath );
	 		if (dat->windowFilterState > 3) {
	 			dat->windowFilterState = 2;
	 		}
			
	 		dat->windowDisableTransitionsFlg = GetPrivateProfileInt( "WINDOW", "disableMenuTransitions", 0, dat->iniPath );
			
	 		dat->windowHeight = (int)(((float)(dat->windowWidth)/(float)640.0)*480.0);
		}
		
		
		{
			char path[200];
			memset( path, 0, sizeof( path ) );
			
			if( strcmp( th075_path, "th075.exe" ) ){
				//path指定のときのため
				strcpy( path, th075_path );
			}else{
				if( strcmp( dat->nowDir, "fail" ) && strlen( dat->nowDir ) < 180 ){
					strcpy( path ,dat->nowDir );
					strcat( path, "\\th075.exe\0" );
				}else{
					strcpy( path, "fail\0" );
				}
			}
			
			if( strcmp( path, "fail" ) ){
				if( !CreateProcess( path, NULL, NULL, NULL, FALSE, DEBUG_PROCESS, NULL, NULL, &si, &(dat->pi) ) ){
					cout << "ERROR : th075 start failed." << endl;
					dat->hProcess = NULL;
					if( dat->hCheckEvent ){
						SetEvent(dat->hCheckEvent);
					}
					return 1;
				}
			}else{
				cout << "ERROR : th075path set failed." <<endl;
			}
		}
		dat->hProcess = dat->pi.hProcess;
		if( dat->hCheckEvent ){
			SetEvent(dat->hCheckEvent);
		}
	}
	
	//mainRoop
	dat->mainRoop();
	
	
	CloseHandle(dat->pi.hThread);
	dat->pi.hThread = NULL;
	
	CloseHandle(dat->pi.hProcess);
	dat->pi.hProcess = NULL;
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : th075th end" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	return 0;
}


unsigned __stdcall sendThread(void* Address){
	mainDatClass* dat = (mainDatClass*)Address;
	if( !dat ) return 1;
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : sendThread start" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	
	
	int Counter;
	sTaskClass* sTask;
	SOCKADDR_IN addr;
	int addrSize = sizeof(SOCKADDR_IN);
	while( dat->continueFlg ){
		for(Counter=0; Counter<50; Counter++){
			if(dat->sTask[Counter].Flg){
//				cout << "sTask." << Counter << " send" << endl;
				sTask = &( dat->sTask[Counter] );
				
				if( sTask->dest == dest_away ){
					WaitForSingleObject( dat->hMutex, INFINITE );
					addr = dat->Away;
					ReleaseMutex( dat->hMutex );
					if( addr.sin_addr.s_addr ){
						if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
						if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
					}
					
				}else if( sTask->dest == dest_root ){
					WaitForSingleObject( dat->hMutex, INFINITE );
					addr = dat->Root;
					ReleaseMutex( dat->hMutex );
					if( addr.sin_addr.s_addr ){
						if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
						if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
					}
					
				}else if( sTask->dest == dest_branch ){
					WaitForSingleObject( dat->hMutex, INFINITE );
					addr = dat->Branch;
					ReleaseMutex( dat->hMutex );
					if( addr.sin_addr.s_addr ){
						if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
						if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
					}
					
				}else if( sTask->dest == dest_subbranch ){
					WaitForSingleObject( dat->hMutex, INFINITE );
					addr = dat->subBranch;
					ReleaseMutex( dat->hMutex );
					if( addr.sin_addr.s_addr ){
						if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
						if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
					}
					
				}else if( sTask->dest == dest_leaf ){
					int leafCounter;
					for(leafCounter = 0; leafCounter<4; leafCounter++){
						WaitForSingleObject( dat->hMutex, INFINITE );
						addr = dat->Leaf[ leafCounter ];
						ReleaseMutex( dat->hMutex );
						if( addr.sin_addr.s_addr ){
							if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
							if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
						}
					}
					
				}else if( sTask->dest == dest_addr ){
//					addr = sTask->addr;
					if( sTask->addr.sin_addr.s_addr ){
						if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&(sTask->addr), addrSize);
						if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&(sTask->addr), addrSize);
					}
				}else if( sTask->dest == dest_access ){
					WaitForSingleObject( dat->hMutex, INFINITE );
					addr = dat->Access;
					ReleaseMutex( dat->hMutex );
					if( addr.sin_addr.s_addr ){
						if(sTask->Flg == stask_data) sendto(dat->s, (const char*)sTask->data, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
						if(sTask->Flg == stask_area) sendto(dat->s, (const char*)sTask->Address, sTask->size, 0, (SOCKADDR*)&addr, addrSize);
					}
				}
				sTask->Flg = 0;
			}
		}
		WaitForSingleObject( dat->hSendEvent, INFINITE );
	}
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : sendThread end" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	return 0;
}





//ローカルIPを弾く
//未テスト
//127.0.0.1→7F 00 00 01
int TestIP( DWORD IP ){
	if( !IP ) return 1;
	BYTE* ip = (BYTE*)&IP;
	
//	cout << (WORD)ip[0] << (WORD)ip[1] << (WORD)ip[2] << (WORD)ip[3] << endl;
	
	//10.0.0.0～10.255.255.255
	if( ip[0] == 10 ) return 1;
	
	//172.16.0.0～172.31.255.255
	if( ip[0] == 172 && ip[1] >= 16 && ip[1] <= 31 ) return 1;
	
	//192.168.0.0～192.168.255.255
	if( ip[0] == 192 && ip[1] == 168 ) return 1;
	
	//127.0.0.1
	if( IP == 0x100007F ) return 1;
	
	return 0;
}

BYTE GetCWflg( mainDatClass* dat, BYTE* recvData, int size ){
	BYTE enFlg = 0;
	BYTE flg = 0xF;
	
	if( size > 11 ){
		if( !recvData[5] ){
			enFlg = recvData[6];
		}else{
			BYTE preSize = 6;	//id[5] + flg[1]
			BYTE longNameLength;
			BYTE shortNameLength;
			
			//longname length
			longNameLength = recvData[ preSize ];
			
			//shortname length
			if( longNameLength < 100 ){
				shortNameLength = recvData[ preSize + 1 + longNameLength ];
			}
			
			if( longNameLength < 100 && shortNameLength < 50 ){
				if( size > 5 + preSize + 1 + longNameLength + 1 + shortNameLength ){
					enFlg = recvData[ preSize + 1 + longNameLength + 1 + shortNameLength ];
				}
			}
		}
	}
	
	if( enFlg ){
		//Away can use newCW
		
		if( enFlg == 1 ){
			//away can select
			switch( dat->floatControlFlg ){
			case 0 :
				//oldCW
				flg = 0;
				break;
			case 1 :
				//newCW
				flg = 1;
				break;
			case 2 :
				//newCW
				flg = 1;
				break;
			}
		}else if( enFlg == 2 ){
			//away use newCW
			switch( dat->floatControlFlg ){
			case 0 :
				//oldCW
				flg = 0xF;
				break;
			case 1 :
				//newCW
				flg = 1;
				break;
			case 2 :
				//newCW
				flg = 1;
				break;
			}
		}
	}else{
		//Away don't use newCW
		switch( dat->floatControlFlg ){
		case 0 :
			//oldCW
			flg = 0;
			break;
		case 1 :
			//newCW
			flg = 0;
			break;
		case 2 :
			//newCW
			flg = 0xF;
			break;
		}
	}
	return flg;
}


//addrの中身を弄るときはMUTEXを取得する
unsigned __stdcall recvThread(void* Address){
	mainDatClass* dat = (mainDatClass*)Address;
	if(!dat) return 1;
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : rcvThread start" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	
	
	
	//バージョン情報もあったほうがいい
	int	addrSize = sizeof(SOCKADDR_IN);
	int	size;
	SOCKADDR_IN addr;
	
	BYTE data[1024];
	
	BYTE	recvBuf[ recv_buf_size ];
	BYTE* recvData = &recvBuf[5];
	BYTE* command = &recvBuf[4];
	lastTimeStruct* lastTime = &(dat->lastTime);
	DWORD* nowTime = &(dat->nowTime);
	
	DWORD reqTime[4][2];
	memset( reqTime, 0, sizeof(reqTime) );
	
	DWORD transTime = 0;
	DWORD readyTime = 0;
	
	//z_buf
	BYTE* zBuf = (BYTE*)malloc( z_buf_size );
	if( !zBuf ){
		return 1;
	}
	
	//cmd_exit
	DWORD cmdExitTime[20][2];
	memset( cmdExitTime, 0, sizeof( cmdExitTime ) );
	
	while( dat->continueFlg ){
		size = recvfrom( dat->s, (char*)recvBuf, recv_buf_size, 0, (SOCKADDR*)&addr, &addrSize);
		
		if( size < 0) {
			//要検討
			if( addr.sin_addr.s_addr == 0 ){
				//手違いのときのための対処
				if( dat->continueFlg ){
					Sleep(2);
				}
			}
		}else{
			if(size > recv_buf_size) size = 0;
			if( size ){
				//debug
//				if( TestIP( addr.sin_addr.s_addr ) ) cout << "debug : Local IP" << endl;
				
				
				if( recvBuf[0] == cmd_version && recvBuf[1] == cmd_casters && recvBuf[2] == cmd_space_2 && recvBuf[3] == cmd_space_3 ){
					if( addr.sin_addr.s_addr == dat->Away.sin_addr.s_addr){
						//対戦相手からの通信
						if( *command != cmd_exit ){
							lastTime->Away = *nowTime;
						}
						
						switch( *command ){
						case cmd_exit :
							dat->roopFlg = 0;
							break;
						case cmd_continue :
							dat->SendCmdR( dest_away, res_continue );
							break;
						case cmd_echo :
							dat->SendCmdR( dest_away, res_echo );
							break;
						case res_echo :
							dat->echoFlg.Away = 1;
							break;
						case res_access :
							{
								BYTE enCWflg = 0;
								if( size > 14 ){
									if( !recvData[8] ){
										enCWflg = recvData[9];
									}else{
										BYTE preSize = 9;	//accessFlg[1] + id[5] + port[2] + flg[1]
										BYTE longNameLength;
										BYTE shortNameLength;
										
										//longname length
										longNameLength = recvData[ preSize ];
										
										//shortname length
										if( longNameLength < 100 ){
											shortNameLength = recvData[ preSize + 1 + longNameLength ];
										}
										
										if( longNameLength < 100 && shortNameLength < 50 ){
											if( size > 5 + preSize + 1 + longNameLength + 1 + shortNameLength ){
												enCWflg = recvData[ preSize + 1 + longNameLength + 1 + shortNameLength ];
											}
										}
									}
								}
								
								BYTE flg = 0xF;
								if( enCWflg ){
									//away use newCW
									switch( dat->floatControlFlg ){
									case 0 :
										flg = 0xF;
										break;
									case 1 :
										flg = 1;
										break;
									case 2 :
										flg = 1;
										break;
									}
								}else{
									//away don't use newCW
									switch( dat->floatControlFlg ){
									case 0 :
										flg = 0;
										break;
									case 1 :
										flg = 0;
										break;
									case 2 :
										flg = 0xF;
										break;
									}
								}
								switch( flg ){
								case 0 :
									dat->newCWflg = 0;
									dat->accessFlg = recvData[0];
									break;
								case 1 :
									dat->newCWflg = 1;
									dat->accessFlg = recvData[0];
									break;
								case 0xF :
									cout << "ERROR : float setting error" << endl;
									dat->accessFlg = status_error;
									
									WaitForSingleObject( dat->hMutex, INFINITE );
									memset( &(dat->Away), 0, sizeof(SOCKADDR_IN));
									ReleaseMutex( dat->hMutex );
									dat->roopFlg = 0;
									
									break;
								}
							}
							break;
						case cmd_access :
							{
								BYTE flg = GetCWflg( dat, recvData, size );
								switch( flg ){
								case 0 :
								case 1 :
									dat->newCWflg = flg;
									
									dat->accessFlg = status_ok;
									data[0] = status_ok;
									data[1] = 0;	//id[5]
									data[2] = 0;
									data[3] = 0;
									data[4] = 0;
									data[5] = 0;
									data[6] = 0;	//port[2]
									data[7] = 0;
									data[8] = 0;	//flg
									data[9] = flg;	//CW flg
									dat->SendCmdR( dest_away, res_access, data, 10 );
									break;
								case 0xF :
									//return error
									data[0] = status_error;
									dat->SendCmdR( dest_away, res_access, data, 1 );
									break;
								}
							}
							break;
						case cmd_sendinput :
							//戦闘中の入力データを格納する
							{
								BYTE enSide = 0;
								if( dat->myInfo.playerSide == 0xA ){
									enSide = 0xB;
								}else if( dat->myInfo.playerSide == 0xB ){
									enSide = 0xA;
								}
								if( enSide ){
									DWORD gameTimeTemp = *(DWORD*)&recvData[2];
									if( gameTimeTemp < 30 ){
										dat->inputData.SetInputData( recvData[0], gameTimeTemp , enSide, recvData[6] );
									}else{
										dat->inputData.SetInputData( recvData[0], gameTimeTemp     , enSide, recvData[ 6 ] );
										dat->inputData.SetInputData( recvData[0], gameTimeTemp - 2 , enSide, recvData[ 7 ] );
										dat->inputData.SetInputData( recvData[0], gameTimeTemp - 4 , enSide, recvData[ 8 ] );
										dat->inputData.SetInputData( recvData[0], gameTimeTemp - 6 , enSide, recvData[ 9 ] );
										dat->inputData.SetInputData( recvData[0], gameTimeTemp - 8 , enSide, recvData[ 10 ] );
										dat->inputData.SetInputData( recvData[0], gameTimeTemp - 10 , enSide, recvData[ 11 ] );
									}
									
									gameTimeTemp = *(DWORD*)&recvData[12];
									if( size > 17 && !( gameTimeTemp%20 ) ){
										//set syncData
//										cout << "debug : set syncData Away" << endl;
										dat->syncData.SetSyncDataAwayA( gameTimeTemp, *(DWORD*)&recvData[16], *(DWORD*)&recvData[20] );
										dat->syncData.SetSyncDataAwayB( gameTimeTemp, *(DWORD*)&recvData[24], *(DWORD*)&recvData[28] );
									}
								}
								if( dat->perfectFreezeFlg ){
									PulseEvent( dat->hInputRecvEvent );
									//SetEvent( dat->hInputRecvEvent );
								}
							}
							break;
						case res_input_req :
							//戦闘中の入力データを格納する
							{
								BYTE enSide = 0;
								if( dat->myInfo.playerSide == 0xA ){
									enSide = 0xB;
								}else if( dat->myInfo.playerSide == 0xB ){
									enSide = 0xA;
								}
								if( enSide ){
									dat->inputData.SetInputData( recvData[0], *(DWORD*)&recvData[2] , enSide, recvData[6] );
								}
								if( dat->perfectFreezeFlg ){
									PulseEvent( dat->hInputRecvEvent );
									//SetEvent( dat->hInputRecvEvent );
								}
							}
							break;
						case cmd_input_req :
							//入力を返信する
							{
								BYTE mySide = dat->myInfo.playerSide;
								if( !( dat->inputData.GetInputData( recvData[0], *(DWORD*)&recvData[2], mySide, &data[6] ) ) ){
									data[0] = recvData[0];
									data[1] = recvData[1];
									*(DWORD*)&data[2] = *(DWORD*)&recvData[2];
									dat->SendCmdR( dest_away, res_input_req, data, 7 );
								}
							}
							break;
						case res_time :
							//相手の時間を格納
							dat->enInfo.gameTime = *(DWORD*)recvData;
							break;
						case cmd_time :
							//時間を送る
							dat->SendCmdR( dest_away, res_time, &( dat->myInfo.gameTime ), 4 );
							break;
						case res_gameInfo :
							//メニューの情報を格納
							memcpy( &(dat->enInfo), recvData, sizeof(dat->enInfo) );
							break;
						case cmd_gameInfo :
							//メニューの情報を送る
							dat->SendCmdR( dest_away, res_gameInfo, &(dat->myInfo), sizeof(dat->myInfo) );
							break;
						case cmd_delay :
							*(WORD*)&data[0] = dat->delayTime;
							*(WORD*)&data[2] = (WORD)(dat->delay * 10);
							dat->SendCmdR( dest_away, res_delay, data, 4 );
							break;
						case res_delay :
							//delayObsも表示させたい
							dat->delayTime = *(WORD*)recvData;
							dat->delay = ( (float)*(WORD*)&recvData[2] ) / 10;
							break;
						case cmd_delayobs :
							dat->SendCmdR( dest_away, res_delayobs, recvData, 4 );
							break;
						case res_delayobs :
							if( dat->delayTimeObsNo < 5 ){
								DWORD timeTemp = timeGetTime();
								dat->delayTimeObs[ dat->delayTimeObsNo ] = (float)(timeTemp - *(DWORD*)recvData );
								dat->delayTimeObsNo = dat->delayTimeObsNo + 1;
							}
							break;
						case cmd_rand :	//できれば細かいデータはまとめる方向で
							data[0] = dat->myRandNo;
							data[1] = dat->myRand;
							dat->SendCmdR( dest_away, res_rand, data, 2 );
							break;
						case res_rand :
							dat->enRandNo = recvData[0];
							dat->enRand = recvData[1];
							break;
						case cmd_playerside :
							dat->SendCmdR( dest_away, res_playerside, &( dat->myInfo.playerSide), 1 );
							break;
						case res_playerside :
							dat->enInfo.playerSide = recvData[0];
							break;
						case cmd_session :
							data[0] = dat->myInfo.sessionNo;
							data[1] = dat->myInfo.sessionID;
							data[2] = dat->myInfo.sessionIDNext;
							dat->SendCmdR( dest_away, res_session, data, 3 );
							break;
						case res_session :
							dat->enInfo.sessionNo = recvData[0];
							dat->enInfo.sessionID = recvData[1];
							dat->enInfo.sessionIDNext = recvData[2];
							break;
						case cmd_initflg :
							dat->SendCmdR( dest_away, res_initflg, &( dat->myInitFlg ), 1 );
							break;
						case res_initflg :
							dat->enInitFlg = recvData[0];
							break;
						case cmd_echoes :
							if( dat->myPort ){
								dat->SendCmdR( &addr, res_echoes, &(dat->myPort), 2 );
							}
							break;
						case res_echoes :
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Away.sin_port = addr.sin_port;
							ReleaseMutex( dat->hMutex );
							dat->echoFlg.Away = 1;
							
//							cout << "debug : res_echoes" << endl;
							break;
						}
					}else if( addr.sin_addr.s_addr == dat->Leaf[0].sin_addr.s_addr
						|| addr.sin_addr.s_addr == dat->Leaf[1].sin_addr.s_addr
						|| addr.sin_addr.s_addr == dat->Leaf[2].sin_addr.s_addr
						|| addr.sin_addr.s_addr == dat->Leaf[3].sin_addr.s_addr
					){
						//Leaf
						BYTE Index = 0;
						if( addr.sin_addr.s_addr == dat->Leaf[0].sin_addr.s_addr ){
							Index = 0;
						}else if( addr.sin_addr.s_addr == dat->Leaf[1].sin_addr.s_addr ){
							Index = 1;
						}else if( addr.sin_addr.s_addr == dat->Leaf[2].sin_addr.s_addr ){
							Index = 2;
						}else if( addr.sin_addr.s_addr == dat->Leaf[3].sin_addr.s_addr ){
							Index = 3;
						}
						if( *command != cmd_exit ){
							lastTime->Leaf[ Index ] = *nowTime;
						}
						
						switch( *command ){
						case cmd_exit :
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Leaf[ Index ]), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
							break;
						case res_seek_leaf :
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Leaf[ Index ] = addr;
							ReleaseMutex( dat->hMutex );
							break;
						case cmd_continue :
							dat->SendCmdR( &addr, res_continue );
							break;
						case cmd_delayobs :
							dat->SendCmdR( &addr, res_delayobs, recvData, 4 );
							break;
						case res_echo :
							dat->echoFlg.Leaf[ Index ] = 1;
							break;
						case cmd_echo :
							dat->SendCmdR( &addr, res_echo );
							break;
						case cmd_access :
							data[0] = status_bad;
							data[1] = 0;	//id[5]
							data[2] = 0;
							data[3] = 0;
							data[4] = 0;
							data[5] = 0;
							data[6] = 0;	//port[2]
							data[7] = 0;
							data[8] = 0;	//flg
							data[9] = (BYTE)( dat->newCWflg );	//CW flg
							dat->SendCmdR( &addr, res_access, data, 10 );
							break;
						case cmd_join :
							data[0] = status_ok;
							dat->SendCmdR( &addr, res_join, data, 1 );
							break;
						case cmd_inputdata_req :
							//両サイドの情報を送る
//							cout << "cmd_inputdata_req : " << (WORD)recvData[0] << ", " << *(DWORD*)&recvData[2] << endl;
							{
								DWORD gameTimeTemp = *(DWORD*)&recvData[2];
								//抑制テスト
								if( reqTime[ Index ][0] == gameTimeTemp && reqTime[ Index ][1] + 950 > *nowTime ){
									//抑制
									//要検討
								}else{
									if( !( dat->inputData.GetInputDataA( recvData[0], gameTimeTemp, &data[6] ) )
									 && !( dat->inputData.GetInputDataB( recvData[0], gameTimeTemp, &data[7] ) )
									){
										if( dat->zlibFlg ){
											//zlib
											
											if( size == 11 ){
												//old
												BYTE* buf = dat->inputData.GetInputDataAddress( recvData[0], gameTimeTemp , 60);
												if( buf ){
													data[0] = recvData[0];
													data[1] = recvData[1];
													*(DWORD*)&data[2] = gameTimeTemp;
													memcpy( data + 6,  buf, 60 );
													dat->SendCmdR( &addr, res_inputdata_area, data, 66 );
												}else{
													data[0] = recvData[0];
													data[1] = recvData[1];
													*(DWORD*)&data[2] = gameTimeTemp;
													dat->SendCmdR( &addr, res_inputdata_req, data, 8 );
												}
											}else if( size > 11 ){
//											}else if( size == 12 && recvData[6] == 0 ){
												//new
												BYTE* Buf;
												WORD dataSize = 0;
												
												Buf = dat->inputData.GetInputDataAddress( recvData[0], gameTimeTemp , 180);
												if( Buf && dat->inputData.GetTime( recvData[0] ) >= gameTimeTemp + 180 ){
													dataSize = 180;
												}else{
													Buf = dat->inputData.GetInputDataAddress( recvData[0], gameTimeTemp , 60);
													if( Buf && dat->inputData.GetTime( recvData[0] ) >= gameTimeTemp + 60){
														dataSize = 60;
													}else{
														data[0] = recvData[0];
														data[1] = recvData[1];
														*(DWORD*)&data[2] = gameTimeTemp;
														dat->SendCmdR( &addr, res_inputdata_req, data, 8 );
													}
												}
												
												if( Buf && dataSize ){
													//zlib使用
													unsigned long bufSize = 1014;
													if( compress( &data[10], &bufSize, Buf, dataSize ) == Z_OK ){
														//send
														data[0] = recvData[0];
														data[1] = recvData[1];
														*(DWORD*)&data[2] = gameTimeTemp;
														*(WORD*)&data[6] = bufSize;
														*(WORD*)&data[8] = dataSize;
														
														dat->SendCmdR( &addr, res_inputdata_z, data, 10 + bufSize );
														
														//debug
//														cout << "zSize : " << bufSize << endl;
													}else{
														//zlib不使用
														data[0] = recvData[0];
														data[1] = recvData[1];
														*(DWORD*)&data[2] = gameTimeTemp;
														memcpy( data + 6,  Buf, 60 );
														dat->SendCmdR( &addr, res_inputdata_area, data, 66 );
													}
												}
											}
										}else{
											//no zlib
											BYTE* buf = dat->inputData.GetInputDataAddress( recvData[0], gameTimeTemp , 60);
											if( buf ){
												data[0] = recvData[0];
												data[1] = recvData[1];
												*(DWORD*)&data[2] = gameTimeTemp;
												memcpy( data + 6,  buf, 60 );
												dat->SendCmdR( &addr, res_inputdata_area, data, 66 );
											}else{
												data[0] = recvData[0];
												data[1] = recvData[1];
												*(DWORD*)&data[2] = gameTimeTemp;
												dat->SendCmdR( &addr, res_inputdata_req, data, 8 );
											}
										}
									}
									reqTime[ Index ][0] = gameTimeTemp;
									reqTime[ Index ][1] = *nowTime;
								}
							}
							break;
						case cmd_dataInfo :
							dat->SendCmdR( &addr, res_dataInfo, &(dat->dataInfo), sizeof(dat->dataInfo), &(dat->myInfo.terminalMode), 1 );
							break;
						}
					}else if( addr.sin_addr.s_addr == dat->Root.sin_addr.s_addr ){
						//Rootからの通信
						if( *command != res_dataInfo && *command != cmd_exit ){
							lastTime->Root = *nowTime;
						}
						
						switch( *command ){
						case cmd_exit :
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Root), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
							
							//観戦してて、データが残っているときの対処
							if( dat->myInfo.phase == phase_battle && ( dat->myInfo.terminalMode == mode_branch || dat->myInfo.terminalMode == mode_subbranch ) ){
								//none
							}else{
								dat->roopFlg = 0;
							}
							break;
						case cmd_addr_leaf :
//							cout << "debug : cmd_addr_leaf ( Root )" << endl;
							if( transTime + 200 < *nowTime ){
								//nowTimeは500刻み
								SOCKADDR_IN addrTemp = *(SOCKADDR_IN*)recvData;
								dat->SendCmdR( &addrTemp, cmd_echo );
								if( size > sizeof( SOCKADDR_IN ) + 6 ){
									if( *(WORD*)&recvData[ sizeof( SOCKADDR_IN ) ] ){
//										cout << "debug : cmd_addr_leaf ( Root )" << endl;
										addrTemp.sin_port = htons( *(WORD*)&recvData[ sizeof( SOCKADDR_IN ) ] );
										dat->SendCmdR( &addrTemp, cmd_seek_leaf );
									}
								}
								transTime = *nowTime;
							}
							break;
						case cmd_seek_leaf :
//							cout << "debug : cmd_seek_leaf ( Root )" << endl;
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Root.sin_port = addr.sin_port;
							ReleaseMutex( dat->hMutex );
							dat->SendCmdR( dest_root, res_seek_leaf );
							break;
						case cmd_continue :
							dat->SendCmdR( dest_root, res_continue );
							break;
						case cmd_delayobs :
							dat->SendCmdR( dest_root, res_delayobs, recvData, 4 );
							break;
						case cmd_echo :
							dat->SendCmdR( dest_root, res_echo );
							break;
						case res_echo :
							dat->echoFlg.Root = 1;
							break;
						case res_dataInfo :
							//メニューの情報を格納
							memcpy( &(dat->dataInfo), recvData, sizeof(dat->dataInfo) );
							if( size > 37 ){
								dat->targetMode = recvData[ sizeof(dat->dataInfo) ];
							}
							
							//要検討
							//Leafへの伝達で使う（観戦中かどうか）
							if( dat->roopFlg ) dat->dataInfo.terminalMode = dat->myInfo.terminalMode;
							break;
						case res_join :
							dat->joinRes = recvData[0];
							break;
						case cmd_cast :
//							cout << "cmd_cast recv : " << (WORD)recvData[0] << ", " << *(DWORD*)&recvData[2] << endl;
							
							{
								DWORD gameTimeTemp = *(DWORD*)&recvData[2];
								//要検討
								dat->dataInfo.gameTime = gameTimeTemp;
								dat->inputData.SetTime( recvData[0], gameTimeTemp );
								
								BYTE* castBufA = &recvData[6];
								BYTE* castBufB = &recvData[12];
//								cout << "cast recv : " << gameTimeTemp << " : " << (WORD)castBufA[0] << " : " << (WORD)recvData[4] << endl;
								dat->inputData.SetInputDataA( recvData[0], gameTimeTemp, castBufA[0] );
								dat->inputData.SetInputDataB( recvData[0], gameTimeTemp, castBufB[0] );
								if( gameTimeTemp > 100 ){
									dat->inputData.SetInputDataA( recvData[0], gameTimeTemp - 2, castBufA[1] );
									dat->inputData.SetInputDataA( recvData[0], gameTimeTemp - 4, castBufA[2] );
									dat->inputData.SetInputDataA( recvData[0], gameTimeTemp - 6, castBufA[3] );
									dat->inputData.SetInputDataA( recvData[0], gameTimeTemp - 8, castBufA[4] );
									dat->inputData.SetInputDataA( recvData[0], gameTimeTemp - 10, castBufA[5] );
									
									dat->inputData.SetInputDataB( recvData[0], gameTimeTemp - 2, castBufB[1] );
									dat->inputData.SetInputDataB( recvData[0], gameTimeTemp - 4, castBufB[2] );
									dat->inputData.SetInputDataB( recvData[0], gameTimeTemp - 6, castBufB[3] );
									dat->inputData.SetInputDataB( recvData[0], gameTimeTemp - 8, castBufB[4] );
									dat->inputData.SetInputDataB( recvData[0], gameTimeTemp - 10, castBufB[5] );
								}
								if( dat->perfectFreezeFlg ){
									PulseEvent( dat->hInputRecvEvent );
									//SetEvent( dat->hInputRecvEvent );
								}
							}
							break;
						case res_inputdata_req :
							{
//								cout << "res_inputdata_req" << endl;
								DWORD gameTimeTemp = *(DWORD*)&recvData[2];
								dat->inputData.SetInputDataA( recvData[0], gameTimeTemp, recvData[6] );
								dat->inputData.SetInputDataB( recvData[0], gameTimeTemp, recvData[7] );
							}
							break;
						case res_inputdata_area :
							{
//								cout << "res_inputdata_area" << endl;
								DWORD gameTimeTemp = *(DWORD*)&recvData[2];
								dat->inputData.SetInputDataArea( recvData[0], gameTimeTemp, &recvData[6], 60 );
							}
							break;
						case res_inputdata_z :
							//実験
//							cout << "res_inputdata_z" << endl;
							if( dat->zlibFlg ){
								if( *(WORD*)&recvData[8] < z_buf_size ){
									if( *(WORD*)&recvData[6] < 1024 ){
										DWORD gameTimeTemp = *(DWORD*)&recvData[2];
										unsigned long bufSize = z_buf_size;
										if( uncompress( zBuf, &bufSize, &recvData[10], *(WORD*)&recvData[6]) == Z_OK ){
											if( bufSize == (DWORD)*(WORD*)&recvData[8] ){
												//debug
//												cout << "debug : res_inputdata_z size " << *(WORD*)&recvData[6] << " -> " << bufSize << endl;
												dat->inputData.SetInputDataArea( recvData[0], gameTimeTemp, zBuf, *(WORD*)&recvData[8] );
											}
										}
									}
								}
							}
							break;
						case cmd_addr_branch :
							//実験
							//要検討
//							cout << "debug : cmd_addr_branch ( Root )" << endl;
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Root = *(SOCKADDR_IN*)recvData;
							ReleaseMutex( dat->hMutex );
							break;
						}
					}else if( addr.sin_addr.s_addr == dat->Access.sin_addr.s_addr ){
						//アクセス相手からの通信
						if( *command != cmd_exit ){
							lastTime->Access = *nowTime;
						}
						
						switch( *command ){
						case cmd_exit :
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Access), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
							
							dat->roopFlg = 0;
							break;
						case cmd_testport :
							//TestPort
							{
								SOCKADDR_IN addrTemp;
								memset( &addrTemp, 0, sizeof(SOCKADDR_IN) );
								
								WaitForSingleObject( dat->hMutex, INFINITE );
								addrTemp.sin_family = AF_INET;
								addrTemp.sin_addr.s_addr = addr.sin_addr.s_addr;
								addrTemp.sin_port = htons( *(WORD*)recvData );
								ReleaseMutex( dat->hMutex );
								
								dat->SendCmdR( &addrTemp, res_testport );
							}
							break;
						case cmd_continue :
							dat->SendCmdR( dest_access, res_continue );
							break;
						case cmd_delayobs :
							dat->SendCmdR( dest_access, res_delayobs, recvData, 4 );
							break;
						case res_delayobs :
							if( dat->delayTimeObsNo < 5 ){
								DWORD timeTemp = timeGetTime();
								dat->delayTimeObs[ dat->delayTimeObsNo ] = (float)(timeTemp - *(DWORD*)recvData);
								dat->delayTimeObsNo = dat->delayTimeObsNo + 1;
							}
							break;
						case cmd_echo :
							dat->SendCmdR( dest_access, res_echo );
							if( dat->portSeekFlg ){
								WaitForSingleObject( dat->hMutex, INFINITE );
								dat->Access.sin_port = addr.sin_port;
								ReleaseMutex( dat->hMutex );
							}
							break;
						case res_echo :
							dat->echoFlg.Access = 1;
							if( dat->portSeekFlg ){
								WaitForSingleObject( dat->hMutex, INFINITE );
								dat->Access.sin_port = addr.sin_port;
								ReleaseMutex( dat->hMutex );
							}
							break;
						case cmd_access :
							{
								BYTE flg = GetCWflg( dat, recvData, size );
								switch( flg ){
								case 0 :
								case 1 :
									dat->newCWflg = flg;
									
									dat->accessFlg = status_ok;
									data[0] = status_ok;
									data[1] = 0;	//id[5]
									data[2] = 0;
									data[3] = 0;
									data[4] = 0;
									data[5] = 0;
									data[6] = 0;	//port[2]
									data[7] = 0;
									data[8] = 0;	//flg
									data[9] = flg;	//CW flg
									dat->SendCmdR( dest_access, res_access, data, 10 );
									break;
								case 0xF :
									//return error
									data[0] = status_error;
									dat->SendCmdR( dest_access, res_access, data, 1 );
									break;
								}
							}
							break;
						case res_access :
							{
								BYTE enCWflg = 0;
								if( size > 14 ){
									if( !recvData[8] ){
										enCWflg = recvData[9];
									}else{
										BYTE preSize = 9;	//accessFlg[1] + id[5] + port[2] + flg[1]
										BYTE longNameLength;
										BYTE shortNameLength;
										
										//longname length
										longNameLength = recvData[ preSize ];
										
										//shortname length
										if( longNameLength < 100 ){
											shortNameLength = recvData[ preSize + 1 + longNameLength ];
										}
										
										if( longNameLength < 100 && shortNameLength < 50 ){
											if( size > 5 + preSize + 1 + longNameLength + 1 + shortNameLength ){
												enCWflg = recvData[ preSize + 1 + longNameLength + 1 + shortNameLength ];
											}
										}
									}
								}
								
								BYTE flg = 0xF;
								if( enCWflg ){
									//away use newCW
									switch( dat->floatControlFlg ){
									case 0 :
										flg = 0xF;
										break;
									case 1 :
										flg = 1;
										break;
									case 2 :
										flg = 1;
										break;
									}
								}else{
									//away don't use newCW
									switch( dat->floatControlFlg ){
									case 0 :
										flg = 0;
										break;
									case 1 :
										flg = 0;
										break;
									case 2 :
										flg = 0xF;
										break;
									}
								}
								switch( flg ){
								case 0 :
									dat->newCWflg = 0;
									dat->accessFlg = recvData[0];
									break;
								case 1 :
									dat->newCWflg = 1;
									dat->accessFlg = recvData[0];
									break;
								case 0xF :
									cout << "ERROR : float setting error" << endl;
									dat->accessFlg = status_error;
									
									WaitForSingleObject( dat->hMutex, INFINITE );
									memset( &(dat->Access), 0, sizeof(SOCKADDR_IN));
									ReleaseMutex( dat->hMutex );
									dat->roopFlg = 0;
									
									break;
								}
							}
							break;
						case cmd_addr_branch :
							//実験
							//要検討
//							cout << "debug : cmd_addr_branch ( Access )" << endl;
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Access = *(SOCKADDR_IN*)recvData;
							ReleaseMutex( dat->hMutex );
							break;
						case res_dataInfo :
							//メニューの情報を格納
							memcpy( &(dat->dataInfo), recvData, sizeof(dat->dataInfo) );
							if( size > 37 ){
								dat->targetMode = recvData[ sizeof(dat->dataInfo) ];
							}
							
							//要検討
							//Leafへの伝達で使う（観戦中かどうか）
							if( dat->roopFlg ) dat->dataInfo.terminalMode = dat->myInfo.terminalMode;
							break;
						case cmd_ready :
							//要検討
							if( readyTime + 200 < *nowTime ){
								//nowTimeは500刻み
								SOCKADDR_IN addrTemp = *(SOCKADDR_IN*)recvData;
								dat->SendCmdR( &addrTemp, res_continue );
								readyTime = *nowTime;
							}
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Ready = *(SOCKADDR_IN*)recvData;
							dat->readyPort = *(WORD*)&recvData[ sizeof(SOCKADDR_IN) ];
							ReleaseMutex( dat->hMutex );
							break;
						case cmd_echoes :
							if( dat->myPort ){
								dat->SendCmdR( &addr, res_echoes, &(dat->myPort), 2 );
							}
							break;
						case res_echoes :
							WaitForSingleObject( dat->hMutex, INFINITE );
							dat->Access.sin_port = addr.sin_port;
							ReleaseMutex( dat->hMutex );
							dat->echoFlg.Access = 1;
							
//							cout << "debug : res_echoes" << endl;
							break;
						}
					}else{
						//Other
						switch( *command ){
						case cmd_testport :
							//TestPort
							{
								SOCKADDR_IN addrTemp;
								memset( &addrTemp, 0, sizeof(SOCKADDR_IN) );
								
								WaitForSingleObject( dat->hMutex, INFINITE );
								addrTemp.sin_family = AF_INET;
								addrTemp.sin_addr.s_addr = addr.sin_addr.s_addr;
								addrTemp.sin_port = htons( *(WORD*)recvData );
								ReleaseMutex( dat->hMutex );
								
								dat->SendCmdR( &addrTemp, res_testport );
							}
							break;
						case res_testport :
							//TestPort res
							dat->testPortFlg = 1;
							break;
						case cmd_standby : 
							if( addr.sin_addr.s_addr == dat->Standby[0].sin_addr.s_addr ){
								lastTime->Standby[0] = *nowTime;
								if( dat->Standby[1].sin_addr.s_addr ){
									WaitForSingleObject( dat->hMutex, INFINITE );
									SOCKADDR_IN addrTemp = dat->Standby[1];
									ReleaseMutex( dat->hMutex );
									if( addrTemp.sin_addr.s_addr ){
										*(SOCKADDR_IN*)&data[0] = addrTemp;
										*(WORD*)&data[ sizeof(addrTemp) ] = dat->stdbyPort[1];
										dat->SendCmdR( &addr, cmd_ready, data, sizeof(addrTemp) + 2 );
										
										*(SOCKADDR_IN*)&data[0] = addr;
										*(WORD*)&data[ sizeof(addr) ] = dat->stdbyPort[0];
										dat->SendCmdR( &addrTemp, cmd_ready, data, sizeof(addr) + 2 );
									}
								}else{
									dat->SendCmdR( &addr, res_continue );
								}
							}else if( addr.sin_addr.s_addr == dat->Standby[1].sin_addr.s_addr ){
								lastTime->Standby[1] = *nowTime;
								if( dat->Standby[0].sin_addr.s_addr ){
									WaitForSingleObject( dat->hMutex, INFINITE );
									SOCKADDR_IN addrTemp = dat->Standby[0];
									ReleaseMutex( dat->hMutex );
									if( addrTemp.sin_addr.s_addr ){
										*(SOCKADDR_IN*)data = addrTemp;
										*(WORD*)&data[ sizeof(addrTemp) ] = dat->stdbyPort[0];
										dat->SendCmdR( &addr, cmd_ready, data, sizeof(addrTemp) + 2 );
										
										*(SOCKADDR_IN*)data = addr;
										*(WORD*)&data[ sizeof(addr) ] = dat->stdbyPort[1];
										dat->SendCmdR( &addrTemp, cmd_ready, data, sizeof(addr) + 2 );
									}
								}else{
									dat->SendCmdR( &addr, res_continue );
								}
							}else if( !(dat->Standby[0].sin_addr.s_addr) ){
								lastTime->Standby[0] = *nowTime;
								WaitForSingleObject( dat->hMutex, INFINITE );
								dat->Standby[0] = addr;
								dat->stdbyPort[0] = *(WORD*)recvData;
								ReleaseMutex( dat->hMutex );
								
							}else if( !(dat->Standby[1].sin_addr.s_addr) ){
								lastTime->Standby[1] = *nowTime;
								WaitForSingleObject( dat->hMutex, INFINITE );
								dat->Standby[1] = addr;
								dat->stdbyPort[1] = *(WORD*)recvData;
								ReleaseMutex( dat->hMutex );
							}
							break;
						case cmd_cast :
						case cmd_gameInfo :
						case cmd_continue :
							{
								int Counter;
								WORD Flg = 0;
								for(Counter = 0; Counter<20; Counter++){
									if( cmdExitTime[Counter][0] ){
										if( cmdExitTime[Counter][1] + 30000 < dat->nowTime ){
											cmdExitTime[Counter][0] = 0;
											cmdExitTime[Counter][1] = 0;
										}
									}
								}
								for(Counter = 0; Counter<20; Counter++){
									if( cmdExitTime[Counter][0] ){
										if( cmdExitTime[Counter][0] == addr.sin_addr.s_addr ){
											Flg = 0;
											break;
										}
									}else{
										Flg = 1;
									}
								}
								if( Flg ){
									dat->SendCmdR( &addr, cmd_exit );
									for(Counter = 0; Counter<20; Counter++){
										if( !cmdExitTime[Counter][0] ){
											cmdExitTime[Counter][0] = addr.sin_addr.s_addr;
											cmdExitTime[Counter][1] = dat->nowTime;
											break;
										}
									}
								}
							}
							break;
						case cmd_delayobs :
							dat->SendCmdR( &addr, res_delayobs, recvData, 4 );
							break;
						case cmd_echo :
							dat->SendCmdR( &addr, res_echo );
							break;
						case res_echo :
							dat->echoFlg.Other = 1;
							break;
						case cmd_access :
							if( dat->myInfo.terminalMode == mode_wait || (dat->myInfo.terminalMode == mode_wait_target && addr.sin_addr.s_addr == dat->waitTargetIP ) ){
								//接続待機中
								if( !(dat->Access.sin_addr.s_addr) && !(dat->Away.sin_addr.s_addr) ){
									//要検討
									{
										BYTE flg = GetCWflg( dat, recvData, size );
										switch( flg ){
										case 0 :
										case 1 :
											dat->newCWflg = flg;
											
											lastTime->Away = *nowTime;
											WaitForSingleObject( dat->hMutex, INFINITE );
											dat->Away = addr;
											ReleaseMutex( dat->hMutex );
											dat->accessFlg = status_ok;
											data[0] = status_ok;
											data[1] = 0;	//id[5]
											data[2] = 0;
											data[3] = 0;
											data[4] = 0;
											data[5] = 0;
											data[6] = 0;	//port[2]
											data[7] = 0;
											data[8] = 0;	//flg
											data[9] = flg;	//CW flg
											dat->SendCmdR( dest_away, res_access, data, 10 );
											break;
										case 0xF :
											//return error
											data[0] = status_error;
											dat->SendCmdR( &addr, res_access, data, 1 );
											break;
										}
									}
									break;
								}
							}else if( dat->myInfo.terminalMode == mode_access ){
								dat->SendCmd( &addr, res_continue );
							}else if( dat->myInfo.terminalMode == mode_default ){
								//none
							}else{
								//返信など
								BYTE enFlg = 0;
								
								if( size > 11 ){
									if( !recvData[5] ){
										enFlg = recvData[6];
									}else{
										BYTE preSize = 6;	//id[5] + flg[1]
										BYTE longNameLength;
										BYTE shortNameLength;
										
										//longname length
										longNameLength = recvData[ preSize ];
										
										//shortname length
										if( longNameLength < 100 ){
											shortNameLength = recvData[ preSize + 1 + longNameLength ];
										}
										
										if( longNameLength < 100 && shortNameLength < 50 ){
											if( size > 5 + preSize + 1 + longNameLength + 1 + shortNameLength ){
												enFlg = recvData[ preSize + 1 + longNameLength + 1 + shortNameLength ];
											}
										}
									}
								}
								if( ( dat->newCWflg && !enFlg ) || ( !(dat->newCWflg) && enFlg == 2 ) ){
									data[0] = status_error;
									dat->SendCmdR( &addr, res_access, data, 1 );
								}else{
									data[0] = status_bad;
									data[1] = 0;	//id[5]
									data[2] = 0;
									data[3] = 0;
									data[4] = 0;
									data[5] = 0;
									data[6] = 0;	//port[2]
									data[7] = 0;
									data[8] = 0;	//flg
									data[9] = (BYTE)(dat->newCWflg);	//CW flg
									dat->SendCmdR( &addr, res_access, data, 10 );
								}
							}
							break;
						case cmd_dataInfo :
							//メニューの情報を送る
							dat->SendCmdR( &addr, res_dataInfo, &(dat->dataInfo), sizeof(dat->dataInfo), &(dat->myInfo.terminalMode), 1 );
							break;
						case cmd_join :
							{
								WORD usePort = 0;
								if( size > 6 ){
									usePort = *(WORD*)&recvData[0];
								}
								
								if( dat->obsCountFlg && ( dat->allowObsFlg || dat->myInfo.terminalMode != mode_root ) ){
									int Counter = 0;
									for(;;){
										if( dat->obsIP[Counter] ){
											if( dat->obsIP[Counter] == addr.sin_addr.s_addr ){
												break;
											}
										}else{
											dat->obsIP[Counter] = addr.sin_addr.s_addr;
											cout << "debug : Observer " << Counter + 1 << endl;
											
											if( Counter == 63 ){
												cout << "debug : Counter Limit" << endl;
											}
											break;
										}
										
										if( Counter >= 63 ) break;
										Counter++;
									}
								}
								
								if( !(dat->Leaf[0].sin_addr.s_addr) ){
									if( dat->myInfo.terminalMode == mode_branch || dat->allowObsFlg ){
										lastTime->Leaf[0] = *nowTime;
										WaitForSingleObject( dat->hMutex, INFINITE );
										dat->Leaf[0] = addr;
										ReleaseMutex( dat->hMutex );
										
										data[0] = status_ok;
										dat->SendCmdR( &addr, res_join, data, 1 );
									}
								}else if( dat->myInfo.terminalMode == mode_branch && !(dat->Leaf[1].sin_addr.s_addr) ){
									lastTime->Leaf[1] = *nowTime;
									WaitForSingleObject( dat->hMutex, INFINITE );
									dat->Leaf[1] = addr;
									ReleaseMutex( dat->hMutex );
									
									data[0] = status_ok;
									dat->SendCmdR( &addr, res_join, data, 1 );
								}else if( dat->myInfo.terminalMode == mode_branch && !(dat->Leaf[2].sin_addr.s_addr) ){
									lastTime->Leaf[2] = *nowTime;
									WaitForSingleObject( dat->hMutex, INFINITE );
									dat->Leaf[2] = addr;
									ReleaseMutex( dat->hMutex );
									
									data[0] = status_ok;
									dat->SendCmdR( &addr, res_join, data, 1 );
								}else if( dat->myInfo.terminalMode == mode_branch && !(dat->Leaf[3].sin_addr.s_addr) ){
									lastTime->Leaf[3] = *nowTime;
									WaitForSingleObject( dat->hMutex, INFINITE );
									dat->Leaf[3] = addr;
									ReleaseMutex( dat->hMutex );
									
									data[0] = status_ok;
									dat->SendCmdR( &addr, res_join, data, 1 );
								}else{
									if( addr.sin_addr.s_addr && TestIP( addr.sin_addr.s_addr ) && dat->Leaf[0].sin_addr.s_addr && TestIP( dat->Leaf[0].sin_addr.s_addr ) ){
										//要検討
										WaitForSingleObject( dat->hMutex, INFINITE );
										SOCKADDR_IN addrTemp = dat->Leaf[0];
										ReleaseMutex( dat->hMutex );
										dat->SendCmdR( &addr, cmd_addr_branch, &addrTemp, sizeof(addrTemp) );
									}else if( !TestIP( dat->Leaf[0].sin_addr.s_addr ) ){
										WaitForSingleObject( dat->hMutex, INFINITE );
										SOCKADDR_IN addrTemp = dat->Leaf[0];
										ReleaseMutex( dat->hMutex );
										dat->SendCmdR( &addr, cmd_addr_branch, &addrTemp, sizeof(addrTemp) );
										if( usePort ){
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr), &usePort, 2 );
										}else{
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr) );
										}
									}else if( !TestIP( dat->Leaf[1].sin_addr.s_addr ) ){
										WaitForSingleObject( dat->hMutex, INFINITE );
										SOCKADDR_IN addrTemp = dat->Leaf[1];
										ReleaseMutex( dat->hMutex );
										dat->SendCmdR( &addr, cmd_addr_branch, &addrTemp, sizeof(addrTemp) );
										if( usePort ){
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr), &usePort, 2 );
										}else{
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr) );
										}
									}else if( !TestIP( dat->Leaf[2].sin_addr.s_addr ) ){
										WaitForSingleObject( dat->hMutex, INFINITE );
										SOCKADDR_IN addrTemp = dat->Leaf[2];
										ReleaseMutex( dat->hMutex );
										dat->SendCmdR( &addr, cmd_addr_branch, &addrTemp, sizeof(addrTemp) );
										if( usePort ){
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr), &usePort, 2 );
										}else{
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr) );
										}
									}else if( !TestIP( dat->Leaf[3].sin_addr.s_addr ) ){
										WaitForSingleObject( dat->hMutex, INFINITE );
										SOCKADDR_IN addrTemp = dat->Leaf[3];
										ReleaseMutex( dat->hMutex );
										dat->SendCmdR( &addr, cmd_addr_branch, &addrTemp, sizeof(addrTemp) );
										if( usePort ){
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr), &usePort, 2 );
										}else{
											dat->SendCmdR( &addrTemp, cmd_addr_leaf, &addr, sizeof(addr) );
										}
									}else{
										data[0] = status_bad;
										dat->SendCmdR( &addr, res_join, data, 1 );
									}
								}
							}
							break;
						}
					}
				}else{
					//バージョンが違うとき
					if( addr.sin_addr.s_addr == dat->Access.sin_addr.s_addr ){
						dat->accessFlg = status_error;
					}
					if( recvBuf[0] == cmd_version_error ){
						if( addr.sin_addr.s_addr == dat->Root.sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Root), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
							
							cout << "ERROR : Caster Header Check ( Command Version / Games )" << endl;
							dat->roopFlg = 0;
							
						}else if( addr.sin_addr.s_addr == dat->Away.sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Away), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
							
							cout << "ERROR : Caster Header Check ( Command Version / Games )" << endl;
							dat->roopFlg = 0;
							
						}else if( addr.sin_addr.s_addr == dat->Leaf[0].sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Leaf[0]), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
						}else if( addr.sin_addr.s_addr == dat->Leaf[1].sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Leaf[1]), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
						}else if( addr.sin_addr.s_addr == dat->Leaf[2].sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Leaf[2]), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
						}else if( addr.sin_addr.s_addr == dat->Leaf[3].sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Leaf[3]), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
						}else if( addr.sin_addr.s_addr == dat->Access.sin_addr.s_addr ){
							WaitForSingleObject( dat->hMutex, INFINITE );
							memset( &(dat->Access), 0, sizeof(SOCKADDR_IN));
							ReleaseMutex( dat->hMutex );
							
							cout << "ERROR : Caster Header Check ( Command Version / Games )" << endl;
							dat->roopFlg = 0;
							
						}
					}else{
						data[0] = cmd_version_error;
						data[1] = cmd_version;	//071010で追加
						data[2] = cmd_casters;
						data[3] = cmd_space_2;
						data[4] = cmd_space_3;
						dat->SendDataR( &addr, data, 5 );
					}
				}
			}
			
		}
	}
	
	if( zBuf ){
		free( zBuf );
		zBuf = NULL;
		
		//警告消しのため
		if( zBuf ){}
	}
	
	#if debug_mode_thread
		WaitForSingleObject( dat->hPrintMutex, INFINITE );
		cout << "debug : rcvThread end" << endl;
		ReleaseMutex( dat->hPrintMutex );
	#endif
	return 0;
}
