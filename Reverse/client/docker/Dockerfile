FROM ghcr.io/w4terdr0p/w4terctf-2023/gmpy2:alpine

RUN apk add build-base cmake

COPY --chown=ctf:ctf keystone /home/ctf/keystone
COPY --chown=ctf:ctf server.c /home/ctf
COPY --chmod=500 init.sh /init.sh

RUN cd /home/ctf/keystone && mkdir build && cd build && ../make-share.sh && make install
RUN cd /home/ctf && gcc server.c -o server -lkeystone && rm -rf ./keystone && rm ./server.c

CMD ["/init.sh"]