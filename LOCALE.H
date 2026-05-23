/***************************************************************************
 *
 *  LOCALE.H - Calmira localized string resource IDs
 *  Translated from LOCALE.PAS (Delphi/Object Pascal)
 *
 *  All user-visible strings live in LOCALE.RC / LOCALE.RES as a
 *  STRINGTABLE.  Use LoadString(hInst, Sxxx, buf, sizeof(buf)) to
 *  retrieve them at runtime.
 *
 ***************************************************************************/

#ifndef LOCALE_H
#define LOCALE_H

/* General / miscellaneous */
#define SCannotFindLicense           1024
#define SCannotRunNotepad            1025
#define SWelcome                     1026
#define SCannotMoveToBin             1027
#define SCannotRestoreItem           1028
#define SCannotRestoreFolderOverFile 1029
#define SQueryReplaceFile            1030
#define SDriveRefPrompt              1031
#define SFolderRefPrompt             1032
#define SFileRefPrompt               1033
#define SInternetRefPrompt           1034

/* Desktop context menu items */
#define SMenuCascadeBrowsers  1040
#define SMenuArrangeIcons     1041
#define SMenuLineUpIcons      1042
#define SMenuCloseBrowsers    1043
#define SMenuClearDesktop     1044
#define SMenuProperties       1045
#define SMenuAbout            1046
#define SSelectFolder         1047
#define SAliasProperties      1048
#define SShortcutProperties   1049
#define SMenuShortcutProperties 1050
#define SMenuShortcutOpen     1051
#define SMenuShortcutRemove   1052
#define SMenuShortcutMove     1053
#define SMenuBinEmpty         1054

/* Dialog messages */
#define SCloseUnnamedDialog   1056
#define SCloseSpecificDialog  1057
#define SNotifyEndWindows     1058
#define SQueryQuit            1059
#define SOpenFolder           1060
#define SFolderName           1061
#define SUnknownCommand       1062
#define SCannotOpenFolder     1063
#define SQueryReplaceAlias    1064
#define SCannotOpenTarget     1065
#define SQueryDeleteShortcut  1066

/* File system errors */
#define SFileAlreadyExists       1072
#define SFolderAlreadyExists     1073
#define SCannotChangeAttr        1074
#define SInvalidFilename         1075
#define SCannotRename            1076
#define SChangeDescription       1077
#define SDescribe                1078
#define SCannotOpenFileWith      1079
#define SCannotPutFolderOnSelf   1080
#define SCannotPutFolderInSelf   1081
#define SCannotPutFolderOverFile 1082

/* File operation confirmations */
#define SQueryAffectProtected  1088
#define SCannotCopyFileToSelf  1089
#define SCannotMoveFileToSelf  1090
#define SCannotCreateFolder    1091
#define SCannotReplaceFile     1092
#define SCannotMoveFile        1093
#define SCannotCyclicCopy      1094
#define SCannotCyclicMove      1095
#define SCannotDeleteFile      1096
#define SNoFilesFound          1097
#define SCannotRunDefViewer    1098
#define SFileNotAssociated     1099
#define SCannotRunOrView       1100
#define SConfirmNewFolder      1101

/* Network errors */
#define SNetSuccess            1104
#define SNetNotSupported       1105
#define SOutOfMemory           1106
#define SNetError              1107
#define SBadPointer            1108
#define SBadNetworkResource    1109
#define SBadLocalName          1110
#define SInvalidPassword       1111
#define SAccessDenied          1112
#define SFilesWereOpen         1113
#define SAlreadyConnected      1114
#define SNetOperationFailed    1115
#define SQueryCloseOpenFiles   1116

/* File operation queries */
#define SQueryBinItems         1119
#define SQueryCopyItems        1120
#define SQueryMoveItems        1121
#define SQueryDeleteItems      1122
#define SCannotPutToSelf       1123
#define SCreateFolder          1124
#define SNewFolderName         1125
#define SRename                1126
#define SNewFilename           1127
#define SCreateAlias           1128
#define SAliasFilename         1129
#define SPrintFile             1130
#define SFilename              1131
#define SDuplicateFile         1132
#define SDirectoryOf           1133

/* Progress operation labels */
#define SCopy                  1136
#define SMove                  1137
#define SDelete                1138
#define SCopyFile              1139
#define SMoveFile              1140
#define SCopyFolder            1141
#define SMoveFolder            1142
#define SDeleteFile            1143
#define SDeleteFolder          1144
#define SProgressCopy          1145
#define SProgressMove          1146
#define SProgressDelete        1147
#define SProgressBinning       1148
#define SProgressRestoring     1149
#define SProgressEmptying      1150

