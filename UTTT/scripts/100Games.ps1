# Dataset Generator

Set-Location "$PSScriptRoot\..\bin\debug"

1..100 | ForEach-Object {
    Write-Host "`nRUN $_ / 100"

    $process = Start-Process .\UTTT_Template.exe `
        -NoNewWindow `
        -RedirectStandardOutput "output.log" `
        -PassThru

    while ($true) {
        Start-Sleep -Milliseconds 500

        if (Test-Path "output.log") {
            $content = Get-Content "output.log" -Raw

            if ($content -match "MAIN FINISHED") {
                Write-Host "Game batch finished -> killing process"

                Stop-Process -Id $process.Id -Force -ErrorAction SilentlyContinue
                Remove-Item "output.log" -ErrorAction SilentlyContinue
                break
            }
        }

        if ($process.HasExited) {
            break
        }
    }
}