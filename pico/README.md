Main objective: provide an interface spi to control 3 motors.

Stepper motor: moves in 10° increments in one direction.
DC motor: 3 states – Top, Bottom, Moving; can only transition between adjacent states.
Camera servo: 4 positions – HIGH_A, HIGH_B, LOW_A, LOW_B, Moving; can only transition between adjacent positions.

insure stability : two accellerometre, one on the plate for the object and one beside the camera,
					insure that everything is stable.

System states: LISTENING, INTERPRETING, EXECUTING.

Each use case (UC) manages a different motor and keeps track of its current logical state.
After boot:
DC motor checks position via the top button and exits on failure.
Servo moves to initial position.
Stepper considers its current position as 0.

Metrics to drive improvement:

1) Time of answer
2) Stability score: measures the stability of the camera and central object during capture to ensure sharp photos and reliable 3D reconstruction.
3) Time for an action