/* Start menu editor */
#define SAddMenuItem           1152
#define SInsertMenuItem        1153
#define SMenuItemProperties    1154
#define SQueryDeleteMenu       1155
#define SCannotFindProgman     1156
#define SCaptionNeedsText      1157
#define SQuerySaveAdvanced     1158
#define SAddAdvanced           1159
#define SIdentValue            1160
#define SModifyAdvanced        1161
#define SAddExclusion          1162
#define SModifyExclusion       1163
#define SModuleClass           1164
#define SAddApplet             1165
#define SModifyApplet          1166
#define SAppletFormat          1167

/* Properties dialog labels */
#define SUnknown               1168
#define SFolderContents        1169
#define SSelectionContents     1170
#define STotalContents         1171
#define SVersion               1172
#define SAssociation           1173
#define SAssociateTypeWith     1174
#define SNoRegCommand          1175
#define SSpecifyFiles          1176
#define SStopSearch            1177
#define SStartSearch           1178
#define SNoMatchingFiles       1179
#define SFindListboxFull       1180

/* Version info field names */
#define SProductName           1184
#define SLegalCopyright        1185
#define SDescription           1186
#define SType                  1187
#define SSubType               1188
#define SFileOS                1189
#define SComments              1190
#define SProductVersion        1191
#define SFileVersion           1192
#define SCompany               1193
#define SLegalTrademarks       1194
#define SInternalName          1195
#define SPrivateBuild          1196
#define SSpecialBuild          1197
#define SOriginalFilename      1198
#define SWOWVersion            1199

/* Multi-user / logon */
#define SQueryTerminate        1200
#define SCreateNewUser         1201
#define SUserName              1202
#define SEnterPassword         1203
#define SConfirmPassword       1204
#define SChangePassword        1205
#define SEnterOldPassword      1206
#define SEnterNewPassword      1207
#define SConfirmNewPassword    1208
#define SInvalidUserName       1209
#define SPasswordMismatch      1210
#define SEditEntryCaption      1211
#define SInvalidOldPassword    1212

/* Icon window object context menu */
#define SMObjOpen              1216
#define SMObjOpenWith          1217
#define SMObjInspect           1218
#define SMObjCut               1219
#define SMObjCopy              1220
#define SMObjPaste             1221
#define SMObjDelete            1222
#define SMObjRename            1223
#define SMObjDescribe          1224
#define SMObjEditAlias         1225
#define SMObjDuplicate         1226
#define SMObjProperties        1227

/* Icon window View menu */
#define SMWinLargeIcons        1232
#define SMWinSmallIcons        1233
#define SMWinDetails           1234
#define SMWinSortByName        1235
#define SMWinSortByType        1236
#define SMWinSortBySize        1237
#define SMWinSortByDate        1238
#define SMWinFileSystem        1239
#define SMWinSetFilter         1240
#define SMWinCreateFolder      1241
#define SMWinCreateAlias       1242

/* WinExec error strings */
#define SCorruptEXE            1250
#define SFileNotFound          1251
#define SSharingError          1252
#define SDLLFailed             1253
#define SAppOutOfMemory        1254
#define SIncorrectWinVer       1255
#define SInvalidEXE            1256
#define SNonWindowsApp         1257
#define SRealModeApp           1258
#define SSecondInstance        1259
#define SCompressedEXE         1260
#define SCorruptDLL            1261
#define SRequiresWin32         1262

/* Toolbar tooltips */
#define STHintParent           1270
#define STHintNewFolder        1271
#define STHintConnect          1272
#define STHintDisConnect       1273
#define STHintCut              1274
#define STHintCopy             1275
#define STHintPaste            1276
#define STHintDelete           1277
#define STHintRename           1278
#define STHintProperties       1279
#define STHintLargeIcons       1280
#define STHintSmallIcons       1281
#define STHintDetails          1282
#define STHintSortName         1283
#define STHintSortType         1284
#define STHintSortSize         1285
#define STHintSortDate         1286

/* Details column headers */
#define SCFilename             1290
#define SCSize                 1291
#define SCType                 1292
#define SCModified             1293
#define SCAttributes           1294
#define SCDescription          1295
#define SCFile                 1296
#define SCFolder               1297
#define SCAlias                1298

#endif /* LOCALE_H */
