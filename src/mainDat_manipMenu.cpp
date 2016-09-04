#include "mainDatClass.h"
using namespace std;

BYTE ManipMenuSub( charInfoStruct* now, charInfoStruct* dest){
	
	if(dest->phase > now->phase){
		switch( now->phase ){
		case 0 :
			//まずキャラの位置を合わせる
			if( dest->ID != now->ID ){
				if(dest->ID > now->ID){
					if( dest->ID > now->ID + 5 ){
						return key_left;
					}else{
						return key_right;
					}
				}else{
					if( dest->ID + 5 < now->ID ){
						return key_right;
					}else{
						return key_left;
					}
				}
			}else{
				//決定
				return key_A;
			}
		case 1 :
			if( dest->ID != now->ID ){
				return key_B;
			}
			if(dest->firstSpell != now->spellPlace){
				switch( dest->firstSpell ){
				case 0 : 
					return key_up;
				case 1 :
					return key_left;
				case 2 :
					return key_down;
				}
			}else{
				//決定
				return key_A;
			}
		case 2 :
			if( dest->ID != now->ID ){
				return key_B;
			}
			if( dest->firstSpell != now->firstSpell ){
				return key_B;
			}
			if(dest->secondSpell != now->spellPlace){
				switch( dest->secondSpell ){
				case 0 : 
					return key_up;
				case 1 :
					return key_left;
				case 2 :
					return key_down;
				}
			}else{
				//決定
				return key_A;
			}
		case 4 :
			//場所
			if( dest->ID != now->ID ){
				return key_B;
			}
			if( dest->firstSpell != now->firstSpell ){
				return key_B;
			}
			if(dest->secondSpell != now->secondSpell ){
				return key_B;
			}
			if(dest->place != now->place){
//				return key_right;
				
				if(dest->place > now->place){
					return key_right;
				}else{
					return key_left;
				}
			}else{
				if(dest->placeTime != now->placeTime){
					return key_D;
				}else{
					return key_A;
				}
			}
		}
	}
	
	if(dest->phase < now->phase){
		//キャンセル
		return key_B;
	}
	
	if(dest->phase == now->phase){
		//カーソル移動だけ
		switch( now->phase ){
		case 0 :
			//キャラ横方向
			if(dest->ID != now->ID){
				if(dest->ID > now->ID){
					if( dest->ID > now->ID + 5 ){
						return key_left;
					}else{
						return key_right;
					}
				}else{
					if( dest->ID + 5 < now->ID ){
						return key_right;
					}else{
						return key_left;
					}
				}
			}
			break;
		case 1 :
			if(dest->ID != now->ID){
				return key_B;
			}
			//スペル縦方向
			if(dest->spellPlace != now->spellPlace){
				switch( dest->spellPlace ){
				case 0 : 
					return key_up;
				case 1 :
					return key_left;
				case 2 :
					return key_down;
				}
			}
			break;
		case 2 :
			if( dest->ID != now->ID ){
				return key_B;
			}
			if( dest->firstSpell != now->firstSpell ){
				return key_B;
			}
			//スペル縦方向
			if(dest->spellPlace != now->spellPlace){
				switch( dest->spellPlace ){
				case 0 : 
					return key_up;
				case 1 :
					return key_left;
				case 2 :
					return key_down;
				}
			}
			break;
		case 4 :
			if( dest->ID != now->ID ){
				return key_B;
			}
			if( dest->firstSpell != now->firstSpell ){
				return key_B;
			}
			if(dest->secondSpell != now->secondSpell ){
				return key_B;
			}
			//場所
			if(dest->place != now->place){
				if(dest->place > now->place){
					if(dest->place > now->place + 4){
						return key_left;
					}else{
						return key_right;
					}
				}else{
					if(dest->place + 4 < now->place){
						return key_right;
					}else{
						return key_left;
					}
				}
			}else{
				if(dest->placeTime != now->placeTime){
					return key_D;
				}
			}
			break;
		}
	}
	
	return 0;
}



