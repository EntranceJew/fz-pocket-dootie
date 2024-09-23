$magick = "C:\Program Files\ImageMagick-7.1.1-Q16-HDRI\magick.exe"
$ase = "C:\Program Files (x86)\Steam\steamapps\common\Aseprite\Aseprite.exe"
$gfx = "C:\\git\\flipperzero-firmware\\applications_user\\fz-pocket-dootie\\gfx"
$imgs = "C:\\git\\flipperzero-firmware\\applications_user\\fz-pocket-dootie\\images"
$tmp = "C:\\git\\flipperzero-firmware\\applications_user\\fz-pocket-dootie\\temp"

New-Item -Path $tmp -ItemType Directory | Out-Null

$exports = @(
  "$gfx\dootie.aseprite",
  "$gfx\cursor.aseprite"
);

foreach ($export in $exports) {
  # export all the pictures
  $launch = @("-b",
    $export,
    "--layer", "base",
    "--save-as",
    "$tmp\\{title}_{tag}_{tagframe01}.png"
  );
  Start-Process -FilePath $ase -ArgumentList $launch -NoNewWindow -Wait
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
  "-dither", "None",
  "-type", "palette",
  "-remap", "$gfx\palette.png",
  'PNG8:%[filename:out]'
);


Get-ChildItem -Path $tmp -Recurse -Include ('*.png') | Foreach-Object {
  # export the "_s" SHAPE
  Start-Process -FilePath $magick -NoNewWindow -Wait `
    -ArgumentList (@($_.FullName, "-set", "filename:out", "$imgs\\%t_s.%e") + $magick_no_alpha + $magick_red_to_white + $magick_blue_to_white + $magick_green_to_black + $magick_force_palette)
  # export the "_f" FILL filenames, which currently handle fill and outline
  Start-Process -FilePath $magick -NoNewWindow -Wait `
    -ArgumentList (@($_.FullName, "-set", "filename:out", "$imgs\\%t_f.%e") + $magick_no_alpha + $magick_red_to_white + $magick_blue_to_black + $magick_green_to_white + $magick_force_palette)
  # export the "_o" OUTLINE filenames
  Start-Process -FilePath $magick -NoNewWindow -Wait `
    -ArgumentList (@($_.FullName, "-set", "filename:out", "$imgs\\%t_o.%e") + $magick_no_alpha + $magick_red_to_black + $magick_green_to_white + $magick_blue_to_white + $magick_force_palette)


  #remove the base file
  Remove-Item $_.FullName | Out-Null
}

# nuke the temp directory
Remove-Item -Path $tmp | Out-Null

# does this thing work
ufbt launch
