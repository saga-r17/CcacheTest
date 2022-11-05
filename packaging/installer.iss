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
Source: "{src}..\Builds\masc_artefacts\Release\VST3\masc.vst3\Contents\x86_64-win\masc.vst3"; DestDir: "{commoncf64}\VST3\masc.vst3\"; Check: Is64BitInstallMode; Flags: external overwritereadonly ignoreversion; Attribs: hidden system;
