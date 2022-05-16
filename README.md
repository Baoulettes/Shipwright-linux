# Welcome to the Ship of Harkinian 
## Twisted build !
This a PC port of OOT bla bla you know it if you are there.
Here some usefull information : 
```
Build team: `zelda@srd022j`
Build date: `03-02-21 00:49:18` (year-month-day)
sha1: cee6bc3c2a634b41728f2af8da54d9bf8cc14099
```
This build feature all my mods pre-installed, they can be toggle on and off with the DEV menu.
DEV Menu can be open with F1.
I will manually add PR from the main repo -> https://github.com/HarbourMasters/Shipwright

Before starting be sure to open a terminal or what ever you use in a folder that has no space in it's path.
How to build (All thanks to Random :) )
```
git clone https://github.com/Baoulettes/Shipwright-linux.git
cd Shipwright-linux
```

Add your rom in soh folder baserom_non_mq.z64 sha1:cee6bc3c2a634b41728f2af8da54d9bf8cc14099

## Docker Procedure
Now do the docker build (be sure to have docker :P)
```
sudo docker build . -t soh
sudo docker run --rm -it -v $(pwd):/soh soh /bin/bash
```

now you will be in "docker mode"
Do these one by one and it should be fine 

```
git clone https://github.com/ladislav-zezula/StormLib external/StormLib
cmake -B external/StormLib/build -S external/StormLib
cmake --build external/StormLib/build
cp external/StormLib/build/libstorm.a external
cp /usr/local/lib/libGLEW.a external

cd soh
# Extract the assets/Compile the exporter/Run the exporter
make setup -j$(nproc)
# Compile the code
make -j $(nproc)
```
And that should be good to go, enjoy :)  

# This build you build on Windows too following original readme on original repo :  
# [Original readme on the original repo](https://github.com/HarbourMasters/Shipwright/blob/develop/README.md)
