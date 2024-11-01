$buildDir = "build"
if (-Not (Test-Path $buildDir)) {
    New-Item -ItemType Directory -Path $buildDir
}

cmake ..

cmake --build .
