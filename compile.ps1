$build_path = 'build'
if (Test-Path -Path $build_path) {
    Remove-Item -Path $build_path -Recurse }
New-Item -Path $build_path -ItemType Directory
cmake -S . -B $build_path
cmake --build $build_path