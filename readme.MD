# Bad Apple! On the Phoenix Contact `AXL F DO32/1 1F`

This little project has been made to test out the guts this module can have. I don't plan to modify this code in the future.

## How to build the project

### Downloading the libraries

#### 🌍 Libmodbus

Under the `vendor` folder inside `libmodbus` you will have to go in a bash and type those commands :

```sh
mkdir bin
mkdir lib
mkdir include
```

Download the latest Windows x64 version of the libmodbus [here](https://github.com/stephane/libmodbus/wiki/Compile-dll-under-Windows-with-Visual-Studio-2008-Express). Copy the **Release** `.lib` and put it in the `lib` folder, do the same for the `.dll` inside the bin, and then copy and paste all the header files.

#### 💡 OpenCV

Under the `vendor` folder inside `opencv` you will have to go in a bash and type those commands :

```sh
mkdir bin
mkdir lib
mkdir include
```

Download the latest OpenCV for Windows [here](https://opencv.org/releases/). Inside the extracted folder go to "opencv\build\x64\vc16", copy both `lib` and `bin` folders. then go to "opencv\build\include" and copy its content in the `include` folder.

### Compiling

This code only compiles on Windows unfortunetly, to compile, go to the root folder and create the build folder :

```sh
mkdir build
cd build
```
Then you can build in two build modes:
```sh
cmake -DCMAKE_BUILD_TYPE=Debug ..
cmake --build . --config Debug
```
or
```sh
cmake -DCMAKE_BUILD_TYPE=Release ..
cmake --build . --config Release
```
**Simply run the program afterwards!**

### ❗ If you are using CMake on Visual Studio Code ❗

Be sure that in your `settings.json` you add the following lines:
```json
"cmake.configureSettings": {
   "CMAKE_BUILD_TYPE": "${buildType}"
}
```

Otherwise it won't compile. The library used (mainly openCV) requires a build type for each `.dll`/`.lib`.