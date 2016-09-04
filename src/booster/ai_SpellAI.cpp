#include "conf.h"
#include "boosterDatClass.h"


void boosterDatClass::ReadSpellAI(){
	if(eigenValueSpell[0][0]){
		if(myGameInfo[ _info ][3][0]){
			if(myGameInfo[ _info ][5][0] == 0){
				if(myGameInfo[ _para ][5][0] < 3000){	//HP���Ȃ��A�錾�\
					Address = SpellAIbase + eigenValueSpell[0][3];
					if(*Address == 0x25){
						Address2 = Address;
						if(*(Address2 + 1) > 90){
							//�X�y���J�[�h�錾
							commandInput[0] = __22D;
							decodeTime = 0;
							#if debug_mode_SpellAI
								cout << "�錾" << endl;
							#endif
						}
					}
				}
			}else{
				//�X�y���J�[�h�錾��
				if(myID==9){	//��
					if(statusID >= 0xC0 && statusID <= 0xC7){	//�~�b�V���O�n
						SpellAIBuf[0][0][0]=1;
					}else{
						SpellAIBuf[0][0][0]=0;
					}
				}
				
				if(spell_control == 0 || *enGameInfo[ _status ][1][0] == 2 || *enGameInfo[ _status ][1][0] == 5){
					if(SpellAIBuf[0][0][0]==0){
						if(myGameInfo[ _info ][2][0]==0){	//��͂̃`�F�b�N
							//�X�y�������\�@
							Address = SpellAIbase + eigenValueSpell[0][3];
							for(Counter=2; Counter<40; Counter+=2){
								Address2 = Address + Counter;
								if(*Address2 == (BYTE)myGameInfo[ _info ][8][2]){
									Counter = 40;
									if(*(Address2 + 1) > 80){
										commandInput[0] = statusArray[ *Address2 ][2];
										decodeTime = 0;
										#if debug_mode_SpellAI
											cout << "�X�y���J�[�h����" << endl;
										#endif
									}
								}
							}
						}
					}
				}
				if(SpellAIBuf[0][0][0]==1){	//2�͔��ˁE�W�J��
					//�X�y���J���i�I�[�����[�Y�T���Ȃǁj
					Address = SpellAIbase + eigenValueSpell[0][3];
					
					if(spell_control == 0 || *enGameInfo[ _status ][1][0] == 2 || *enGameInfo[ _status ][1][0] == 5){
						if(myID==1){
							if(myGameInfo[ _info ][8][2] == 0xBC){	//�I�[�����[�Y�T��
								for(Counter=2; Counter<40; Counter+=2){
									Address2 = Address + Counter;
									if(*Address2 == 0){
										Counter = 40;
									}
									if(*Address2 == 0xBD || *Address2 == 0xBE){	//���ˁE�W�J
										if(*(Address2 + 1) > 80){
											commandInput[0] = statusArray[ *Address2 ][2];
											Counter = 40;
										}
									}
								}
							}
							if(myGameInfo[ _info ][8][2] == 0xBF){	//�I�[�����[�Y�\�[���[�V�X�e��
								for(Counter=2; Counter<40; Counter+=2){
									Address2 = Address + Counter;
									if(*Address2 == 0){
										Counter = 40;
									}
									if(*Address2 == 0xC0 || *Address2 == 0xC1){	//���ˁE�W�J
										if(*(Address2 + 1) > 80){
											commandInput[0] = statusArray[ *Address2 ][2];
											Counter = 40;
										}
									}
								}
							}
						}
					}
					if(myID==9){	//��	//�~�b�V���O�n
						if(myGameInfo[ _info ][8][2] == 0xBC || myGameInfo[ _info ][8][2] == 0xBF){
							if(myGameInfo[ _para ][1][0] > 4){
								commandInput[0] = __6;
							}else{
								Address2 = Address + 20;
								if(*(Address2 + 1) > 50){	//�v����
									commandInput[0] = statusArray[ *Address2 ][2];
								}else{
									commandInput[0] = __6;
								}
							}
						}
					}
				}
			}
		}
	}
		
		
}

