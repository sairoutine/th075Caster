#include "mainDatClass.h"
using namespace std;

int mainDatClass::mainStep(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : mainStep()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	UnRockTime();
	
	/*
	CONTEXT ctFPU;
	ctFPU.ContextFlags = CONTEXT_FLOATING_POINT;
	GetThreadContext( hProcessTh, &ctFPU );
	cout << hex << "debug : FPU CW " << ctFPU.FloatSave.ControlWord << endl;
	*/
	
	
	DWORD deInfo;
	DWORD gameTime;
	DWORD gameTimeNoCast = 0;
	BYTE InputA;
	BYTE InputB = 0;
	BYTE InputABuf;
	BYTE InputBBuf;
	BYTE pauseFlg;
	DWORD FlgA;
	DWORD FlgB;
	BYTE	menuFlg;
	BYTE  sendBufBody[40];
	BYTE* sendBuf = &sendBufBody[6];
	BYTE  castBufBody[40];
	BYTE* castBufA = &castBufBody[6];
	BYTE* castBufB = &castBufBody[12];
	memset( sendBufBody, 0, sizeof( sendBufBody ) );
	memset( castBufBody, 0, sizeof( castBufBody ) );
	BYTE diInputA;
	BYTE diInputB;
	WORD fastFlg = 0;
	HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
	
	BYTE HPCountA = 0;
	BYTE HPCountB = 0;
	
	
	myInfo.phase = phase_battle;
	dataInfo.phase = phase_battle;
	
	BYTE sessionNo;
	BYTE sessionID;
	
	//要検討
	if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug || myInfo.terminalMode == mode_broadcast ){
		sessionNo = myInfo.sessionNo;
		sessionID = myInfo.sessionID;
	}else if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
		sessionNo = dataInfo.sessionNo;
		sessionID = dataInfo.sessionID;
	}
	
	sendBufBody[0] = sessionNo;
	sendBufBody[1] = sessionID;
	
	castBufBody[0] = sessionNo;
	castBufBody[1] = sessionID;
	
	DWORD menuAddress;
	BYTE gamePhase;
	
