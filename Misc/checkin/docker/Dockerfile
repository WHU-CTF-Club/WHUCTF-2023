FROM ghcr.io/w4terdr0p/w4terctf-2023/xinetd:alpine

COPY init.sh /init.sh
COPY xinetd.conf /etc/xinetd.conf

RUN chmod +x /init.sh && \
    chown -R ctf:ctf /home/ctf && \
    chmod -R 750 /home/ctf

COPY --chown=ctf:ctf --chmod=500 checkin /home/ctf/

CMD ["xinetd", "-dontfork"]