void boosterDatClass::CallSpellAI(){
	if(eigenValueSpell[0][0]){
		//2byte * 20
		
//		SpellAIBuf[0][0][0]	//�t���O�ɗ��p
		
		
		//�ŗL�A�h���X�X�V
		//������	//�]��������������Ă��Ȃ��H
		eigenValueSpell[1][2] = myGameInfo[ _para ][1][1];	//����
		eigenValueSpell[2][2] = myGameInfo[ _para ][2][1];	//��������
		eigenValueSpell[3][2] = *enGameInfo[ _para ][2][1];	//���荂��
		eigenValueSpell[4][2] = *enGameInfo[ _status ][0][0];	//�e�L��ԁE����1
		
		
		for(Counter=9; Counter>0; Counter--){
			if(eigenValueSpell[Counter][0]){
				if(eigenValueSpell[Counter][2] >= eigenValueSpell[Counter][0]){		//�l���`�F�b�N
					eigenValueSpell[Counter][2] = eigenValueSpell[Counter][0] -1;
				}											//�ŗL�A�h���X�v�Z
				eigenValueSpell[Counter - 1][3] = eigenValueSpell[Counter][1] * eigenValueSpell[Counter][2] + eigenValueSpell[Counter][3];
			}
		}
		
		
		
		if(myGameInfo[ _status ][2][2] && myGameInfo[ _status ][2][2] != __5){
			if(myGameInfo[ _status ][0][2]==8 || myGameInfo[ _status ][5][0]==0x25){	//�X�y���֌W
				for(Line=1; Line<10; Line++){
					if(SpellAIBuf[Line][0][0]==0){
						SpellAIBuf[Line][0][0] = 0xFF;	//�H����
						
						if(myGameInfo[ _status ][5][0]==0x25){	//�X�y���J�[�h�錾
							SpellAIBuf[Line][4][0] = 1;
							SpellAIBuf[Line][1][1] = 0x30;//����
							SpellAIBuf[Line][2][1] = 65;	//���	//�����N���Ȃ������s
							Counter = 0;	//�錾���擪
						}else{
							//�錾�ȊO
							SpellAIBuf[Line][4][0] = 0;
							SpellAIBuf[Line][1][1] = 128;//����
							SpellAIBuf[Line][2][1] = 64;	//���
							
							//�����������ȃX�y��
							//���֒l�̏����l�����Ȃ�
							//SpellAIBuf[Line][4][0]���t���O
							
							if(myID==1){	//�������X�y��
								if(myGameInfo[ _status ][5][0]==0xBC || myGameInfo[ _status ][5][0]==0xBF){	//����
									SpellAIBuf[Line][4][0] = 2;
									SpellAIBuf[Line][1][1] = 0x30;//����
									SpellAIBuf[Line][2][1] = 65;	//���
								}
								if(myGameInfo[ _status ][5][0]==0xBD || myGameInfo[ _status ][5][0]==0xBE
								|| myGameInfo[ _status ][5][0]==0xC0 || myGameInfo[ _status ][5][0]==0xC1){	//���ˁE�W�J
									SpellAIBuf[0][0][0] = 2;
									SpellAIBuf[Line][4][0] = 3;
									SpellAIBuf[Line][1][1] = 128;//����
									SpellAIBuf[Line][2][1] = 64;	//���
								}
							}
							if(myID==2){	//���X�y��
								if(myGameInfo[ _status ][5][0]==0xB0 || myGameInfo[ _status ][5][0]==0xB3
								|| myGameInfo[ _status ][5][0]==0xB6 || myGameInfo[ _status ][5][0]==0xB7
								|| myGameInfo[ _status ][5][0]==0xB9 || myGameInfo[ _status ][5][0]==0xBA){
									SpellAIBuf[Line][4][0] = 1;
									SpellAIBuf[Line][1][1] = 0x70;//����	//���߂�����
									SpellAIBuf[Line][2][1] = 65;	//���
								}
							}
							if(myID==4){	//�p�`�����[�X�y��
								if(myGameInfo[ _status ][5][0]==0xBF){
									SpellAIBuf[Line][4][0] = 1;
									SpellAIBuf[Line][1][1] = 0x60;//����
									SpellAIBuf[Line][2][1] = 65;	//���
								}
							}
							if(myID==5){	//�d���X�y��
								if(myGameInfo[ _status ][5][0]==0xBC || myGameInfo[ _status ][5][0]==0xBF){
									SpellAIBuf[Line][4][0] = 1;
									SpellAIBuf[Line][1][1] = 0x60;//����
									SpellAIBuf[Line][2][1] = 65;	//���
								}
							}
							if(myID==8){	//���X�y��
								if(myGameInfo[ _status ][5][0]==0xBF){
									SpellAIBuf[Line][4][0] = 1;
									SpellAIBuf[Line][1][1] = 0x60;//����
									SpellAIBuf[Line][2][1] = 65;	//���
								}
							}
							if(myID==9){	//���X�y��
								if(statusID==0xB0 || statusID==0xB3 || statusID==0xB6 || statusID==0xB9){
									SpellAIBuf[Line][4][0] = 0;
									SpellAIBuf[Line][1][1] = 0x60;//����
									SpellAIBuf[Line][2][1] = 64;	//���
								}
								
								if(myGameInfo[ _status ][5][0]==0xBC || myGameInfo[ _status ][5][0]==0xBF){
									SpellAIBuf[Line][4][0] = 0;	//���͒ʏ�X�y���Ƃ��Ĉ���
									SpellAIBuf[Line][1][1] = 0x60;//����
									SpellAIBuf[Line][2][1] = 64;	//���
								}
								if(statusID >= 0xC0 && statusID <= 0xC7){	//�~�b�V���O�n
									SpellAIBuf[Line][4][0] = 4;
									SpellAIBuf[Line][1][1] = 0xFF;//����
									SpellAIBuf[Line][2][1] = 65;	//64;	//���	//�v����	//�ǈĂ��Ȃ�
								}
							}
							Counter = 2;	//�錾���擪�Ȃ̂�
						}
						
						Address = SpellAIbase + eigenValueSpell[0][3];
						SpellAIBuf[Line][0][1] = 0;
						if(SpellAIBuf[Line][4][0] != 4){
							for(Counter; Counter<20; Counter+=2){
								Address2 = Address + Counter;
								if(*Address2 == 0){
									*(Address2) = statusID;
									*(Address2 + 1) = 95;	//�����l
								}
								if(*Address2 == statusID){	//myGameInfo[ _status ][5][0]�Ɠ���	//�ǂ���ŏ����̂�����������
									Counter = 20;
									SpellAIBuf[Line][0][1] = (DWORD)Address2;
								}
							}
						}else{	//SpellAIBuf[Line][4][0] == 4	//�~�b�V���O�n
							SpellAIBuf[Line][0][1] = (DWORD)Address;
						}
						SpellAIBuf[Line][3][1] = 0;
						SpellAIBuf[Line][4][1] = (DWORD)statusID;	//(DWORD)myGameInfo[ _info ][8][0];	//�����
						
						Line = 10;
					}
				}
			}
		}
		
		for(Line=1; Line<10; Line++){
			if(SpellAIBuf[Line][0][0]){
				if(SpellAIBuf[Line][1][1] > __magnification){
					SpellAIBuf[Line][1][1] = SpellAIBuf[Line][1][1] - __magnification;
				}else{
					SpellAIBuf[Line][1][1] = 0;
				}
				
				//�ʏ�
				if(SpellAIBuf[Line][4][0]==0){
					if(myGameInfo[ _para ][5][1]){
						SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] - 10;
						SpellAIBuf[Line][1][1] = 0;
						#if debug_mode_SpellAI
							cout << "�_���[�W" << endl;
						#endif
					}
					if(statusArray[ *enGameInfo[ _status ][9][1] ][0]==2 || (*enGameInfo[ _status ][0][0]==2 && *enGameInfo[ _para ][5][1])){
						SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] + 10;
						SpellAIBuf[Line][1][1] = 0;
						#if debug_mode_SpellAI
							cout << "�^�_���[�W" << endl;
						#endif
					}
					if(myID==9 && (statusID == 0xB7 || statusID == 0xBA)){
						SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] + 10;
						SpellAIBuf[Line][1][1] = 0;
						#if debug_mode_SpellAI
							cout << "�S����n����" << endl;
						#endif
					}
				}
				
				//�錾�Ȃ�
				if(SpellAIBuf[Line][4][0]==1 || SpellAIBuf[Line][4][0]==2){	//���֒l�̏����l�����Ȃ�
					if(myGameInfo[ _para ][5][1]){
						SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] - 10;
						SpellAIBuf[Line][1][1] = 0;
						#if debug_mode_SpellAI
							cout << "�_���[�W" << endl;
						#endif
					}
				}
				
				//�I�[�����[�Y�T���Ȃ�
				if(SpellAIBuf[Line][4][0]==3){
					if((myID==1 && SpellAIBuf[Line][1][1] > 60) || myID !=1){
						if(myGameInfo[ _para ][5][1]){
							if(SpellAIBuf[Line][2][1] > 10){
								SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] - 10;
							}else{
								SpellAIBuf[Line][2][1] = 0;
							}
							#if debug_mode_SpellAI
								cout << "�_���[�W" << endl;
							#endif
						}
						if(*enGameInfo[ _status ][0][0]==2){
							if(SpellAIBuf[Line][2][1] < 118){
								SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] + 10;
							}else{
								SpellAIBuf[Line][2][1] = 128;
							}
							#if debug_mode_SpellAI
								cout << "�^�_���[�W" << endl;
							#endif
						}
					}
				}
				
				//�~�b�V���O�n
				if(SpellAIBuf[Line][4][0]==4){
					if(statusID >= 0xC0 && statusID <= 0xC7){
						if(*enGameInfo[ _status ][0][0]==2){
							if(SpellAIBuf[Line][2][1] < 118){
								SpellAIBuf[Line][2][1] = SpellAIBuf[Line][2][1] + 10;
							}else{
								SpellAIBuf[Line][2][1] = 128;
							}
							SpellAIBuf[Line][1][1] = 0;
							#if debug_mode_SpellAI
								cout << "�^�_���[�W" << endl;
							#endif
						}
					}else{
						SpellAIBuf[0][0][0] = 0;
						SpellAIBuf[Line][1][1] = 0;	//���ւ�؂�
					}
				}
				
				if(SpellAIBuf[Line][1][1] < 8){
					//�I��
					/*
					#if debug_mode_SpellAI
						cout << "�I��" << endl;
					#endif
					*/
					if(SpellAIBuf[Line][0][1]){
						if(SpellAIBuf[Line][4][0] != 4){
							Address2 = (BYTE*)SpellAIBuf[Line][0][1];
							SpellAIBuf[Line][0][1] = 0;
							if(*Address2 == (BYTE)SpellAIBuf[Line][4][1]){
								if(SpellAIBuf[Line][2][1] > 64){
									//�D���
									//�����c���f�[�^�Ȃ̂ŕꐔ��傫��
									if(*(Address2 + 1) < 109){
										*(Address2 + 1) = *(Address2 + 1) + 1;
									}
									if(SpellAIBuf[Line][4][0]==2){
										SpellAIBuf[0][0][0] = 1;	//��������
										#if debug_mode_SpellAI
											cout << "�I�[�����[�Y�n��������" << endl;
										#endif
									}
									if(SpellAIBuf[Line][4][0]==3){
										SpellAIBuf[0][0][0] = 0;	//���ː���
										#if debug_mode_SpellAI
											cout << "�I�[�����[�Y�n����" << endl;
										#endif
									}
									
									#if debug_mode_SpellAI
										cout << "�X�y���D���" << endl;
									#endif
								}else{
									//�����
									*(Address2 + 1) = *(Address2 + 1) - (BYTE)(*(Address2 + 1) / 10);
									if(SpellAIBuf[Line][4][0]==3){
										SpellAIBuf[0][0][0] = 0;	//���ˎ��s
										#if debug_mode_SpellAI
											cout << "�I�[�����[�Y�n���s" << endl;
										#endif
									}
									#if debug_mode_SpellAI
										cout << "�X�y�������" << endl;
									#endif
								}
							}
						}else{	//SpellAIBuf[Line][4][0] == 4;	//�~�b�V���O�n
							Address = (BYTE*)SpellAIBuf[Line][0][1];
							for(Counter=20; Counter<40; Counter+=2){
								Address2 = Address + Counter;
								if(*Address2==0){
									*(Address2) = (BYTE)SpellAIBuf[Line][4][1];
									*(Address2 + 1) = 95;
								}
								if(*Address2==(BYTE)SpellAIBuf[Line][4][1]){
									if(SpellAIBuf[Line][2][1] > 64){
										//�D���
										if(*(Address2 + 1) < 110){
											*(Address2 + 1) = *(Address2 + 1) + 1;
										}
										if(Counter!=20){
											if(*(Address2 + 1) >= *(Address2 - 1)){
												//�����N�A�b�v
												DWORD Temp;
												Temp                   = (DWORD)*(WORD*)(Address2 - 2);
												*(WORD*)(Address2 - 2) = *(WORD*)Address2;
												*(WORD*)Address2       = (WORD)Temp;
											}
										}
										#if debug_mode_SpellAI
											cout << "�~�b�V���O�n�D���" << endl;
										#endif
									}else{
										//�����
										if(*(Address2 + 1)){
											*(Address2 + 1) = *(Address2 + 1) - 1;
										}
										if(Counter != 38 && *(Address2 + 2)){
											if(*(Address2 + 1) <= *(Address2 + 3)){
												DWORD Temp;
												Temp                   = (DWORD)*(WORD*)Address2;
												*(WORD*)Address2       = *(WORD*)(Address2 + 2);
												*(WORD*)(Address2 + 2) = (WORD)Temp;
											}
										}
										#if debug_mode_SpellAI
											cout << "�~�b�V���O�n�����" << endl;
										#endif
									}
									Counter = 40;
								}
							}
						}
					}
					SpellAIBuf[Line][0][0] = 0;
				}
			}
		}
		
		
	}
}
