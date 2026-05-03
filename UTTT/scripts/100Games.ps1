Set-Location "$PSScriptRoot\..\bin\debug"

1..100 | ForEach-Object {
    Write-Host "RUN $_ / 100"

    $logFile = "run.log"
    $errFile = "run.err"

    $process = Start-Process `
        -FilePath ".\UTTT_Template.exe" `
        -RedirectStandardOutput $logFile `
        -RedirectStandardError $errFile `
        -PassThru

    while (-not $process.HasExited) {
        Start-Sleep -Milliseconds 300
        $process.Refresh()

        if (Test-Path $errFile) {
            $errContent = Get-Content $errFile -Raw

            if ($errContent -match "MAIN FINISHED") {
                Write-Host "MAIN FINISHED detected -> killing process $($process.Id)"
                taskkill /PID $process.Id /T /F | Out-Null
                break
            }
        }
    }
}