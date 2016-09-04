#include "mainDatClass.h"
using namespace std;

//http://nienie.com/~masapico/doc_ApiSpy.html
//↑のページ「MASAPICO'S Page」を参考

int mainDatClass::th075Roop( DWORD* deInfo ){
	CONTEXT ct;
	
	*deInfo = 0;
	/* デバッグの継続 */
	if( deInitFlg ){
		if( !ContinueDebugEvent(de.dwProcessId, de.dwThreadId, ContinueStatus) ){
			WaitForSingleObject( hPrintMutex, INFINITE );
			cout << "ERROR : ContinueDebugEvent" << endl;
			ReleaseMutex( hPrintMutex );
			return 1;
		}
	}else{
		deInitFlg = 1;
	}
	if( !WaitForDebugEvent(&de, INFINITE) ){
		WaitForSingleObject( hPrintMutex, INFINITE );
		cout << "ERROR : WaitForDebugEvent" << endl;
		ReleaseMutex( hPrintMutex );
		return 1;
	}
	
	ContinueStatus = DBG_EXCEPTION_NOT_HANDLED;
	
	switch(de.dwDebugEventCode) {
	case CREATE_PROCESS_DEBUG_EVENT:
		{
			LDT_ENTRY ldtEntry;
			LPVOID pBase;
			NT_TIB tib;
			DWORD cbRead = 0;
			int StackOffset = 0;
			
			
			th075Flg = 1;
			hProcessTh  = de.u.CreateProcessInfo.hThread;
			processThID = de.dwThreadId;
			
			// Get StackBase
			ct.ContextFlags = CONTEXT_SEGMENTS;
			GetThreadContext( hProcessTh, &ct );
			GetThreadSelectorEntry( hProcessTh, ct.SegFs, &ldtEntry );
			pBase = (LPVOID)( ldtEntry.BaseLow +
					( ldtEntry.HighWord.Bytes.BaseMid << 16 ) +
					( ldtEntry.HighWord.Bytes.BaseHi << 24 ) );
			ReadProcessMemory( hProcess, pBase, &tib, sizeof( NT_TIB ), &cbRead );
			
			/*
			{
				//for vista
				OSVERSIONINFO ovinfo;
				ZeroMemory(&ovinfo, sizeof(ovinfo));
				ovinfo.dwOSVersionInfoSize = sizeof(ovinfo);
				GetVersionEx(&ovinfo);
				
				if (ovinfo.dwMajorVersion > 5) {
					StackOffset = 0x20;
				}
			}
			*/
			if( strcmp( iniPath, "fail" ) ){
				if( GetPrivateProfileInt( "VISTA", "onoff", 0, iniPath ) ){
					StackOffset = GetPrivateProfileInt( "VISTA", "StackOffset", 0x20, iniPath );
				}
			}
			pStackBase = (void *)((DWORD)tib.StackBase - StackOffset);
//			cout << "StackBase = 0x" << hex << pStackBase << endl;
//			cout << " MenuFlag = 0x" << hex << ((DWORD)pStackBase - 420) << endl;
//			cout << " MenuAddr = 0x" << hex << ((DWORD)pStackBase - 748) << endl;
			
			//priority
			if( priorityFlg == 1 ){
				SetPriorityClass( hProcess, ABOVE_NORMAL_PRIORITY_CLASS );
				SetThreadPriority( hTh075Th, THREAD_PRIORITY_ABOVE_NORMAL );
			}
			if( priorityFlg == 2 ){
				SetPriorityClass( hProcess, HIGH_PRIORITY_CLASS );
				SetThreadPriority( hTh075Th, THREAD_PRIORITY_HIGHEST );
			}
			/*
			if( priorityFlg == 3 ){
				SetPriorityClass( hProcess, REALTIME_PRIORITY_CLASS );
				SetThreadPriority( hTh075Th, THREAD_PRIORITY_TIME_CRITICAL );
				
				WaitForSingleObject( hPrintMutex, INFINITE );
				cout << "debug : REALTIME_PRIORITY_CLASS" << endl;
				ReleaseMutex( hPrintMutex );
			}
			*/
			
			//windowMode
			if( myInfo.terminalMode == mode_branch || myInfo.terminalMode == mode_subbranch || myInfo.terminalMode == mode_debug ){
				if( windowModeFlg ){
					WriteCode( (void*)0x41939D, 0xB5 );	//PUSH 6714B5
					WriteCode( (void*)0x41939E, 0x14 );
					
					//WriteCode( (void*)0x602E67, 0xEB );	//JMP SHORT th075.00602E90
				}
			}
			
			if( stageLimitCancelFlg ){
				//ステージ制限無効
				BYTE code[8];
				code[0] = 0xE9;
				code[1] = 0x9F;
				code[2] = 0x00;
				code[3] = 0x00;
				code[4] = 0x00;
				code[5] = 0x90;
				code[6] = 0x90;
				WriteMemory( (void*)0x43541D, code, 7 );
			}
			
			if( replaySaveFlg ){
				//対戦終了時のメニューのデフォルトをリプレイ保存にする
				WriteCode( (void*)0x43BCBE, 2 );
			}
			
			// Kill DInput-Error box
			WriteCode( (void*)0x403d00, 0xc3 ); // RETN
			
			if( windowResizeFlg == 1 ){
				SetWindowResize();
			}
			
			SetBodyBreakPoint();
			SetCharBreakPoint();
			FlushInstructionCache(pi.hProcess, NULL, 0);
		}
		break;
		
	case EXIT_PROCESS_DEBUG_EVENT:
		th075Flg = 0;
		
		/*
		if( myInfo.terminalMode == mode_debug ){
			WaitForSingleObject( hPrintMutex, INFINITE );
			cout << "debug : EXIT_PROCESS_DEBUG_EVENT" << endl;
			ReleaseMutex( hPrintMutex );
		}
		*/
		return 1;
	
	case EXCEPTION_DEBUG_EVENT: /* 例外発生 */
		switch(de.u.Exception.ExceptionRecord.ExceptionCode) {
		case EXCEPTION_BREAKPOINT:
			/* ブレークポイントに遭遇した場合 */
			ct.ContextFlags = CONTEXT_CONTROL;
			
			if( de.dwThreadId != processThID ){
				WaitForSingleObject( hPrintMutex, INFINITE );
				cout << "ERROR : processThID" << endl;
				ReleaseMutex( hPrintMutex );
				return 1;
			}
			if( !GetThreadContext(hProcessTh, &ct) ){
				WaitForSingleObject( hPrintMutex, INFINITE );
				cout << "ERROR : GetThreadContext" << endl;
				ReleaseMutex( hPrintMutex );
				return 1;
			}
			
			if(ct.Eip - 1 == body_int3_address) {
				*deInfo = de_body;
				
				/*
				//何らかの理由でシングルステップを使うときのため
				//様子を見て削除
				{
					ct.Eip--;
					BYTE code = 0x90;
					if(!WriteProcessMemory(pi.hProcess, (void *)ct.Eip, &code, 1, NULL)){
						WaitForSingleObject( hPrintMutex, INFINITE );
						cout << "ERROR : Set old code ( body )" << endl;
						ReleaseMutex( hPrintMutex );
						return 1;
					}
					FlushInstructionCache(pi.hProcess, NULL, 0);
					ct.EFlags |= EFLAGS_TF;
					ct.ContextFlags = CONTEXT_CONTROL;
					if(!SetThreadContext(hProcessTh, &ct)){
						WaitForSingleObject( hPrintMutex, INFINITE );
						cout << "ERROR : SetThreadContext ( body )" << endl;
						ReleaseMutex( hPrintMutex );
						return 1;
					}
				}
				*/
				
			}else if(ct.Eip - 1 == rand_int3_address){
				//rand
				int rand;
				ReadMemory( (void*)rand_address, &rand, 4 );
				rand = 22695477 * rand + 1;
				rand = (rand >> 16) & 0x7FFF;
				cout << hex << rand << endl;
			}else if(ct.Eip - 1 == rand_int3_address2){
				//rand
				CONTEXT ctRand;
				ctRand.ContextFlags = CONTEXT_INTEGER;
				if( !GetThreadContext(hProcessTh, &ctRand) ){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : GetThreadContext ( rand )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
				cout << hex << ctRand.Eax << endl;
				
			}else if(ct.Eip - 1 == char_int3_address){
				*deInfo = de_char;
				
				ct.ContextFlags = CONTEXT_CONTROL;
				ct.Ebp = ct.Esp;
				if(!SetThreadContext(hProcessTh, &ct)){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : SetThreadContext ( char )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
			}else if (ct.Eip - 1 == render_scale_int3_address) {
				// Scaling factor call
				
				// All of IaMP's rendering functions are scaled through a universal scaling factor
				// that gets updated when it switches between scene and UI graphics, so we simply
				// override that and multiply them by our own value to retain proper scale at
				// any resolution.
				
				// IaMP normally sends untranslated vertices to D3D so this is by far the least
				// annoying way to do things, but breaks the fader pretty bad.
				
				ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
				if( !GetThreadContext(hProcessTh, &ct) ){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : GetThreadContext ( render_scale )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
				
				ct.Ebp = ct.Esp;
				
				float f[3];
				ReadProcessMemory(pi.hProcess, (void *)(ct.Esp + 0x8), (void *)&f, sizeof(f), 0);
				f[0] *= windowMultiplier;
				f[1] *= windowMultiplier;
				f[2] *= windowMultiplier;
				WriteProcessMemory(pi.hProcess, (void *)(ct.Esp + 0x8), (void *)&f, sizeof(f), 0);
				
				if (!SetThreadContext(hProcessTh, &ct) ) {
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : SetThreadContext ( render_scale )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
			}else if (ct.Eip - 1 == high_res_fix_int3_address) {
				// High resolutions (>1024) do not properly reset the screen offset
				// coordinates due to Direct3D whining about the fader texture not
				// being large enough for the screen. So we reset them here.
				ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
				if( !GetThreadContext(hProcessTh, &ct) ){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : GetThreadContext ( high_res_fix )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
				
				ct.Ebp = ct.Esp;
				
				float f = 0.0;
				WriteProcessMemory(pi.hProcess, (void *)0x66C084, (void *)&windowMultiplier, sizeof(windowMultiplier), 0);
				WriteProcessMemory(pi.hProcess, (void *)0x671220, (void *)&f, sizeof(f), 0);
				WriteProcessMemory(pi.hProcess, (void *)0x671224, (void *)&f, sizeof(f), 0);
				
				if (!SetThreadContext(hProcessTh, &ct) ) {
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : SetThreadContext ( high_res_fix )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
			}else if (ct.Eip - 1 == fader_scale_int3_address1 || ct.Eip - 1 == fader_scale_int3_address2) {
				// Fader - Startup, reset to original scaling factor
				
				// Fader works by taking a screenshot of the screen, storing in a texture, and then
				// rendering the texture to the screen with some multiple value. Instead of, like,
				// drawing an alpha-blended rectangle over the screen to darken it. Dumb!
				// So we have to reset to normal screen scaling factors for this.
				ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
				if( !GetThreadContext(hProcessTh, &ct) ){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : GetThreadContext ( fader_scale )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
				
				ct.Ebp = ct.Esp;
				
				float f = 1.0;
				WriteProcessMemory(pi.hProcess, (void *)0x66C084, (void *)&f, sizeof(f), 0);
				
				if (!SetThreadContext(hProcessTh, &ct) ) {
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : SetThreadContext ( fader_scale )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
			}else if (ct.Eip - 1 == fader_scale_int3_address3 || ct.Eip - 1 == fader_scale_int3_address4) {
				// Fader - Cleanup, reset to original scaling factor
				WriteProcessMemory(pi.hProcess, (void *)0x66C084, (void *)&windowMultiplier, sizeof(windowMultiplier), 0);
			}else if (ct.Eip - 1 == sprite_filter_int3_address_start || ct.Eip - 1 == sprite_filter_int3_address_end
					|| ct.Eip - 1 == sprite_filter_int3_address_end2) {
				// Disable bilinear filtering for sprites and possibly effects, then
				// turn them back on afterwards.
				ct.ContextFlags = CONTEXT_INTEGER | CONTEXT_CONTROL;
				if( !GetThreadContext(hProcessTh, &ct) ){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : GetThreadContext ( sprite_filter )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
				
				// To do this, we simply push the d3d call onto the stack and change the
				// instruction pointer. Additionally, since we're overriding a push call,
				// dump that on the stack too while we're here.
				DWORD d3dMemLoc, d3dCallTable, d3dSetTextureStageStateCall;
				
				ReadProcessMemory(pi.hProcess, (void *)0x671230, &d3dMemLoc, 4, 0);
				ReadProcessMemory(pi.hProcess, (void *)d3dMemLoc, &d3dCallTable, 4, 0);
				ReadProcessMemory(pi.hProcess, (void *)(d3dCallTable+0xFC), &d3dSetTextureStageStateCall, 4, 0);
				
				DWORD callTable[6];
				
				callTable[0] = ct.Eip;
				callTable[1] = d3dMemLoc;
				callTable[2] = 0; // stage 0
				callTable[3] = 16; // D3DTSS_MAGFILTER
				
				// off or on?
				if (ct.Eip -1 == sprite_filter_int3_address_start) {
					callTable[4] = 1; // D3DTEXF_POINT
				} else {
					callTable[4] = 2; // D3DTEXF_LINEAR
				}
				
				// address_end is a push 1 statement, both address_start and address_start0 are push 0
				if (ct.Eip - 1 == sprite_filter_int3_address_end) {
					callTable[5] = 1;
				} else {
					callTable[5] = 0;
				}
				
				ct.Esp -= 4*6;
				WriteProcessMemory(pi.hProcess, (void *)ct.Esp, callTable, 4*6, 0);
				
				ct.Eip = d3dSetTextureStageStateCall;
				
				if (!SetThreadContext(hProcessTh, &ct) ) {
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : SetThreadContext ( sprite_filter )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
				
				FlushInstructionCache(pi.hProcess, NULL, 0);
			}
			
			//起動時に常に必要
			ContinueStatus = DBG_CONTINUE;
			
			break;
			
		case EXCEPTION_SINGLE_STEP: /* シングルステップ実行例外 */
			/* 再びブレークポイントを設置する */
			
			{
				BYTE newCode = 0xCC;
				if( !WriteProcessMemory(pi.hProcess, (void*)body_int3_address, &newCode, 1, NULL) ){
					WaitForSingleObject( hPrintMutex, INFINITE );
					cout << "ERROR : Set new code ( body )" << endl;
					ReleaseMutex( hPrintMutex );
					return 1;
				}
			}
			FlushInstructionCache(pi.hProcess, NULL, 0);
			/* シングルステップモードを中止 */
			ct.ContextFlags = CONTEXT_CONTROL;
			if(!GetThreadContext(hProcessTh, &ct)){
				WaitForSingleObject( hPrintMutex, INFINITE );
				cout << "ERROR : GetThreadContext ( single step )" << endl;
				ReleaseMutex( hPrintMutex );
				return 1;
			}
			ct.EFlags &= ~EFLAGS_TF;
			if(!SetThreadContext(hProcessTh, &ct)){
				WaitForSingleObject( hPrintMutex, INFINITE );
				cout << "ERROR : SetThreadContext ( single step )" << endl;
				ReleaseMutex( hPrintMutex );
				return 1;
			}
			
			ContinueStatus = DBG_CONTINUE;
			break;
		}
		break;
	}
	
	
	return 0;
}

