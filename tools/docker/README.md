# Build container

## Clone repo

* `cd ~/`
* `git clone https://github.com/CoreELEC/CoreELEC.git CoreELEC`

## Build the container

* `cd ~/CoreELEC`
* `docker build --pull -t coreelec tools/docker/xenial`

## Build image inside container

* `docker run -v ~/:/home/docker -h coreelec -it coreelec`
* `cd ~/CoreELEC`
* `make image`
