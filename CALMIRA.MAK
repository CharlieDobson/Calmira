# Microsoft Visual C++ generated build script - Do not modify

PROJ = CALMIRA
DEBUG = 0
PROGTYPE = 0
CALLER = 
ARGS = 
DLLS = 
D_RCDEFINES = /d_DEBUG
R_RCDEFINES = /dNDEBUG
ORIGIN = MSVC
ORIGIN_VER = 1.00
PROJPATH = S:\CPP\CALMIRA\CALMIRA\
USEMFC = 0
CC = cl
CPP = cl
CXX = cl
CCREATEPCHFLAG = 
CPPCREATEPCHFLAG = 
CUSEPCHFLAG = 
CPPUSEPCHFLAG = 
FIRSTC =             
FIRSTCPP = ABOUT.CPP   
RC = rc
CFLAGS_D_WEXE = /nologo /G2 /W3 /Zi /AM /Od /D "_DEBUG" /FR /GA /Fd"CALMIRA.PDB"
CFLAGS_R_WEXE = /nologo /G2 /W3 /AM /O1 /D "NDEBUG" /FR /GA 
LFLAGS_D_WEXE = /NOLOGO /NOD /PACKC:61440 /STACK:10240 /ALIGN:16 /ONERROR:NOEXE /CO  
LFLAGS_R_WEXE = /NOLOGO /NOD /PACKC:61440 /ALIGN:16 /ONERROR:NOEXE  
LIBS_D_WEXE = oldnames libw mlibcew commdlg.lib olecli.lib olesvr.lib shell.lib 
LIBS_R_WEXE = oldnames libw mlibcew commdlg.lib ddeml.lib olecli.lib olesvr.lib shell.lib 
RCFLAGS = /nologo
RESFLAGS = /nologo
RUNFLAGS = 
DEFFILE = CALMIRA.DEF
OBJS_EXT = 
LIBS_EXT = 
!if "$(DEBUG)" == "1"
CFLAGS = $(CFLAGS_D_WEXE)
LFLAGS = $(LFLAGS_D_WEXE)
LIBS = $(LIBS_D_WEXE)
MAPFILE = nul
RCDEFINES = $(D_RCDEFINES)
!else
CFLAGS = $(CFLAGS_R_WEXE)
LFLAGS = $(LFLAGS_R_WEXE)
LIBS = $(LIBS_R_WEXE)
MAPFILE = nul
RCDEFINES = $(R_RCDEFINES)
!endif
!if [if exist MSVC.BND del MSVC.BND]
!endif
SBRS = ABOUT.SBR \
		ALIAS.SBR \
		ASKDROP.SBR \
		BINPROP.SBR \
		CALMIRA.SBR \
		DDESHELL.SBR \
		COMPSYS.SBR \
		DESK.SBR \
		DESKPROP.SBR \
		DIRECTRY.SBR \
		DISKPROP.SBR \
		DYNAMENU.SBR \
		FILEFIND.SBR \
		FILEMAN.SBR \
		FILEPROP.SBR \
		FILTER.SBR \
		FOURDOS.SBR \
		FSYSPROP.SBR \
		GOTOFLD.SBR \
		HELPERS.SBR \
		HOOKS.SBR \
		ICONIC.SBR \
		ICONWIN.SBR \
		INCSRCH.SBR \
		LOGON.SBR \
		MENUEDIT.SBR \
		OPENFILE.SBR \
		PASSWORD.SBR \
		PROFILE.SBR \
		PROGCONV.SBR \
		PROGRESS.SBR \
		REFEDIT.SBR \
		REFERENC.SBR \
		REPLACE.SBR \
		RUNPROG.SBR \
		SELECT.SBR \
		SETTINGS.SBR \
		SHORTS.SBR \
		SHUTDWN.SBR \
		SPLASH.SBR \
		START.SBR \
		STRTPROP.SBR \
		SYSPROP.SBR \
		TASK.SBR \
		TASKMAN.SBR \
		TASKPROP.SBR \
		TREE.SBR \
		WASTEBN.SBR


ABOUT_DEP = s:\cpp\calmira\calmira\about.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\fileman.h


ALIAS_DEP = s:\cpp\calmira\calmira\alias.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\fileman.h


