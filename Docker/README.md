# Run in docker

Simple and fast setup of AAC.IO on Docker is also available.

## Install Dependencies

- [Docker](https://docs.docker.com) Docker 17.05 or higher is required
- [docker-compose](https://docs.docker.com/compose/) version >= 1.10.0

## Docker Requirement

- At least 7GB RAM (Docker -> Preferences -> Advanced -> Memory -> 7GB or above)
- If the build below fails, make sure you've adjusted Docker Memory settings and try again.

## Build aac image

```bash
git clone https://github.com/AAC-Mainnet/aac.git --recursive  --depth 1
cd aac/Docker
docker build . -t aacio/aac
```

The above will build off the most recent commit to the master branch by default. If you would like to target a specific branch/tag, you may use a build argument. For example, if you wished to generate a docker image based off of the v1.1.3 tag, you could do the following:

```bash
docker build -t aacio/aac:v1.1.3 --build-arg branch=v1.1.3 .
```

By default, the symbol in aacio.system is set to SYS. You can override this using the symbol argument while building the docker image.

```bash
docker build -t aacio/aac --build-arg symbol=<symbol> .
```

## Start nodaac docker container only

```bash
docker run --name nodaac -p 8888:8888 -p 9876:9876 -t aacio/aac nodaacd.sh -e --http-alias=nodaac:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

By default, all data is persisted in a docker volume. It can be deleted if the data is outdated or corrupted:

```bash
$ docker inspect --format '{{ range .Mounts }}{{ .Name }} {{ end }}' nodaac
fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
$ docker volume rm fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
```

Alternately, you can directly mount host directory into the container

```bash
docker run --name nodaac -v /path-to-data-dir:/opt/aacio/bin/data-dir -p 8888:8888 -p 9876:9876 -t aacio/aac nodaacd.sh -e --http-alias=nodaac:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888 arg1 arg2
```

## Get chain info

```bash
curl http://127.0.0.1:8888/v1/chain/get_info
```

## Start both nodaac and kaacd containers

```bash
docker volume create --name=nodaac-data-volume
docker volume create --name=kaacd-data-volume
docker-compose up -d
```

After `docker-compose up -d`, two services named `nodaacd` and `kaacd` will be started. nodaac service would expose ports 8888 and 9876 to the host. kaacd service does not expose any port to the host, it is only accessible to claac when running claac is running inside the kaacd container as described in "Execute claac commands" section.

### Execute claac commands

You can run the `claac` commands via a bash alias.

```bash
alias claac='docker-compose exec kaacd /opt/aacio/bin/claac -u http://nodaacd:8888 --wallet-url http://localhost:8900'
claac get info
claac get account inita
```

Upload sample exchange contract

```bash
claac set contract exchange contracts/exchange/
```

If you don't need kaacd afterwards, you can stop the kaacd service using

```bash
docker-compose stop kaacd
```

### Develop/Build custom contracts

Due to the fact that the aacio/aac image does not contain the required dependencies for contract development (this is by design, to keep the image size small), you will need to utilize the aacio/aac-dev image. This image contains both the required binaries and dependencies to build contracts using aaciocpp.

You can either use the image available on [Docker Hub](https://hub.docker.com/r/aacio/aac-dev/) or navigate into the dev folder and build the image manually.

```bash
cd dev
docker build -t aacio/aac-dev .
```

### Change default configuration

You can use docker compose override file to change the default configurations. For example, create an alternate config file `config2.ini` and a `docker-compose.override.yml` with the following content.

```yaml
version: "2"

services:
  nodaac:
    volumes:
      - nodaac-data-volume:/opt/aacio/bin/data-dir
      - ./config2.ini:/opt/aacio/bin/data-dir/config.ini
```

Then restart your docker containers as follows:

```bash
docker-compose down
docker-compose up
```

### Clear data-dir

The data volume created by docker-compose can be deleted as follows:

```bash
docker volume rm nodaac-data-volume
docker volume rm kaacd-data-volume
```

### Docker Hub

Docker Hub image available from [docker hub](https://hub.docker.com/r/aacio/aac/).
Create a new `docker-compose.yaml` file with the content below

```bash
version: "3"

services:
  nodaacd:
    image: aacio/aac:latest
    command: /opt/aacio/bin/nodaacd.sh --data-dir /opt/aacio/bin/data-dir -e --http-alias=nodaacd:8888 --http-alias=127.0.0.1:8888 --http-alias=localhost:8888
    hostname: nodaacd
    ports:
      - 8888:8888
      - 9876:9876
    expose:
      - "8888"
    volumes:
      - nodaac-data-volume:/opt/aacio/bin/data-dir

  kaacd:
    image: aacio/aac:latest
    command: /opt/aacio/bin/kaacd --wallet-dir /opt/aacio/bin/data-dir --http-server-address=127.0.0.1:8900 --http-alias=localhost:8900 --http-alias=kaacd:8900
    hostname: kaacd
    links:
      - nodaacd
    volumes:
      - kaacd-data-volume:/opt/aacio/bin/data-dir

volumes:
  nodaac-data-volume:
  kaacd-data-volume:

```

*NOTE:* the default version is the latest, you can change it to what you want

run `docker pull aacio/aac:latest`

run `docker-compose up`

### AACIO Testnet

We can easily set up a AACIO local testnet using docker images. Just run the following commands:

Note: if you want to use the mongo db plugin, you have to enable it in your `data-dir/config.ini` first.

```
# create volume
docker volume create --name=nodaac-data-volume
docker volume create --name=kaacd-data-volume
# pull images and start containers
docker-compose -f docker-compose-aacio-latest.yaml up -d
# get chain info
curl http://127.0.0.1:8888/v1/chain/get_info
# get logs
docker-compose logs -f nodaacd
# stop containers
docker-compose -f docker-compose-aacio-latest.yaml down
```

The `blocks` data are stored under `--data-dir` by default, and the wallet files are stored under `--wallet-dir` by default, of course you can change these as you want.

### About MongoDB Plugin

Currently, the mongodb plugin is disabled in `config.ini` by default, you have to change it manually in `config.ini` or you can mount a `config.ini` file to `/opt/aacio/bin/data-dir/config.ini` in the docker-compose file.
