#include "mainDatClass.h"
#include <mmsystem.h>
using namespace std;

#define rand_show 0


int mainDatClass::SetWindowResize() {
	if( !th075Flg ) return 1;
	
	// Kill the fader off if window is too big or if transitions are disabled, whacking dgraphics-error too
	if (windowWidth > 1024 || windowDisableTransitionsFlg) {
		DWORD frames = 0;
		WriteCode( (void *)0x432038, frames );
		WriteCode( (void *)0x432130, frames );
		// DGraphics-Error
		WriteCode( (void*)0x401a00, 0xc3 ); // RETN
	}
	
	if (windowWidth == 640) {
		return 0;
	}
	
	windowMultiplier = (float)windowWidth / 640.0;
	
	// Window width/height
	WriteProcessMemory( pi.hProcess, (void *)0x602AF2, &windowWidth, 4, 0);
	WriteProcessMemory( pi.hProcess, (void *)0x602B18, &windowHeight, 4, 0);
	WriteProcessMemory( pi.hProcess, (void *)0x602DA5, &windowHeight, 4, 0);
	WriteProcessMemory( pi.hProcess, (void *)0x602DAA, &windowWidth, 4, 0);
	
	// Fader windowWidth/windowHeight
	WriteProcessMemory( pi.hProcess, (void *)0x43241A, &windowHeight, 4, 0);
	WriteProcessMemory( pi.hProcess, (void *)0x43241F, &windowWidth, 4, 0);
	WriteProcessMemory( pi.hProcess, (void *)0x43236A, &windowHeight, 4, 0);
	WriteProcessMemory( pi.hProcess, (void *)0x43236F, &windowWidth, 4, 0);
	
	WriteProcessMemory( pi.hProcess, (void *)0x66c084, &windowMultiplier, 4, 0);
	BYTE code[2] = {
		0xCC, // INT3
		0x90  // NOP
	};
	
	WriteMemory( (void*)render_scale_int3_address, code, 2);
	
	if (windowWidth > 1024) {
		WriteMemory( (void*)high_res_fix_int3_address, code, 2);
	}
	
	WriteMemory( (void*)fader_scale_int3_address1, code, 2);
	WriteMemory( (void*)fader_scale_int3_address2, code, 2);
	
	BYTE code2[2] = {
		0xCC, // INT3
		0xC3  // RETN
	};
	WriteMemory( (void*)fader_scale_int3_address3, code2, 2);
	WriteMemory( (void*)fader_scale_int3_address4, code2, 2);
	
	if (windowFilterState > 0) {
		// Force bilinear filtering of entire screen
		DWORD filter = 2; // D3DTEXF_LINEAR
		WriteCode( (void *)0x402440, filter);
		
		if (windowFilterState < 3) {
			// ... Except the character sprites.
			WriteMemory( (void*)sprite_filter_int3_address_start, code, 2);
			if (windowFilterState == 1) {
				WriteMemory( (void*)sprite_filter_int3_address_end2, code, 2);
			} else {
				WriteMemory( (void*)sprite_filter_int3_address_end, code, 2);
			}
		}
	}
	
	return 0;
}