ASKDROP_DEP = s:\cpp\calmira\calmira\askdrop.h


BINPROP_DEP = s:\cpp\calmira\calmira\binprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


CALMIRA_DEP = s:\cpp\calmira\calmsgs.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\desk.h \
	s:\cpp\calmira\calmira\task.h \
	s:\cpp\calmira\calmira\start.h \
	s:\cpp\calmira\calmira\compsys.h \
	s:\cpp\calmira\calmira\wastebn.h \
	s:\cpp\calmira\calmira\splash.h \
	s:\cpp\calmira\calmira\about.h \
	s:\cpp\calmira\calmira\hooks.h \
	s:\cpp\calmira\wndhooks\wndhooks.h \
	s:\cpp\calmira\calmira\helpers.h


DDESHELL_DEP = s:\cpp\calmira\calmira\ddeshell.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\helpers.h


COMPSYS_DEP = s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\compsys.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\tree.h \
	s:\cpp\calmira\calmira\iconwin.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\filefind.h \
	s:\cpp\calmira\calmsgs.h


DESK_DEP = s:\cpp\calmira\calmira\desk.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\shorts.h \
	s:\cpp\calmira\calmira\compsys.h \
	s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\hooks.h \
	s:\cpp\calmira\wndhooks\wndhooks.h \
	s:\cpp\calmira\calmira\task.h \
	s:\cpp\calmira\calmsgs.h \
	s:\cpp\calmira\calmira\shutdwn.h \
	s:\cpp\calmira\calmira\runprog.h \
	s:\cpp\calmira\calmira\about.h


DESKPROP_DEP = s:\cpp\calmira\calmira\deskprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


DIRECTRY_DEP = s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\fourdos.h


DISKPROP_DEP = s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\diskprop.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


DYNAMENU_DEP = s:\cpp\calmira\calmira\dynamenu.h


FILEFIND_DEP = s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\filefind.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h


FILEMAN_DEP = s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\helpers.h


FILEPROP_DEP = s:\cpp\calmira\calmira\fileprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\fourdos.h \
	s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\helpers.h


FILTER_DEP = s:\cpp\calmira\calmira\filter.h


FOURDOS_DEP = s:\cpp\calmira\calmira\fourdos.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


FSYSPROP_DEP = s:\cpp\calmira\calmira\fsysprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


GOTOFLD_DEP = s:\cpp\calmira\calmira\gotofld.h


HELPERS_DEP = s:\cpp\calmira\calmira\helpers.h


HOOKS_DEP = s:\cpp\calmira\calmira\hooks.h \
	s:\cpp\calmira\wndhooks\wndhooks.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


ICONIC_DEP = s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\helpers.h


ICONWIN_DEP = s:\cpp\calmira\calmira\iconwin.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmsgs.h


INCSRCH_DEP = s:\cpp\calmira\calmira\incsrch.h


LOGON_DEP = s:\cpp\calmira\calmira\logon.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


MENUEDIT_DEP = s:\cpp\calmira\calmira\menuedit.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


OPENFILE_DEP = s:\cpp\calmira\calmira\openfile.h


PASSWORD_DEP = s:\cpp\calmira\calmira\password.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


PROFILE_DEP = s:\cpp\calmira\calmira\profile.h


PROGCONV_DEP = s:\cpp\calmira\calmira\progconv.h


PROGRESS_DEP = s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\progress.h


REFEDIT_DEP = s:\cpp\calmira\calmira\refedit.h \
	s:\cpp\calmira\calmira\start.h


REFERENC_DEP = s:\cpp\calmira\calmira\referenc.h \
	s:\cpp\calmira\calmira\desk.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


REPLACE_DEP = s:\cpp\calmira\calmira\replace.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


RUNPROG_DEP = s:\cpp\calmira\calmira\runprog.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\fileman.h


SELECT_DEP = s:\cpp\calmira\calmira\select.h


SETTINGS_DEP = s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmsgs.h \
	s:\cpp\calmira\calmira\helpers.h


SHORTS_DEP = s:\cpp\calmira\calmira\shorts.h \
	s:\cpp\calmira\calmira\desk.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\alias.h


SHUTDWN_DEP = s:\cpp\calmira\calmira\shutdwn.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


SPLASH_DEP = s:\cpp\calmira\calmira\splash.h


