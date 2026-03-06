# Kubernetes MPC Worker Deployment

This directory contains configurations for deploying MPC (Multi-Party Computation) workers using Kubernetes.

## Overview

The MPC layer performs encrypted argmin operations for cluster assignment. Three semi-honest parties coordinate via Kubernetes pods to:

1. Receive encrypted distance shares from the HE pipeline
2. Securely compute the minimum distance without decryption
3. Output plaintext cluster assignments

## Prerequisites

- **Kubernetes cluster** (local minikube or AWS EKS)
- **kubectl** command-line tool configured
- **Docker** (if building custom images)
- **OpenFHE & SCALE-MAMBA** libraries installed in container image

## Deployment Architecture

```
┌─────────────────────────────────────────┐
│ HE Pipeline (src/)                      │
│ Computes encrypted distances            │
└──────────────┬──────────────────────────┘
               │
               ├─ Share 1 ──► Pod 1 (Party A)
               ├─ Share 2 ──► Pod 2 (Party B)
               └─ Share 3 ──► Pod 3 (Party C)
                            │
                            └─► Consensus ──► Cluster Assignments
```

## Quick Start

### 1. Configure Kubernetes

For **local development** (minikube):
```bash
minikube start --cpus=4 --memory=8192
```

For **AWS EKS**:
```bash
eksctl create cluster --name tips-cluster --region us-east-1
```

### 2. Build MPC Worker Docker Image

```bash
# Create Dockerfile (if not present)
cat > Dockerfile.mpc-worker << 'EOF'
FROM ubuntu:22.04
RUN apt-get update && apt-get install -y openfhe python3 python3-pip
COPY --from=builder /usr/local/lib/libscale_mamba* /usr/local/lib/
WORKDIR /app
COPY mpc_worker.py .
CMD ["python3", "mpc_worker.py"]
EOF

# Build and push image
docker build -f Dockerfile.mpc-worker -t your-registry/mpc-worker:latest .
docker push your-registry/mpc-worker:latest
```

### 3. Deploy Workers

Create and apply deployment manifest:

```bash
cat > mpc-worker-deployment.yaml << 'EOF'
apiVersion: v1
kind: ConfigMap
metadata:
  name: mpc-config
data:
  party_id: "0"
  num_parties: "3"

---
apiVersion: apps/v1
kind: Deployment
metadata:
  name: mpc-worker
  labels:
    app: mpc-worker
spec:
  replicas: 3
  selector:
    matchLabels:
      app: mpc-worker
  template:
    metadata:
      labels:
        app: mpc-worker
    spec:
      containers:
      - name: mpc-worker
        image: your-registry/mpc-worker:latest
        ports:
        - containerPort: 5000
        env:
        - name: PARTY_ID
          valueFrom:
            configMapKeyRef:
              name: mpc-config
              key: party_id
        - name: NUM_PARTIES
          valueFrom:
            configMapKeyRef:
              name: mpc-config
              key: num_parties
        resources:
          requests:
            memory: "2Gi"
            cpu: "1000m"
          limits:
            memory: "4Gi"
            cpu: "2000m"

---
apiVersion: v1
kind: Service
metadata:
  name: mpc-worker-service
spec:
  selector:
    app: mpc-worker
  ports:
  - port: 5000
    targetPort: 5000
  type: LoadBalancer
EOF

kubectl apply -f mpc-worker-deployment.yaml
```

### 4. Verify Deployment

```bash
# Check pod status
kubectl get pods -l app=mpc-worker

# View logs
kubectl logs -l app=mpc-worker

# Test connectivity
kubectl port-forward svc/mpc-worker-service 5000:5000
```

## Configuration

Modify environment variables in the YAML to:
- Change number of parties (default: 3)
- Adjust resource limits (CPU, memory)
- Set party IDs (0, 1, 2)

## Integration with HE Pipeline

The HE clustering pipeline (`src/clustering.cpp`) communicates with MPC workers via:
- **RPC calls** to send encrypted distance shares
- **gRPC or HTTP** for inter-pod communication
- **TLS** for secure party-to-party channels

See `src/mpc_interface.cpp` for protocol details.

## Troubleshooting

### Workers not starting
```bash
kubectl describe pod <pod-name>
kubectl logs <pod-name>
```

### Network connectivity issues
```bash
kubectl exec -it <pod-name> -- ping <other-pod-name>
kubectl get svc
```

### Resource constraints
```bash
# Increase node capacity
kubectl top nodes
kubectl scale nodes --replicas=5
```

## Security Considerations

- ⚠️ **Semi-honest model**: Parties follow protocol but may infer data from messages
- **TLS required**: All inter-pod communication must use encryption
- **Isolated network**: Use Kubernetes NetworkPolicy to restrict traffic
- **Audit logging**: Enable Kubernetes audit logs for compliance

## Advanced: Custom Worker Implementation

To implement custom MPC protocols:

1. Extend `mpc_worker.py` with your algorithm
2. Update `mpc_interface.cpp` with new RPC definitions
3. Rebuild Docker image and redeploy

## References

- [Kubernetes Official Docs](https://kubernetes.io/docs/)
- [SCALE-MAMBA Documentation](https://homes.esat.kuleuven.be/~nsmart/SCALE/)
- [OpenFHE Integration Guide](https://github.com/openfheorg/openfhe-development)

## Support

For issues, see the main project README or file an issue on GitHub.
