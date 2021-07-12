$ArchiveName = "gorillafriends_v0.1.0.qmod"
$TempArchiveName = "gorillafriends_v0.1.0.qmod.zip"

Compress-Archive -Path "./libs/arm64-v8a/libgorillafriends.so", "./libs/arm64-v8a/libbeatsaber-hook_1_3_5.so", "./mod.json" -DestinationPath $TempArchiveName -Force
Move-Item $TempArchiveName $ArchiveName -Force