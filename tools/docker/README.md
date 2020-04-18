# Build container

## Clone repo

* `cd ~/`
* `git clone git://github.com/CoreELEC/CoreELEC.git CoreELEC`

## Build the container

* `cd ~/CoreELEC`
* `docker build --pull -t coreelec tools/docker/bionic`

## Build image inside container

* `docker run -v ~/:/home/docker -h coreelec -it coreelec`
* `cd ~/CoreELEC`
* `time(PROJECT=Amlogic-ng ARCH=arm make image)`  
Use `PROJECT=Amlogic` to build images for older S912 and S905/X/D devices
