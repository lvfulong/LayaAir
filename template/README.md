

#Linux 

#dependences

#sudo apt-get install libgl1-mesa-dev
#sudo apt-get install libx11-dev
#libxext-dev

可能上面的不用了， 用下面的就行
sudo apt-get install build-essential
sudo apt-get install libx11-dev 
sudo apt-get install libxft-dev
sudo apt-get install libsdl2-dev
sudo apt-get install libfreetype6-dev 
sudo apt-get install libfontconfig1-dev
sudo apt-get install libcairo2-dev

如果python的版本为2，设置python3为默认版本
sudo apt install python3
sudo update-alternatives --install /usr/bin/python python /usr/bin/python3 1

#gdb debug

ulimit -a
ulimit -c unlimited

ulimit -c