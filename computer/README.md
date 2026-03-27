
Main goal: retrieve data and transform it into a 3D model.

System states
	INITIALIZATION
		Start server on localhost:3333,  ping the Raspberry Pi to check availability.
	READING
		Receive images and metadata from the Pi.
	WRITING
		Store images on disk in a structured way.
	PROCESSING
		Send data to a more powerful computer for 3D reconstruction.

Monitoring
The server provides a real-time dashboard displaying:

	Scanner state
	Raspberry Pi and Pico status
	Motor positions
	Accelerometer data

It also includes:

	History of scans and system evolution
	Progress tracking (implemented vs pending features)
	Buttons to:
	run hardware tests
	test connections
	Processing strategy

Since the local computer is not powerful enough:

	Send data to a remote machine
	Run reconstruction there using:
	Meshroom

Metrics : 

	reconstruction success rate
	model quality
	scan completion time
	Processing time


connection : use a python webserver
		the websocket connection is used for get metrics and send order,
		http protocol for upload


Web Monitor

The web monitor provides a lightweight interface to control and monitor the 3D scanner system. It consists of four main pages:

Index – main dashboard, displays overall Pico + Pi status, allows launching scans and running hardware tests in real time.
Status – detailed view of motors, camera positions, and sensor readings.
Metrics – shows performance metrics for each objective (response time, action duration, stability, etc.) and their evolution.
Productivity – tracks Accelerate metrics: lead time, deployment frequency, time to recover, and change failure rate.

A WebSocket connection keeps the dashboard updated in near real-time, while the frontend remains minimal and fast using only HTML, CSS, and vanilla JavaScript.


