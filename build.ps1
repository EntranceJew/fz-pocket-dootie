param (
    [Switch]$Art,
    [Switch]$Launch,
    [Switch]$View,
    [Switch]$Log
)

$magick = "C:\Program Files\ImageMagick-7.1.1-Q16-HDRI\magick.exe"
$ase = "C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"
$gfx = "C:\\git\\flipperzero-firmware\\applications_user\\fz-pocket-dootie\\gfx"
$imgs = "C:\\git\\flipperzero-firmware\\applications_user\\fz-pocket-dootie\\images"
$tmp = "C:\\git\\flipperzero-firmware\\applications_user\\fz-pocket-dootie\\temp"
$qflipper = "C:\Program Files\qFlipper\qFlipper.exe"

function Kill-Tree {
    param(
        [int]$ppid
    )
    Get-CimInstance Win32_Process | Where-Object { $_.ParentProcessId -eq $ppid } | ForEach-Object { Kill-Tree $_.ProcessId }
    Stop-Process -Id $ppid -Force
}


if ($Art) {
    New-Item -Path $tmp -ItemType Directory | Out-Null

    Get-ChildItem -Path "$gfx\\*" -Include ('*.aseprite') | Foreach-Object {
        Write-Host "exporting: $_"

        # export all the pictures
        $ase_args = @("-b",
            $_.FullName,
            "--layer", "base",
            "--save-as",
            "$tmp\\{title}_{tag}_{tagframe01}.png"
        );
        Start-Process -FilePath $ase -ArgumentList $ase_args -NoNewWindow -Wait
    }

    # prepare to mess with them
    $magick_no_alpha = @("-background", "#ffffff", "-alpha", "remove", "-alpha", "off", "+repage");
    $magick_red_to_white = @("-fill", "#ffffff", "-opaque", "#ff0000", "+repage");
    $magick_green_to_white = @("-fill", "#ffffff", "-opaque", "#00ff00", "+repage");
    $magick_blue_to_white = @("-fill", "#ffffff", "-opaque", "#0000ff", "+repage");
    $magick_red_to_black = @("-fill", "#000000", "-opaque", "#ff0000", "+repage");
    $magick_green_to_black = @("-fill", "#000000", "-opaque", "#00ff00", "+repage");
    $magick_blue_to_black = @("-fill", "#000000", "-opaque", "#0000ff", "+repage");

    $magick_force_palette = @(
        "-define", "preserve-timestamp=true",
        "-dither", "None",
        "-type", "palette",
        "-remap", "$gfx\palette.png",
        'PNG8:%[filename:out]'
    );


    Get-ChildItem -Path "$tmp\\*" -Recurse -Include ('*.png') | Foreach-Object {
        # export the "_s" SHAPE
        Start-Process -FilePath $magick -NoNewWindow -Wait `
        -ArgumentList (@($_.FullName, "-set", "filename:out", "$imgs\\%t_shape.%e") + $magick_no_alpha + $magick_red_to_white + $magick_blue_to_white + $magick_green_to_black + $magick_force_palette)
        # export the "_f" FILL filenames, which currently handle fill and outline
        Start-Process -FilePath $magick -NoNewWindow -Wait `
        -ArgumentList (@($_.FullName, "-set", "filename:out", "$imgs\\%t_fill.%e") + $magick_no_alpha + $magick_red_to_white + $magick_blue_to_black + $magick_green_to_white + $magick_force_palette)
        # export the "_o" OUTLINE filenames
        Start-Process -FilePath $magick -NoNewWindow -Wait `
        -ArgumentList (@($_.FullName, "-set", "filename:out", "$imgs\\%t_outline.%e") + $magick_no_alpha + $magick_red_to_black + $magick_green_to_white + $magick_blue_to_white + $magick_force_palette)

        Write-Host "processed: $_"

        #remove the base file
        Remove-Item $_.FullName | Out-Null
    }

    Get-ChildItem -Path "$imgs\\*" -Recurse -Include ('*.png') | Foreach-Object {
        # remove all the funny file metadata, please
        # (this will be illegal in the future)
        Start-Process -FilePath $magick -NoNewWindow -Wait `
            -ArgumentList ("mogrify", "-strip", "-define", "preserve-timestamp=true", $_.FullName)

        Write-Host "stripped: $_"
    }

    # nuke the temp directory
    Remove-Item -Path $tmp | Out-Null
    Write-Host "Art: done ✔"
}

$NoLog = $False

if ($Launch) {
    $proc = Get-CimInstance Win32_Process | Where-Object { $_.Name -eq "qFlipper.exe" }
    if ($proc) {
        Kill-Tree -ppid $proc.ProcessId
    }
    Start-Sleep -Seconds 5

    ufbt APPID=pocket_dootie VERBOSE=true format
    $NoLog = $NoLog -or $LastExitCode -gt 0
    ufbt APPID=pocket_dootie VERBOSE=true launch
    $NoLog = $NoLog -or $LastExitCode -gt 0
}

if ($View) {
    & $qflipper
}

if ($NoLog) {
    Write-Host "Launch: error ❌"
}
if ($Log -and -not $NoLog) {
    # mode COM5 BAUD=230400 PARITY=n DATA=8
#    "log trace" | plink -serial \\.\COM5 -sercfg "230400,8,n,1,n"
    "log debug" | plink -serial \\.\COM5 -sercfg "230400,8,n,1,n"
#    "log info " | plink -serial \\.\COM5 -sercfg "230400,8,n,1,n"
#    "log warn" | plink -serial \\.\COM5 -sercfg "230400,8,n,1,n"
#    "log error" | plink -serial \\.\COM5 -sercfg "230400,8,n,1,n"
}
