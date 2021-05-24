kubeadm init \
  --apiserver-advertise-address=10.17.0.123 \
  --image-repository registry.aliyuncs.com/google_containers \
  --kubernetes-version v1.16.0 \
  --service-cidr=10.10.0.0/16 \
  --pod-network-cidr=10.244.0.0/16

kubectl run --image=nginx:alpine nginx-app --port=80
下载下来的image可以node上用docker image查看到

kubectl delete node k8s-node1

kubectl create deployment nginx --image=nginx
kubectl delete deployment nginx
kubectl expose deployment nginx --port=80 --type=NodePort
将Pod暴露为一个 NodePort服务
kubectl expose pod mc3          --port=80 --type=NodePort
kubectl get pod,svc
kubectl get pod,service
kubectl get deployment
kubectl get ns
kubectl get network-attachment-definitions

kubectl get pods -o wide
kubectl get pods --all-namespaces
kubectl -n kube-system get pods
kubectl logs kube-sriov-device-plugin-amd64-d8sbx -n kube-system
kubectl delete pod [pod_name]

kubectl get nodes --show-labels

扩容命令：kubectl scale deployment nginx-deployment --replicas 10 或者 kubectl scale --replicas=3 -f XXX.yaml
k8s设置自动扩容命令：kubectl autoscale deployment nginx-deployment --min=10 --max=15 --cpu-percent=80

kubectl exec $pod_name -- sh -c "cat xxx"
kubectl exec -c <container> <pod> -n <namespace> -- command
kubectl exec -c <container> <pod> -n <namespace> -- sh -c "command"
kubectl exec <pod_name>mc1 -c 2nd -- /bin/cat /html/index.html 

kubeadm config images list --kubernetes-version v1.16.0

kubectl describe pod nginx-86c57db685-264s8
kubectl describe service nginx

kubectl create -f embed_net.yaml
kubectl delete pod test
kubectl scale --replicas=3 deployment/nginx
kubectl rolling-update滚动升级与回滚

pod/service/rolling-update/set(resource limit)/probe(healthy check)

kubectl api-resources -o wide --namespaced=false
kubectl api-resources -o wide --namespaced=true

kubectl delete pod sriovpod1
kubectl delete -f deployments/k8s-v1.16/sriovdp-daemonset.yaml
kubectl delete -f deployments/configMap.yaml

kubectl create -f deployments/configMap.yaml
kubectl create -f deployments/k8s-v1.16/sriovdp-daemonset.yaml
kubectl create -f deployments/pod-tc1.yaml
kubectl get pod
kubectl -n kube-system get pods
kubectl get nodes
kubectl get node k8s-node1 -o json | jq '.status.allocatable'
kubectl logs kube-sriov-device-plugin-amd64-** -n kube-system

kubectl get pods --all-namespaces | grep -i multus
kubectl exec -it sriovpod1 bash

kubectl delete pod virtiopod1
kubectl delete -f deployments/k8s-v1.16/sriovdp-embedway.yaml
kubectl delete -f deployments/configMap_embedway.yaml
kubectl delete -f deployments/embedway-crd.yaml

kubectl create -f deployments/embedway-crd.yaml
#kubectl get network-attachment-definitions
kubectl create -f deployments/configMap_embedway.yaml
#kubectl get configmaps -A
#kubectl describe configmaps sriovdp-config -n kube-system
kubectl create -f deployments/k8s-v1.16/sriovdp-embedway.yaml
kubectl create -f deployments/pod-embd1.yaml
kubectl describe pod virtiopod1
journalctl -xefu kubelet


kubectl delete -f deployments/pod-eth4.yaml
kubectl delete -f deployments/eth4-crd.yaml
kubectl create -f deployments/eth4-crd.yaml
kubectl create -f deployments/pod-eth4.yaml

kubectl delete -f deployments/pod-eth5.yaml
kubectl delete -f deployments/eth5-crd.yaml
kubectl create -f deployments/eth5-crd.yaml
kubectl create -f deployments/pod-eth5.yaml
