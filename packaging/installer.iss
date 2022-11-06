#define Version Trim(FileRead(FileOpen("..\VERSION")))
#define Year GetDateTimeString("yyyy","","")

[Setup]
AppName=masc
OutputBaseFilename=masc-{#Version}-Windows
AppCopyright=Copyright (C) {#Year} Neux Ltd
AppPublisher=Neux Ltd
AppVersion={#Version}
DefaultDirName="{commoncf64}\VST3"
DisableStartupPrompt=yes

[Files]
Source: "{src}..\builds\masc_artefacts\Release\VST3\masc.vst3\*.*"; DestDir: "{commoncf64}\VST3\masc.vst3\";