START_DEP = s:\cpp\calmira\calmira\start.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\runprog.h \
	s:\cpp\calmira\calmira\shutdwn.h \
	s:\cpp\calmira\calmira\filefind.h \
	s:\cpp\calmira\calmira\strtprop.h \
	s:\cpp\calmira\calmira\taskprop.h \
	s:\cpp\calmira\calmira\deskprop.h \
	s:\cpp\calmira\calmira\fsysprop.h \
	s:\cpp\calmira\calmira\binprop.h \
	s:\cpp\calmira\calmira\sysprop.h \
	s:\cpp\calmira\calmira\fileman.h \
	s:\cpp\calmira\calmira\helpers.h


STRTPROP_DEP = s:\cpp\calmira\calmira\strtprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


SYSPROP_DEP = s:\cpp\calmira\calmira\sysprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


TASK_DEP = s:\cpp\calmira\calmira\task.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmsgs.h \
	s:\cpp\calmira\calmira\hooks.h \
	s:\cpp\calmira\wndhooks\wndhooks.h \
	s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\start.h \
	s:\cpp\calmira\calmira\shutdwn.h \
	s:\cpp\calmira\calmira\taskman.h \
	s:\cpp\calmira\calmira\strtprop.h


TASKMAN_DEP = s:\cpp\calmira\calmira\taskman.h


TASKPROP_DEP = s:\cpp\calmira\calmira\taskprop.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h


TREE_DEP = s:\cpp\calmira\calmira\tree.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\helpers.h


WASTEBN_DEP = s:\cpp\calmira\calmira\wastebn.h \
	s:\cpp\calmira\calmira\settings.h \
	s:\cpp\calmira\calmira\profile.h \
	s:\cpp\calmira\calmira\helpers.h \
	s:\cpp\calmira\calmira\directry.h \
	s:\cpp\calmira\calmira\iconic.h \
	s:\cpp\calmira\calmira\fileman.h


CALMIRA_RCDEP = s:\cpp\calmira\calmira\res\explorer.ico \
	s:\cpp\calmira\calmira\res\computer.ico \
	s:\cpp\calmira\calmira\res\binicon.ico \
	s:\cpp\calmira\calmira\res\fullbini.ico \
	s:\cpp\calmira\calmira\res\folderic.ico \
	s:\cpp\calmira\calmira\res\busyptr.cur \
	s:\cpp\calmira\calmira\res\drgfile.cur \
	s:\cpp\calmira\calmira\res\drgmulfi.cur \
	s:\cpp\calmira\calmira\res\dropfile.cur \
	s:\cpp\calmira\calmira\res\dropfold.cur \
	s:\cpp\calmira\calmira\res\drpalias.cur \
	s:\cpp\calmira\calmira\res\drpfilco.cur \
	s:\cpp\calmira\calmira\res\drpfldco.cur \
	s:\cpp\calmira\calmira\res\drpmlfco.cur \
	s:\cpp\calmira\calmira\res\drpmulfi.cur \
	s:\cpp\calmira\calmira\res\finger.cur \
	s:\cpp\calmira\calmira\res\no.cur \
	s:\cpp\calmira\calmira\res\nodrgfil.cur \
	s:\cpp\calmira\calmira\res\nodrgfol.cur \
	s:\cpp\calmira\calmira\res\nodrgmul.cur \
	s:\cpp\calmira\calmira\res\resizeh.cur \
	s:\cpp\calmira\calmira\res\resizev.cur \
	s:\cpp\calmira\calmira\res\sqlwait.cur \
	s:\cpp\calmira\calmira\res\terminat.cur


all:	$(PROJ).EXE $(PROJ).BSC

ABOUT.OBJ:	ABOUT.CPP $(ABOUT_DEP)
	$(CPP) $(CFLAGS) $(CPPCREATEPCHFLAG) /c ABOUT.CPP

