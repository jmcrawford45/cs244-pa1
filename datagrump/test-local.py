from subprocess import call
window = WINDOW_MIN = 25
WINDOW_MAX = 1000
STEP = 1.5
SAMPLE_SIZE = 3

while True:
	for i in range(0, SAMPLE_SIZE):
		call(["./run-local", str(window)])
	if window == WINDOW_MAX:
		break
	window = min(int(window * STEP), WINDOW_MAX)