int mainDatClass::TestPort( SOCKADDR_IN* TargetTemp ){
	if( !TargetTemp ) return status_error;
	
	int Res;
	SOCKET sTemp;
	SOCKADDR_IN Target;
	SOCKADDR_IN HereTemp;
	memset( &HereTemp, 0, sizeof(SOCKADDR_IN) );
	
	WaitForSingleObject( hMutex, INFINITE );
	Target = *TargetTemp;
	ReleaseMutex( hMutex );
	
	
	
	HereTemp.sin_family = AF_INET;
	HereTemp.sin_addr.s_addr = htonl( INADDR_ANY );
	HereTemp.sin_port = htons( 10000 + rand()%1000 );
	
	Res = status_error;
	
	sTemp = socket(AF_INET , SOCK_DGRAM , 0);
	if( sTemp != INVALID_SOCKET ){
		if( bind( sTemp, (SOCKADDR*)&HereTemp, sizeof(SOCKADDR_IN)) >= 0 ){
			
			BYTE data[8];
			data[0] = cmd_version;
			data[1] = cmd_casters;
			data[2] = cmd_space_2;
			data[3] = cmd_space_3;
			data[4] = cmd_testport;
			*(WORD*)&data[5] = myPort;
			
			testPortFlg = 0;
			int Counter = 0;
			for(;;){
				sendto( sTemp, (const char*)data, 7, 0, (SOCKADDR*)&Target, sizeof(SOCKADDR_IN) );
				Sleep(200);
				if( testPortFlg ){
					Res = status_ok;
					break;
				}
				if( Counter > 10 ){
					Res = status_bad;
					break;
				}
				if( GetEsc() ){
					break;
				}
				Counter++;
			}
		}
		closesocket(sTemp);
	}
	return Res;
}



typedef struct{
	WORD Port;
	SOCKET s;
}echoThStruct;

unsigned __stdcall echoTh( void* Address ){
	if( !Address ) return 1;
	echoThStruct* echoThData = (echoThStruct*)Address;
	
	SOCKADDR_IN addr;
	int size;
	int addrSize = sizeof(SOCKADDR_IN);
	size = recvfrom( echoThData->s, NULL, NULL, 0, (SOCKADDR*)&addr, &addrSize);
	if( size < 0 ){
		echoThData->Port = 0;
		return 1;
	}
	echoThData->Port = addr.sin_port;
	return 0;
}

int GetMyPortSub( SOCKET* s, WORD* myPortTemp ){
	if( !myPortTemp ) return 1;
	*myPortTemp = 0;
	
	DWORD res;
	echoThStruct echoThData;
	HANDLE hEchoTh;
	SOCKADDR_IN Echo;
	memset( &Echo, 0, sizeof(SOCKADDR_IN) );
	Echo.sin_family = AF_INET;
	Echo.sin_addr.s_addr = inet_addr( "127.0.0.1" );
	
	echoThData.Port = 0;
	echoThData.s = socket(AF_INET , SOCK_DGRAM , 0);
	if( echoThData.s == INVALID_SOCKET ) return 1;
	
	int Counter = 0;
	for(;;){
		Echo.sin_port = 10000 + rand()%10000;
		if( bind( echoThData.s, (SOCKADDR*)&Echo, sizeof(Echo) ) >= 0 ) break;
		if( Counter > 100 ){
			closesocket(echoThData.s);
			return 1;
		}
		Counter++;
		Sleep( 1 );
	}
	
	
	hEchoTh = (HANDLE)_beginthreadex(NULL, 0, echoTh, &echoThData, 0, NULL);
	if( !hEchoTh ) return 1;
	
	Counter = 0;
	for(;;){
		sendto( *s, NULL, 0, 0, (SOCKADDR*)&Echo, sizeof(Echo) );
		
		res = WaitForSingleObject( hEchoTh, 10 );
		if( echoThData.Port ){
			*myPortTemp = echoThData.Port;
			break;
		}
		if( res == WAIT_OBJECT_0 || res == WAIT_FAILED ){
			closesocket(echoThData.s);
			return 1;
		}
		if( Counter > 20 ) return 1;
		Counter++;
	}
	closesocket(echoThData.s);
	Sleep(10);
	CloseHandle(hEchoTh);
	
	
	return 0;
}

int mainDatClass::GetMyPort(){
	WORD myPortTemp;
	if( GetMyPortSub( &s, &myPortTemp ) ){
//		cout << "Port : BAD ( UDP." << myPort << " Open failed )" << endl;
		cout << "Port : Unknown" << endl;
		myPort = 0;
		return 1;
	}
	myPort = ntohs( myPortTemp );
	cout << "Port : Using UDP." << dec << myPort << endl;
	return 0;
}