ALIAS.OBJ:	ALIAS.CPP $(ALIAS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ALIAS.CPP

ASKDROP.OBJ:	ASKDROP.CPP $(ASKDROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ASKDROP.CPP

BINPROP.OBJ:	BINPROP.CPP $(BINPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c BINPROP.CPP

CALMIRA.OBJ:	CALMIRA.CPP $(CALMIRA_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c CALMIRA.CPP

DDESHELL.OBJ:	DDESHELL.CPP $(DDESHELL_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DDESHELL.CPP

COMPSYS.OBJ:	COMPSYS.CPP $(COMPSYS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c COMPSYS.CPP

DESK.OBJ:	DESK.CPP $(DESK_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DESK.CPP

DESKPROP.OBJ:	DESKPROP.CPP $(DESKPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DESKPROP.CPP

DIRECTRY.OBJ:	DIRECTRY.CPP $(DIRECTRY_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DIRECTRY.CPP

DISKPROP.OBJ:	DISKPROP.CPP $(DISKPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DISKPROP.CPP

DYNAMENU.OBJ:	DYNAMENU.CPP $(DYNAMENU_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c DYNAMENU.CPP

FILEFIND.OBJ:	FILEFIND.CPP $(FILEFIND_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FILEFIND.CPP

FILEMAN.OBJ:	FILEMAN.CPP $(FILEMAN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FILEMAN.CPP

FILEPROP.OBJ:	FILEPROP.CPP $(FILEPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FILEPROP.CPP

FILTER.OBJ:	FILTER.CPP $(FILTER_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FILTER.CPP

FOURDOS.OBJ:	FOURDOS.CPP $(FOURDOS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FOURDOS.CPP

FSYSPROP.OBJ:	FSYSPROP.CPP $(FSYSPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c FSYSPROP.CPP

GOTOFLD.OBJ:	GOTOFLD.CPP $(GOTOFLD_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c GOTOFLD.CPP

HELPERS.OBJ:	HELPERS.CPP $(HELPERS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c HELPERS.CPP

HOOKS.OBJ:	HOOKS.CPP $(HOOKS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c HOOKS.CPP

ICONIC.OBJ:	ICONIC.CPP $(ICONIC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ICONIC.CPP

ICONWIN.OBJ:	ICONWIN.CPP $(ICONWIN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c ICONWIN.CPP

INCSRCH.OBJ:	INCSRCH.CPP $(INCSRCH_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c INCSRCH.CPP

LOGON.OBJ:	LOGON.CPP $(LOGON_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c LOGON.CPP

MENUEDIT.OBJ:	MENUEDIT.CPP $(MENUEDIT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c MENUEDIT.CPP

OPENFILE.OBJ:	OPENFILE.CPP $(OPENFILE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c OPENFILE.CPP

PASSWORD.OBJ:	PASSWORD.CPP $(PASSWORD_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PASSWORD.CPP

PROFILE.OBJ:	PROFILE.CPP $(PROFILE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PROFILE.CPP

PROGCONV.OBJ:	PROGCONV.CPP $(PROGCONV_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PROGCONV.CPP

PROGRESS.OBJ:	PROGRESS.CPP $(PROGRESS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c PROGRESS.CPP

REFEDIT.OBJ:	REFEDIT.CPP $(REFEDIT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c REFEDIT.CPP

REFERENC.OBJ:	REFERENC.CPP $(REFERENC_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c REFERENC.CPP

REPLACE.OBJ:	REPLACE.CPP $(REPLACE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c REPLACE.CPP

RUNPROG.OBJ:	RUNPROG.CPP $(RUNPROG_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c RUNPROG.CPP

SELECT.OBJ:	SELECT.CPP $(SELECT_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SELECT.CPP

SETTINGS.OBJ:	SETTINGS.CPP $(SETTINGS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SETTINGS.CPP

SHORTS.OBJ:	SHORTS.CPP $(SHORTS_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SHORTS.CPP

SHUTDWN.OBJ:	SHUTDWN.CPP $(SHUTDWN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SHUTDWN.CPP

SPLASH.OBJ:	SPLASH.CPP $(SPLASH_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SPLASH.CPP

START.OBJ:	START.CPP $(START_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c START.CPP

STRTPROP.OBJ:	STRTPROP.CPP $(STRTPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c STRTPROP.CPP

SYSPROP.OBJ:	SYSPROP.CPP $(SYSPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c SYSPROP.CPP

TASK.OBJ:	TASK.CPP $(TASK_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TASK.CPP

TASKMAN.OBJ:	TASKMAN.CPP $(TASKMAN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TASKMAN.CPP

TASKPROP.OBJ:	TASKPROP.CPP $(TASKPROP_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TASKPROP.CPP

TREE.OBJ:	TREE.CPP $(TREE_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c TREE.CPP

WASTEBN.OBJ:	WASTEBN.CPP $(WASTEBN_DEP)
	$(CPP) $(CFLAGS) $(CPPUSEPCHFLAG) /c WASTEBN.CPP

CALMIRA.RES:	CALMIRA.RC $(CALMIRA_RCDEP)
	$(RC) $(RCFLAGS) $(RCDEFINES) -r CALMIRA.RC


$(PROJ).EXE::	CALMIRA.RES

$(PROJ).EXE::	ABOUT.OBJ ALIAS.OBJ ASKDROP.OBJ BINPROP.OBJ CALMIRA.OBJ DDESHELL.OBJ \
	COMPSYS.OBJ DESK.OBJ DESKPROP.OBJ DIRECTRY.OBJ DISKPROP.OBJ DYNAMENU.OBJ FILEFIND.OBJ \
	FILEMAN.OBJ FILEPROP.OBJ FILTER.OBJ FOURDOS.OBJ FSYSPROP.OBJ GOTOFLD.OBJ HELPERS.OBJ \
	HOOKS.OBJ ICONIC.OBJ ICONWIN.OBJ INCSRCH.OBJ LOGON.OBJ MENUEDIT.OBJ OPENFILE.OBJ \
	PASSWORD.OBJ PROFILE.OBJ PROGCONV.OBJ PROGRESS.OBJ REFEDIT.OBJ REFERENC.OBJ REPLACE.OBJ \
	RUNPROG.OBJ SELECT.OBJ SETTINGS.OBJ SHORTS.OBJ SHUTDWN.OBJ SPLASH.OBJ START.OBJ STRTPROP.OBJ \
	SYSPROP.OBJ TASK.OBJ TASKMAN.OBJ TASKPROP.OBJ TREE.OBJ WASTEBN.OBJ $(OBJS_EXT) $(DEFFILE)
	echo >NUL @<<$(PROJ).CRF
ABOUT.OBJ +
ALIAS.OBJ +
ASKDROP.OBJ +
BINPROP.OBJ +
CALMIRA.OBJ +
DDESHELL.OBJ +
COMPSYS.OBJ +
DESK.OBJ +
DESKPROP.OBJ +
DIRECTRY.OBJ +
DISKPROP.OBJ +
DYNAMENU.OBJ +
FILEFIND.OBJ +
FILEMAN.OBJ +
FILEPROP.OBJ +
FILTER.OBJ +
FOURDOS.OBJ +
FSYSPROP.OBJ +
GOTOFLD.OBJ +
HELPERS.OBJ +
HOOKS.OBJ +
ICONIC.OBJ +
ICONWIN.OBJ +
INCSRCH.OBJ +
LOGON.OBJ +
MENUEDIT.OBJ +
OPENFILE.OBJ +
PASSWORD.OBJ +
PROFILE.OBJ +
PROGCONV.OBJ +
PROGRESS.OBJ +
REFEDIT.OBJ +
REFERENC.OBJ +
REPLACE.OBJ +
RUNPROG.OBJ +
SELECT.OBJ +
SETTINGS.OBJ +
SHORTS.OBJ +
SHUTDWN.OBJ +
SPLASH.OBJ +
START.OBJ +
STRTPROP.OBJ +
SYSPROP.OBJ +
TASK.OBJ +
TASKMAN.OBJ +
TASKPROP.OBJ +
TREE.OBJ +
WASTEBN.OBJ +
$(OBJS_EXT)
$(PROJ).EXE
$(MAPFILE)
c:\msvc\lib\+
$(LIBS)
$(DEFFILE);
<<
	link $(LFLAGS) @$(PROJ).CRF
	$(RC) $(RESFLAGS) CALMIRA.RES $@
	@copy $(PROJ).CRF MSVC.BND

$(PROJ).EXE::	CALMIRA.RES
	if not exist MSVC.BND 	$(RC) $(RESFLAGS) CALMIRA.RES $@

run: $(PROJ).EXE
	$(PROJ) $(RUNFLAGS)


$(PROJ).BSC: $(SBRS)
	bscmake @<<
/o$@ $(SBRS)
<<
