FROM python:3.11-bullseye

RUN mkdir -p /app && \
    pip install --no-cache-dir -i https://pypi.tuna.tsinghua.edu.cn/simple flask scikit-learn && \
    adduser -D -G www-data -s /bin/ash -h /app www-data && \
    chmod 777 /app

COPY --chmod=555 --chown=www-data:www-data app.py ai.py /app/
COPY --chmod=444 --chown=www-data:www-data templates/ /app/templates/
COPY --chmod=444 --chown=www-data:www-data static/ /app/static/
COPY --chmod=777 init.sh /etc/init.sh


WORKDIR /app
USER www-data

CMD ["/etc/init.sh"]