int mainDatClass::GetDelay( BYTE dest, float* delayTemp ){
	#if debug_mode_func
		cout << "debug : GetDelay()" << endl;
	#endif
	clock_t pTime;
	delayTimeObsNo = 0;
	DWORD timeTemp;
	
	
	//取り扱う時間の精度を設定
	TIMECAPS timeCaps;
	WORD timeCapsFlg = 0;
	if( timeGetDevCaps( &timeCaps, sizeof(timeCaps) ) == TIMERR_NOERROR ){
		if( timeBeginPeriod( timeCaps.wPeriodMin ) == TIMERR_NOERROR ){
			timeCapsFlg = 1;
		}
	}
	
	for(;;){
		timeTemp = timeGetTime();
		SendCmd( dest, cmd_delayobs, &timeTemp, 4 );
		if( delayTimeObsNo > 4 ) break;
		if( GetEsc() ) return 1;
		Sleep(50);
	}
	
	
	//時間の精度の設定を戻す
	if( timeCapsFlg ){
		if( TIMERR_NOERROR != timeEndPeriod( timeCaps.wPeriodMin ) ) return 1;
	}
	
	
	*delayTemp = ( delayTimeObs[ 0 ] + delayTimeObs[ 1 ] + delayTimeObs[ 2 ] + delayTimeObs[ 3 ] + delayTimeObs[ 4 ] ) /  5 ;
	return 0;
}

int mainDatClass::GetPlayerSide(){
	#if debug_mode_func
		cout << "debug : GetPlayerSide()" << endl;
	#endif
	myRandNo = 1;
	if( playerSideFlg == 1 ){
		myRand = 0xFF;
	}else if( playerSideFlg == 2 ){
		myRand = 1;
	}else{
		myRand = (BYTE)( 1 + rand()%254 );
	}
	BYTE myRandTemp = myRand;
	
	myInfo.playerSide = 0;
	enInfo.playerSide = 0;
	
	enRandNo = 0;
	enRand = 0;
	for(;;){
//		cout << (WORD)myRandNo << "." << (WORD)myRand << endl;
//		if( enInfo.playerSide ) break;
		if( myRandNo == enRandNo ){
			
			if( enRand == myRand ){
				if( myRandNo > 10 ) srand( (unsigned)time( NULL ) );
				
				if( playerSideFlg && playerSideHostFlg && enRandNo < 10 ){
					if( playerSideFlg == 1 ){
						myRand = 0xFF;
					}else{
						myRand = 1;
					}
				}else if( playerSideFlg && !playerSideHostFlg && enRandNo < 5 ){
					if( playerSideFlg == 1 ){
						myRand = 0xFF;
					}else{
						myRand = 1;
					}
				}else{
					while( myRandTemp == myRand ){
						myRand = (BYTE)( 1 + rand() % 254 );
					}
				}
				
				if( myRandNo > 250 ){
					return 1;
				}else{
					myRandNo = myRandNo + 1;
				}
				myRandTemp = myRand;
			}else{
				if( myRand > enRand ){
					myInfo.playerSide = 0xA;
					enInfo.playerSide = 0xB;
				}else{
					myInfo.playerSide = 0xB;
					enInfo.playerSide = 0xA;
				}
				break;
			}
		}else if( myRandNo < enRandNo ){
			if( playerSideFlg && playerSideHostFlg && enRandNo < 10 ){
				if( playerSideFlg == 1 ){
					myRand = 0xFF;
				}else{
					myRand = 1;
				}
			}else if( playerSideFlg && !playerSideHostFlg && enRandNo < 5 ){
				if( playerSideFlg == 1 ){
					myRand = 0xFF;
				}else{
					myRand = 1;
				}
			}else{
				while( myRandTemp == myRand ){
					myRand = (BYTE)( 1 + rand() % 254 );
				}
			}
			myRandNo = enRandNo;
			myRandTemp = myRand;
		}
		SendCmd( dest_away, cmd_rand );
//		SendCmd( dest_away, cmd_playerside );
		if( GetEsc() ) return 1;
		Sleep(50);
	}
	
	if( !myInfo.playerSide ){
		if( enInfo.playerSide == 0xA ){
			myInfo.playerSide = 0xB;
		}else if( enInfo.playerSide == 0xB ){
			myInfo.playerSide = 0xA;
		}
	}
	//要検討
	enInfo.playerSide = 0;
	for(;;){
		if( enInfo.playerSide ) break;
		SendCmd( dest_away, cmd_playerside );
		if( GetEsc() ) return 1;
		Sleep(100);
	}
	return 0;
}

