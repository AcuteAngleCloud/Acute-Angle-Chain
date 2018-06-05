FROM aacio/builder as builder

RUN git clone -b master --depth 1 https://github.com/AACIO/aac.git --recursive \
    && cd aac \
    && cmake -H. -B"/tmp/build" -GNinja -DCMAKE_BUILD_TYPE=Release -DWASM_ROOT=/opt/wasm -DCMAKE_CXX_COMPILER=clang++ \
       -DCMAKE_C_COMPILER=clang -DCMAKE_INSTALL_PREFIX=/tmp/build  -DSecp256k1_ROOT_DIR=/usr/local \
    && cmake --build /tmp/build --target install


FROM ubuntu:16.04

RUN apt-get update && DEBIAN_FRONTEND=noninteractive apt-get -y install openssl && rm -rf /var/lib/apt/lists/*
COPY --from=builder /usr/local/lib/* /usr/local/lib/
COPY --from=builder /tmp/build/bin /opt/aacio/bin
COPY --from=builder /tmp/build/contracts /contracts
COPY --from=builder /AcuteAngleChain/Docker/config.ini /AcuteAngleChain/genesis.json /
COPY nodaacd.sh /opt/aacio/bin/nodaacd.sh
ENV AACIO_ROOT=/opt/aacio
RUN chmod +x /opt/aacio/bin/nodaacd.sh
ENV LD_LIBRARY_PATH /usr/local/lib
VOLUME /opt/aacio/bin/data-dir
ENV PATH /opt/aacio/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin
