docker run -it -v /home/lance/Project/pcap_file/vpn/:/data mhwong2007/deep_packet bash
docker run -d --privileged=true --name=jovial_mirzakhani -it -v /mnt/ssd/vpn/:/data mhwong2007/deep_packet:net bash
docker run -d --privileged=true --gpus all,capabilities=utility --name=jovial_mirzakhani -it -v /mnt/:/data mhwong2007/deep_packet:net2  bash
10.17.0.65
docker run -d --privileged=true --gpus all,capabilities=utility --name=jovial_mirzakhani -it -v /home/cxxu/vpn/:/data mhwong2007/deep_packet:net2  bash
docker run -d --privileged=true --gpus all,capabilities=utility --name=jovial_mirzakhani -it -v /home/cxxu/vpn/:/data -v /mnt/sdc1/cxxu/deep_packet_github:/mnt/ mhwong2007/deep_packet:net2  bash

docker run -it \
--privileged=true --gpus all,capabilities=utility --name=jovial_mirzakhani \
-v /home/cxxu/vpn/:/data -v /mnt/sdc1/cxxu/deep_packet_github:/mnt/ \
-p 8888:8888 \
mhwong2007/deep_packet:net2 \
jupyter lab --no-browser --ip=0.0.0.0 --port=8888 --NotebookApp.token='' --allow-root

docker commit youthful_cannon mhwong2007/deep_packet:net

docker cp /home/lance/Project/open-source/deep_packet/create_train_test_set.py youthful_cannon:/home/deep_packet/