int mainDatClass::GetEsc(){
	
	if( escSelectFlg ){
		HWND  hForWnd;
		DWORD PID;
		
		hForWnd = GetForegroundWindow();
		if( hForWnd ){
			GetWindowThreadProcessId( hForWnd , &PID );
			if( GetCurrentProcessId() == PID ){
				if(GetKeyState(VK_ESCAPE)<0) return 1;
			}else if( th075Flg ){
				if( hForWnd == FindWindow( NULL , "東方萃夢想 ～ Immaterial and Missing Power. ver1.11" ) ){
					if(GetKeyState(VK_ESCAPE)<0) return 1;
				}
			}
		}else{
			if(GetKeyState(VK_ESCAPE)<0) return 1;
		}
	}else{
		if(GetKeyState(VK_ESCAPE)<0) return 1;
	}
	return 0;
}

int mainDatClass::WriteCode( void* Address, BYTE code ){
	if( !th075Flg ) return 1;
	WriteProcessMemory( hProcess , Address , &code  , 1 , NULL );
	return 0;
}

int mainDatClass::UnRockTime(){
	if( !th075Flg ) return 1;
	BYTE code[6];
	code[0] = 0x8B;	//MOV EAX,DWORD PTR SS:[EBP-184]
	code[1] = 0x85;
	code[2] = 0x7C;
	code[3] = 0xFE;
	code[4] = 0xFF;
	code[5] = 0xFF;
	WriteMemory( (void*)0x602FA1, code, 6 );
	FlushInstructionCache(pi.hProcess, NULL, 0);
	rockFlg = 0;
	return 0;
}
int mainDatClass::RockTime(){
	if( !th075Flg ) return 1;
	BYTE code[6];
	code[0] = 0xE9;	//JMP 00603539
	code[1] = 0x93;
	code[2] = 0x05;
	code[3] = 0x00;
	code[4] = 0x00;
	code[5] = 0x90;	//NOP
	WriteMemory( (void*)0x602FA1, code, 6 );
	FlushInstructionCache(pi.hProcess, NULL, 0);
	rockFlg = 1;
	return 0;
}

int mainDatClass::SetBodyBreakPoint(){
	if( !th075Flg ) return 1;
	BYTE code[14];
	code[0] = 0xCC;	//INT3
	code[1] = 0x8B;	//MOV EAX,DWORD PTR SS:[EBP-184]
	code[2] = 0x85;
	code[3] = 0x7C;
	code[4] = 0xFE;
	code[5] = 0xFF;
	code[6] = 0xFF;
	code[7] = 0x8B;	//MOV EDX,DWORD PTR DS:[EAX]
	code[8] = 0x10;
	code[9] = 0x8B;	//MOV ECX,EAX
	code[10] = 0xC8;
	code[11] = 0x90;	//NOP
	code[12] = 0x90;	//NOP
	code[13] = 0x90;	//NOP
	ReadProcessMemory(hProcess, (void*)body_int3_address, bodyOldCode, 14, NULL);
	WriteProcessMemory(hProcess, (void*)body_int3_address, code, 14, NULL);
	return 0;
}

