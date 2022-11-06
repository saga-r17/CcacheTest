#define Version Trim(FileRead(FileOpen("..\VERSION")))
#define Year GetDateTimeString("yyyy","","")

[Setup]
AppName=masc
OutputBaseFilename=masc-{#Version}-Windows
AppCopyright=Copyright (C) {#Year} Neux Ltd
AppPublisher=Neux Ltd
AppVersion={#Version}
DefaultDirName="{commoncf64}\VST3"

[Files]
Source: "{src}..\packaging\masc.vst3"; DestDir: "{app}";