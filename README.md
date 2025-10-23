# Asynchronous file downloader from URLs via thread pool
## Build
To build the programm, you have `make` installed. Start build:
```bash
make
```
Clean and remove build directory:
```bash
make clean
```
Launch program:
```bash
./build/main.exe urls.txt mydir 8
```
 - `urls.txt` file with target URLs
 - `mydir` directory name for downloaded files
 - `8` number of threads