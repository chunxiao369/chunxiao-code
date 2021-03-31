kubeadm init \
  --apiserver-advertise-address=10.17.0.123 \
  --image-repository registry.aliyuncs.com/google_containers \
  --kubernetes-version v1.16.0 \
  --service-cidr=10.10.0.0/16 \
  --pod-network-cidr=10.244.0.0/16

kubectl run --image=nginx:alpine nginx-app --port=80
下载下来的image可以node上用docker image查看到

kubectl create deployment nginx --image=nginx
kubectl expose deployment nginx --port=80 --type=NodePort
kubectl get pod,svc
kubectl get pod,service

kubectl get pods -o wide
kubectl delete pod [pod_name]

kubectl get nodes --show-labels

扩容命令：kubectl scale deployment nginx-deployment --replicas 10 或者 kubectl scale --replicas=3 -f XXX.yaml
k8s设置自动扩容命令：kubectl autoscale deployment nginx-deployment --min=10 --max=15 --cpu-percent=80

kubectl exec $pod_name -- sh -c "cat xxx"
kubectl exec -c <container> <pod> -n <namespace> -- command
kubectl exec -c <container> <pod> -n <namespace> -- sh -c "command"

kubeadm config images list --kubernetes-version v1.16.0

kubectl describe pod nginx-86c57db685-264s8
kubectl describe service nginx

kubectl create -f embed_net.yaml
kubectl scale --replicas=3 deployment/nginx
kubectl rolling-update滚动升级与回滚

pod/service/rolling-update/set(resource limit)/probe(healthy check)
