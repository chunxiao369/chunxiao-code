
ovs-vsctl list mirror
ovs-vsctl add-port br0 emrr0 -- set interface emrr0 type=internal
ovs-vsctl -- set Bridge br0 mirrors=@m -- --id=@cxxu0 get Port cxxu0 -- --id=@emrr0 get Port emrr0 -- --id=@m create Mirror name=m0 select-dst-port=@cxxu0 select-src-port=@cxxu0 output-port=@emrr0
ovs-vsctl remove Bridge br0 mirrors _uuid  _uuid可以通过ovs-vsctl list mirror查看到

ovs-vsctl list ipfix
ovs-vsctl set Bridge br0 ipfix=@i -- --id=@i create IPFIX targets=\"192.168.3.3:2055\" obs_domain_id=123 obs_point_id=456 cache_active_timeout=60 other_config:enable-input-sampling=false  other_config:enable-tunnel-sampling=false
ovs-vsctl clear bridge br0 ipfix