//	srand( 0 );
	{
		int sRand;
		if( newCWflg ){
			sRand = 0;
		}else{
			sRand = 1;
		}
		WriteMemory( (void*)rand_address, &sRand, 4 );
	}
	
	//緩和
	DWORD reqTime = 0;
	DWORD roopCounterReq = 0;
	DWORD roopCounterKey = 0;
	DWORD roopCounterSend = 0;
	
	//先読み実験
	DWORD preReqTime = 0;
	
	//sync
	DWORD HPA;
	DWORD HPB;
	DWORD XA;	//* 64 and float->DWORD
	DWORD XB;
	WORD deSyncShowFlg = 1;
	
	
	for(;;){
		if( th075Roop( &deInfo ) ) return 0xF;
		if( !roopFlg ){
			if( fastFlg ) WriteCode( (void *)0x66C23C, 16);
			UnRockTime();
			return 1;
		}
		if( deInfo == de_body ){
			//戦闘中の同期など
			ReadMemory( (void*)((DWORD)pStackBase - 420) , &menuFlg , 1 );	//12FE5C
			ReadMemory( (void*)0x6718B4, &gameTime, 4 );
			ReadMemory( (void*)0x67161C, &pauseFlg, 1 );
			ReadMemory( (void*)0x671418 , &FlgA , 4 );
			ReadMemory( (void*)0x67141C , &FlgB , 4 );
			ReadMemory( (void*)((DWORD)pStackBase - 936) , &menuAddress , 4 );	//12FC58
			if( FlgA && FlgB ){
				ReadMemory( (void*)(FlgA + 0x39D) , &HPCountA , 1 );
				ReadMemory( (void*)(FlgB + 0x39D) , &HPCountB , 1 );
			}else{
				HPCountA = 0;
				HPCountB = 0;
			}
			if( menuAddress ){
				ReadMemory( (void*)(menuAddress + 0x18) , &gamePhase , 1 );
			}else{
				gamePhase = 0;
			}
			
			//sync data
			if( gameTime > 100 && FlgA && FlgB ){
				float XATemp = 0;
				float XBTemp = 0;
				HPA = 0;
				HPB = 0;
				
				ReadMemory( (void*)(FlgA + 0x324) , &HPA , 2 );
				ReadMemory( (void*)(FlgA + 0x44) , &XATemp , 4 );
				
				ReadMemory( (void*)(FlgB + 0x324) , &HPB , 2 );
				ReadMemory( (void*)(FlgB + 0x44) , &XBTemp , 4 );
				
				XA = (DWORD)(XATemp * 64);
				XB = (DWORD)(XBTemp * 64);
				
				syncData.SetSyncDataHereA( gameTime, HPA, XA );
				syncData.SetSyncDataHereB( gameTime, HPB, XB );
			}
			
			if( !FlgA || !FlgB || menuFlg != 0 || gameTime==0 || !HPCountA || !HPCountB || gamePhase == 3 ){
				
				//要検討
				//頻度減少とルート無しの観戦への改造の関係で、最後で途切れないようにするため
				if( lessCastFlg ){
					if( !HPCountA || !HPCountB ){
						if( gameTimeNoCast >= 8 ){
							inputData.GetInputData( sessionNo, gameTimeNoCast - 8, 0xA, &castBufA[5] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 6, 0xA, &castBufA[4] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 4, 0xA, &castBufA[3] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 2, 0xA, &castBufA[2] );
							inputData.GetInputData( sessionNo, gameTimeNoCast, 0xA, &castBufA[1] );
							inputData.GetInputData( sessionNo, gameTimeNoCast + 2, 0xA, &castBufA[0] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 8, 0xB, &castBufB[5] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 6, 0xB, &castBufB[4] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 4, 0xB, &castBufB[3] );
							inputData.GetInputData( sessionNo, gameTimeNoCast - 2, 0xB, &castBufB[2] );
							inputData.GetInputData( sessionNo, gameTimeNoCast, 0xB, &castBufB[1] );
							inputData.GetInputData( sessionNo, gameTimeNoCast + 2, 0xB, &castBufB[0] );
							*(DWORD*)&castBufBody[2] = gameTimeNoCast + 2;
							SendCmd( dest_leaf, cmd_cast, castBufBody, 18 );
						}
					}
				}
				
				if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
					//観戦しててタイムアウトしたときはroopFlgが1のままのため
					if( !Root.sin_addr.s_addr ){
						roopFlg = 0;
					}
				}
				
				//send message
				if( ( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug ) && uMsg ){
					if( !HPCountA || !HPCountB ){
						if( ( myInfo.playerSide == 0xA || myInfo.playerSide == 0xB )
						 && !( HPCountA && HPCountB )
						 && !( !HPCountA && !HPCountB ) )
						{
							BYTE wParamArray[4];
							*(DWORD*)wParamArray = 0;
							charInfoStruct* mySideInfo;
							charInfoStruct* enSideInfo;
							
							if( myInfo.playerSide == 0xA ){
								mySideInfo = &myInfo.A;
								enSideInfo = &myInfo.B;
							}else{
								mySideInfo = &myInfo.B;
								enSideInfo = &myInfo.A;
							}
							
							BYTE mySide;
							BYTE myCount;
							BYTE enCount;
							if( myInfo.playerSide == 0xA ){
								mySide = 1;
								myCount = HPCountA;
								enCount = HPCountB;
							}else{
								mySide = 2;
								myCount = HPCountB;
								enCount = HPCountA;
							}
							
							
							//wParamArrayの内容はここから
							
							//win/lose
							if( myCount ){
								//win
								wParamArray[0] &= 0x7F;
								
								if( myCount == 2 ){
									//2-0
									wParamArray[0] &= 0xFE;
								}else{
									//2-1
									wParamArray[0] |= 0x01;
								}
							}else{
								//lose
								wParamArray[0] |= 0x80;
								
								if( enCount == 2 ){
									//0-2
									wParamArray[0] &= 0xFE;
								}else{
									//1-2
									wParamArray[0] |= 0x01;
								}
								
								
							}
							
							//playerSide
							wParamArray[1] = mySide;
							
							//mySide Info
							wParamArray[2] = mySideInfo->ID;
							wParamArray[2] |= mySideInfo->firstSpell << 4;
							wParamArray[2] |= mySideInfo->secondSpell << 6;
							
							//enSide Info
							wParamArray[3] = enSideInfo->ID;
							wParamArray[3] |= enSideInfo->firstSpell << 4;
							wParamArray[3] |= enSideInfo->secondSpell << 6;
							
							if( !( myInfo.terminalMode == mode_debug && boosterFlg ) ){
								PostMessage( HWND_BROADCAST, uMsg, *(WPARAM*)wParamArray, 0 );
							}
							
							if( myInfo.terminalMode == mode_debug && boosterFlg == 0 ){
								cout  << hex << "debug : th075_result " << (WORD)wParamArray[0] << ","
									<< (WORD)wParamArray[1] << "," << (WORD)wParamArray[2] << "," << (WORD)wParamArray[3] << "," << endl;
							}
						}
					}
				}
				
				//要検討
				myInfo.phase = phase_default;
				dataInfo.phase = phase_default;
				
				UnRockTime();
				datA.SetInput( diInputA );
				datB.SetInput( diInputB );
				break;
			}
			
			if( gameTime > 800 && !(gameTime%20) ){
				int Res;
				Res = syncData.TestSyncData( gameTime - 200 );
				
				/*
				if( Res == 0 ){
					cout << "debug : Sync Good " << Res << endl;
				}
				if( Res == 1 || Res == 2 || Res == 3 ){
					//相手が同期チェックに対応してないときなど
					cout << "debug : sync error " << Res << endl;
				}
				*/
				if( Res == 0xF ){
					if( deSyncShowFlg ){
						deSyncShowFlg = 0;
						
						cout << "ERROR : deSync" << endl;
						if( deSyncSoundFlg ){
							if( strcmp( nowDir, "fail" ) && strlen( nowDir ) < 180 ){
								char path[200];
								strcpy( path, nowDir );
								strcat( path, "\\desync.wav\0" );
								if( !_access( path, 0 ) ){
									PlaySound( path, NULL, SND_ASYNC );
								}else if( deSyncSoundFlg == 2 ){
									strcpy( path, nowDir );
									strcat( path, "\\sound.wav\0" );
									if( !_access( path, 0 ) ){
										PlaySound( path, NULL, SND_ASYNC );
									}
								}
							}
						}
//						roopFlg = 0;
					}
				}
			}
			
			
			if( (DWORD)( gameTime / 2 ) * 2 != gameTime ) gameTime--;
			if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_broadcast || myInfo.terminalMode == mode_debug ){
				dataInfo.gameTime = gameTime;
				inputData.SetTime( sessionNo, gameTime );
			}
			myInfo.gameTime = gameTime;
			
			//diInput
			InputA = 0;
			InputB = 0;
			diInputA = datA.GetInput();
			diInputB = datB.GetInput();
			if( diInputA == key_P ){
				if( datA.inputDeviceType == 0xFF ){
					if( hWnd != GetForegroundWindow() ){
						diInputA = 0;
					}
				}
			}
			if( diInputB == key_P ){
				if( datB.inputDeviceType == 0xFF ){
					if( hWnd != GetForegroundWindow() ){
						diInputB = 0;
					}
				}
			}
			if( diInputA == key_P ){
				if( gameTime <= 400 ){
					diInputA = 0;
				}
			}
			if( diInputB == key_P ){
				if( gameTime <= 400 ){
					diInputB = 0;
				}
			}
			
			if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
				//早送り
				if( rockFlg ){
					if( fastFlg ){
						WriteCode( (void *)0x66C23C, 16);
						fastFlg = 0;
					}
				}else{
					if( fastFlg ){
						if( gameTime + 30 > inputData.GetTime( sessionNo ) ){
							WriteCode( (void *)0x66C23C, 16);
							fastFlg = 0;
						}else if( gameTime + 120 < inputData.GetTime( sessionNo ) ){
							WriteCode( (void *)0x66C23C, 8);
							fastFlg = 1;
						}
					}else{
						if( gameTime + 120 < inputData.GetTime( sessionNo ) ){
							WriteCode( (void *)0x66C23C, 8);
							fastFlg = 1;
						}else if( gameTime + 60 < inputData.GetTime( sessionNo ) ){
							WriteCode( (void *)0x66C23C, 14);
							fastFlg = 1;
						}
					}
				}
			}
			
			//先読み
			//要検討
			if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
				if( gameTime >= preReqTime + 60 ){
					if( inputData.GetTime( sessionNo ) >= gameTime + 300 ){
						if( !inputData.GetInputDataA( sessionNo, gameTime + 2, NULL ) && !inputData.GetInputDataB( sessionNo, gameTime + 2, NULL ) ){
							if( inputData.GetInputDataA( sessionNo, gameTime + 62, NULL ) && inputData.GetInputDataB( sessionNo, gameTime + 62, NULL ) ){
								//送信
								if( zlibFlg ){
									BYTE data[7];
									data[0] = sessionNo;
									data[1] = sessionID;
									*(DWORD*)&data[2] = gameTime + 62;
									data[6] = 0;	//仕様を拡張するため
									SendCmd( dest_root, cmd_inputdata_req, data, 7 );
								}else{
									BYTE data[6];
									data[0] = sessionNo;
									data[1] = sessionID;
									*(DWORD*)&data[2] = gameTime + 62;
									SendCmd( dest_root, cmd_inputdata_req, data, 6 );
								}
								
								preReqTime = gameTime;
								
//								cout << "debug : preReqTime " << preReqTime << endl;
							}
						}
					}
				}
			}
			
			//bodyInput
			if( gameTime <= 200 ){
				diInputA = 0;
				diInputB = 0;
			}
			
			if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug ){
				if( myInfo.terminalMode == mode_debug && boosterFlg ){
					inputData.SetInputData( sessionNo, gameTime + 2, 0xB, boosterInput );
				}
				
				if( inputData.GetInputData( sessionNo, gameTime + delayTime + 2, myInfo.playerSide, NULL ) ){
					inputData.SetInputData( sessionNo, gameTime + delayTime + 2, myInfo.playerSide, diInputA );
					
					//データ送信
					if( gameTime + delayTime - 8 >= 0 ){
						inputData.GetInputData( sessionNo, gameTime + delayTime - 8, myInfo.playerSide, &sendBuf[5] );
					}else{
						sendBuf[5] = 0;
					}
					if( gameTime + delayTime - 6 >= 0 ){
						inputData.GetInputData( sessionNo, gameTime + delayTime - 6, myInfo.playerSide, &sendBuf[4] );
					}else{
						sendBuf[4] = 0;
					}
					if( gameTime + delayTime - 4 >= 0 ){
						inputData.GetInputData( sessionNo, gameTime + delayTime - 4, myInfo.playerSide, &sendBuf[3] );
					}else{
						sendBuf[3] = 0;
					}
					if( gameTime + delayTime - 2 >= 0 ){
						inputData.GetInputData( sessionNo, gameTime + delayTime - 2, myInfo.playerSide, &sendBuf[2] );
					}else{
						sendBuf[2] = 0;
					}
					if( gameTime + delayTime >= 0 ){
						inputData.GetInputData( sessionNo, gameTime + delayTime , myInfo.playerSide, &sendBuf[1] );
					}else{
						sendBuf[1] = 0;
					}
					inputData.GetInputData( sessionNo, gameTime + delayTime + 2, myInfo.playerSide, &sendBuf[0] );
					
					*(DWORD*)&sendBufBody[2] = gameTime + delayTime + 2;
					
					if( myInfo.terminalMode == mode_debug && boosterFlg ){
						//none
						//vs th075Booster
					}else{
						if( gameTime%20 ){
							SendCmd( dest_away, cmd_sendinput, sendBufBody, 12 );
						}else{
							//send with syncData
//							cout << "debug : send syncData" << endl;
							*(DWORD*)&sendBufBody[12] = gameTime;
							*(DWORD*)&sendBufBody[16] = HPA;
							*(DWORD*)&sendBufBody[20] = XA;
							*(DWORD*)&sendBufBody[24] = HPB;
							*(DWORD*)&sendBufBody[28] = XB;
							
							SendCmd( dest_away, cmd_sendinput, sendBufBody, 32 );
						}
					}
				}
			}else if( myInfo.terminalMode == mode_broadcast ){
				if( inputData.GetInputData( sessionNo, gameTime + delayTimeA + 2, 0xA, NULL ) ){
					inputData.SetInputData( sessionNo, gameTime + delayTimeA + 2, 0xA, diInputA );
				}
				if( inputData.GetInputData( sessionNo, gameTime + delayTimeB + 2, 0xB, NULL ) ){
					inputData.SetInputData( sessionNo, gameTime + delayTimeB + 2, 0xB, diInputB );
				}
			}
			
			
			if( !roopCounterKey ){
				//戦闘終了後に自動で進む
				if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch || myInfo.terminalMode == mode_debug ){
					if( GetKeyState( 219 ) < 0 ){	//[
						if( autoNextFlg ){
							if( hWnd == GetForegroundWindow() ){
								cout << "debug : AutoNext OFF" << endl;
								autoNextFlg = 0;
							}
						}
					}else if( GetKeyState( 221 ) < 0 ){	//]
						if( !autoNextFlg ){
							if( hWnd == GetForegroundWindow() ){
								cout << "debug : AutoNext ON" << endl;
								autoNextFlg = 1;
							}
						}
					}
				}
				
				//delayTime変更
				if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug ){
					WORD delayTimeNew = 0;
					if( GetKeyState( '1' ) < 0 ) delayTimeNew = 2;
					if( GetKeyState( '2' ) < 0 ) delayTimeNew = 4;
					if( GetKeyState( '3' ) < 0 ) delayTimeNew = 6;
					if( GetKeyState( '4' ) < 0 ) delayTimeNew = 8;
					if( GetKeyState( '5' ) < 0 ) delayTimeNew = 10;
					if( GetKeyState( '6' ) < 0 ) delayTimeNew = 12;
					if( GetKeyState( '7' ) < 0 ) delayTimeNew = 14;
					if( GetKeyState( '8' ) < 0 ) delayTimeNew = 16;
					if( GetKeyState( '9' ) < 0 ) delayTimeNew = 18;
					if( GetKeyState( '0' ) < 0 ) delayTimeNew = 20;
					
					if( delayTimeNew ){
						if( hWnd == GetForegroundWindow() ){
							if( delayTime != delayTimeNew ){
								if( delayTime < delayTimeNew ){
									//データの空きを埋める必要がある
									WORD delayTimeStep;
									for(delayTimeStep = delayTime; delayTimeStep <= delayTimeNew; delayTimeStep += 2){
										if( inputData.GetInputData( sessionNo, gameTime + delayTimeStep + 2, myInfo.playerSide, NULL ) ){
											inputData.SetInputData( sessionNo, gameTime + delayTimeStep + 2, myInfo.playerSide, diInputA );
										}
									}
									delayTime = delayTimeNew;
									
									//データ送信
									if( gameTime + delayTime - 8 >= 0 ){
										inputData.GetInputData( sessionNo, gameTime + delayTime - 8, myInfo.playerSide, &sendBuf[5] );
									}else{
										sendBuf[5] = 0;
									}
									if( gameTime + delayTime - 6 >= 0 ){
										inputData.GetInputData( sessionNo, gameTime + delayTime - 6, myInfo.playerSide, &sendBuf[4] );
									}else{
										sendBuf[4] = 0;
									}
									if( gameTime + delayTime - 4 >= 0 ){
										inputData.GetInputData( sessionNo, gameTime + delayTime - 4, myInfo.playerSide, &sendBuf[3] );
									}else{
										sendBuf[3] = 0;
									}
									if( gameTime + delayTime - 2 >= 0 ){
										inputData.GetInputData( sessionNo, gameTime + delayTime - 2, myInfo.playerSide, &sendBuf[2] );
									}else{
										sendBuf[2] = 0;
									}
									if( gameTime + delayTime >= 0 ){
										inputData.GetInputData( sessionNo, gameTime + delayTime , myInfo.playerSide, &sendBuf[1] );
									}else{
										sendBuf[1] = 0;
									}
									inputData.GetInputData( sessionNo, gameTime + delayTime + 2, myInfo.playerSide, &sendBuf[0] );
									
									*(DWORD*)&sendBufBody[2] = gameTime + delayTime + 2;
									SendCmd( dest_away, cmd_sendinput, sendBufBody, 12 );
									
								}else{
									//none
									delayTime = delayTimeNew;
								}
								cout << "debug : Buffer Margin ( delayTime ) " << delayTime / 2 << endl;
							}
						}
					}
				}
			}
			roopCounterKey++;
			if( roopCounterKey > 5 ) roopCounterKey = 0;
			
			
			if( pauseFlg ){
				//ポーズ中
				//ポーズから復帰するとき進んでしまうが
				//UnRockしないと次の処理に行かないためしょうがない
				UnRockTime();
				if( fastFlg ){
					WriteCode( (void *)0x66C23C, 16);
					fastFlg = 0;
				}
				
				
				//要検討
				BYTE menuIndex;
				if( menuAddress ){
					ReadMemory( (void*)(menuAddress + 0x118) , &menuIndex , 1 );
				}else{
					menuIndex = 0;
				}
				
				if( menuIndex == 0 ){
					//ゲーム再開
					//none
				}else if( menuIndex == 1 ){
					//ゲーム終了
					if( diInputA != key_P ){
						if( diInputA & key_A || diInputB & key_A ){
							break;
						}
					}
				}else if( menuIndex == 2 ){
					//リプレイを保存して終了
					if( diInputA != key_P ){
						if( diInputA & key_A || diInputB & key_A ){
							break;
						}
					}
				}
				
			}else if( rockFlg ){
				//ロック中
				if( gameTime <= 200 ){
					UnRockTime();
				}else{
					if( diInputA == 0xFF || diInputB == 0xFF ){	//0xFF == key_P
						//ポーズにする
						//フォアグラウンドのときだけ入力
						//要検討
						//余分な入力が入る余地がある
						UnRockTime();
					}else{
						if( !inputData.GetInputDataA( sessionNo, gameTime + 2, NULL ) && !inputData.GetInputDataB( sessionNo, gameTime + 2, NULL ) ){
							//通常へ
							UnRockTime();
						}else{
							//データが届いていないとき
//							if( datA.GetInputBuf( gameTime + 2, &InputA ) ) cout << "error at a." << gameTime + 2 << endl;
//							if( datB.GetInputBuf( gameTime + 2, &InputB ) ) cout << "error at b." << gameTime + 2 << endl;
							
							if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug){
								BYTE mySide = 0;
								BYTE enSide = 0;
								if( myInfo.playerSide == 0xA ){
									mySide = 0xA;
									enSide = 0xB;
								}else if( myInfo.playerSide == 0xB ){
									mySide = 0xB;
									enSide = 0xA;
								}
								if( mySide ){
									if( inputData.GetInputData( sessionNo, gameTime + 2, enSide, NULL ) ){
										roopCounterSend++;
										if( roopCounterSend > 3 ){
											BYTE data[6];
											data[0] = sessionNo;
											data[1] = sessionID;
											*(DWORD*)&data[2] = gameTime + 2;
											SendCmd( dest_away, cmd_input_req, data, 6 );
											
											roopCounterSend = 0;
										}
									}
									if( inputData.GetInputData( sessionNo, gameTime + 2, mySide, NULL ) ){
										//破綻への対処
										cout << "ERROR : recover" << endl;
										inputData.SetInputData( sessionNo, gameTime + 2, mySide, 0 );
									}
								}
							}else if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
								
								//観戦しててタイムアウトしたときはroopFlgが1のままのため
								if( !Root.sin_addr.s_addr ){
									roopFlg = 0;
								}else{
									if( reqTime == gameTime + 2 ){
										if( roopCounterReq > 15 ){
											if( zlibFlg ){
												BYTE data[7];
												data[0] = sessionNo;
												data[1] = sessionID;
												*(DWORD*)&data[2] = gameTime + 2;
												data[6] = 0;	//仕様を拡張するため
												SendCmd( dest_root, cmd_inputdata_req, data, 7 );
											}else{
												BYTE data[6];
												data[0] = sessionNo;
												data[1] = sessionID;
												*(DWORD*)&data[2] = gameTime + 2;
												SendCmd( dest_root, cmd_inputdata_req, data, 6 );
											}
											
											reqTime = gameTime + 2;
											roopCounterReq = 0;
											
											
//											cout << "debug : cmd_inputdata_req " << gameTime + 2 << endl;
										}
										roopCounterReq++;
									}else{
										/*
										if( zlibFlg ){
											BYTE data[7];
											data[0] = sessionNo;
											data[1] = sessionID;
											*(DWORD*)&data[2] = gameTime + 2;
											data[6] = 0;	//仕様を拡張するため
											SendCmd( dest_root, cmd_inputdata_req, data, 7 );
										}else{
											BYTE data[6];
											data[0] = sessionNo;
											data[1] = sessionID;
											*(DWORD*)&data[2] = gameTime + 2;
											SendCmd( dest_root, cmd_inputdata_req, data, 6 );
										}
										
										reqTime = gameTime + 2;
										roopCounterReq = 0;
										
										cout << "debug : cmd_inputdata_req " << gameTime + 2 << endl;
										*/
										
										//応急措置
										reqTime = gameTime + 2;
									}
								}
							}else if( myInfo.terminalMode == mode_broadcast ){
								//破綻への対処
								inputData.SetInputDataA( sessionNo, gameTime + 2, diInputA );
								inputData.SetInputDataB( sessionNo, gameTime + 2, diInputB );
							}
						}
					}
				}
			}else{
				if( diInputA == 0xFF || diInputB == 0xFF ){
					//none
				}else{
					if( !inputData.GetInputDataA( sessionNo, gameTime + 2, NULL ) && !inputData.GetInputDataB( sessionNo, gameTime + 2, NULL ) ){
						//none
					}else{
						if( gameTime > 200 ){
							if( perfectFreezeFlg == 2 || ( perfectFreezeFlg == 1 && ( myInfo.A.ID == 8 || myInfo.B.ID == 8 ) ) ){
//								cout << "debug : PF" << endl;
								int Counter = 0;
								for(;;){
									if( !roopFlg ){
										if( fastFlg ) WriteCode( (void *)0x66C23C, 16);
										UnRockTime();
										return 1;
									}
									if( fastFlg ){
										WriteCode( (void *)0x66C23C, 16);
										fastFlg = 0;
									}
									
									if( myInfo.terminalMode == mode_root ){
										WaitForSingleObject( hInputRecvEvent, 16 );
									}else{
										Sleep(20);
									}
									
									if( !inputData.GetInputDataA( sessionNo, gameTime + 2, NULL )
									 && !inputData.GetInputDataB( sessionNo, gameTime + 2, NULL ) ){
										break;
									}
									if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug){
										if( Counter > 4 ){
											BYTE mySide = 0;
											BYTE enSide = 0;
											if( myInfo.playerSide == 0xA ){
												mySide = 0xA;
												enSide = 0xB;
											}else if( myInfo.playerSide == 0xB ){
												mySide = 0xB;
												enSide = 0xA;
											}
											if( mySide ){
												if( inputData.GetInputData( sessionNo, gameTime + 2, enSide, NULL ) ){
													roopCounterSend++;
													if( roopCounterSend > 3 ){
														BYTE data[6];
														data[0] = sessionNo;
														data[1] = sessionID;
														*(DWORD*)&data[2] = gameTime + 2;
														SendCmd( dest_away, cmd_input_req, data, 6 );
														
														roopCounterSend = 0;
													}
												}
												if( inputData.GetInputData( sessionNo, gameTime + 2, mySide, NULL ) ){
													//破綻への対処
													cout << "ERROR : recover" << endl;
													inputData.SetInputData( sessionNo, gameTime + 2, mySide, 0 );
												}
											}
											Counter = 0;
										}
									}else if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
										if( Counter > 30 ){
											//観戦しててタイムアウトしたときはroopFlgが1のままのため
											if( !Root.sin_addr.s_addr ){
												roopFlg = 0;
											}else{
												if( zlibFlg ){
													BYTE data[7];
													data[0] = sessionNo;
													data[1] = sessionID;
													*(DWORD*)&data[2] = gameTime + 2;
													data[6] = 0;	//仕様を拡張するため
													SendCmd( dest_root, cmd_inputdata_req, data, 7 );
												}else{
													BYTE data[6];
													data[0] = sessionNo;
													data[1] = sessionID;
													*(DWORD*)&data[2] = gameTime + 2;
													SendCmd( dest_root, cmd_inputdata_req, data, 6 );
												}
											}
											Counter = 0;
										}
									}else if( myInfo.terminalMode == mode_broadcast ){
										//破綻への対処
										inputData.SetInputDataA( sessionNo, gameTime + 2, diInputA );
										inputData.SetInputDataB( sessionNo, gameTime + 2, diInputB );
										break;
									}
									Counter++;
								}
							}else{
								RockTime();
							}
						}else{
							//none
						}
					}
				}
			}
			
			if( rockFlg ){
				diInputA = 0;
				diInputB = 0;
			}
			
			//bodyの入力
			datA.SetBodyInput( diInputA );
			datB.SetBodyInput( diInputB );
			
		}else if( deInfo == de_char ){
			//charの入力
			ReadMemory( (void*)0x6718B4, &gameTime, 4 );
			if( (DWORD)( gameTime / 2 ) * 2 != gameTime ) gameTime--;
			
			//test
			roopCounterSend = 0;
			roopCounterReq = 0;
			
			/*
			{
				//debug
				BYTE stateA;
				BYTE stateB;
				ReadMemory( (void*)( FlgA + 0x60 ) , &stateA , 1 );
				ReadMemory( (void*)( FlgB + 0x60 ) , &stateB , 1 );
				cout << gameTime << hex << " : " << (WORD)stateA << " " << (WORD)stateB << endl;
			}
			*/
			
			//対処（ポーズの後など）
			if( gameTime > 200 ){
				if( inputData.GetInputData( sessionNo, gameTime + 2, 0xA, NULL ) ){
					BYTE inputTemp;
					inputData.GetInputData( sessionNo, gameTime, 0xA, &inputTemp );
					inputData.SetInputData( sessionNo, gameTime + 2, 0xA, inputTemp );
					cout << "ERROR : inputData check" << endl;
				}
				if( inputData.GetInputData( sessionNo, gameTime + 2, 0xB, NULL ) ){
					BYTE inputTemp;
					inputData.GetInputData( sessionNo, gameTime, 0xB, &inputTemp );
					inputData.SetInputData( sessionNo, gameTime + 2, 0xB, inputTemp );
					cout << "ERROR : inputData check" << endl;
				}
			}
			
			//観戦データ送信頻度減少
			//現状では環境が悪い
			if( !lessCastFlg || gameTime / 2 & 1 ){
				
				//発信
				//side A
				if( gameTime - 8 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 8, 0xA, &castBufA[5] );
				}else{
					castBufA[5] = 0;
				}
				if( gameTime - 6 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 6, 0xA, &castBufA[4] );
				}else{
					castBufA[4] = 0;
				}
				if( gameTime - 4 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 4, 0xA, &castBufA[3] );
				}else{
					castBufA[3] = 0;
				}
				if( gameTime - 2 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 2, 0xA, &castBufA[2] );
				}else{
					castBufA[2] = 0;
				}
				if( gameTime >= 0 ){
					inputData.GetInputData( sessionNo, gameTime, 0xA, &castBufA[1] );
				}else{
					castBufA[1] = 0;
				}
				inputData.GetInputData( sessionNo, gameTime + 2, 0xA, &castBufA[0] );
				
				//side B
				if( gameTime - 8 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 8, 0xB, &castBufB[5] );
				}else{
					castBufB[5] = 0;
				}
				if( gameTime - 6 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 6, 0xB, &castBufB[4] );
				}else{
					castBufB[4] = 0;
				}
				if( gameTime - 4 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 4, 0xB, &castBufB[3] );
				}else{
					castBufB[3] = 0;
				}
				if( gameTime - 2 >= 0 ){
					inputData.GetInputData( sessionNo, gameTime - 2, 0xB, &castBufB[2] );
				}else{
					castBufB[2] = 0;
				}
				if( gameTime >= 0 ){
					inputData.GetInputData( sessionNo, gameTime, 0xB, &castBufB[1] );
				}else{
					castBufB[1] = 0;
				}
				inputData.GetInputData( sessionNo, gameTime + 2, 0xB, &castBufB[0] );
				*(DWORD*)&castBufBody[2] = gameTime + 2;
				SendCmd( dest_leaf, cmd_cast, castBufBody, 18 );
			}else{
				gameTimeNoCast = gameTime;
			}
			
			//入力
			if( gameTime <= 200 ){
				InputA = 0;
				InputB = 0;
			}else{
				inputData.GetInputDataA( sessionNo, gameTime + 2, &InputA );
				inputData.GetInputDataB( sessionNo, gameTime + 2, &InputB );
			}
			datA.SetCharInput( InputA );
			datB.SetCharInput( InputB );
			
			/*
			{
				//debug
				int inputTemp[6];
				ReadMemory( (void*)0x671418 , &FlgA , 4 );
				ReadMemory( (void*)(FlgA + 0x4C0), inputTemp, 24 );
				cout	<< gameTime + 2 << " : " << inputTemp[0] << " " << inputTemp[1] << " "
					<< inputTemp[2] << " " << inputTemp[3] << " " << inputTemp[4] << " " << inputTemp[5] << endl;
			}
			*/
			/*
			{
				//debug
				int inputTemp[6];
				ReadMemory( (void*)0x67141C , &FlgB , 4 );
				ReadMemory( (void*)(FlgB + 0x4C0), inputTemp, 24 );
				cout	<< gameTime + 2 << " : " << inputTemp[0] << " " << inputTemp[1] << " "
					<< inputTemp[2] << " " << inputTemp[3] << " " << inputTemp[4] << " " << inputTemp[5] << endl;
			}
			*/
		}
	}
	if( fastFlg ) WriteCode( (void *)0x66C23C, 16);
	UnRockTime();
	
	myInfo.A.phase = 0;
	myInfo.B.phase = 0;
	
	dataInfo.A.phase = 0;
	dataInfo.B.phase = 0;
	
	dataInfo.phase = phase_default;
	
	return 0;
}

