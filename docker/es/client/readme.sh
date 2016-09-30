#!/bin/sh
#docker run -d --restart=always -p 9200:9200 -p 9300:9300 --name=elasticsearch-client --oom-kill-disable=true --memory-swappiness=1 -v /home/lance/Project/chunxiao-code/docker/es/client/data:/usr/share/elasticsearch/data -v /home/lance/Project/chunxiao-code/docker/es/client/logs:/usr/share/elasticsearch/logs elasticsearch:2.3.3
docker run -d --restart=always \
-p 9200:9200 -p 9300:9300 \
--name=elasticsearch-client --oom-kill-disable=true \
--memory-swappiness=1 \
-v `pwd`/data:/usr/share/elasticsearch/data \
-v `pwd`/logs:/usr/share/elasticsearch/logs \
-v `pwd`/config/elasticsearch.yml:/usr/share/elasticsearch/config/elasticsearch.yml \
elasticsearch:2.3.3
#docker cp elasticsearch.yml elasticsearch-client:/usr/share/elasticsearch/config/elasticsearch.yml
#docker restart elasticsearch-client
