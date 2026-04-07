# Computer — Web server & 3D reconstruction

Receives photos from the Pi, serves a real-time web monitor, and runs Meshroom for 3D reconstruction.

## System states

```
INITIALIZATION → READING → WRITING → PROCESSING
```

| State | Description |
|-------|-------------|
| INITIALIZATION | Start server on :3333, ping the Pi to check availability |
| READING | Receive images and metadata from the Pi via HTTP upload |
| WRITING | Store images on disk in a structured layout |
| PROCESSING | Send data to a remote machine and run Meshroom reconstruction |

## Communication

| Link | Protocol | Port | Purpose |
|------|----------|------|---------|
| Pi → Computer | HTTP 1.1 | :3333 | Image upload |
| Browser → Computer | WebSocket | :3333 | Real-time dashboard updates |
| Browser → Computer | HTTP 1.1 | :3333 | Web monitor pages |

## Web monitor

Four pages served by the Python webserver:

| Page | Content |
|------|---------|
| Index | Main dashboard — Pico + Pi status, launch scans, run hardware tests |
| Status | Motor positions, camera angles, accelerometer readings |
| Metrics | Response time, action duration, stability score, image quality |
| Productivity | Accelerate metrics — lead time, deployment frequency, time to restore, change failure rate |

WebSocket keeps the dashboard updated in near real-time. Frontend: HTML + CSS + vanilla JS only.

## 3D Reconstruction

The local machine offloads reconstruction to a more powerful remote host:

1. Upload structured image set via SSH/HTTP.
2. Run **Meshroom** on the remote host.
3. Retrieve the resulting 3D model.
4. Serve the model via the web monitor.
5. Clean up raw images on success.

## Build

```bash
make -C webserver/webserv
```

## Metrics

- Reconstruction success rate
- Model quality score
- Scan completion time
- Processing time (upload + reconstruction + retrieval)
