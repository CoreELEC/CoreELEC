# Build container

**Docker containers**
- Ubuntu
  - bionic  (Ubuntu 18.04)
  - focal   (Ubuntu 20.04)
  - groovy  (Ubuntu 20.10)
  - hirsute (Ubuntu 21.04)
- Debian
  - stretch (Debian  9.0)
  - buster  (Debian 10.0)
  - sid     (Debian unstable)

**Build docker image**

Use the following command to create a docker image and tag it with `coreelec`.

```
docker build --pull -t coreelec tools/docker/focal
```

See https://docs.docker.com/engine/reference/commandline/build/ for details on `docker build` usage.

**Build CoreELEC image inside a container**

Use the following command to build CoreELEC images inside a new container based on the docker image tagged with `coreelec`.

```
docker run --rm -v `pwd`:/build -w /build -it coreelec make image
```

Use `--env`, `-e` or `--env-file` to pass environment variables used by the CoreELEC buildsystem.

```
docker run --rm -v `pwd`:/build -w /build -it -e PROJECT=Amlogic-ce -e DEVICE=Amlogic-ng -e ARCH=arm coreelec make image
```

See https://docs.docker.com/engine/reference/commandline/run/ for details on `docker run` usage.
