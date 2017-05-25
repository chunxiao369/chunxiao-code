
#文件处理
'''
cut -d ',' -f 1,2,7,8 460003916957662.csv > filter.loc
sudo docker cp filter.loc elk5:/data/
sudo docker cp imsi_loc.conf elk5:/etc/logstash/conf.d/
sudo docker cp imsi_loc.json elk5:/tmp/logstash/
/opt/logstash/bin/logstash -f /etc/logstash/conf.d/imsi_loc.conf >/var/log/logstash/imsi_loc.log &
'''