int mainDatClass::RemoveBodyBreakPoint(){
	if( !th075Flg ) return 1;
	WriteProcessMemory(hProcess, (void*)body_int3_address, bodyOldCode, 14, NULL);
	return 0;
}



//Char
int mainDatClass::SetCharBreakPoint(){
	if( !th075Flg ) return 1;
	BYTE code[2];
	code[0] = 0xCC;
	code[1] = 0x90;
	WriteProcessMemory(hProcess, (void*)0x445831, code, 2, NULL);
	return 0;
}
int mainDatClass::RemoveCharBreakPoint(){
	if( !th075Flg ) return 1;
	BYTE code[2];
	code[0] = 0x8B;
	code[1] = 0xEC;
	WriteProcessMemory(hProcess, (void*)0x445831, code, 2, NULL);
	return 0;
}

//rand
int mainDatClass::SetRandBreakPoint(){
	if( !th075Flg ) return 1;
	
	BYTE code[40];
	code[0] = 0x90;	//INT3 or NOP
	code[1] = 0x69;	//IMUL EAX, DWORD PTR DS : [66C000], 15A4E35	//66C000→rand_address
	code[2] = 0x05;
	/*
	code[3] = 0x00;
	code[4] = 0xC0;
	code[5] = 0x66;
	code[6] = 0x00;
	*/
	*(DWORD*)&code[3] = rand_address;
	code[7] = 0x35;
	code[8] = 0x4E;
	code[9] = 0x5A;
	code[10] = 0x01;
	code[11] = 0x40;	//INC EAX
	code[12] = 0xA3;	//MOV DWORD PTR DS : [66C000]	//66C000→rand_address
	/*
	code[13] = 0x00;
	code[14] = 0xC0;
	code[15] = 0x66;
	code[16] = 0x00;
	*/
	*(DWORD*)&code[13] = rand_address;
	code[17] = 0xC1;	//SHR EAX, 10
	code[18] = 0xE8;
	code[19] = 0x10;
	code[20] = 0x25;	//AND EAX, 7FFF
	code[21] = 0xFF;
	code[22] = 0x7F;
	code[23] = 0x00;
	code[24] = 0x00;
	
	#if rand_show
		code[25] = 0xCC;	//INT3
	#else
		code[25] = 0x90;	//NOP
	#endif
	code[26] = 0xEB;	//JMP SHORT 4555CB
	code[27] = 0x20;
	
	WriteMemory( (void*)rand_int3_address, code, 28 );
	
	return 0;
}
int mainDatClass::RemoveRandBreakPoint(){
	if( !th075Flg ) return 1;
	
	BYTE code[40];
	code[0] = 0x0F;	//MOVSX EAX,BYTE PTR DS:[66C238]
	code[1] = 0xBE;
	code[2] = 0x05;
	code[3] = 0x38;
	code[4] = 0xC2;
	code[5] = 0x66;
	code[6] = 0x00;
	code[7] = 0x83;	//CMP EAX, 2
	code[8] = 0xF8;
	code[9] = 0x02;
	code[10] = 0x75;	//JNZ SHORT 4555C6
	code[11] = 0x2B;
	code[12] = 0xB9;	//MOV ECX, 6718B8
	code[13] = 0xB8;
	code[14] = 0x18;
	code[15] = 0x67;
	code[16] = 0x00;
	code[17] = 0xE8;	//CALL 414620
	code[18] = 0x7B;
	code[19] = 0xF0;
	code[20] = 0xFB;
	code[21] = 0xFF;
	code[22] = 0x85;	//TEST EAX, EAX
	code[23] = 0xC0;
	code[24] = 0x76;	//JBE SHORT 4555C4
	code[25] = 0x1B;
	code[26] = 0xB9;
	code[27] = 0xB8;
	WriteMemory( (void*)rand_int3_address, code, 28 );
	
	return 0;
}