int mainDatClass::ManipMenu(){
	#if debug_mode_mainRoop
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "debug : ManipMenu()" << endl;
		ReleaseMutex( hPrintMutex );
	#endif
	
	memset( &enInfo, 0, sizeof(enInfo));
	
	DWORD menuAddress = 0;	//0x12FD14			//DWORD
	DWORD menuFlg = 0;	//0x12FE5C			//BYTE
	DWORD gameMode = 0;	//0x671418 + 0x218	//BYTE
	DWORD battleFlgA = 0;	//0x671418			//DWORD
	DWORD battleFlgB = 0;	//0x67141C			//DWORD
	
	WORD pushFlg = 0;
	BYTE InputA;
	BYTE InputB;
	
	myInfo.A.phase = 0;
	myInfo.B.phase = 0;
	
	enInfo.A.phase = 0;
	enInfo.B.phase = 0;
	
	dataInfo.A.phase = 0;
	dataInfo.B.phase = 0;
	
	datA.SetBodyInput( 0 );
	datB.SetBodyInput( 0 );
	
	myInfo.gameTime = 0;
	enInfo.gameTime = 0;
	dataInfo.gameTime = 0;
	
	//test
	DWORD menuAddressTemp = 0;
	
	//緩和
	DWORD roopCounter = 0;
	DWORD stageRandCounter = 0;
	DWORD roopCounterKey = 0;
	
	WORD roundShowFlg = 0;
	
	
	HWND hWnd = FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" );
	
	//ステージ選択の異常に応急措置
	WORD placeRecoverFlg = 0;
	
	//ステージ選択を自由に
	WORD placeFreeFlg = 0;
	
	DWORD keyCancelCounter = 0;
	
	DWORD deInfo;
	for(;;){
		if( th075Roop( &deInfo ) ) return 0xF;
		if( !roopFlg ){
			UnRockTime();
			return 1;
		}
		if( deInfo == de_body ){
			myInfo.phase = phase_menu;
			InputA = 0;
			InputB = 0;
//			ReadProcessMemory( hProcess, (void*)(0x12FE5C) , &menuFlg , 1 , NULL );
			ReadProcessMemory( hProcess, (void *)((DWORD)pStackBase - 420) , &menuFlg , 1 , NULL );
			ReadProcessMemory( hProcess, (void*)(0x671418 + 0x218) , &gameMode , 1 , NULL );
			ReadProcessMemory( hProcess, (void*)(0x671418) , &battleFlgA , 4 , NULL );
			ReadProcessMemory( hProcess, (void*)(0x67141C) , &battleFlgB , 4 , NULL );
			
			
			if(menuFlg != 3 && menuFlg != 1 && battleFlgA==0 && battleFlgB==0){
				datA.SetInput( 0 );
				datB.SetInput( 0 );
				return 1;
			}else{
				if( menuFlg == 3 ){
//					ReadProcessMemory( hProcess, (void*)(0x12FD14) , &menuAddress , 4 , NULL );
					ReadProcessMemory( hProcess, (void *)((DWORD)pStackBase - 748) , &menuAddress , 4 , NULL );
					ReadProcessMemory( hProcess, (void*)(0x671418 + 0x1F8) , &myInfo.A.firstSpell  ,1 ,NULL );
					ReadProcessMemory( hProcess, (void*)(0x671418 + 0x1F9) , &myInfo.B.firstSpell  ,1 ,NULL );
					ReadProcessMemory( hProcess, (void*)(0x671418 + 0xB4)  , &myInfo.A.secondSpell ,1 ,NULL );
					ReadProcessMemory( hProcess, (void*)(0x671418 + 0xB5)  , &myInfo.B.secondSpell ,1 ,NULL );
					
					if(menuAddress){
						if( !menuAddressTemp ) menuAddressTemp = menuAddress;
						if( menuAddress == menuAddressTemp ){
							ReadProcessMemory( hProcess, (void*)(menuAddress + 0x96), &myInfo.A.phase , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(menuAddress + 0x97), &myInfo.B.phase , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(menuAddress + 0xE0), &myInfo.A.ID , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(menuAddress + 0xE1), &myInfo.B.ID , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(menuAddress + 0xE6), &myInfo.A.spellPlace , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(menuAddress + 0xE7), &myInfo.B.spellPlace , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(0x6714AC), &myInfo.A.color , 1 , NULL );
							ReadProcessMemory( hProcess, (void*)(0x6714AD), &myInfo.B.color , 1 , NULL );
							
							if( placeRecoverFlg ){
								//manual
								myInfo.place = enInfo.place;
								myInfo.placeTime = enInfo.placeTime;
							}else{
								ReadProcessMemory( hProcess, (void*)(menuAddress + 0xF1), &myInfo.place , 1 , NULL );
								ReadProcessMemory( hProcess, (void*)(menuAddress + 0xF0), &myInfo.placeTime , 1 , NULL );
							}
							myInfo.A.place = myInfo.place;
							myInfo.B.place = myInfo.place;
							myInfo.A.placeTime = myInfo.placeTime;
							myInfo.B.placeTime = myInfo.placeTime;
							
							//debug
//							cout << (WORD)myInfo.place << " " << (WORD)myInfo.placeTime << endl;
							
						}else{
//							cout << "debug : menuAddress != menuAddressTemp" << endl;
						}
					}
				}
				
				if( myInfo.terminalMode == mode_broadcast || myInfo.terminalMode == mode_root ){
					dataInfo = myInfo;
				}
				
				
				if( myInfo.terminalMode == mode_root || myInfo.terminalMode == mode_debug ){
					//ルートの場合
					if( enInfo.phase == phase_battle || enInfo.phase == phase_read ){
						enInfo.A.phase = 5;
						enInfo.B.phase = 5;
					}
					
					//データを要求
					if( myInfo.terminalMode == mode_debug ){
						enInfo = myInfo;
					}else{
						SendCmd( dest_away, cmd_gameInfo );
					}
					
					//sessionNoで対応
//					if( enInfo.gameTime > 2 ) return 1;
					
					//要検討
//					cout << menuFlg << " " << battleFlgA << " " << battleFlgB << endl;
					
					//ステージランダム後はしばらく決定できないようにする
					if( stageRandCounter ) stageRandCounter++;
					if( stageRandCounter > 80 ) stageRandCounter = 0;
					
					//何回戦ったかを表示
					if( !roundShowFlg ){
						if( GetKeyState( 221 ) < 0 ){	// ]
							//roundCount show
							cout << "debug : RoundCount " << (WORD)myInfo.sessionNo << endl;
							roundShowFlg = 1;
						}
					}
					
					//stageRecover
					if( myInfo.playerSide == 0xB ){
						if( !placeRecoverFlg ){
							if( GetKeyState( 219 ) < 0 ){	// [
								cout << "debug : Stage Manual" << endl;
								placeRecoverFlg = 1;
							}
						}
					}
					
					//free
					if( myInfo.playerSide == 0xA ){
						if( !placeFreeFlg ){
							if( GetKeyState( 219 ) < 0 ){	// [
								cout << "debug : Stage Free" << endl;
								placeFreeFlg = 1;
							}
						}
					}
					
					//delay
					if( !roopCounterKey ){
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
									delayTime = delayTimeNew;
									cout << "debug : Buffer Margin ( delayTime ) " << delayTime / 2 << endl;
								}
							}
						}
						
						roopCounterKey = 0;
					}
					roopCounterKey++;
					if( roopCounterKey > 5 ) roopCounterKey = 0;
					
					
					
					if(menuFlg == 3 && battleFlgA == 0 && battleFlgB == 0 ){
						if( myInfo.playerSide == 0xA ){
							InputA = datA.GetInput();
							
							if( myInfo.terminalMode == mode_debug ){
								if( myInfo.A.phase > 3 && myInfo.B.phase > 3 ){
									if( keyCancelCounter > 40 ){
										//none
									}else{
										InputA = 0;
										keyCancelCounter++;
									}
								}
							}
							
							
							//前面かどうか
							if( datA.inputDeviceType == 0xFF ){
								if( hWnd != GetForegroundWindow() ){
									InputA = 0;
								}
							}
							
							InputB = ManipMenuSub( &myInfo.B, &enInfo.B );
							if( myInfo.B.phase == 4 && InputB != key_B ){
								InputB = 0;
							}
							
							//stageRand
							if( myInfo.A.phase == 4 && myInfo.B.phase > 3 && InputA & key_C ){
								stageRandCounter = 1;
							}
							if( stageRandCounter && InputA & key_A && InputA != key_P ){
								InputA = 0;
							}
							
							if( !placeFreeFlg ){
								//いくつかの場所を選択不可にする
								if( myInfo.A.phase == 4 && InputA & key_A ){
									if( myInfo.place == 9	//幻想郷
									|| (myInfo.place == 5 && myInfo.placeTime == 0)		//大木のある墓地昼
									|| (myInfo.place == 2 && myInfo.placeTime == 0)	)	//時計台昼
									{
										InputA = 0;
									}
								}
							}
							
							if( myInfo.A.ID != enInfo.B.ID ){
								if( myInfo.B.color != enInfo.B.color ){
									if( enInfo.B.color == 0 || enInfo.B.color == 1 ){
										WriteProcessMemory( hProcess, (void*)0x6714AD, &enInfo.B.color, 1 , NULL );
									}
								}
							}
						}
						if( myInfo.playerSide == 0xB ){
							//テスト
							if( enInfo.A.place > 10 ){
								enInfo.A.place = 0;
								enInfo.A.placeTime = 0;
							}
							
							InputA = ManipMenuSub( &myInfo.A, &enInfo.A );
							
							InputB = datA.GetInput();
							
							//前面かどうか
							if( datB.inputDeviceType == 0xFF ){
								if( hWnd != GetForegroundWindow() ){
									InputB = 0;
								}
							}
							if( myInfo.B.phase == 4 && InputB != key_B ){
								if( !placeRecoverFlg ){
									InputB = 0;
								}else{
									if( InputB & key_A ){
										InputB = 0;
									}
								}
							}
						}
						
						if( ( enInfo.phase == phase_battle || enInfo.phase == phase_read && myInfo.A.phase == 4 && myInfo.B.phase == 4 ) && myInfo.sessionNo == enInfo.sessionNo ){
							//検証する
							if( myInfo.A.ID == enInfo.A.ID && myInfo.A.firstSpell == enInfo.A.firstSpell && myInfo.A.secondSpell == enInfo.A.secondSpell
							 && myInfo.B.ID == enInfo.B.ID && myInfo.B.firstSpell == enInfo.B.firstSpell && myInfo.B.secondSpell == enInfo.B.secondSpell
							 && myInfo.place == enInfo.place && myInfo.placeTime == enInfo.placeTime )
							{
								if( myInfo.playerSide == 0xB ){
									//Color
									//できれば入力で対応したいところ
									BYTE dataTemp;
									if( enInfo.A.color ){
										//2P
										dataTemp = 1;
										WriteProcessMemory( hProcess, (void*)0x6714AC, &dataTemp, 1 , NULL );
									}else{
										dataTemp = 0;
										WriteProcessMemory( hProcess, (void*)0x6714AC, &dataTemp, 1 , NULL );
									}
									if( enInfo.B.color ){
										dataTemp = 1;
										WriteProcessMemory( hProcess, (void*)0x6714AD, &dataTemp, 1 , NULL );
									}else{
										dataTemp = 0;
										WriteProcessMemory( hProcess, (void*)0x6714AD, &dataTemp, 1 , NULL );
									}
								}
								InputA = key_A;
							}
						}
					}else{
						datA.SetInput( 0 );
						datB.SetInput( 0 );
						
						if( myInfo.playerSide == 0xA ){
							break;
						}else{
							//検証する
							if( myInfo.A.ID == enInfo.A.ID && myInfo.A.firstSpell == enInfo.A.firstSpell && myInfo.A.secondSpell == enInfo.A.secondSpell
							 && myInfo.B.ID == enInfo.B.ID && myInfo.B.firstSpell == enInfo.B.firstSpell && myInfo.B.secondSpell == enInfo.B.secondSpell
							 && myInfo.place == enInfo.place && myInfo.placeTime == enInfo.placeTime )
							{
								//次のステップへ
								break;
							}else{
								return 1;
							}
						}
					}
					
					
					//連打修正
					if( pushFlg ){
						if( myInfo.playerSide == 0xA ) InputB = 0;
						if( myInfo.playerSide == 0xB ) InputA = 0;
					}
					
					//sessionNo
					if( myInfo.sessionNo != enInfo.sessionNo ){
						if( myInfo.playerSide == 0xA ){
							InputB = 0;
						}
						if( myInfo.playerSide == 0xB ){
							InputA = 0;
						}
					}
					
				}else if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch ){
					//転送の場合
					if( dataInfo.phase == phase_battle || dataInfo.phase == phase_read ){
						dataInfo.A.phase = 5;
						dataInfo.B.phase = 5;
					}
					
					
					//データを要求
					if( !roopCounter ){
//						SendCmd( dest_branch, cmd_dataInfo );
						SendCmd( dest_root, cmd_dataInfo );
					}
					roopCounter++;
					if( roopCounter > 6 ) roopCounter = 0;
					
					if( menuFlg == 3 && !battleFlgA && !battleFlgB ){
						InputA = ManipMenuSub( &myInfo.A, &dataInfo.A );
						InputB = ManipMenuSub( &myInfo.B, &dataInfo.B );
						
						if( myInfo.B.phase == 4 && InputB != key_B ){
							InputB = 0;
						}
						//検証する
						if( ( dataInfo.phase == phase_battle || dataInfo.phase == phase_read ) && ( myInfo.A.phase == 4 && myInfo.B.phase == 4 ) ){
							if( myInfo.A.ID == dataInfo.A.ID && myInfo.A.firstSpell == dataInfo.A.firstSpell && myInfo.A.secondSpell == dataInfo.A.secondSpell
							 && myInfo.B.ID == dataInfo.B.ID && myInfo.B.firstSpell == dataInfo.B.firstSpell && myInfo.B.secondSpell == dataInfo.B.secondSpell
							 && myInfo.place == dataInfo.place && myInfo.placeTime == dataInfo.placeTime )
							{
								BYTE dataTemp;
								if( dataInfo.A.color ){
									dataTemp = 1;
									WriteProcessMemory( hProcess, (void*)0x6714AC, &dataTemp, 1 , NULL );
								}else{
									dataTemp = 0;
									WriteProcessMemory( hProcess, (void*)0x6714AC, &dataTemp, 1 , NULL );
								}
								if( dataInfo.B.color ){
									dataTemp = 1;
									WriteProcessMemory( hProcess, (void*)0x6714AD, &dataTemp, 1 , NULL );
								}else{
									dataTemp = 0;
									WriteProcessMemory( hProcess, (void*)0x6714AD, &dataTemp, 1 , NULL );
								}
								InputA = key_A;
							}
						}
					}else{
						datA.SetInput( 0 );
						datB.SetInput( 0 );
						
						//検証する
						if( myInfo.A.ID == dataInfo.A.ID && myInfo.A.firstSpell == dataInfo.A.firstSpell && myInfo.A.secondSpell == dataInfo.A.secondSpell
						 && myInfo.B.ID == dataInfo.B.ID && myInfo.B.firstSpell == dataInfo.B.firstSpell && myInfo.B.secondSpell == dataInfo.B.secondSpell
						 && myInfo.place == dataInfo.place && myInfo.placeTime == dataInfo.placeTime )
						{
							//次のステップへ
							break;
						}else{
							return 1;
						}
					}
					
					//連打修正
					if( pushFlg ){
						InputA = 0;
						InputB = 0;
					}
				}else if( myInfo.terminalMode == mode_broadcast ){
					//ブロードキャストの場合
					if( menuFlg == 3 && !battleFlgA && !battleFlgB ){
						InputA = datA.GetInput();
						if( datA.inputDeviceType == 0xFF ){
							if( hWnd != GetForegroundWindow() ){
								InputA = 0;
							}
						}
						InputB = datB.GetInput();
						if( datB.inputDeviceType == 0xFF ){
							if( hWnd != GetForegroundWindow() ){
								InputB = 0;
							}
						}
					}else{
						//次のステップへ
						break;
					}
				}
				
				//debug
				if( myInfo.terminalMode == mode_debug){
					if( myInfo.A.phase > 2 && myInfo.B.phase < 3 ){
						if( keyCancelCounter > 20 ){
							InputB = datB.GetInput() | InputA;
							if( myInfo.B.phase == 0 && InputA & key_B ){
								//none
							}else{
								InputA = 0;
							}
						}else{
							keyCancelCounter++;
						}
					}else{
						if( myInfo.A.phase > 3 && myInfo.B.phase > 3 ){
							//none
						}else{
							InputB = datB.GetInput();
							keyCancelCounter = 0;
						}
					}
					if( !(myInfo.playerSide==0xB && myInfo.B.phase == 4) ){
						if( !pushFlg ){
							if( GetKeyState(VK_RETURN) < 0 ){
								InputA = key_A;
								InputB = key_A;
							}else{
								BYTE* myInput = 0;
								if( myInfo.playerSide == 0xA ){
									myInput = &InputA;
								}else if( myInfo.playerSide == 0xB ){
									myInput = &InputB;
								}
								if( myInput ){
									if( GetKeyState(VK_LEFT) < 0 ){
										*myInput = key_left;
									}else if( GetKeyState(VK_RIGHT) < 0 ){
										*myInput = key_right;
									}else if( GetKeyState(VK_UP) < 0 ){
										*myInput = key_up;
									}else if( GetKeyState(VK_DOWN) < 0 ){
										*myInput = key_down;
									}else if( GetKeyState( 90 ) < 0 ){
										*myInput = key_A;
									}else if( GetKeyState( 88 ) < 0 ){
										*myInput = key_B;
									}
								}
							}
						}
					}
				}
				
				if( pushFlg ){
					pushFlg--;
				}else{
					pushFlg = 5;
				}
				
			}
			datA.SetBodyInput( InputA );
			datB.SetBodyInput( InputB );
			
		}
	}
	return 0;
}
