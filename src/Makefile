CPP = bcc32
CPPFLAGS = -WM -O2

TARGET	= th075Caster
OBJ1	= th075Caster.obj
OBJ2	= charDat.obj charDat_di.obj inputData.obj inputDataSub.obj
OBJ3	= mainDat.obj mainDat_func.obj mainDat_mainRoop.obj mainDat_th075.obj mainDat_thread.obj
OBJ4	= mainDat_main.obj mainDat_manipMenu.obj mainDat_main_sub.obj mainDat_mainStep.obj syncData.obj syncDataSub.obj
OBJ5	= booster/class.obj booster/init.obj booster/develop.obj booster/file.obj booster/roop.obj
OBJ6	= booster/HL.obj booster/ai_LNAI.obj booster/ai_SNAI.obj booster/ai_ManageAI.obj
OBJ7	= booster/ai_SpellAI.obj booster/ai_SWAI.obj booster/ai_RecoverAI.obj booster/ai_BackAI.obj  booster/script.obj
LIB	= dinput.lib cw32mt.lib winmm.lib zlib.lib shlwapi.lib


all: $(TARGET)
$(TARGET) : $(OBJ1)
$(TARGET) : $(OBJ2)
$(TARGET) : $(OBJ3)
$(TARGET) : $(OBJ4)
	cd booster
	make
	cd ..
	$(CPP) $(OBJ1) $(OBJ2) $(OBJ3) $(OBJ4) $(OBJ5) $(OBJ6) $(OBJ7) $(LIB)

.cpp.obj:
	$(CPP) $(CPPFLAGS) -c $<

# cleanup
clean:
	-del *.obj
	-del *.exe
	-del *.tds
	
	cd booster
	-del *.obj

th075Caster.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
charDat.obj: charDatClass.h etc.h const.h
charDat_di.obj: charDatClass.h etc.h const.h
inputData.obj: etc.h const.h inputDataClass.h syncDataClass.h
inputDataSub.obj: etc.h const.h inputDataClass.h syncDataClass.h
mainDat.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_func.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_mainRoop.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_th075.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_thread.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h booster/boosterDatClass.h syncDataClass.h
mainDat_main.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_manipMenu.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_main_sub.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h
mainDat_mainStep.obj: mainDatClass.h charDatClass.h etc.h const.h inputDataClass.h syncDataClass.h