int mainDatClass::SetCode(){
	if( !th075Flg ) return 1;
	
	//DirectInputを無効にする
	WriteCode( (void*)0x406E80, 0xC3 );	//RETN
	
	//ポーズのバッファを無効にする
	WriteCode( (void*)0x43B4C8, 0xEB );	//JMP SHORT
	
	//ランダム
	SetRandBreakPoint();
	
	//set FPU CW
	if( newCWflg ){
		CONTEXT ctFPU;
		ctFPU.ContextFlags = CONTEXT_FLOATING_POINT;
		GetThreadContext( hProcessTh, &ctFPU );
		ctFPU.FloatSave.ControlWord &= ~0x300; // normally 0x027f, now 0x007f
		SetThreadContext( hProcessTh, &ctFPU );
//		cout << hex << "debug : FPU CW set " << ctFPU.FloatSave.ControlWord << endl;
		
		//replay name
		DWORD status;
		VirtualProtectEx( hProcess, (void*)0x65791F, 6, PAGE_READWRITE, &status);
		
		char repName[30];
		strcpy( repName, "rpycwc\0" );
//		cout << repName << endl;
		WriteMemory( (void*)0x65791F, repName, 6 );
		
		VirtualProtectEx( hProcess, (void*)0x65791F, 6, status, &status);
	}
	
	return 0;
}
int mainDatClass::RemoveCode(){
	if( !th075Flg ) return 1;
	
	//DirectInputを有効にする
	WriteCode( (void*)0x406E80, 0x55 );	//PUSH EBP
	
	//ポーズのバッファを有効にする
	WriteCode( (void*)0x43B4C8 , 0x75 );	//JNZ	SHORT
	
	//ランダム
//	RemoveRandBreakPoint();
	
	//reset repName
	if( newCWflg ){
		//replay name
		DWORD status;
		VirtualProtectEx( hProcess, (void*)0x65791F, 6, PAGE_READWRITE, &status);
		
		char repName[30];
		strcpy( repName, "replay\0" );
		WriteMemory( (void*)0x65791F, repName, 6 );
		
		VirtualProtectEx( hProcess, (void*)0x65791F, 6, status, &status);
	}
	return 0;
}



int mainDatClass::ReadMemory(void* Address, void* data, DWORD size){
	DWORD	sizeRet;
	if( !th075Flg ) return 1;
	ReadProcessMemory( hProcess , Address , data , size , &sizeRet );
	if(sizeRet != size) return 2;
	return 0;
}

int mainDatClass::WriteMemory(void* Address, void* data, DWORD size){
	DWORD	sizeRet;
	if( !th075Flg ) return 1;
	WriteProcessMemory( hProcess , Address , data , size , &sizeRet );
	if(sizeRet != size) return 2;
	return 0;
}

//データ送信
int mainDatClass::SendData(int dest, void* Address, DWORD size){
	return SendDataSub( dest, Address, size, task_main );
}
int mainDatClass::SendData(SOCKADDR_IN* addr, void* Address, DWORD size){
	return SendDataSub( addr, Address, size, task_main );
}

int mainDatClass::SendDataR(int dest, void* Address, DWORD size){
	return SendDataSub( dest, Address, size, task_recv );
}
int mainDatClass::SendDataR(SOCKADDR_IN* addr, void* Address, DWORD size){
	return SendDataSub( addr, Address, size, task_recv );
}

