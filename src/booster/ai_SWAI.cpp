#include "conf.h"
#include "boosterDatClass.h"
#include "HL.h"

void boosterDatClass::ReadSWAI(){
	if(eigenValueSW[0][0]){
		
		DWORD Temp = 0;
		Flg = 0;
		
		#if debug_height
			if(myGameInfo[ _para ][2][0] && *enGameInfo[ _para ][2][0]){	//������
//			if(*enGameInfo[ _para ][2][0]){	//������
				Temp = 5;
			}
			if(gameInfoPara[15][1]==4){
				Temp += 10;
			}
			
			if(*enGameInfo[ _para ][2][0] && (*enGameInfo[ _status ][0][0] == 2 || *enGameInfo[ _status ][0][0] == 9)){
				Flg = 1;
			}
		#endif
		
		Address = SWAIbase + eigenValueSW[0][3];
		for(Counter=0; Counter<4800; Counter+=160){
			Address2 = Address + Counter;
			if(*Address2 == 0){
				Counter = 4800;
			}else{
				if(*Address2 == (BYTE)myGameInfo[ _status ][6][0]){	//�����O�s��
					Counter = 4800;
					for(Counter2=8; Counter2<160; Counter2+=8){
						Address3 = Address2 + Counter2;
						if(*Address3==0){
							Counter2 = 160;
						}else{
							if(!(*Address3==0x24 && myGameInfo[ _info ][4][0]==0)){
								//��̓`�F�b�N
								if(!(myGameInfo[ _info ][2][0]
								&& (statusArray[ *Address3 ][0] == 4 || statusArray[ *Address3 ][0] == 5 || statusArray[ *Address3 ][0] == 6))){
									Time = (DWORD)((gameTime - myGameInfo[ _status ][6][1]) / 4);
									if(((BYTE)(Time + 0xF) >= *(Address3 + 1) && (BYTE)Time < 3+ *(Address3 + 1)) || myGameInfo[ _status ][6][0]==0xFF){
										if(!(Flg && statusArray[ *Address3 ][0] == 3
										&& !(GetH(Address3 + 3) >= (BYTE)myGameInfo[ _para ][3][2]
										  && GetH(Address3 + 3) <= debug_height_value + (BYTE)myGameInfo[ _para ][3][2]))){	//�����␳	//������
											Counter2 = 160;
											if(((BYTE)(Time + Temp) >= *(Address3 + 1) && (BYTE)Time < 3+ *(Address3 + 1)) || myGameInfo[ _status ][6][0]==0xFF){
												commandInput[0] = statusArray[ *Address3 ][2];
												decodeTime = 0;
												#if debug_mode_SWAI
													cout << "action : " << dec << (DWORD)commandInput[0] << endl;
												#endif
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
	}
}

void boosterDatClass::CallSWAI(){
	if(eigenValueSW[0][0]){
//		Line==0�͑S�̂ɑ΂���o�b�t�@�Ƃ��Ďg�p
//		SWAIBuf[0][0][0] = ;
//		SWAIBuf[0][1][0] = ;
//		SWAIBuf[0][2][0] = ;
//		SWAIBuf[0][3][0] = myGameInfo[ _status ][5][1];	//����
//		SWAIBuf[0][4][0] = myGameInfo[ _status ][6][0];	//����ID
//		SWAIBuf[0][5][0] = myGameInfo[ _para ][3][2];	//�����␳���
//		�A���U���́A���炢or�󒆃_�E����Ԃł�Index��i�߂�Ƃ��������H
		
		if(SWAIBuf[0][4][0]==0){
			SWAIBuf[0][4][0] = 0xFF;
		}
		
		Flg = 0;
		if(myGameInfo[ _status ][2][2]){
			if(myGameInfo[ _info ][1][0] == 2 && myGameInfo[ _para ][1][0] < 3){	//�G���Ǎ�
				if(myGameInfo[ _status ][2][2] != __5 && myGameInfo[ _status ][2][2] != __4 && myGameInfo[ _status ][2][2] != __1
//				&& myGameInfo[ _status ][2][2] != __D4 && myGameInfo[ _status ][2][2] != __D7
				&& myGameInfo[ _status ][2][2] != __22D && myGameInfo[ _status ][0][0] != 8
				&& myGameInfo[ _status ][5][0] != 0x90 && myGameInfo[ _status ][5][0] != 0x91 && myGameInfo[ _status ][5][0] != 0x92 && myGameInfo[ _status ][5][0] != 0x93
				){
					if(!(myGameInfo[ _para ][1][0] > 4 && (myGameInfo[ _status ][2][2] == __D4 || myGameInfo[ _status ][2][2] == __D7))){
						if(myGameInfo[ _info ][0][1]==0){
							//�������łȂ�
							Flg = 1;
							if(myGameInfo[ _status ][2][2] == __D4 || myGameInfo[ _status ][2][2] == __D7){
								if(statusArray[ (BYTE)SWAIBuf[0][4][0] ][2] == __D4 || statusArray[ (BYTE)SWAIBuf[0][4][0] ][2] == __D7){
									Flg = 0;
								}
							}
						}
					}
				}
			}
		}
		if(Flg){
			for(Line=1; Line<20; Line++){
				if(SWAIBuf[Line][0][0]==0){
					//���C���V��
					SWAIBuf[Line][0][0] = 0xFF;
					SWAIBuf[Line][1][0] = 0;
					SWAIBuf[Line][2][0] = 0;
					SWAIBuf[Line][3][0] = SWAIBuf[0][3][0];	//�O���͎���
					SWAIBuf[Line][4][0] = SWAIBuf[0][4][0];	//�O�s��
					SWAIBuf[Line][5][0] = 0;
					
					//���֏����l�ݒ�
					SWAIBuf[Line][1][1] = 0xFF;
					
					//��ۏ����l�ݒ�
					if(AIMode==2){
						SWAIBuf[Line][2][1] = 64;
					}else{
						SWAIBuf[Line][2][1] = 65;
					}
					
					//��O
					if(myGameInfo[ _status ][2][2]==__22C){
						//�쌂
						SWAIBuf[Line][1][1] = 0x60;	//����
						SWAIBuf[Line][2][1] = 64;	//���
					}
					if(myGameInfo[ _status ][2][2] == __D4 || myGameInfo[ _status ][2][2] == __D7){
						//��_�b�V��
						SWAIBuf[Line][1][1] = 0xFF;	//����
						SWAIBuf[Line][2][1] = 64;	//���
					}
					Line = 20;
				}
			}
		}
		for(Line=1; Line<20; Line++){
			if(SWAIBuf[Line][0][0]){
				if(Flg && SWAIBuf[Line][0][0]==0xFF){
					if(SWAIBuf[Line][0][0]==0xFF){
						SWAIBuf[Line][0][0] = 1;
					}else{
						SWAIBuf[Line][0][0] = SWAIBuf[Line][0][0] + 1;
					}
					Index = SWAIBuf[Line][0][0];
					
					SWAIBuf[Line][0][Index] = 0;
					Address = SWAIbase + eigenValueSW[0][3];
					for(Counter=0; Counter<4800; Counter+=160){
						Address2 = Address + Counter;
						if(*Address2 == (BYTE)SWAIBuf[Line][4][Index -1]){	//�����O�s��
							Counter = 4800;
							SWAIBuf[Line][0][Index] = (DWORD)Address2;
						}else{
							if(*Address2 == 0){
								Counter = 4800;
								SWAIBuf[Line][0][Index] = (DWORD)Address2;
								*Address2 = (BYTE)SWAIBuf[Line][4][Index - 1];
							}
						}
					}
					if(Index != 1){
						SWAIBuf[Line][1][Index] = SWAIBuf[Line][1][Index - 1];	//����
						SWAIBuf[Line][2][Index] = SWAIBuf[Line][2][Index - 1];	//���
					}
					SWAIBuf[Line][3][Index] = (myGameInfo[ _status ][5][1] - SWAIBuf[0][3][0]) / 4;	//����
					SWAIBuf[Line][4][Index] = myGameInfo[ _status ][5][0];	//���́iID�j
					SWAIBuf[Line][5][Index] = SWAIBuf[0][5][0];			//�����␳���
				}
				if(SWAIBuf[Line][0][0]==0xFF){
					Index = 1;
				}else{
					Index = SWAIBuf[Line][0][0];
				}
				if(SWAIBuf[Line][1][Index] >= __magnification){
					SWAIBuf[Line][1][Index] = SWAIBuf[Line][1][Index] - __magnification;	//���֒l�͌�������
				}else{
					SWAIBuf[Line][1][Index] = 0;
				}
				
				if(*enGameInfo[ _status ][2][2]){
					SWAIBuf[Line][1][Index] = SWAIBuf[Line][1][Index] * 3 / 4;
				}
				
				if(*enGameInfo[ _para ][5][1] || *enGameInfo[ _status ][9][0]==0x22){
					#if debug_mode_SWAI
						cout << "�^�_���[�W" << endl;
					#endif
					if(SWAIBuf[Line][2][Index] <= 245){
						SWAIBuf[Line][2][Index] = SWAIBuf[Line][2][Index] + 10;
					}else{
						SWAIBuf[Line][2][Index] = 255;
					}
					SWAIBuf[Line][1][Index] = 0;	//���ւ�؂�
				}
				
				if(myGameInfo[ _para ][5][1]){
					#if debug_mode_SWAI
						cout << "��_���[�W" << endl;
					#endif
					if(SWAIBuf[Line][2][Index] >= 10){
						SWAIBuf[Line][2][Index] = SWAIBuf[Line][2][Index] - 10;
					}else{
						SWAIBuf[Line][2][Index] = 0;
					}
					SWAIBuf[Line][1][Index] = 0;	//���ւ�؂�
				}
				
				if(Index >= 19 || SWAIBuf[Line][1][Index] < 5){	//���ցE�v����
					//�I��
					#if debug_mode_SWAI
						cout << hex << playerSide << ".line" << Line << " Close" << endl;
					#endif
					
					if(SWAIBuf[Line][0][Index]){
						Address2 = (BYTE*)SWAIBuf[Line][0][Index];
						SWAIBuf[Line][0][Index] = 0;
						if(*Address2 == (BYTE)SWAIBuf[Line][4][Index -1]){	//�m�F
							for(Counter2=8; Counter2<160; Counter2+=8){
								Address3 = Address2 + Counter2;
								if(*(Address3)==(BYTE)SWAIBuf[Line][4][Index]){
									if(SWAIBuf[Line][2][Index] > 64){
//									if((SWAIBuf[Line][2][Index] >= 64 && AIMode==1) || (SWAIBuf[Line][2][Index] > 64 && AIMode==2)){	//������
//									if((SWAIBuf[Line][2][Index] >= 64 && gameMode==5) || SWAIBuf[Line][2][Index] > 64){	//������
										//�D���
										//���ԍ��̏���
										if((BYTE)SWAIBuf[Line][3][Index]    < 2+ *(Address3 + 1)
										&& (BYTE)SWAIBuf[Line][3][Index] +2 >    *(Address3 + 1)){	//���ԍ������Ȃ�
											FloatH(Address3 + 2, 1);
											FloatL(Address3 + 7, 1);
										}else{
											//���ԏC��
											if(GetH(Address3 + 2) < 0xA){
												if((BYTE)SWAIBuf[Line][3][Index] > *(Address3 + 1)){
													if(*(Address3 + 1) < 0xFF){
														*(Address3 + 1) = *(Address3 + 1) + 1;
													}
												}else{
													if(*(Address3 + 1)){
														*(Address3 + 1) = *(Address3 + 1) - 1;
													}
												}
											}
										}
										
										//�����␳
										if((BYTE)SWAIBuf[Line][5][Index]    < 1+ GetH(Address3 + 3)
										&& (BYTE)SWAIBuf[Line][5][Index] +1 >    GetH(Address3 + 3)){
											//�������߂�
											FloatL(Address3 + 3, 1);
										}else{
											//�������߂��Ȃ�
											FloatL(Address3 + 3, -1);	//���x����
										}
										if(GetL(Address3 + 3) < 0xA){
											//�����␳���s
											if((BYTE)SWAIBuf[Line][5][Index] > GetH(Address3 + 3)){
												FloatH(Address3 + 3, 1);
											}
											if((BYTE)SWAIBuf[Line][5][Index] < GetH(Address3 + 3)){
												FloatH(Address3 + 3, -1);
											}
										}
										
										if(Counter2 != 8){
											if(GetL(Address3 + 7) >= GetL(Address3 - 1)){
												ULONGLONG Temp;
												Temp                       = *(ULONGLONG*)(Address3 - 8);
												*(ULONGLONG*)(Address3 - 8) = *(ULONGLONG*)(Address3);
												*(ULONGLONG*)(Address3)    = Temp;
											}
										}
									}
									if(SWAIBuf[Line][2][Index] < 64){
										//�����
										if((BYTE)SWAIBuf[Line][3][Index]    < 3+ *(Address3 + 1)
										&& (BYTE)SWAIBuf[Line][3][Index] +3 >    *(Address3 + 1)){	//���ԍ������Ȃ�
											FloatH(Address3 + 2, -1);
											FloatL(Address3 + 7, -1);
										}
										if(Counter2 != 152){
											if(*(Address3 + 8)){
												if(GetL(Address3 + 15) >= GetL(Address3 + 7)){
													ULONGLONG Temp;
													Temp                    = *(ULONGLONG*)(Address3);
													*(ULONGLONG*)(Address3) = *(ULONGLONG*)(Address3 + 8);
													*(ULONGLONG*)(Address3 + 8)    = Temp;
												}
											}else{
												//�����Ȃ�
												if(GetL(Address3 + 7) < 6){
													*(ULONGLONG*)(Address3) = 0;
												}
											}
										}
									}
									Counter2 = 160;
								}else{
									if(*(Address3)==0 || Counter2 == 152){
										Counter2 = 160;
										if(SWAIBuf[Line][2][Index] > 64){
//										if((SWAIBuf[Line][2][Index] >= 64 && AIMode==1) || (SWAIBuf[Line][2][Index] > 64 && AIMode==2)){	//������
//										if((SWAIBuf[Line][2][Index] >= 64 && gameMode==5) || SWAIBuf[Line][2][Index] > 64){	//������
											*(Address3) = (BYTE)SWAIBuf[Line][4][Index];		//����ID
											*(Address3 + 1) = (BYTE)SWAIBuf[Line][3][Index];	//���ԍ�
											*(Address3 + 2) = 0;
											SetH(Address3 + 3, (BYTE)SWAIBuf[Line][5][Index]);
											SetL(Address3 + 3, 8);
											*(Address3 + 7) = 8;
										}
									}
								}
							}
						}
					}
					SWAIBuf[Line][0][0] = 0;
				}
			}
		}
		
		
		SWAIBuf[0][3][0] = myGameInfo[ _status ][6][1];	//����
		SWAIBuf[0][4][0] = myGameInfo[ _status ][6][0];	//����ID
		SWAIBuf[0][5][0] = myGameInfo[ _para ][3][2];
		
		//�ŗL�A�h���X�X�V
		eigenValueSW[1][2] = myGameInfo[ _para ][1][0];	//����
		eigenValueSW[2][2] = myGameInfo[ _para ][2][2];	//��������	//������
		eigenValueSW[3][2] = *enGameInfo[ _para ][2][0];	//���荂��	//������
		eigenValueSW[4][2] = *enGameInfo[ _status ][1][0];	//�e�L��ԁE����2
		
		
		for(Counter=9; Counter>0; Counter--){
			if(eigenValueSW[Counter][0]){
				if(eigenValueSW[Counter][2] >= eigenValueSW[Counter][0]){		//�l���`�F�b�N
					eigenValueSW[Counter][2] = eigenValueSW[Counter][0] -1;
				}											//�ŗL�A�h���X�v�Z
				eigenValueSW[Counter - 1][3] = eigenValueSW[Counter][1] * eigenValueSW[Counter][2] + eigenValueSW[Counter][3];
			}
		}
	}
}
