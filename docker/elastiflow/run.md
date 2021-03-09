mkdir /var/lib/elastiflow
chown -R 1000:1000 /var/lib/elastiflow
docker-compose up -d

curl -XDELETE 'http://127.0.0.1:9200/elastiflow-3.5.1-2021.02.09'
