# Build container

**Docker containers**
- Ubuntu
  - jammy     (Ubuntu 22.04)
  - noble     (Ubuntu 24.04)
  - questing  (Ubuntu 25.10)
  - resolute  (Ubuntu 26.04)
- Debian
  - bookworm  (Debian 12)
  - trixie    (Debian 13)

**Build docker image**

Use the following command to create a docker image and tag it with `coreelec`.

```
docker build --pull -t coreelec tools/docker/noble
```

See https://docs.docker.com/engine/reference/commandline/build/ for details on `docker build` usage.

**Build CoreELEC image inside a container**

Change to your CoreELEC development directory that you checked out with <br>
 `git clone https://github.com/`**myname**`/CoreELEC.git`

 ```
 cd CoreELEC
 ```

Then use the following command to build CoreELEC images inside a new container based on the docker image tagged with `coreelec`. (The `pwd` uses the current directory - which must have the CoreELEC `Makefile` in it.)

```
docker run --rm -v `pwd`:/build -w /build -it coreelec make image
```

Use `--env`, `-e` or `--env-file` to pass environment variables used by the CoreELEC buildsystem.

```
docker run --rm -v `pwd`:/build -w /build -it -e PROJECT=Amlogic-ce -e DEVICE=Amlogic-no -e ARCH=aarch64 coreelec make image
```

See https://docs.docker.com/engine/reference/commandline/run/ for details on `docker run` usage.

Note: `dockerd` is set to send all its logs to journald using the setting `--log-driver=journald` (so if you don't set the `--log-driver none` for your `docker run` these logs will be sent through to your log.
Refer:

https://github.com/LibreELEC/LibreELEC.tv/blob/1810c97fb2839486e63f6694dd093428ba24c39a/packages/addons/service/docker/source/system.d/service.system.docker.service#L12
