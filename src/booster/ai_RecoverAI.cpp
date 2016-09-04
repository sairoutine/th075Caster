#include "conf.h"
#include "boosterDatClass.h"


void boosterDatClass::ReadRecoverAI(){
	if(eigenValueRecover[0][0]){
		if((statusID==0x20 && myGameInfo[ _para ][2][0]) || (statusID==0x21 && myGameInfo[ _para ][2][0]==0)){
			if(myGameInfo[ _info ][2][0] != 2 || myGameInfo[ _para ][2][0] == 0){
				Address = RecoverAIbase + eigenValueRecover[0][3];
				commandInput[0] = *Address;	//0�ł��悢�i���̓L�����Z���̂��߁j
				#if debug_mode_RecoverAI
					cout << "Recover" << endl;
				#endif
			}
		}
	}
}

void boosterDatClass::CallRecoverAI(){
	if(eigenValueRecover[0][0]){
		
		if(myGameInfo[ _status ][2][2] && (myGameInfo[ _status ][5][0] == 0x90 || myGameInfo[ _status ][5][0] == 0x91 || myGameInfo[ _status ][5][0] == 0x92 || myGameInfo[ _status ][5][0] == 0x93)){
			//2Byte * 10��
			Address = RecoverAIbase + eigenValueRecover[0][3];
			RecoverAIBuf[0] = (DWORD)Address;
			RecoverAIBuf[1] = 0x30;	//����
			RecoverAIBuf[2] = 1;	//���
			RecoverAIBuf[4] = myGameInfo[ _status ][2][0];
		}
		
		if(RecoverAIBuf[0]){
			if(RecoverAIBuf[1] > __magnification){
				RecoverAIBuf[1] = RecoverAIBuf[1] - __magnification;
			}else{
				RecoverAIBuf[1] = 0;
			}
			
			if(myGameInfo[ _para ][5][1]){
				RecoverAIBuf[1] = 0;
				RecoverAIBuf[2] = 0;
			}
			
			if(RecoverAIBuf[1] == 0){
				if(RecoverAIBuf[0]){
					Address = (BYTE*)RecoverAIBuf[0];
					RecoverAIBuf[0] = 0;
					for(Counter=0; Counter<20; Counter+=2){
						Address2 = Address + Counter;
						if(*Address2==0){
							*(Address2) = RecoverAIBuf[4];
							*(Address2 + 1) = 8;
						}
						if(*Address2 == (BYTE)RecoverAIBuf[4]){
							#if debug_mode_RecoverAI
								cout << "Recover end" << endl;
							#endif
							if(RecoverAIBuf[2]){
								//�D���
								//���_
								if(*(Address2 + 1) < 0xF){
									*(Address2 + 1) = *(Address2 + 1) + 1;
								}
								if(Counter != 0){
									//�����N�A�b�v����
									if(*(Address2 + 1) >= *(Address2 - 1)){
										DWORD Temp;
										Temp                   = (DWORD)*(WORD*)(Address2 - 2);
										*(WORD*)(Address2 - 2) = *(WORD*)Address2;
										*(WORD*)Address2       = (WORD)Temp;
									}
								}
							}else{
								//�����
								//���_
								if(*(Address2 + 1)){
									*(Address2 + 1) = *(Address2 + 1) - 1;
								}
								if(Counter != 18 && *(Address2 + 2)){
									//�����N�_�E������
									if(*(Address2 + 1) <= *(Address2 + 3)){
										DWORD Temp;
										Temp                   = (DWORD)*(WORD*)Address2;
										*(WORD*)Address2       = *(WORD*)(Address2 + 2);
										*(WORD*)(Address2 + 2) = (WORD)Temp;
									}
								}
							}
							Counter = 20;
						}
					}
					RecoverAIBuf[0] = 0;
					RecoverAIBuf[1] = 0;
					RecoverAIBuf[2] = 0;
					RecoverAIBuf[3] = 0;
					RecoverAIBuf[4] = 0;
				}
			}
		}
		
		
		//�ŗL�A�h���X�X�V
		eigenValueRecover[1][2] = myGameInfo[ _para ][1][1];	//����
		eigenValueRecover[2][2] = myGameInfo[ _para ][2][1];	//��������
		eigenValueRecover[3][2] = *enGameInfo[ _para ][2][1];	//���荂��
		if(myGameInfo[ _info ][1][0] == 1){
			if(eigenValueRecover[1][2] == 0 || eigenValueRecover[1][2] == 1){
				eigenValueRecover[1][2] = eigenValueRecover[1][2] + 5;
			}
		}
		
		for(Counter=9; Counter>0; Counter--){
			if(eigenValueRecover[Counter][0]){
				if(eigenValueRecover[Counter][2] >= eigenValueRecover[Counter][0]){		//�l���`�F�b�N
					eigenValueRecover[Counter][2] = eigenValueRecover[Counter][0] -1;
				}											//�ŗL�A�h���X�v�Z
				eigenValueRecover[Counter - 1][3] = eigenValueRecover[Counter][1] * eigenValueRecover[Counter][2] + eigenValueRecover[Counter][3];
			}
		}
		
	}
}
