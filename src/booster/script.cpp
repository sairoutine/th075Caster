#include "scriptClass.h"
#include <windows.h>
#include <iostream> 
#include <fstream>
#include <cstdlib>
#include <cstring>


scriptClass::scriptClass(){
	body = (char*)malloc( body_size );
}

scriptClass::~scriptClass(){
	if( body ) free( body );
	body = NULL;
}


BYTE scriptClass::GetInput(){
	
	
	return 0;
}


int scriptClass::init( char* scriptName ){
	if( !scriptName ) return 1;
	
	/*
	if( !body ) return 1;
	ifstream fin("script\\script.cpp", ios::in);
	if( fin.fail() ) return 1;
	fin.read(body, 102400);
	*/
	
	return 0;
}

int GetLine( char*, DWORD ){
	
	
	return 0;
}


int DivideLine( char* , contextStruct*){
	





	/*
	if������
	enID������
	==������
	0x**��ǂݍ����INT�ɕϊ�
	�^�U����
	then��
	���s
	endif
	
	*/

	
	return 0;
}
