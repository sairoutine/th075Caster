#include "conf.h"
#include "boosterDatClass.h"


void boosterDatClass::CalcAddress(){
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".CalcAddress() " << endl;
	#endif
	
	
	//DWORD gameInfoIni[10][6]
	for(Counter =0;Counter<10;Counter++){gameInfoIni[Counter][0]=0;}
	gameInfoIni[0][0] = (DWORD)&bodyBuf[0] + myIniOffset;			//�p�b�h�̔ԍ��H00�Ńp�b�h FF�ŃL�[�{�[�h
	gameInfoIni[1][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x04;	//left	//�p�b�h�̏ꍇFF�ŕ����L�[���̂܂�//�L�[�{�[�h��DirectInput�̃L�[�R�[�h
	gameInfoIni[2][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x08;	//right	//�p�b�h�̏ꍇFF�ŕ����L�[���̂܂�
	gameInfoIni[3][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x0C;	//up	//�p�b�h�̏ꍇFF�ŕ����L�[���̂܂�
	gameInfoIni[4][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x10;	//down	//�p�b�h�̏ꍇFF�ŕ����L�[���̂܂�
	gameInfoIni[5][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x14;	//A	//PAD�̂Ƃ��̓{�^���̔ԍ�
	gameInfoIni[6][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x18;	//B
	gameInfoIni[7][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x1C;	//C
	gameInfoIni[8][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x20;	//D
	gameInfoIni[9][0] = (DWORD)&bodyBuf[0] + myIniOffset + 0x24;	//pause
	
	//����
	//DWORD gameInfoInput[10][3]	//��O���ڂɃe�L�̃A�h���X�B
	//myInfo[3] = &gameInfoInput[0][0]
	for(Counter =0;Counter<10;Counter++){gameInfoInput[Counter][0]=0;gameInfoInput[Counter][2]=0;}
	gameInfoInput[0][0] = (DWORD)&myBuf[0] + 0x4C4;	//��	//���́B�{�^���������Ă���t���[����
	gameInfoInput[1][0] = 0;				//��	//������
	gameInfoInput[2][0] = (DWORD)&myBuf[0] + 0x4C0;	//��	//�㉺���́@�E����//������
	gameInfoInput[3][0] = 0;				//�E				//�t����
	gameInfoInput[4][0] = (DWORD)&myBuf[0] + 0x4C8;	//A
	gameInfoInput[5][0] = (DWORD)&myBuf[0] + 0x4CC;	//B
	gameInfoInput[6][0] = (DWORD)&myBuf[0] + 0x4D0;	//C
	gameInfoInput[7][0] = (DWORD)&myBuf[0] + 0x4D4;	//D
	
	//�������Ɋւ�����
	//DWORD gameInfoPara[20][3]
	//myInfo[4] = &gameInfoPara[0][0]
	for(Counter =0;Counter<20;Counter++){gameInfoPara[Counter][0]=0;gameInfoPara[Counter][2]=0;}
	gameInfoPara[0][0] = (DWORD)&myBuf[0] + 0x44;	//����X���W	float	//�L����
	gameInfoPara[1][0] = 0;
	gameInfoPara[2][0] = (DWORD)&myBuf[0] + 0x48;	//����	float	//0xBC;
	gameInfoPara[3][0] = (DWORD)&myBuf[0] + 0x3B0;	//���
	gameInfoPara[4][0] = (DWORD)&myBuf[0] + 0x36C;	//���� float
	gameInfoPara[5][0] = (DWORD)&myBuf[0] + 0x366;	//�쌂
	gameInfoPara[6][0] = (DWORD)&myBuf[0] + 0x3B6;	//��̓Q�[�W�񕜂܂ł̎���
	gameInfoPara[7][0] = (DWORD)&myBuf[0] + 0x3A0;	//�X�y���J�[�h�錾�I���܂ł̎���
	gameInfoPara[8][0] = (DWORD)&myBuf[0] + 0x3AC;	//�󒆃_�b�V��
	gameInfoPara[9][0] = (DWORD)&myBuf[0] + 0x47C;	//�_�b�V���S�ʁi���_�b�V���̔��肽�߁j
	gameInfoPara[10][0] = (DWORD)&myBuf[0] + 0x398;	//�땄���	�����̃X�y���J�[�h�̎��
	gameInfoPara[11][0] = (DWORD)&myBuf[0] + 0x39A;	//�󕄎��
	gameInfoPara[12][0] = (DWORD)&myBuf[0] + 0x39D;	//�̗̓o�[�{��
	gameInfoPara[13][0] = (DWORD)&myBuf[0] + 0xC0;	//�E��������������
	gameInfoPara[15][0] = (DWORD)&myBuf[0] + 0x60;	//���
	gameInfoPara[16][0] = (DWORD)&myBuf[0] + 0x3B8;	//�����p
	gameInfoPara[17][0] = (DWORD)&myBuf[0] + 0x3BA;	//�����p
	
	//��ۂ̌W���Ɋւ�����
	//DWORD gameInfoImpress[10][3]
	//myInfo[5] = &gameInfoImpress[0][0]
	for(Counter =0;Counter<10;Counter++){gameInfoImpress[Counter][0]=0;gameInfoImpress[Counter][2]=0;}
	gameInfoImpress[0][0] = (DWORD)&myBuf[0] + 0xA4;	//�n���f	3F00 / 3F40 / 3F80 / 3FC0 / 4000
	gameInfoImpress[1][0] = (DWORD)&myBuf[0] + 0x324;	//�����̗̓Q�[�W
	gameInfoImpress[2][0] = (DWORD)&myBuf[0] + 0x3C6;	//�����`�F�C���_���[�W�i�����Ń��Z�b�g����Ȃ��j
	gameInfoImpress[3][0] = (DWORD)&myBuf[0] + 0x3C4;	//�����`�F�C���q�b�g���i�����Ń��Z�b�g����Ȃ��j
	
	//DWORD* enGameInfo[5][10][5]
	//myInfo[7] = &myGameInfo[0][0][0]
	for(Counter=0; Counter< _PAGE; Counter++){
		for(Counter2=0; Counter2<10; Counter2++){
			enGameInfo[Counter][Counter2][0] = &(enDat->myGameInfo[Counter][Counter2][0]);
			enGameInfo[Counter][Counter2][1] = &(enDat->myGameInfo[Counter][Counter2][1]);
			enGameInfo[Counter][Counter2][2] = &(enDat->myGameInfo[Counter][Counter2][2]);
			enGameInfo[Counter][Counter2][3] = &(enDat->myGameInfo[Counter][Counter2][3]);
			enGameInfo[Counter][Counter2][4] = &(enDat->myGameInfo[Counter][Counter2][4]);
		}
	}
	
}

int boosterDatClass::GetName(){
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".GetName() " << endl;
	#endif
	Flg = 1;
	if(myName == 0x006590d8){strcpy(Name , "reimu\0" );	myID = 0;	Flg = 0;}
	if(myName == 0x006592b8){strcpy(Name , "marisa\0");	myID = 1;	Flg = 0;}
	if(myName == 0x006594a8){strcpy(Name , "sakuya\0");	myID = 2;	Flg = 0;}
	if(myName == 0x00659608){strcpy(Name , "alice\0");	myID = 3;	Flg = 0;}
	if(myName == 0x00659768){strcpy(Name , "patchouli\0");	myID = 4;	Flg = 0;}
	if(myName == 0x006598D0){strcpy(Name , "youmu\0");	myID = 5;	Flg = 0;}
	if(myName == 0x00659a40){strcpy(Name , "remilia\0");	myID = 6;	Flg = 0;}
	if(myName == 0x00659b80){strcpy(Name , "yuyuko\0");	myID = 7;	Flg = 0;}
	if(myName == 0x00659cc0){strcpy(Name , "yukari\0");	myID = 8;	Flg = 0;}
	if(myName == 0x00659e30){strcpy(Name , "suika\0");	myID = 9;	Flg = 0;}
	if(myName == 0x00659f50){strcpy(Name , "meiling\0");	myID = 10;	Flg = 0;}
	if(Flg){
		strcpy(Name, "error\0");
		myID = 0xF;
		return 1;
	}
	return 0;
}


int boosterDatClass::ConvertIni(){
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".iniConvert() " << endl;
	#endif
	if(bodyIniFlg){
		bufSize = bodyBuf_size;						//bodyBuf[] �� CHAR
		hProcess  = OpenProcess(PROCESS_ALL_ACCESS, FALSE, PID);
		ReadProcessMemory( hProcess , (void *)(baseAddress) , &bodyBuf[0] , bufSize, &bufSize );
		CloseHandle( hProcess );
		if(bufSize != bodyBuf_size){return 1;}
		
		for(Counter=0;Counter<10;Counter++){
			if(gameInfoIni[Counter][0]){
				gameInfoIni[Counter][1] = *(DWORD *)gameInfoIni[Counter][0];
			}
		}
		if(gameInfoIni[0][1]==0xFF){					//gameInfoIni[ ][2]�ɃL�[�R�[�h��ϊ����Ċi�[�i���Ƃ�DirectInput�̃L�[�R�[�h�j
			for(Counter=1;Counter<10;Counter++){		//keycodeArray�ł�������ق�����������
				if(gameInfoIni[Counter][1] == 0xCB){gameInfoIni[Counter][2]=0x25; gameInfoIni[Counter][3]=1;}	//��
				if(gameInfoIni[Counter][1] == 0xC8){gameInfoIni[Counter][2]=0x26; gameInfoIni[Counter][3]=1;}	//��
				if(gameInfoIni[Counter][1] == 0xCD){gameInfoIni[Counter][2]=0x27; gameInfoIni[Counter][3]=1;}	//��
				if(gameInfoIni[Counter][1] == 0xD0){gameInfoIni[Counter][2]=0x28; gameInfoIni[Counter][3]=1;}	//��
				if(gameInfoIni[Counter][1] == 0x4B){gameInfoIni[Counter][2]=0x64; gameInfoIni[Counter][3]=1;}	//�e���L�[4:��
				if(gameInfoIni[Counter][1] == 0x48){gameInfoIni[Counter][2]=0x68; gameInfoIni[Counter][3]=1;}	//�e���L�[8:��
				if(gameInfoIni[Counter][1] == 0x4D){gameInfoIni[Counter][2]=0x66; gameInfoIni[Counter][3]=1;}	//�e���L�[6:��
				if(gameInfoIni[Counter][1] == 0x50){gameInfoIni[Counter][2]=0x62; gameInfoIni[Counter][3]=1;}	//�e���L�[2:��
				if(gameInfoIni[Counter][1] == 0x0B){gameInfoIni[Counter][2]=0x30; gameInfoIni[Counter][3]=0;}	//0
				if(gameInfoIni[Counter][1] == 0x02){gameInfoIni[Counter][2]=0x31; gameInfoIni[Counter][3]=0;}	//1
				if(gameInfoIni[Counter][1] == 0x03){gameInfoIni[Counter][2]=0x32; gameInfoIni[Counter][3]=0;}	//2
				if(gameInfoIni[Counter][1] == 0x04){gameInfoIni[Counter][2]=0x33; gameInfoIni[Counter][3]=0;}	//3
				if(gameInfoIni[Counter][1] == 0x05){gameInfoIni[Counter][2]=0x34; gameInfoIni[Counter][3]=0;}	//4
				if(gameInfoIni[Counter][1] == 0x06){gameInfoIni[Counter][2]=0x35; gameInfoIni[Counter][3]=0;}	//5
				if(gameInfoIni[Counter][1] == 0x07){gameInfoIni[Counter][2]=0x36; gameInfoIni[Counter][3]=0;}	//6
				if(gameInfoIni[Counter][1] == 0x08){gameInfoIni[Counter][2]=0x37; gameInfoIni[Counter][3]=0;}	//7
				if(gameInfoIni[Counter][1] == 0x09){gameInfoIni[Counter][2]=0x38; gameInfoIni[Counter][3]=0;}	//8
				if(gameInfoIni[Counter][1] == 0x0A){gameInfoIni[Counter][2]=0x39; gameInfoIni[Counter][3]=0;}	//9
				if(gameInfoIni[Counter][1] == 0x10){gameInfoIni[Counter][2]=0x51; gameInfoIni[Counter][3]=0;}	//Q
				if(gameInfoIni[Counter][1] == 0x11){gameInfoIni[Counter][2]=0x57; gameInfoIni[Counter][3]=0;}	//W
				if(gameInfoIni[Counter][1] == 0x12){gameInfoIni[Counter][2]=0x45; gameInfoIni[Counter][3]=0;}	//E
				if(gameInfoIni[Counter][1] == 0x13){gameInfoIni[Counter][2]=0x52; gameInfoIni[Counter][3]=0;}	//R
				if(gameInfoIni[Counter][1] == 0x14){gameInfoIni[Counter][2]=0x54; gameInfoIni[Counter][3]=0;}	//T
				if(gameInfoIni[Counter][1] == 0x15){gameInfoIni[Counter][2]=0x59; gameInfoIni[Counter][3]=0;}	//Y
				if(gameInfoIni[Counter][1] == 0x16){gameInfoIni[Counter][2]=0x55; gameInfoIni[Counter][3]=0;}	//U
				if(gameInfoIni[Counter][1] == 0x17){gameInfoIni[Counter][2]=0x49; gameInfoIni[Counter][3]=0;}	//I
				if(gameInfoIni[Counter][1] == 0x18){gameInfoIni[Counter][2]=0x4F; gameInfoIni[Counter][3]=0;}	//O
				if(gameInfoIni[Counter][1] == 0x19){gameInfoIni[Counter][2]=0x50; gameInfoIni[Counter][3]=0;}	//P
				if(gameInfoIni[Counter][1] == 0x1A){gameInfoIni[Counter][2]=0xDB; gameInfoIni[Counter][3]=0;}	//[
				if(gameInfoIni[Counter][1] == 0x1B){gameInfoIni[Counter][2]=0xDD; gameInfoIni[Counter][3]=0;}	//]
				if(gameInfoIni[Counter][1] == 0x1C){gameInfoIni[Counter][2]=0x0D; gameInfoIni[Counter][3]=0;}	//Return
				if(gameInfoIni[Counter][1] == 0x1D){gameInfoIni[Counter][2]=0x11; gameInfoIni[Counter][3]=1;}	//Ctrl(L)
				if(gameInfoIni[Counter][1] == 0x1E){gameInfoIni[Counter][2]=0x41; gameInfoIni[Counter][3]=0;}	//A
				if(gameInfoIni[Counter][1] == 0x1F){gameInfoIni[Counter][2]=0x53; gameInfoIni[Counter][3]=0;}	//S
				if(gameInfoIni[Counter][1] == 0x20){gameInfoIni[Counter][2]=0x44; gameInfoIni[Counter][3]=0;}	//D
				if(gameInfoIni[Counter][1] == 0x21){gameInfoIni[Counter][2]=0x46; gameInfoIni[Counter][3]=0;}	//F
				if(gameInfoIni[Counter][1] == 0x22){gameInfoIni[Counter][2]=0x47; gameInfoIni[Counter][3]=0;}	//G
				if(gameInfoIni[Counter][1] == 0x23){gameInfoIni[Counter][2]=0x48; gameInfoIni[Counter][3]=0;}	//H
				if(gameInfoIni[Counter][1] == 0x24){gameInfoIni[Counter][2]=0x4A; gameInfoIni[Counter][3]=0;}	//J
				if(gameInfoIni[Counter][1] == 0x25){gameInfoIni[Counter][2]=0x4B; gameInfoIni[Counter][3]=0;}	//K
				if(gameInfoIni[Counter][1] == 0x26){gameInfoIni[Counter][2]=0x4C; gameInfoIni[Counter][3]=0;}	//L
				if(gameInfoIni[Counter][1] == 0x27){gameInfoIni[Counter][2]=0xBB; gameInfoIni[Counter][3]=0;}	//;
				if(gameInfoIni[Counter][1] == 0x2A){gameInfoIni[Counter][2]=0x10; gameInfoIni[Counter][3]=1;}	//Shift(L)
				if(gameInfoIni[Counter][1] == 0x2B){gameInfoIni[Counter][2]=0xE2; gameInfoIni[Counter][3]=0;}	//��
				if(gameInfoIni[Counter][1] == 0x2C){gameInfoIni[Counter][2]=0x5A; gameInfoIni[Counter][3]=0;}	//Z
				if(gameInfoIni[Counter][1] == 0x2D){gameInfoIni[Counter][2]=0x58; gameInfoIni[Counter][3]=0;}	//X
				if(gameInfoIni[Counter][1] == 0x2E){gameInfoIni[Counter][2]=0x43; gameInfoIni[Counter][3]=0;}	//C
				if(gameInfoIni[Counter][1] == 0x2F){gameInfoIni[Counter][2]=0x56; gameInfoIni[Counter][3]=0;}	//V
				if(gameInfoIni[Counter][1] == 0x30){gameInfoIni[Counter][2]=0x42; gameInfoIni[Counter][3]=0;}	//B
				if(gameInfoIni[Counter][1] == 0x31){gameInfoIni[Counter][2]=0x4E; gameInfoIni[Counter][3]=0;}	//N
				if(gameInfoIni[Counter][1] == 0x32){gameInfoIni[Counter][2]=0x4D; gameInfoIni[Counter][3]=0;}	//M
				if(gameInfoIni[Counter][1] == 0x33){gameInfoIni[Counter][2]=0xBC; gameInfoIni[Counter][3]=0;}	//,
				if(gameInfoIni[Counter][1] == 0x34){gameInfoIni[Counter][2]=0xBE; gameInfoIni[Counter][3]=0;}	//.
				if(gameInfoIni[Counter][1] == 0x35){gameInfoIni[Counter][2]=0xBF; gameInfoIni[Counter][3]=0;}	///
				if(gameInfoIni[Counter][1] == 0x36){gameInfoIni[Counter][2]=0x20; gameInfoIni[Counter][3]=0;}	//Space
			}
		}
		bodyIniFlg=0;
	}
	/*
	if(gameInfoIni[0][1] != 0xFF){
		//�L�[�{�[�h�ł͂Ȃ�
		if(AIMode == 2){
			//�p�b�h���Ɠ��͂����i���Ȃ�
			AIMode = 1;
		}
	}
	*/
	return 0;
}


void boosterDatClass::statusInit(){								//statusInit()
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".statusInit() " << endl;
	#endif
	
	for(Counter=0; Counter<800; Counter++){
		statusIDArray[Counter] = 0xFF;			//�Y�������ŏ�����
	}
	for(Counter=0; Counter<256; Counter++){
		statusArray[Counter][0] = 0xFF;
		statusArray[Counter][1] = 0xFF;
		statusArray[Counter][2] = 0;
	}
	
	//statusIDArray[800]�̐ݒ�
	
	statusIDArray[0x0] = 0x01;
	statusIDArray[0x1] = 0x02;
	statusIDArray[0x2] = 0x02;
	statusIDArray[0x3] = 0x01;
	statusIDArray[0x4] = 0x03;
	statusIDArray[0x5] = 0x04;
	statusIDArray[0x6] = 0x05;
	statusIDArray[0x7] = 0x06;
	statusIDArray[0x8] = 0x07;
	statusIDArray[0x9] = 0x08;
	statusIDArray[0xA] = 0x01;
	statusIDArray[0xC8] = 0x10;
	statusIDArray[0xC9] = 0x11;
	statusIDArray[0xCA] = 0x12;
	statusIDArray[0xCB] = 0x15;
	statusIDArray[0xCC] = 0x18;
	statusIDArray[0xCD] = 0x19;
	statusIDArray[0xCE] = 0x1A;
	statusIDArray[0xCF] = 0x10;
	
	//���炢
	for(Counter = 0x30; Counter <= 0x4F; Counter++){
		statusIDArray[Counter] = 0x20;
	}
	
	//�_�E��
	for(Counter = 0x50; Counter <= 0x7F; Counter++){
		statusIDArray[Counter] = 0x21;
	}
	//�Ђ��
	for(Counter = 0x80; Counter <= 0x95; Counter++){
		statusIDArray[Counter] = 0x22;
	}
	
	//��g
	for(Counter = 0xB0; Counter <= 0xBF; Counter++){
		statusIDArray[Counter] = 0x23;
	}
	
	//�쌂
	for(Counter = 0x250; Counter <= 0x255; Counter++){
		statusIDArray[Counter] = 0x24;
	}
	
	//�錾
	statusIDArray[0x31B] = 0x25;
	
	//�K�[�h
	for(Counter = 0x96; Counter <= 0xAF; Counter++){
		statusIDArray[Counter] = 0x26;
	}
	
	//���i�K�[�h
	statusIDArray[0x9A] = 0x28;
	statusIDArray[0x9B] = 0x28;
	statusIDArray[0x9C] = 0x28;
	statusIDArray[0xA3] = 0x28;
	statusIDArray[0xA4] = 0x28;
	statusIDArray[0xA5] = 0x28;
	
	//��i�U�������i�K�[�h
	//���i�U������i�K�[�h
	//[15][1]����̏��ł͂Ƃ�Ȃ������B
	//�Ђ�݂ɂȂ����Ƃ��̏�񂩂���̂͌����I�ł͂Ȃ��B
	//�ǂ���ł悭�K�[�h���Ă��邩�Ŕ��肷�邵���Ȃ������B
	
	//statusArray[256][3] ���ʕ����̐ݒ�
	
	for(Counter= 0x0; Counter <= 0x0F; Counter++){
		statusArray[Counter][0] = 0;
		statusArray[Counter][1] = 0;
	}
	statusArray[0x02][1] = 4;
	
	
	for(Counter= 0x10; Counter <= 0x1F; Counter++){
		statusArray[Counter][0] = 1;
		statusArray[Counter][1] = 1;
	}
	
	statusArray[0x20][0] = 2;
	statusArray[0x20][1] = 2;
	
	statusArray[0x21][0] = 9;
	statusArray[0x21][1] = 5;
	
	statusArray[0x22][0] = 0;
	statusArray[0x22][1] = 0;
	
	statusArray[0x23][0] = 0;
	statusArray[0x23][1] = 0;
	
	statusArray[0x24][0] = 3;
	statusArray[0x24][1] = 3;
	
	statusArray[0x25][0] = 7;
	statusArray[0x25][1] = 0;
	
	statusArray[0x26][0] = 0xA;
	statusArray[0x26][1] = 0;
	statusArray[0x27][0] = 0xA;
	statusArray[0x27][1] = 0;
	
	statusArray[0x28][0] = 0xA;
	statusArray[0x28][1] = 4;
	statusArray[0x29][0] = 0xA;
	statusArray[0x29][1] = 4;
	
	
	for(Counter= 0x30; Counter <= 0x5F; Counter++){
		statusArray[Counter][0] = 3;
		statusArray[Counter][1] = 3;
	}
	
	for(Counter= 0x60; Counter <= 0x7F; Counter++){
		statusArray[Counter][0] = 4;
		statusArray[Counter][1] = 3;
	}
	
	for(Counter= 0x80; Counter <= 0x9F; Counter++){
		statusArray[Counter][0] = 5;
		statusArray[Counter][1] = 3;
	}
	
	for(Counter= 0x90; Counter <= 0x9F; Counter++){
		statusArray[Counter][0] = 6;
		statusArray[Counter][1] = 0;
	}
	
	for(Counter= 0xA0; Counter <= 0xAF; Counter++){
		statusArray[Counter][0] = 7;
		statusArray[Counter][1] = 3;
	}
	
	for(Counter= 0xB0; Counter <= 0xCF; Counter++){
		statusArray[Counter][0] = 8;
		statusArray[Counter][1] = 3;
	}
	
	//statusArray[256][2]	//���͂̋��ʕ����̐ݒ�
	statusArray[0x01][2] = __5;
	statusArray[0x02][2] = __5;
//	statusArray[0x02][2] = __2;
	statusArray[0x03][2] = __6;
	statusArray[0x04][2] = __4;
	statusArray[0x05][2] = __8;
	statusArray[0x06][2] = __9;
	statusArray[0x07][2] = __7;
	
	statusArray[0x10][2] = __D6_;
	statusArray[0x11][2] = __D4;
	statusArray[0x12][2] = __D6;
	statusArray[0x13][2] = __D6;
	statusArray[0x14][2] = __D6;
	statusArray[0x15][2] = __D4;
	statusArray[0x16][2] = __D4;
	statusArray[0x17][2] = __D4;
	statusArray[0x18][2] = __D8;
	statusArray[0x19][2] = __D9;
	statusArray[0x1A][2] = __D7;
	
	//��g�͈ړ��R�}���h�̂Ƃ���Őݒ�
	statusArray[0x24][2] = __22C;
	statusArray[0x25][2] = __22D;
	statusArray[0x26][2] = __4;
	statusArray[0x27][2] = __4;
	statusArray[0x28][2] = __1;
	statusArray[0x29][2] = __1;
	
	
	Index = 0;
	
	//statusIDArray[ ] ,statusArray[ statusID ][2] �ݒ�i�Ή�����R�}���h�j
	//���͂̍ۂ̓~�X�ɒ���
	if(myID == 0x00){							//�얲
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;		//�U��
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __A_;
		Index = 0x35; statusIDArray[ 0x132 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x40; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x41; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x42; statusIDArray[ 0x1F7 ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x43; statusIDArray[ 0x1F8 ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x44; statusIDArray[ 0x1FA ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x45; statusIDArray[ 0x1FB ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;		//�ˌ�
		Index = 0x61; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x195 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x25C ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x71; statusIDArray[ 0x25D ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x72; statusIDArray[ 0x25E ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x73; statusIDArray[ 0x25F ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x80; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __214A;	//�x���ˌ��E�ړ��E����
		Index = 0x81; statusIDArray[ 0x259 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x82; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x83; statusIDArray[ 0x25B ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x90; statusIDArray[ 0xB4 ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;	//�X�y���J�[�h
		Index = 0xB3; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x01){							//������
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;	//�U��
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x40; statusIDArray[ 0x20C ] = Index;
				  statusIDArray[ 0x20E ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x41; statusIDArray[ 0x20D ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x42; statusIDArray[ 0x20A ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x43; statusIDArray[ 0x20B ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x60; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19D ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A1 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x199 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x71; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x72; statusIDArray[ 0x270 ] = Index; statusArray[ Index ][2] = __41236A;
		Index = 0x73; statusIDArray[ 0x271 ] = Index; statusArray[ Index ][2] = __41236B;
		Index = 0x90; statusIDArray[ 0xB4 ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
				  statusIDArray[ 0x2BD ] = Index;
		Index = 0xB3; statusIDArray[ 0x2C8 ] = Index; statusArray[ Index ][2] = __236D;
				  statusIDArray[ 0x2C9 ] = Index;
		Index = 0xB6; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBD; statusIDArray[ 0x2C3 ] = Index; statusArray[ Index ][2] = __214D;
		Index = 0xBE; statusIDArray[ 0x2C4 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C2 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xC0; statusIDArray[ 0x2CA ] = Index; statusArray[ Index ][2] = __214D;
		Index = 0xC1; statusIDArray[ 0x2CB ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	
	if(myID == 0x02){							//���
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x40; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x41; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x42; statusIDArray[ 0x1F6 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x43; statusIDArray[ 0x1F7 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x199 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19D ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x69; statusIDArray[ 0x19F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x195 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x25C ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x71; statusIDArray[ 0x25E ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x80; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x81; statusIDArray[ 0x25F ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x82; statusIDArray[ 0x260 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x83; statusIDArray[ 0x261 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x84; statusIDArray[ 0x262 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x85; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x86; statusIDArray[ 0x259 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x90; statusIDArray[ 0xB4 ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0x94; statusIDArray[ 0x1F9 ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x95; statusIDArray[ 0x1F8 ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x96; statusIDArray[ 0x1FA ] = Index; statusArray[ Index ][2] = __421C;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB7; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __214D;
		Index = 0xB9; statusIDArray[ 0x2C2 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBA; statusIDArray[ 0x2C3 ] = Index; statusArray[ Index ][2] = __214D;
		Index = 0xBC; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x03){							//�A���X
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x60; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19D ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19F ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A2 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A2 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x70; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x71; statusIDArray[ 0x259 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x72; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x73; statusIDArray[ 0x25E ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x74; statusIDArray[ 0x25F ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x75; statusIDArray[ 0x260 ] = Index; statusArray[ Index ][2] = __214C;
		Index = 0x76; statusIDArray[ 0x264 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x77; statusIDArray[ 0x265 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x80; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x81; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x82; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x83; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x84; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x90; statusIDArray[ 0xB4 ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x04){							//�p�`�����[
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x68; statusIDArray[ 0x1A1 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x71; statusIDArray[ 0x259 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x72; statusIDArray[ 0x25C ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x73; statusIDArray[ 0x263 ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x74; statusIDArray[ 0x264 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x75; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x76; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x77; statusIDArray[ 0x260 ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x78; statusIDArray[ 0x261 ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x79; statusIDArray[ 0x262 ] = Index; statusArray[ Index ][2] = __421C;
		Index = 0x7A; statusIDArray[ 0x137 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x7B; statusIDArray[ 0x138 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x80; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x81; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x90; statusIDArray[ 0xB4 ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x94; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x95; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x05){							//�d��
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x40; statusIDArray[ 0x1F6 ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x41; statusIDArray[ 0x201 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x42; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x43; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x44; statusIDArray[ 0x1FD ] = Index; statusArray[ Index ][2] = __41236A;
		Index = 0x45; statusIDArray[ 0x1FF ] = Index; statusArray[ Index ][2] = __5;
		Index = 0x46; statusIDArray[ 0x1FE ] = Index; statusArray[ Index ][2] = __41236B;
		Index = 0x47; statusIDArray[ 0x200 ] = Index; statusArray[ Index ][2] = __5;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x90; statusIDArray[ 0xB4 ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0xA0; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x06){							//���~���A
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x40; statusIDArray[ 0x200 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x41; statusIDArray[ 0x201 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x42; statusIDArray[ 0x1F6 ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x43; statusIDArray[ 0x1F7 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x44; statusIDArray[ 0x1F8 ] = Index; statusArray[ Index ][2] = __214C;
		Index = 0x45; statusIDArray[ 0x1FE ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x46; statusIDArray[ 0x1FF ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A1 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x195 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x90; statusIDArray[ 0xB4 ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5 ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D ]  = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E ]  = Index; statusArray[ Index ][2] = __D4;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2C4 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C5 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x07){							//�H�X�q
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x40; statusIDArray[ 0x263 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x41; statusIDArray[ 0x264 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x42; statusIDArray[ 0x265 ] = Index; statusArray[ Index ][2] = __623C;
		Index = 0x43; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __412A;
		Index = 0x44; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __412B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x25F ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x71; statusIDArray[ 0x260 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x72; statusIDArray[ 0x261 ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x73; statusIDArray[ 0x262 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x80; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x81; statusIDArray[ 0x259 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x82; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x90; statusIDArray[ 0xB4  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2C2 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C3 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x08){							//��
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x3E; statusIDArray[ 0x140 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x3F; statusIDArray[ 0x141 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x40; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x41; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x65; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19D ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x19F ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x19F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x195 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x70; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x71; statusIDArray[ 0x25B ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x72; statusIDArray[ 0x25C ] = Index; statusArray[ Index ][2] = __236C;
		Index = 0x73; statusIDArray[ 0x261 ] = Index; statusArray[ Index ][2] = __412A;
		Index = 0x74; statusIDArray[ 0x262 ] = Index; statusArray[ Index ][2] = __412B;
		Index = 0x75; statusIDArray[ 0x263 ] = Index; statusArray[ Index ][2] = __412C;
		Index = 0x90; statusIDArray[ 0xB4  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x94; statusIDArray[ 0x1F7 ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x95; statusIDArray[ 0x1F8 ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x96; statusIDArray[ 0x1F9 ] = Index; statusArray[ Index ][2] = __421C;
		Index = 0xA0; statusIDArray[ 0x25D ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0xA1; statusIDArray[ 0x25E ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0xA2; statusIDArray[ 0x25F ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0xA3; statusIDArray[ 0x260 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBA; statusIDArray[ 0x2C1 ] = Index; statusArray[ Index ][2] = __214D;
		Index = 0xBC; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C2 ] = Index; statusArray[ Index ][2] = __236D;
	}
	
	if(myID == 0x09){							//��
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12E ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x3E; statusIDArray[ 0x132 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x40; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x41; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x42; statusIDArray[ 0x1F6 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x43; statusIDArray[ 0x1F7 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19B ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x191 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x195 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x70; statusIDArray[ 0x258 ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x71; statusIDArray[ 0x259 ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x72; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x73; statusIDArray[ 0x25B ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x90; statusIDArray[ 0xB4  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0xA0; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0xA1; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0xA2; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2D0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB7; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = 0;
		Index = 0xB9; statusIDArray[ 0x2D2 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBA; statusIDArray[ 0x2D3 ] = Index; statusArray[ Index ][2] = 0;
		Index = 0xBC; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2D4 ] = Index; statusArray[ Index ][2] = __236D;
		
		//�~�b�V���O�n
		Index = 0xC0; statusIDArray[ 0x2C1 ] = Index; statusIDArray[ 0x2D5 ] = Index;
				  statusIDArray[ 0x2C9 ] = Index; statusIDArray[ 0x2DD ] = Index; statusArray[ Index ][2] = __5;	//�����E�k��
		Index = 0xC1; statusIDArray[ 0x2C2 ] = Index; statusIDArray[ 0x2D6 ] = Index; statusArray[ Index ][2] = __6;	//�ړ�
		Index = 0xC2; statusIDArray[ 0x2C3 ] = Index; statusIDArray[ 0x2D7 ] = Index; statusArray[ Index ][2] = __9;	//�W�����v
		Index = 0xC3; statusIDArray[ 0x2C4 ] = Index; statusIDArray[ 0x2D8 ] = Index; statusArray[ Index ][2] = __A;	//A
		Index = 0xC4; statusIDArray[ 0x2C5 ] = Index; statusIDArray[ 0x2D9 ] = Index; statusArray[ Index ][2] = __B;	//B
		Index = 0xC5; statusIDArray[ 0x2C6 ] = Index; statusIDArray[ 0x2DA ] = Index; statusArray[ Index ][2] = __C;	//C
		Index = 0xC6; statusIDArray[ 0x2C7 ] = Index; statusIDArray[ 0x2DB ] = Index; statusArray[ Index ][2] = __A;	//��A
		Index = 0xC7; statusIDArray[ 0x2C8 ] = Index; statusIDArray[ 0x2DC ] = Index; statusArray[ Index ][2] = __B;	//��B
	}
	
	if(myID == 0x0A){							//����
		Index = 0x30; statusIDArray[ 0x12C ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x31; statusIDArray[ 0x12D ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x32; statusIDArray[ 0x12F ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x33; statusIDArray[ 0x130 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x34; statusIDArray[ 0x131 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x35; statusIDArray[ 0x132 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x36; statusIDArray[ 0x134 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x37; statusIDArray[ 0x133 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x38; statusIDArray[ 0x136 ] = Index; statusArray[ Index ][2] = __22A;
		Index = 0x39; statusIDArray[ 0x135 ] = Index; statusArray[ Index ][2] = __22B;
		Index = 0x3A; statusIDArray[ 0x186 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x3B; statusIDArray[ 0x187 ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x3C; statusIDArray[ 0x188 ] = Index; statusArray[ Index ][2] = __3A;
		Index = 0x3D; statusIDArray[ 0x189 ] = Index; statusArray[ Index ][2] = __3B;
		Index = 0x40; statusIDArray[ 0x1F4 ] = Index; statusArray[ Index ][2] = __236A;
		Index = 0x41; statusIDArray[ 0x1F5 ] = Index; statusArray[ Index ][2] = __236B;
		Index = 0x42; statusIDArray[ 0x25A ] = Index; statusArray[ Index ][2] = __214A;
		Index = 0x43; statusIDArray[ 0x25B ] = Index; statusArray[ Index ][2] = __214B;
		Index = 0x44; statusIDArray[ 0x1FA ] = Index; statusArray[ Index ][2] = __623A;
		Index = 0x45; statusIDArray[ 0x1FB ] = Index; statusArray[ Index ][2] = __623B;
		Index = 0x46; statusIDArray[ 0x1FC ] = Index; statusArray[ Index ][2] = __421A;
		Index = 0x47; statusIDArray[ 0x1FD ] = Index; statusArray[ Index ][2] = __421B;
		Index = 0x48; statusIDArray[ 0x140 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x49; statusIDArray[ 0x141 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x4A; statusIDArray[ 0x142 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x4B; statusIDArray[ 0x143 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x4C; statusIDArray[ 0x144 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x4D; statusIDArray[ 0x145 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x4E; statusIDArray[ 0x146 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x60; statusIDArray[ 0x196 ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x61; statusIDArray[ 0x197 ] = Index; statusArray[ Index ][2] = __6A;
		Index = 0x62; statusIDArray[ 0x198 ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x63; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __A;
		Index = 0x64; statusIDArray[ 0x19A ] = Index; statusArray[ Index ][2] = __2A;
		Index = 0x65; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x66; statusIDArray[ 0x19C ] = Index; statusArray[ Index ][2] = __6B;
		Index = 0x67; statusIDArray[ 0x19E ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x68; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __B;
		Index = 0x69; statusIDArray[ 0x1A0 ] = Index; statusArray[ Index ][2] = __2B;
		Index = 0x6A; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6B; statusIDArray[ 0x190 ] = Index; statusArray[ Index ][2] = __6C;
		Index = 0x6C; statusIDArray[ 0x192 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x6D; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __C;
		Index = 0x6E; statusIDArray[ 0x194 ] = Index; statusArray[ Index ][2] = __2C;
		Index = 0x90; statusIDArray[ 0xB4  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x91; statusIDArray[ 0xB5  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0x92; statusIDArray[ 0x5D  ] = Index; statusArray[ Index ][2] = __D6;
		Index = 0x93; statusIDArray[ 0x5E  ] = Index; statusArray[ Index ][2] = __D4;
		Index = 0xB0; statusIDArray[ 0x2BC ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB3; statusIDArray[ 0x2BD ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB6; statusIDArray[ 0x2BE ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xB9; statusIDArray[ 0x2BF ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBC; statusIDArray[ 0x2C0 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xBF; statusIDArray[ 0x2C2 ] = Index; statusArray[ Index ][2] = __236D;
		Index = 0xC0; statusIDArray[ 0x2C3 ] = Index; statusArray[ Index ][2] = __5;
	}
	
	
	for(Counter=0;Counter<256;Counter++){
		commandArray[Counter] = 0;
	}
	
	//commandArray��8�o�C�g
	commandArray[ __5 ]	= 0x0F;		//0F ���͖���
	commandArray[ __1 ]	= 0x2E01;		//2E �����ԉ���
	commandArray[ __2 ]	= 0x2E01;
//	commandArray[ __2 ]	= 0x2E02;
	commandArray[ __3 ]	= 0x03;
	commandArray[ __4 ]	= 0x2E04;
	commandArray[ __6 ]	= 0x2E06;
	commandArray[ __7 ]	= 0x07;
	commandArray[ __8 ]	= 0x08;
	commandArray[ __9 ]	= 0x09;
	commandArray[ __D4 ]	= 0x04140D;		//D �� 4�𓯎�����	��0D �� 14
	commandArray[ __D6 ]	= 0x06160D;
	commandArray[ __D6_ ]	= 0x2E06160D;
	commandArray[ __D7 ]	= 0x07170D;
	commandArray[ __D8 ]	= 0x08180D;
	commandArray[ __D9 ]	= 0x09190D;
	commandArray[ __A ]	= 0x0A0F;
	commandArray[ __A_ ]	= 0x1E0A0F;		//1E ������
	commandArray[ __B ]	= 0x0B0F;
	commandArray[ __B_ ]	= 0x1E0B0F;
	commandArray[ __C ]	= 0x0C0F;
	commandArray[ __C_ ]	= 0x1E0C0F;
	commandArray[ __2A ]	= 0x0F1A02;	//�Ō��0x0F�͎�����	//���܂ɔF�����Ȃ����Ƃ��L��
	commandArray[ __2B ]	= 0x0F1B02;
	commandArray[ __2C ]	= 0x0F1C02;
	commandArray[ __6A ]	= 0x0F1A06;
	commandArray[ __6B ]	= 0x0F1B06;
	commandArray[ __6C ]	= 0x0F1C06;
	commandArray[ __3A ]	= 0x0F1A03;
	commandArray[ __3B ]	= 0x0F1B03;
	commandArray[ __22A ]	= 0x0A020F02;	//0F ���͖���	//���܂ɔF�����Ȃ����Ƃ��L��
	commandArray[ __22B ]	= 0x0B020F02;
	commandArray[ __22C ]	= 0x0C020F02;
	commandArray[ __22D ]	= 0x0D020F02;
	commandArray[ __236A ]	= 0x0A060302;
	commandArray[ __236B ]	= 0x0B060302;
	commandArray[ __236C ]	= 0x0C060302;
	commandArray[ __236D ]	= 0x0D060302;
	commandArray[ __623A ]	= 0x0A030206;
	commandArray[ __623B ]	= 0x0B030206;
	commandArray[ __623C ]	= 0x0C030206;
	commandArray[ __214A ]	= 0x0A040102;
	commandArray[ __214B ]	= 0x0B040102;
	commandArray[ __214C ]	= 0x0C040102;
	commandArray[ __214D ]	= 0x0D040102;
	commandArray[ __421A ]	= 0x0A010204;
	commandArray[ __421B ]	= 0x0B010204;
	commandArray[ __421C ]	= 0x0C010204;
	commandArray[ __412A ]	= 0x0A020104;
	commandArray[ __412B ]	= 0x0B020104;
	commandArray[ __412C ]	= 0x0C020104;
#if defined(__BORLANDC__)
	commandArray[ __41236A ] = 0x0A0603020104;
	commandArray[ __41236B ] = 0x0B0603020104;
#else
	commandArray[ __41236A ] = 0x0A0603020104LL;
	commandArray[ __41236B ] = 0x0B0603020104LL;
#endif
	commandArray[ __D6A ] = 0x0A061D06;		//������
	commandArray[ __D6B ] = 0x0B061D06;
	commandArray[ __D63A ] = 0x0A031D06;
	commandArray[ __D63B ] = 0x0B031D06;
	
}
