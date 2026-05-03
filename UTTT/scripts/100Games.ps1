# Dataset Generator - Ultimate Tic Tac Toe
# Lauches game engine for 100 batches
# Each batch is saved to dataset.jsonl

Set-Location ..
Set-Location .\bin\debug

1..100 | ForEach-Object {
    Write-Host "`n===================="
    Write-Host "RUN $_ / 100"
    Write-Host "===================="

    Start-Process -Wait -FilePath .\UTTT_Template.exe
}