#include "conf.h"
#include "boosterDatClass.h"
#include "HL.h"

void boosterDatClass::ReadLNAI(){
//	if(eigenValueLN[0][0] && *enGameInfo[ _status ][5][0] != 0){
	if(eigenValueLN[0][0] && *enGameInfo[ _status ][5][0] != 0 && *enGameInfo[ _status ][0][0] != 2 && *enGameInfo[ _status ][0][0] != 9){	//������
		
		//�b��
		//�{����ManageAI���s���ׂ�����
		BYTE	commandTemp = 0;
		DWORD	decodeTimeTemp = 0;
		
		
		//�����O�s��������
		Address = LNAIbase + eigenValueLN[0][3];
		for(Counter=1000; Counter<50000; Counter+=1000){
			Address2 = Address + Counter;
			if(*(Address2) == (BYTE)myGameInfo[ _status ][5][0]){
				Counter = 50000;
			}
			if(Counter == 49000 || *(Address2)==0){	//hit���Ă�����50000�ɂȂ��Ă��锤
				Address2 = 0;
				Counter = 50000;
			}
		}
		if(Address2){
			for(Line=0; Line<20; Line++){
				if(LNAIBuf[Line][0][0] && LNAIBuf[Line][0][0] != 0xFF){
					if(LNAIBuf[Line][1][0] == 4 || LNAIBuf[Line][1][0] == 5 || LNAIBuf[Line][1][0] == 8){
						for(Counter2=20; Counter2<1000; Counter2+=20){
							Address3 = Address2 + Counter2;
							if(*(Address3) == 0){
								Counter2 = 1000;
							}else{
								if(*(Address3) == (BYTE)LNAIBuf[Line][2][0]){
									Counter2 = 1000;
									Address4 = Address3 + 4;
									if(*Address4 && (GetL(Address4 + 2) > 0xA || *(Address3 + 2)==0)){
										if(*(Address4 + 1) == (BYTE)((gameTime - LNAIBuf[Line][3][0]) / 10)){
											commandTemp = *Address4;
											decodeTimeTemp = 0;
										}
									}else{
										if(*(Address3 + 2)
										&& (LNAIBuf[Line][1][0]== 3 || LNAIBuf[Line][1][0]== 4 || LNAIBuf[Line][1][0]== 5
										 || LNAIBuf[Line][1][0]== 7 || LNAIBuf[Line][1][0]== 8)){
											if(*(Address3 + 1) == (BYTE)((gameTime - LNAIBuf[Line][3][0]) / 10)){
												if(GetL(Address3 + 2) > 0x7){
													commandTemp = __4;
												}else{
													commandTemp = __1;
												}
												decodeTimeTemp = 0;
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
		Address = LNAIbase + eigenValueLN[0][3];
		if(*(Address) == 0xFF){					//�ŏ��͕K��0xFF
			Address2 = Address;
			for(Line=0; Line<20; Line++){
				if(LNAIBuf[Line][0][0] == 0xFF){
					for(Counter2=20; Counter2<1000; Counter2+=20){
						Address3 = Address2 + Counter2;
						if(*(Address3) == 0){
							Counter2 = 1000;
						}else{
							if(*(Address3) == (BYTE)LNAIBuf[Line][2][0]){
								Counter2 = 1000;
								Address4 = Address3 + 4;
								if(*Address4 && (GetL(Address4 + 2) > 0xA || *(Address3 + 2)==0)){
									if(*(Address4 + 1) == (BYTE)((gameTime - LNAIBuf[Line][3][0]) / 10)){
										//������	//�������̃o�b�N�_�b�V���𖳌��ɂ���
										if( !(*Address4==__D4 && *(Address4 + 1)<10 && myGameInfo[ _para ][1][1]<2) ){
											commandTemp = *Address4;
											decodeTimeTemp = 0;
										}
									}
								}else{
									if(*(Address3 + 2)
									&& (LNAIBuf[Line][1][0]== 3 || LNAIBuf[Line][1][0]== 4 || LNAIBuf[Line][1][0]== 5
									 || LNAIBuf[Line][1][0]== 7 || LNAIBuf[Line][1][0]== 8)){
										if(*(Address3 + 1) == (BYTE)((gameTime - LNAIBuf[Line][3][0]) / 10)){
											if(GetL(Address3 + 2) > 0x5){
												commandTemp = __4;
											}else{
												commandTemp = __1;
											}
											decodeTimeTemp = 0;
										}
									}
								}
							}
						}
					}
				}
			}
		}
		
		//�������̂Ƃ��̏���
		if(myGameInfo[ _info ][0][1]){
			if(commandTemp == __D4 || commandTemp == __D6 || commandTemp == __D7 || commandTemp == __D8 || commandTemp == __D9
			|| commandTemp == __7 || commandTemp == __9
			|| commandTemp == __1 || commandTemp == __4){
				if(commandTemp == __D4 || commandTemp == __D6){
					if(commandTemp == __D4){
						commandTemp = __D6;
					}else{
						commandTemp = __D4;
					}
				}
				if(commandTemp == __D7 || commandTemp == __D9){
					if(commandTemp == __D7){
						commandTemp = __D9;
					}else{
						commandTemp = __D7;
					}
				}
				if(commandTemp == __7 || commandTemp == __9){
					if(commandTemp == __7){
						commandTemp = __9;
					}else{
						commandTemp = __7;
					}
				}
			}else{
				commandTemp = 0;
			}
		}
		if(commandTemp){
			commandInput[0] = commandTemp;
			decodeTime = decodeTimeTemp;
			commandInput[3] = 0;
		}
		
	}
}


void boosterDatClass::CallLNAI(){
	if(eigenValueLN[0][0]){
		if( !( gameTime < intervalFlg || gameTime < enDat->intervalFlg ) ){
			if(*enGameInfo[ _status ][2][2] && *enGameInfo[ _status ][2][2] != __5){
				for(Line=0; Line<20; Line++){
					if(LNAIBuf[Line][0][0]==0){
						#if debug_mode_LNAI
							cout << "���C���V��" << endl;
						#endif
						LNAIBuf[Line][0][0] = 0xFF;					//�H����������
						LNAIBuf[Line][1][0] = *enGameInfo[ _status ][0][0];	//�G�s���^�C�v�E���ނP
						LNAIBuf[Line][2][0] = *enGameInfo[ _status ][5][0];	//�G����ID
						LNAIBuf[Line][3][0] = *enGameInfo[ _status ][5][1];	//�G���͎���	//[5][1]���̃I�[�o�[�t���[�Ƃ��͍���
						LNAIBuf[Line][4][0] = 0xFF;					//�����J�n	//�ǂ̏�Ԃ��痈���̂��w�肵�Ȃ��Ƃ����Ӗ�
						
						if(LNAIBuf[Line][1][0]==5 || LNAIBuf[Line][1][0]==8){
							LNAIBuf[Line][1][1] = 0xFF;		//���֒���
						}else{
							LNAIBuf[Line][1][1] = 128;		//����
						}
						LNAIBuf[Line][2][1] = 64;			//��ےl������
						Line = 20;
					}
				}
			}
		}
		for(Line=0; Line<20; Line++){
			if(LNAIBuf[Line][0][0]){
				//DWORD LNAIBuf[10][5][20]
				//LNAIBuf[Line][0][0]�H����
				//LNAIBuf[Line][1][0]�G���̓^�C�v�E���ނP
				//LNAIBuf[Line][2][0]�G����ID
				//LNAIBuf[Line][3][0]�G���͎���
				//LNAIBuf[Line][4][0]�O����ID
				//type 0:�ŗL�A�h���X 1:���֒l 2:��ےl 3:���� 4:����ID
				
				//��������e���C���̏���
				//Line��Index�͊�{�I�ɕύX���Ȃ�
				
				/*
				//�򉻂��ǂ����邩
				//��������������������Ƃ肠�������u
				*/
				if(myGameInfo[ _status ][2][2] && myGameInfo[ _status ][2][2] != __5){
					if(LNAIBuf[Line][0][0] == 0xFF){
						LNAIBuf[Line][0][0] = 1;
					}else{
						LNAIBuf[Line][0][0] = LNAIBuf[Line][0][0] + 1;	//�H������i�߂�
					}
					Index = LNAIBuf[Line][0][0];
					
					//�ŗL�A�h���X�v�Z�i�f�[�^�̎n�߂܂Ō����j
					//�������ޏꏊ�����������ꍇ��0
					//0xFF�͍ŏ��̍��ɏ�������
					LNAIBuf[Line][0][Index] = 0;
					if(myGameInfo[ _status ][2][2] != __22C && myGameInfo[ _status ][2][2] != __22D && myGameInfo[ _status ][2][2] != __236D && myGameInfo[ _status ][2][2] != __214D){
						if(LNAIBuf[Line][0][0]==1){
							Counter = 0;
						}else{
							Counter = 1000;
						}
						Address = LNAIbase + eigenValueLN[0][3];
						for(Counter; Counter<50000; Counter+=1000){
							Address2 = Address + Counter;
							if(*(Address2) == 0){
								*(Address2) = (BYTE)LNAIBuf[Line][4][Index -1];
							}
							if(*(Address2) == (BYTE)LNAIBuf[Line][4][Index -1]){
								Counter = 50000;
								for(Counter2=20; Counter2<1000; Counter2+=20){
									Address3 = Address2 + Counter2;
									if(*(Address3) == 0){
										*(Address3) = (BYTE)LNAIBuf[Line][2][0];
									}
									if(*(Address3) == (BYTE)LNAIBuf[Line][2][0]){
										LNAIBuf[Line][0][Index] = (DWORD)(Address3);	//�ŗL�P�ʃA�h���X
										Counter2 = 1000;
									}
								}
							}
						}
					}
					
					if(Index != 1){
						LNAIBuf[Line][1][Index] = LNAIBuf[Line][1][Index - 1] * 3 / 4;	//���֒l���p��
						LNAIBuf[Line][2][Index] = LNAIBuf[Line][2][Index - 1];		//��ےl���p��
					}
					LNAIBuf[Line][3][Index] = myGameInfo[ _status ][5][1];	//����
					LNAIBuf[Line][4][Index] = myGameInfo[ _status ][5][0];	//����
				}
				
				if(LNAIBuf[Line][0][0] == 0xFF){
					Index = 1;
				}else{
					Index = LNAIBuf[Line][0][0];
				}
				
				if(LNAIBuf[Line][1][Index] >= __magnification){
					LNAIBuf[Line][1][Index] = LNAIBuf[Line][1][Index] - __magnification;	//���֒l�͌�������
				}else{
					LNAIBuf[Line][1][Index] = 0;
				}
				
				if(*enGameInfo[ _status ][2][2]){
					LNAIBuf[Line][1][Index] = LNAIBuf[Line][1][Index] * 3 / 4;
				}
				
				//�e�X�g
				if(*enGameInfo[ _para ][5][1]){
					LNAIBuf[Line][1][Index] = 0;	//���ւ�؂�
				}
				
				if(myGameInfo[ _para ][5][1] && (myGameInfo[ _status ][0][0]==2 || myGameInfo[ _status ][0][0]==9 || myGameInfo[ _status ][9][0]==0x22)){
					#if debug_mode_LNAI
						cout << "�_���[�W" << endl;
					#endif
					for(Counter=1; Counter<=Index; Counter++){
						if(LNAIBuf[Line][2][Counter] >= 10){
							LNAIBuf[Line][2][Counter] = LNAIBuf[Line][2][Counter] - 10;
						}else{
							LNAIBuf[Line][2][Counter] = 0;
						}
					}
					LNAIBuf[Line][1][Index] = 0;	//���ւ�؂�
				}
				
				if(myGameInfo[ _info ][2][2] && (LNAIBuf[Line][2][0]== 0x38 || LNAIBuf[Line][2][0]== 0x39)){	//�u���C�N
					//���}����
					if(LNAIBuf[Line][0][Index]){
						Address3 = (BYTE*)LNAIBuf[Line][0][Index];
						if(*Address3==0x38){	//__22A��ID:0x38�����ʂȂ̂�
						#if debug_mode_LNAI
							cout << "��i�K�[�h�t�^" << endl;
						#endif
							FloatL(Address3 + 2, 15);
						}
						if(*Address3==0x39){	//__22B��ID : 0x39
							#if debug_mode_LNAI
								cout << "���i�K�[�h�t�^" << endl;
							#endif
							FloatL(Address3 + 2, -15);
						}
					}
					LNAIBuf[Line][1][Index] = 0;		//���ւ�؂�
				}
				
 				if(myGameInfo[ _status ][2][2]){
 					if(statusArray[ LNAIBuf[Line][4][Index] ][0]==0xA){
	 					//�����ŃK�[�h�]��	//��ۂɊւ�炸�A�s���̕p�x�ɂ��ϓ�����B
						if(LNAIBuf[Line][0][Index]){
							Address3 = (BYTE*)LNAIBuf[Line][0][Index];
		 					if(statusArray[ LNAIBuf[Line][4][Index] ][2]==__4){
		 						FloatL(Address3 + 2, 1);
		 					}
		 					if(statusArray[ LNAIBuf[Line][4][Index] ][2]==__1){	//���ǂ̗]�n�A��
		 						FloatL(Address3 + 2, -2);
		 					}
						}
					}
				}
				
				if(Index >= 19 || LNAIBuf[Line][1][Index] < 5){	//���ցE�v����
					//�I��
					#if debug_mode_LNAI
						cout << playerSide << ".line" << Line << " Close" << endl;
					#endif
					for(Index=1; Index<=LNAIBuf[Line][0][0]; Index++){
						if(LNAIBuf[Line][2][Index] >= 64){
							if(LNAIBuf[Line][0][Index] && LNAIBuf[Line][0][0] !=0xFF){		//�A�h���X���m�ۂ���Ă�����
								Address3 = (BYTE*)LNAIBuf[Line][0][Index];
								LNAIBuf[Line][0][Index] = 0;	//�A�h���X���������i�O�̂��߁j
								if(*Address3 == (BYTE)LNAIBuf[Line][2][0]){	//�m�F
									
									//��������]��
									//�f�[�^�̔�������߂��Ă��܂�����A��ނ₵�Ⴊ�݂�ۑ����Ȃ��B
									//�w�b�_�����ɃK�[�h���
				 					Time = (LNAIBuf[Line][3][Index] - LNAIBuf[Line][3][0]) / 10;
									if(statusArray[ LNAIBuf[Line][4][Index] ][2] == __1 || statusArray[ LNAIBuf[Line][4][Index] ][2] == __4){
										if(statusArray[ LNAIBuf[Line][4][Index] ][0] == 0xA){
											//�K�[�h
											if(*(Address3 + 2)){
												
												//�K�[�h�̎��Ԃ̐��x
							 					if((BYTE)Time    < 3+ *(Address3 + 1)
							 					&& (BYTE)Time +3 >    *(Address3 + 1)){
													FloatH(Address3 + 2, 1);
												}else{
													FloatH(Address3 + 2, -1);
												}
												
												//���Ԓ���
												if(GetH( Address3 + 2 ) < 0xA){
								 					if((BYTE)Time > 2+ *(Address3 + 1) ){
														*(Address3 + 1) = *(Address3 + 1) + 1;
													}
								 					if((BYTE)(Time +2) < *(Address3 + 1)){
														*(Address3 + 1) = *(Address3 + 1) - 1;
													}
												}
												
											}else{
												*(Address3 + 1) = (BYTE)Time;
												*(Address3 + 2) = 0x88;
												*(Address3 + 3) = 0;
											}
										}else{
											//���
											//��ނ��Ă���̂ɃK�[�h�����������Ƃ����x��������
											//���x�����l�ȉ��ɂȂ����Ƃ��̓��Z�b�g
											if(Index != 19 && AIMode ==2){
												if(statusArray[ LNAIBuf[Line][4][Index + 1] ][0] != 0xA){
													//�と���̍s���@�ƂȂ����Ƃ��̗]�n�A��
													FloatH(Address3 + 2, -1);
												}
												if(GetH(Address3 + 2) < 0x4){
													*(Address3 + 1) = 0;
													*(Address3 + 2) = 0;
													*(Address3 + 3) = 0;
												}
											}
										}
									}else{
										#if debug_mode_LNAI
											cout << "�����N�A�b�v����" << endl;
										#endif
									 	for(Counter3=4; Counter3<20; Counter3+=4){
							 				Address4 = Address3 + Counter3;
								 			if(*(Address4) == (BYTE)statusArray[ LNAIBuf[Line][4][Index] ][2]){
								 				
								 				//�]��
								 				FloatL(Address4 + 2, 1);
							 					
							 					//���x
							 					if((BYTE)Time    < 3+ *(Address4 + 1)
							 					&& (BYTE)Time +3 >    *(Address4 + 1)){	//���ԍ������Ȃ�
							 						FloatH(Address4 + 2, 1);
								 				}else{
								 					FloatH(Address4 + 2, -1);
								 				}
								 				
								 				//���Ԓ���
								 				if(GetH(Address4 + 2) < 0xA){
								 					if((BYTE)Time > 2+ *(Address4 + 1)){
														*(Address4 + 1) = *(Address4 + 1) + 1;
													}
								 					if((BYTE)Time +2 < *(Address4 + 1)){
														*(Address4 + 1) = *(Address4 + 1) - 1;
													}
												}
								 				
								 				//����ւ�
								 				if(Counter3 != 4){
								 					if(GetL(Address4 + 2) >= GetL(Address4 - 2)){
														#if debug_mode_LNAI
															cout << "�����N�A�b�v" << endl;
														#endif
														
														DWORD Temp;
								 						Temp				= *(DWORD*)(Address4 - 4);
								 						*(DWORD*)(Address4 - 4)	= *(DWORD*)(Address4);
								 						*(DWORD*)(Address4)	= Temp;
									 				}
								 				}
								 				Counter3 = 20;
								 			}else{
								 				if(*(Address4)==0 || Counter3==16){
								 					//�쐬�E�㏑��
								 					Counter3 = 20;
								 					*(Address4)     = (BYTE)statusArray[ LNAIBuf[Line][4][Index] ][2];
								 					*(Address4 + 1) = (BYTE)Time;
								 					*(Address4 + 2) = 0x08;	//�����l
								 					*(Address4 + 3) = 0;	//�󂫗e�ʁi�򉻏��Ȃǂ��i�[�H�j
								 				}
							 				}
							 			}
							 		}
							 	}
							 }
						}else{
							if(((gameTime - LNAIBuf[Line][3][0]) / 10) < 1){
								Time = 0;	//�K�[�h��t�^����ׂ�����
							}else{
								Time = (gameTime - LNAIBuf[Line][3][0]) / 10 - 1;
							}
							
							//���͂̒x��ɂ��H�炢��␳�E�K�[�h��t�^
							if(LNAIBuf[Line][0][0] == 0xFF){
								Address = LNAIbase + eigenValueLN[0][3];
								if(*Address == 0){
									*Address = 0xFF;
								}
								if(*Address == 0xFF){
									Address2 = Address;
									for(Counter2=20; Counter2<1000; Counter2+=20){
										Address3 = Address2 + Counter2;
										if(*Address3 == (BYTE)LNAIBuf[Line][2][0]){
											Counter2 = 1000;
											
											//�s���̎��ԁE���x�𒲐�
											Address4 = Address3 + 4;
											if(*Address4){
												#if debug_mode_LNAI
													cout << "���Ԓ���" << endl;
												#endif
							 					FloatH(Address4 + 2, -1);
								 				if(GetH(Address4 + 2) < 0xA){
								 					if((BYTE)Time > *(Address4 + 1)){
														*(Address4 + 1) = *(Address4 + 1) + 1;
													}
								 					if((BYTE)Time < *(Address4 + 1)){
														*(Address4 + 1) = *(Address4 + 1) - 1;
													}
												}
											}
											if(*(Address3 + 2)){
												//�K�[�h�̎��ԁE���x�𒲐�
												//����
												if(myGameInfo[ _info ][2][0]==2){
													if(GetH(Address3 + 2) > 4){
														FloatL(Address3 + 2, -2);
													}else{
														FloatL(Address3 + 2, 2);
													}
												}
						 						FloatH(Address3 + 2, -1);
								 				if(GetH(Address3 + 2) < 0xA){
								 					if((BYTE)Time > *(Address3 + 1)){
														*(Address3 + 1) = *(Address3 + 1) + 1;
													}
								 					if((BYTE)Time < *(Address3 + 1)){
														*(Address3 + 1) = *(Address3 + 1) - 1;
													}
												}
											}else{
												#if debug_mode_LNAI
													cout << "�K�[�h�V�K�ǉ�" << endl;
												#endif
												*(Address3 + 1) = (BYTE)Time;
												*(Address3 + 2) = 0x88;
												*(Address3 + 3) = 0;
											}
										}else{
											if(*(Address3) == 0){
												#if debug_mode_LNAI
													cout << "�K�[�h�V�K�ǉ�" << endl;
												#endif
												Counter2 = 1000;
												
												*(Address3) = (BYTE)LNAIBuf[Line][2][0];
												*(Address3 + 1) = (BYTE)Time;
												*(Address3 + 2) = 0x88;
												*(Address3 + 3) = 0;
											}
										}
									}
								}
							}
							
							//�������烉���N�_�E���Ȃ�
							if(LNAIBuf[Line][0][Index] && LNAIBuf[Line][0][0] !=0xFF){	//�A�h���X���m�ۂ���Ă�����
								Address3 = (BYTE*)LNAIBuf[Line][0][Index];
								LNAIBuf[Line][0][Index] = 0;	//�A�h���X���������i�O�̂��߁j
								if(*Address3 == (BYTE)LNAIBuf[Line][2][0]){	//�m�F
									
									if(statusArray[ LNAIBuf[Line][4][Index] ][2] == __1 || statusArray[ LNAIBuf[Line][4][Index] ][2] == __4){
										if(statusArray[ LNAIBuf[Line][4][Index] ][0] == 0xA){
											//�K�[�h
											//�K�[�h�����̂Ƀ}�C�i�X�Ƃ������Ƃ�
											//�E���ꂽ
											//�E�K�[�h�̌�ɐH�����
											//���̂Ƃ��뉽�����Ȃ��B
											//��X�͍U���̃^�C�v�ɂ���ċ敪����
										}else{
											//���
											if(Index != 19 && AIMode == 2){
												if(statusArray[ LNAIBuf[Line][4][Index + 1] ][0] != 0xA){
													//��ނ��Ă���̂ɃK�[�h�������A�}�C�i�X��ۂł̓K�[�h�̐��x��傫��������B
													//�����l��8
													if(myGameInfo[ _info ][2][0]==2){
														if(GetH(Address3 + 2) > 4){
															FloatL(Address3 + 2, -2);
														}else{
															FloatL(Address3 + 2, 2);
														}
													}else{
														FloatH(Address3 + 2, -2);
													}
												}
												if(GetH(Address3 + 2) < 0x4){
													//���x�����l�ȉ��Ȃ�����폜�B
													*(Address3 + 1) = 0;
													*(Address3 + 2) = 0;
													*(Address3 + 3) = 0;
												}
											}
										}
									}else{
										#if debug_mode_LNAI
											cout << "�����N�_�E������" << endl;
										#endif
										
										if(*(Address3 + 2)==0){
											//�K�[�h��V�K�ɒǉ�
											*(Address3 + 1) = (BYTE)Time;
											*(Address3 + 2) = 0x88;
											*(Address3 + 3) = 0;
										}
										
										//�����N�_�E��
									 	for(Counter3=4; Counter3<20; Counter3+=4){
									 		Address4 = Address3 + Counter3;
								 			if(*(Address4) == (BYTE)statusArray[ LNAIBuf[Line][4][Index] ][2]){
												#if debug_mode_LNAI
													cout <<"���_" << endl;
												#endif
									 			
									 			//�]���E���x
									 			//�l�@�̗]�n�A��
							 					if((BYTE)Time    < 3+ *(Address4 + 1)
							 					&& (BYTE)Time +3 >    *(Address4 + 1)){
							 						FloatHL(Address4 + 2, -1, -1);
								 				}
							 					
							 					if(Counter3 != 16){
							 						if(*(Address4 + 4)){
								 						if(GetL(Address4 + 2) <= GetL(Address4 + 6)){
								 							//�����N�_�E��
								 							DWORD Temp;
									 						Temp				= *(DWORD*)(Address4 + 4);
									 						*(DWORD*)(Address4 + 4)	= *(DWORD*)(Address4);
									 						*(DWORD*)(Address4)	= Temp;
									 					}
								 					}
								 				}
							 					Counter3 = 20;
							 				}
								 		}
								 	}
								}
							}
						}
						if(LNAIBuf[Line][0][0]==0xFF){
							LNAIBuf[Line][0][0] = 1;
						}
					}
					//��ہE�ۑ����������܂�
					
					//�O�̂���0�N���A
					//�������Ԃ͂����z�肵�Ȃ��ŏ���
					if(LNAIBuf[Line][0][0]==0xFF){
						LNAIBuf[Line][0][0] = 1;
					}
					for(Counter=0; Counter<=LNAIBuf[Line][0][0]; Counter++){
						LNAIBuf[Line][0][Counter] = 0;
						LNAIBuf[Line][1][Counter] = 0;
						LNAIBuf[Line][2][Counter] = 0;
						LNAIBuf[Line][3][Counter] = 0;
						LNAIBuf[Line][4][Counter] = 0;
					}
					//�ی������܂�
					
					
					//�K�{
					LNAIBuf[Line][0][0] = 0;	//�H���ԍ�������
					LNAIBuf[Line][1][0] = 0;
				}
			}
		}
		
		//�ŗL�A�h���X�X�V
		eigenValueLN[1][2] = myGameInfo[ _para ][1][1];	//����
		eigenValueLN[2][2] = myGameInfo[ _para ][2][1];	//��������
		eigenValueLN[3][2] = *enGameInfo[ _para ][2][1];	//���荂��
		if(myGameInfo[ _info ][1][0] == 1){
			if(eigenValueLN[1][2] == 0 || eigenValueLN[1][2] == 1){
				eigenValueLN[1][2] = eigenValueLN[1][2] + 5;
			}
		}
		
		for(Counter=9; Counter>0; Counter--){
			if(eigenValueLN[Counter][0]){
				if(eigenValueLN[Counter][2] >= eigenValueLN[Counter][0]){		//�l���`�F�b�N
					eigenValueLN[Counter][2] = eigenValueLN[Counter][0] -1;
				}											//�ŗL�A�h���X�v�Z
				eigenValueLN[Counter - 1][3] = eigenValueLN[Counter][1] * eigenValueLN[Counter][2] + eigenValueLN[Counter][3];
			}
		}
	}
}
