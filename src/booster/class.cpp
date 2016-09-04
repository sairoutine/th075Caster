#include "conf.h"
#include "boosterDatClass.h"
//�R���X�g���N�^
//�f�X�g���N�^

boosterDatClass::boosterDatClass(WORD playerSideTemp){
	playerSide = playerSideTemp;
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".boosterDatC()" << endl;
	#endif
	strcpy(Name,"init\0");
	strcpy(NameTemp,"init\0");
	strcpy(enName,"init\0");
	strcpy(enNameTemp,"init\0");
	strcpy(baseAIname,"init\0");
	strcpy(spellAIname,"init\0");
	strcpy(backAIname,"init\0");
	strcpy(individualAImyName,"init\0");
	strcpy(individualAIenName,"init\0");
	strcpy(localAIname,"init\0");
	CreateDirectory("dat", NULL);
	AIMode = 0;
	bodyIniFlg = 1;
	secondaryModeFlg = 0;
	boosterDatInitFlg = 1;
	
	
	if( script.init( "script\\script.cpp" ) ){
		cout << "ERROR : script init failed" << endl;
		scriptFlg = 0;
	}else{
		scriptFlg = 1;
	}
	
	
	listeningMode = 0;
	
	for(Counter=0; Counter<10; Counter++){
		for(Counter2=0; Counter2<6; Counter2++){
			gameInfoIni[Counter][Counter2] = 0;
		}
	}
	for(Counter=0; Counter<10; Counter++){
		for(Counter2=0; Counter2<3; Counter2++){
			gameInfoInput[Counter][Counter2] = 0;
			gameInfoPara[Counter][Counter2] = 0;
			gameInfoPara[Counter + 10][Counter2] = 0;
			gameInfoImpress[Counter][Counter2] = 0;
		}
	}
	for(Counter=0; Counter<8; Counter++){
		AIsizeArray[Counter] =0;
	}
	
	//��������AI�̏����ݒ�
	#if debug_mode_AIsize_show
		cout << "debug : " << hex << playerSide << ".AIsize show" << endl;
	#endif
	
	for(Counter=0;Counter<10;Counter++){
		for(Counter2=0;Counter2<4;Counter2++){
			eigenValueSN[Counter][Counter2]=0;
			eigenValueSW[Counter][Counter2]=0;
			eigenValueLN[Counter][Counter2]=0;
			eigenValueLocal[Counter][Counter2]=0;
			eigenValueSpell[Counter][Counter2]=0;
			eigenValueRecover[Counter][Counter2]=0;
		}
	}
	
	//SNAI
	eigenValueSN[0][0] = 1;	//���R�x
	eigenValueSN[1][0] = 8;	//�������R�x
	eigenValueSN[2][0] = 4;	//�����������R�x
	eigenValueSN[3][0] = 5;	//�e�L�������R�x
	eigenValueSN[4][0] = 6;	//�e�L��Ԏ��R�x
	eigenValueSN[5][0] = 1;	//���R�x		//��{���i�q
	eigenValueSN[6][0] = 0;
	eigenValueSN[7][0] = 0;
	eigenValueSN[8][0] = 0;
	eigenValueSN[9][0] = 0;
	
	eigenValueSN[5][1] = 2400;	//��{���i�q�̗e�� : ��{���P�� 8Byte * 10 * 30��
	
	for(Counter=9; Counter>0; Counter--){
		if(eigenValueSN[Counter][0]){
			eigenValueSN[Counter - 1][1] = eigenValueSN[Counter][0] * eigenValueSN[Counter][1];
		}else{
			eigenValueSN[Counter][1] = 0;
			eigenValueSN[Counter][2] = 0;
			eigenValueSN[Counter][3] = 0;
		}
	}
	
	#if debug_mode_AIsize_show
		cout << "SmallScale N AI : " << dec << eigenValueSN[0][1] << " Byte" << endl;
	#endif
	AIsizeArray[0] = AIsizeArray[0] + eigenValueSN[0][1];	//AI�̑��ʂ��X�V
	if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] > AI_size){
		#if debug_mode_AIsize_show
			cout	<< "Small Scale Normal AI    : AI_size is too small. Require "
				<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] << " Byte" << endl;
		#endif
		AIsizeArray[0] = AIsizeArray[0] - eigenValueSN[0][1];
		eigenValueSN[0][0] = 0;
		eigenValueSN[0][1] = 0;
	}else{
		if(eigenValueSN[0][1] == 0){
			eigenValueSN[0][0] = 0;
		}
	}
	
	
	
	//SWAI
	eigenValueSW[0][0] = 1;	//���R�x
	eigenValueSW[1][0] = 3;	//�������R�x
	eigenValueSW[2][0] = 4;	//�����������R�x
	eigenValueSW[3][0] = 5;	//�e�L�������R�x
	eigenValueSW[4][0] = 6;	//�e�L��Ԏ��R�x
	eigenValueSW[5][0] = 1;	//���R�x		//��{���i�q
	eigenValueSW[6][0] = 0;
	eigenValueSW[7][0] = 0;
	eigenValueSW[8][0] = 0;
	eigenValueSW[9][0] = 0;
	
	eigenValueSW[5][1] = 4800;	//��{���i�q�̗e�� : ��{���P�� 8Byte * 20 * 30��
	
	for(Counter=9; Counter>0; Counter--){
		if(eigenValueSW[Counter][0]){
			eigenValueSW[Counter - 1][1] = eigenValueSW[Counter][0] * eigenValueSW[Counter][1];
		}else{
			eigenValueSW[Counter][1] = 0;
			eigenValueSW[Counter][2] = 0;
			eigenValueSW[Counter][3] = 0;
		}
	}
	
	#if debug_mode_AIsize_show
		cout << "SmallScale W AI : " << dec << eigenValueSW[0][1] << " Byte" << endl;
	#endif
	AIsizeArray[0] = AIsizeArray[0] + eigenValueSW[0][1];	//AI�̑��ʂ��X�V
	if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] > AI_size){
		#if debug_mode_AIsize_show
			cout	<< "Small Scale Wall Side AI : AI_size is too small. Require "
				<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] << " Byte" << endl;
		#endif
		AIsizeArray[0] = AIsizeArray[0] - eigenValueSW[0][1];
		eigenValueSW[0][0] = 0;
		eigenValueSW[0][1] = 0;
	}else{
		if(eigenValueSW[0][1] == 0){
			eigenValueSW[0][0] = 0;
		}
	}
	
	//Spell
	eigenValueSpell[0][0] = 1;	//���R�x
	eigenValueSpell[1][0] = 5;	//�������R�x
	eigenValueSpell[2][0] = 3;	//�����������R�x
	eigenValueSpell[3][0] = 3;	//�e�L�������R�x
	eigenValueSpell[4][0] = 11;	//�e�L��Ԏ��R�x
	eigenValueSpell[5][0] = 1;	//���R�x		//��{���i�q
	eigenValueSpell[6][0] = 0;	
	eigenValueSpell[7][0] = 0;
	eigenValueSpell[8][0] = 0;
	eigenValueSpell[9][0] = 0;
	
	eigenValueSpell[5][1] = 40;	//��{���i�q�̗e�� : ��{���P�� 2Byte * 20��	//�債���ʂł͂Ȃ��̂ő��߂�
	
	for(Counter=9; Counter>0; Counter--){
		if(eigenValueSpell[Counter][0]){
			eigenValueSpell[Counter - 1][1] = eigenValueSpell[Counter][0] * eigenValueSpell[Counter][1];
		}else{
			eigenValueSpell[Counter][1] = 0;
			eigenValueSpell[Counter][2] = 0;
			eigenValueSpell[Counter][3] = 0;
		}
	}
	
	#if debug_mode_AIsize_show
		cout << "Spell        AI : " << dec << eigenValueSpell[0][1] << " Byte" << endl;
	#endif
	AIsizeArray[1] = AIsizeArray[1] + eigenValueSpell[0][1];	//AI�̑��ʂ��X�V
	if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] > AI_size){
		#if debug_mode_AIsize_show
			cout	<< "Spell AI : AI_size is too small. Require "
				<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] << " Byte" << endl;
		#endif
		AIsizeArray[1] = AIsizeArray[1] - eigenValueSpell[0][1];
		eigenValueSpell[0][0] = 0;
		eigenValueSpell[0][1] = 0;
	}else{
		if(eigenValueSpell[0][1] == 0){
			eigenValueSpell[0][0] = 0;
		}
	}
	
	
	
	//�������� _vs_.dat
	
	//LNAI
	eigenValueLN[0][0] = 1;	//���R�x
	eigenValueLN[1][0] = 7;	//�������R�x
	eigenValueLN[2][0] = 3;	//�����������R�x
	eigenValueLN[3][0] = 3;	//�e�L�������R�x
	eigenValueLN[4][0] = 1;	//���R�x		//��{���i�q
	eigenValueLN[5][0] = 0;
	eigenValueLN[6][0] = 0;
	eigenValueLN[7][0] = 0;
	eigenValueLN[8][0] = 0;
	eigenValueLN[9][0] = 0;
	
	eigenValueLN[4][1] = 50000;	//��{���i�q�̗e�� : ��{���P�� 4Byte * 5 * 50 * 50��
	
	for(Counter=9; Counter>0; Counter--){
		if(eigenValueLN[Counter][0]){
			eigenValueLN[Counter - 1][1] = eigenValueLN[Counter][0] * eigenValueLN[Counter][1];
		}else{
			eigenValueLN[Counter][1] = 0;
			eigenValueLN[Counter][2] = 0;
			eigenValueLN[Counter][3] = 0;
		}
	}
	
	#if debug_mode_AIsize_show
		cout << "LargeScale N AI : " << dec << eigenValueLN[0][1] << " Byte" << endl;
	#endif
	AIsizeArray[2] = AIsizeArray[2] + eigenValueLN[0][1];	//AI�̑��ʂ��X�V
	if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] > AI_size){
		#if debug_mode_AIsize_show
			cout	<< "Large Scale AI : AI_size is too small. Require "
				<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] << " Byte" << endl;
		#endif
		AIsizeArray[2] = AIsizeArray[2] - eigenValueLN[0][1];
		eigenValueLN[0][0] = 0;
		eigenValueLN[0][1] = 0;
	}else{
		if(eigenValueLN[0][1] == 0){
			eigenValueLN[0][0] = 0;
		}
	}
	
	//RecoverAI
	eigenValueRecover[0][0] = 1;	//���R�x
	eigenValueRecover[1][0] = 7;	//�������R�x
	eigenValueRecover[2][0] = 3;	//�����������R�x
	eigenValueRecover[3][0] = 3;	//�e�L�������R�x
	eigenValueRecover[4][0] = 1;	//���R�x		//��{���i�q
	eigenValueRecover[5][0] = 0;
	eigenValueRecover[6][0] = 0;
	eigenValueRecover[7][0] = 0;
	eigenValueRecover[8][0] = 0;
	eigenValueRecover[9][0] = 0;
	
	eigenValueRecover[4][1] = 20;	//��{���i�q�̗e�� : ��{���P�� 2Byte * 10��	//�Ƃ肠�������߂�
	
	for(Counter=9; Counter>0; Counter--){
		if(eigenValueRecover[Counter][0]){
			eigenValueRecover[Counter - 1][1] = eigenValueRecover[Counter][0] * eigenValueRecover[Counter][1];
		}else{
			eigenValueRecover[Counter][1] = 0;
			eigenValueRecover[Counter][2] = 0;
			eigenValueRecover[Counter][3] = 0;
		}
	}
	
	#if debug_mode_AIsize_show
		cout << "Recover      AI : " << dec << eigenValueRecover[0][1] << " Byte" << endl;
	#endif
	AIsizeArray[2] = AIsizeArray[2] + eigenValueRecover[0][1];	//AI�̑��ʂ��X�V
	if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] > AI_size){
		#if debug_mode_AIsize_show
			cout	<< "Recover      AI : AI_size is too small. Require "
				<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] << " Byte" << endl;
		#endif
		AIsizeArray[2] = AIsizeArray[2] - eigenValueRecover[0][1];
		eigenValueRecover[0][0] = 0;
		eigenValueRecover[0][1] = 0;
	}else{
		if(eigenValueRecover[0][1] == 0){
			eigenValueRecover[0][0] = 0;
		}
	}
	
	
	//�������� local.dat
	
	Flg = AI_local;
	if(Flg){
		eigenValueLocal[0][0] = AI_local;//���R�x				//eigenValueLocal�̓W�J
		eigenValueLocal[1][0] = 1;	//���R�x	//����
		eigenValueLocal[2][0] = 1;	//���R�x	//����
		eigenValueLocal[3][0] = 1;	//���R�x	//����
		eigenValueLocal[4][0] = 1;	//���R�x	//����
		eigenValueLocal[5][0] = 1;	//���R�x	//����
		eigenValueLocal[6][0] = 1;	//���R�x	//����
		eigenValueLocal[7][0] = 1;	//���R�x	//����
		eigenValueLocal[8][0] = 1;	//���R�x	//����
		eigenValueLocal[9][0] = 1;	//���R�x	//����
		
		eigenValueLocal[9][1] = 0;	//��{���i�q�̗e�� : ��{���P�� ����
		
		for(Counter=9; Counter>0; Counter--){
			if(eigenValueLocal[Counter][0]){
				eigenValueLocal[Counter - 1][1] = eigenValueLocal[Counter][0] * eigenValueLocal[Counter][1];
			}
		}
		
		#if debug_mode_AIsize_show
			cout << "Local        AI : " << dec << eigenValueLocal[0][1] << " Byte" << endl;
		#endif
		AIsizeArray[3] = AIsizeArray[3] + eigenValueLocal[0][1];	//AI�̑��ʂ��X�V
		if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] > AI_size){
			#if debug_mode_AIsize_show
				cout	<< "Local        AI : AI_size is too small. Require "
					<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] << " Byte" << endl;
			#endif
			AIsizeArray[3] = AIsizeArray[3] - eigenValueLocal[0][1];
			eigenValueLocal[0][0] = 0;
			eigenValueLocal[0][1] = 0;
		}else{
			if(eigenValueLocal[0][1] == 0){
				eigenValueLocal[0][0] = 0;
			}
		}
	}
	
	//BackAI
	eigenValueBack[0][0] = 1;	//���R�x
	eigenValueBack[1][0] = 5;	//�������R�x
	eigenValueBack[2][0] = 3;	//�����������R�x
	eigenValueBack[3][0] = 4;	//�e�L�������R�x
	eigenValueBack[4][0] = 1;	//���R�x		//��{���i�q
	eigenValueBack[5][0] = 0;
	eigenValueBack[6][0] = 0;
	eigenValueBack[7][0] = 0;
	eigenValueBack[8][0] = 0;
	eigenValueBack[9][0] = 0;
	
	eigenValueBack[4][1] = 1200;	//��{���i�q�̗e�� : ��{���P�� 4Byte * 10 * 30��
	
	for(Counter=9; Counter>0; Counter--){
		if(eigenValueBack[Counter][0]){
			eigenValueBack[Counter - 1][1] = eigenValueBack[Counter][0] * eigenValueBack[Counter][1];
		}else{
			eigenValueBack[Counter][1] = 0;
			eigenValueBack[Counter][2] = 0;
			eigenValueBack[Counter][3] = 0;
		}
	}
	
	#if debug_mode_AIsize_show
		cout << "Backward     AI : " << dec << eigenValueBack[0][1] << " Byte" << endl;
	#endif
	AIsizeArray[4] = AIsizeArray[4] + eigenValueBack[0][1];	//AI�̑��ʂ��X�V
	if(AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] + AIsizeArray[4] > AI_size){
		#if debug_mode_AIsize_show
			cout	<< "Backward     AI : AI_size is too small. Require "
				<< dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] + AIsizeArray[4] << " Byte" << endl;
		#endif
		AIsizeArray[4] = AIsizeArray[4] - eigenValueBack[0][1];
		eigenValueBack[0][0] = 0;
		eigenValueBack[0][1] = 0;
	}else{
		if(eigenValueBack[0][1] == 0){
			eigenValueBack[0][0] = 0;
		}
	}
	
	//AI�̂܂Ƃ�
	AI = 0;
	AI = (char*)malloc( AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] + AIsizeArray[4] );
	#if debug_mode_AIsize_show
		cout << "ALL             : " << dec << AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] + AIsizeArray[4] << " Byte" << endl;
	#endif
	
	if(AI) memset(AI, 0, AIsizeArray[0] + AIsizeArray[1] + AIsizeArray[2] + AIsizeArray[3] + AIsizeArray[4]);
	
}



