# Run in docker

Simple and fast setup of AAC.IO on Docker is also available.

## Install Dependencies
 - [Docker](https://docs.docker.com) Docker 17.05 or higher is required
 - [docker-compose](https://docs.docker.com/compose/) version >= 1.10.0

## Docker Requirement
 - At least 8GB RAM (Docker -> Preferences -> Advanced -> Memory -> 8GB or above)

## Build aac image

```bash
git clone https://github.com/AACIO/aac.git --recursive
cd AcuteAngleChain/Docker
docker build . -t aacio/aac
```

## Start nodaac docker container only

```bash
docker run --name nodaac -p 8888:8888 -p 9876:9876 -t aacio/aac nodaacd.sh arg1 arg2
```

By default, all data is persisted in a docker volume. It can be deleted if the data is outdated or corrupted:
``` bash
$ docker inspect --format '{{ range .Mounts }}{{ .Name }} {{ end }}' nodaac
fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
$ docker volume rm fdc265730a4f697346fa8b078c176e315b959e79365fc9cbd11f090ea0cb5cbc
```

Alternately, you can directly mount host directory into the container
```bash
docker run --name nodaac -v /path-to-data-dir:/opt/aacio/bin/data-dir -p 8888:8888 -p 9876:9876 -t aacio/aac nodaacd.sh arg1 arg2
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

After `docker-compose up -d`, two services named `nodaacd` and `kaacd` will be started. nodaac service would expose ports 8888 and 9876 to the host. kaacd service does not expose any port to the host, it is only accessible to claac when runing claac is running inside the kaacd container as described in "Execute claac commands" section.


### Execute claac commands

You can run the `claac` commands via a bash alias.

```bash
alias claac='docker-compose exec kaacd /opt/aacio/bin/claac -H nodaacd'
claac get info
claac get account inita
```

Upload sample exchange contract

```bash
claac set contract exchange contracts/exchange/exchange.wast contracts/exchange/exchange.abi
```

If you don't need kaacd afterwards, you can stop the kaacd service using

```bash
docker-compose stop kaacd
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
