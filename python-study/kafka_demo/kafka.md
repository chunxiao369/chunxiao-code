docker run -d --name zookeeper  -p 2181:2181 wurstmeister/zookeeper

docker run -d --name kafka -p 9092:9092 --link zookeeper:zookeeper -e KAFKA_ZOOKEEPER_CONNECT=zookeeper:2181 -e KAFKA_ADVERTISED_HOST_NAME=localhost -e KAFKA_AUTO_CREATE_TOPICS_ENABLE=true wurstmeister/kafka

docker logs kafka | tail -n 20

docker exec -it kafka /opt/kafka/bin/kafka-topics.sh --create --zookeeper zookeeper:2181 --replication-factor 1 --partitions 1 --topic flows
docker exec -it kafka /opt/kafka/bin/kafka-topics.sh --create --zookeeper zookeeper:2181 --replication-factor 1 --partitions 1 --topic aggregated
docker exec -it kafka /opt/kafka/bin/kafka-console-consumer.sh --bootstrap-server localhost:9092 --topic aggregated --from-beginning
docker run --name grafana \
    -it --net=host  -d -p 3000:3000 \
    --env-file $PWD/grafana/grafana.env \
    -v $PWD/grafana/dashboards:/var/lib/grafana/dashboards \
    -v $PWD/grafana/plugins:/var/lib/grafana/plugins \
    -v $PWD/grafana/provisioning:/etc/grafana/provisioning \
    -v /etc/localtime:/etc/localtime:ro \
    grafana/grafana