int mainDatClass::SendDataSub( int dest, void* Address, DWORD size, WORD Flg ){
	if(size > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest;
			memcpy( sTask[Counter].data, Address, size);
			sTask[Counter].size = size;
			sTask[Counter].Flg = stask_data;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}

int mainDatClass::SendDataSub( SOCKADDR_IN* addr, void* Address, DWORD size, WORD Flg ){
	if(size > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest_addr;
			WaitForSingleObject( hMutex, INFINITE );
			sTask[Counter].addr = *addr;
			ReleaseMutex( hMutex );
			
			memcpy( sTask[Counter].data, Address, size);
			sTask[Counter].size = size;
			sTask[Counter].Flg = stask_data;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}


//連結データ送信
int mainDatClass::SendData(int dest, void* Address, DWORD size, void* Address2, DWORD size2){
	return SendDataSub( dest, Address, size, Address2, size2, task_main );
}
int mainDatClass::SendData(SOCKADDR_IN* addr, void* Address, DWORD size, void* Address2, DWORD size2){
	return SendDataSub( addr, Address, size, Address2, size2, task_main );
}

int mainDatClass::SendDataR(int dest, void* Address, DWORD size, void* Address2, DWORD size2){
	return SendDataSub( dest, Address, size, Address2, size2, task_recv );
}
int mainDatClass::SendDataR(SOCKADDR_IN* addr, void* Address, DWORD size, void* Address2, DWORD size2){
	return SendDataSub( addr, Address, size, Address2, size2, task_recv );
}
int mainDatClass::SendDataR(int dest, void* Address, DWORD size, void* Address2, DWORD size2, void* Address3, DWORD size3 ){
	return SendDataSub( dest, Address, size, Address2, size2, Address3, size3, task_recv );
}
int mainDatClass::SendDataR(SOCKADDR_IN* addr, void* Address, DWORD size, void* Address2, DWORD size2, void* Address3, DWORD size3 ){
	return SendDataSub( addr, Address, size, Address2, size2, Address3, size3, task_recv );
}

int mainDatClass::SendDataSub( int dest, void* Address, DWORD size, void* Address2, DWORD size2, void* Address3, DWORD size3, WORD Flg ){
	if(size + size2 + size3 > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest;
			memcpy( sTask[Counter].data, Address, size);
			memcpy( sTask[Counter].data + size, Address2, size2);
			memcpy( sTask[Counter].data + size + size2, Address3, size3);
			sTask[Counter].size = size + size2 + size3;
			sTask[Counter].Flg = stask_data;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}

int mainDatClass::SendDataSub( SOCKADDR_IN* addr, void* Address, DWORD size, void* Address2, DWORD size2, void* Address3, DWORD size3, WORD Flg ){
	if(size + size2 + size3 > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest_addr;
			
			WaitForSingleObject( hMutex, INFINITE );
			sTask[Counter].addr = *addr;
			ReleaseMutex( hMutex );
			memcpy( sTask[Counter].data, Address, size);
			memcpy( sTask[Counter].data + size, Address2, size2);
			memcpy( sTask[Counter].data + size + size2, Address3, size3);
			sTask[Counter].size = size + size2 + size3;
			sTask[Counter].Flg = stask_data;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}


int mainDatClass::SendDataSub( int dest, void* Address, DWORD size, void* Address2, DWORD size2, WORD Flg ){
	if(size + size2 > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest;
			memcpy( sTask[Counter].data, Address, size);
			memcpy( sTask[Counter].data + size, Address2, size2);
			sTask[Counter].size = size + size2;
			sTask[Counter].Flg = stask_data;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}

int mainDatClass::SendDataSub( SOCKADDR_IN* addr, void* Address, DWORD size, void* Address2, DWORD size2, WORD Flg ){
	if(size + size2 > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest_addr;
			
			WaitForSingleObject( hMutex, INFINITE );
			sTask[Counter].addr = *addr;
			ReleaseMutex( hMutex );
			memcpy( sTask[Counter].data, Address, size);
			memcpy( sTask[Counter].data + size, Address2, size2);
			sTask[Counter].size = size + size2;
			sTask[Counter].Flg = stask_data;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}

//アドレス
int mainDatClass::SendArea(int dest, void* Address, DWORD size){
	return SendAreaSub( dest, Address, size, task_main );
}
int mainDatClass::SendArea(SOCKADDR_IN* addr, void* Address, DWORD size){
	return SendAreaSub( addr, Address, size, task_main );
}

int mainDatClass::SendAreaR(int dest, void* Address, DWORD size){
	return SendAreaSub( dest, Address, size, task_recv );
}
int mainDatClass::SendAreaR(SOCKADDR_IN* addr, void* Address, DWORD size){
	return SendAreaSub( addr, Address, size, task_recv );
}

int mainDatClass::SendAreaSub( int dest, void* Address, DWORD size, WORD Flg ){
	if(size > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest;
			sTask[Counter].Address = Address;
			sTask[Counter].size = size;
			sTask[Counter].Flg = stask_area;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter == endValue){
			return 1;
		}
	}
	return 0;
}

int mainDatClass::SendAreaSub( SOCKADDR_IN* addr, void* Address, DWORD size, WORD Flg ){
	if(size > stask_buf_size) return 1;
	
	int startValue = 0;
	int endValue = 0;
	if( Flg == task_main ){
		startValue = 0;
		endValue = 19;
	}else if( Flg == task_recv ){
		startValue = 20;
		endValue = 39;
	}else if( Flg == task_manage ){
		startValue = 40;
		endValue = 49;
	}
	
	int Counter;
	for(Counter=startValue; Counter<=endValue; Counter++){
		if(sTask[Counter].Flg == 0){
			sTask[Counter].dest = dest_addr;
			WaitForSingleObject( hMutex, INFINITE );
			sTask[Counter].addr = *addr;
			ReleaseMutex( hMutex );
			
			sTask[Counter].Address = Address;
			sTask[Counter].size = size;
			sTask[Counter].Flg = stask_area;
			
			SetEvent(hSendEvent);
			break;
		}
		if(Counter==endValue){
			return 1;
		}
	}
	return 0;
}

//SendCmd()
int mainDatClass::SendCmd( int dest, BYTE Cmd ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendData( dest, data, 5 ) ) return 1;
	return 0;
}

int mainDatClass::SendCmd( SOCKADDR_IN* addr, BYTE Cmd ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendData( addr, data, 5 ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdR( int dest, BYTE Cmd ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataR( dest, data, 5 ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdR( SOCKADDR_IN* addr, BYTE Cmd ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataR( addr, data, 5 ) ) return 1;
	return 0;
}

//SendCmd+
int mainDatClass::SendCmd( int dest, BYTE Cmd, void* Address, DWORD size ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendData( dest, data, 5, Address, size ) ) return 1;
	return 0;
}

int mainDatClass::SendCmd( SOCKADDR_IN* addr, BYTE Cmd, void* Address, DWORD size ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendData( addr, data, 5, Address, size ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdR( int dest, BYTE Cmd, void* Address, DWORD size ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataR( dest, data, 5, Address, size ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdR( SOCKADDR_IN* addr, BYTE Cmd, void* Address, DWORD size ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataR( addr, data, 5, Address, size ) ) return 1;
	return 0;
}

//3
int mainDatClass::SendCmdR( int dest, BYTE Cmd, void* Address2, DWORD size2, void* Address3, DWORD size3 ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataR( dest, data, 5, Address2, size2, Address3, size3 ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdR( SOCKADDR_IN* addr, BYTE Cmd, void* Address2, DWORD size2, void* Address3, DWORD size3 ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataR( addr, data, 5, Address2, size2, Address3, size3 ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdM( int dest, BYTE Cmd ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataSub( dest, data, 5, task_manage ) ) return 1;
	return 0;
}

int mainDatClass::SendCmdM( SOCKADDR_IN* addr, BYTE Cmd ){
	BYTE data[5];
	data[0] = cmd_version;
	data[1] = cmd_casters;
	data[2] = cmd_space_2;
	data[3] = cmd_space_3;
	data[4] = Cmd;
	if( SendDataSub( addr, data, 5, task_manage ) ) return 1;
	return 0;
}
