Main goal: capture photos of an object and send them to a computer for processing.

System states:
	INITIALIZATION
		Ensure Pico is ready, connection with computer is established, and enough disk space is available.
	READY
		Wait for commands from the computer (http 1.1).
	PROCESSING
		Control motors via api, position camera/object, ensure stability, take photos.
	SENDING
		Compress data and send it to the computer.

uc :
	CheckConnection
	ListenForOrder
	CaptureData
	SendData

Metrics:
	Time of answer
	Processing time
	Image quality (blur, reflections)
	Sending time (incl. compression efficiency)
	Recovery rate
	
