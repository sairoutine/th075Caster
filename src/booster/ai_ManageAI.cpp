#include "conf.h"
#include "boosterDatClass.h"

void boosterDatClass::ManageAI(){
	//�A���U���̏�񂪂ǂ��Ȃ邩������܂ŕ��u
	//������wAI��ǉ��H
	
	
	if(commandInput[0] == __5){
		commandInput[0] = 0;
	}
	
	//�G�s���������s�����G�s���ւ̃K�[�h���́��G�H�炢����ށ@��h��
	if(commandInput[0]==0 && gameInfoPara[15][1]==5 && (*enGameInfo[ _status ][0][0]==2 || *enGameInfo[ _status ][0][0]==9 || *enGameInfo[ _status ][9][0]==0x22)){
		commandInput[0] = __5;
	}
	
	//�u���C�N���̓W�����v���Ȃ�
	if(commandInput[0]==__7 || commandInput[0]==__8 || commandInput[0]==__9
	|| commandInput[0]==__D7 || commandInput[0]==__D8 || commandInput[0]==__D9){
		if(myGameInfo[ _info ][2][0]==2 && *enGameInfo[ _status ][0][0]!=2 && *enGameInfo[ _status ][0][0]!=9){
			commandInput[0] = 0;
		}
	}
	
	//�Ђ�ݏ�Ԃ̂Ƃ�
	if(statusID==0x22 && !(commandInput[0]==__1 || commandInput[0]==__4)){
		commandInput[0] = __4;
	}
	
	//�n��O�_�b�V����
	if(statusID ==0x10 && commandInput[0]){
		if(!(commandInput[0] == __1 || commandInput[0] == __4
		|| commandInput[0] == __3A || commandInput[0] == __3B
		|| commandInput[0] == __6A || commandInput[0] == __6B
		|| commandInput[0] == __D7 || commandInput[0] == __D8 || commandInput[0] == __D9 || commandInput[0] == __D4
		|| commandInput[0] == __D6A || commandInput[0] == __D6B || commandInput[0] == __D63A || commandInput[0] == __D63B
		|| commandInput[0] == __236D || commandInput[0] == __22D
		)){
			commandInput[0] = 0;
		}
		if(commandInput[0] == __D9){
			commandInput[0] = __9;
		}
	}
	
	//�_�b�V���ł��Ȃ��Ƃ�
	if(myGameInfo[ _info ][6][1]==0){
		if(commandInput[0] == __D4 || commandInput[0] == __D6){
			commandInput[0] = __4;
		}
	}
	
	//�쌂�𐧌�����
	if(commandInput[0]==__22C){
		if(*enGameInfo[ _status ][0][0]==0 || *enGameInfo[ _status ][0][0]==0xA || myGameInfo[ _para ][1][0] > 3){
			commandInput[0] = 0;
		}
	}
	
	//�C���^�[�o��
	if(commandInput[0]){
		if( gameTime < intervalFlg || gameTime < enDat->intervalFlg ){
			if(!(commandInput[0] == __4 || commandInput[0] == __6
			|| commandInput[0] == __7  || commandInput[0] == __8  || commandInput[0] == __9
			|| commandInput[0] == __D4 || commandInput[0] == __D6
			|| commandInput[0] == __D7 || commandInput[0] == __D8 || commandInput[0] == __D9)){
				commandInput[0] = 0;
			}
		}
	}
	
	
	//debug
	if( scriptFlg && script_mode ){
		commandInput[0] = script.GetInput();
	}
	
}