boosterDatClass::~boosterDatClass(){
	#if debug_mode
		cout << "debug : " << hex << playerSide << ".~boosterDatC()" << endl;
	#endif
	
	//�{�^�����͂��J��
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
				gameInfoIni[Counter][5]=0;
			}
		}
	}
	
	if(strcmp(Name, "init") && strcmp(Name, "second")){			//init,second�ȊO�̂Ƃ��ۑ�
		
		Flg = CloseAI();
		
		#if debug_mode
			if( Flg ) cout << "debug : ~boosterDatC().CloseAI failed." << endl;
		#endif
		
		Flg = CloseSpellAI();
		#if debug_mode
			if( Flg ) cout << "debug : ~boosterDatC().CloseSpellAI failed." << endl;
		#endif
		
		if(eigenValueLocal[0][0]){
			Flg = CloseLocalAI();
			#if debug_mode
				if( Flg ) cout << "debug : ~boosterDatC().CloseLocalAI failed." << endl;
			#endif
		}
		
		Flg = CloseBackAI();
		#if debug_mode
			if( Flg ) cout << "debug : ~boosterDatC().CloseBackAI failed." << endl;
		#endif
		
		
		if(strcmp(enName, "init")){
			Flg = CloseIndividualAI();
			#if debug_mode
				if( Flg ) cout << "debug : ~boosterDatC().CloseIndividualAI failed." << endl;
			#endif
		}
	}
	
	if(AI) free(AI);
	AI = NULL;
	
}

