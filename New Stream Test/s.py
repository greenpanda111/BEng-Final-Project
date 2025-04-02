import cv2
import flask
from picamera2 import Picamera2
import numpy as np

# Initialize Flask app
app = flask.Flask(__name__)

# Initialize PiCamera
picam2 = Picamera2()
picam2.configure(picam2.create_preview_configuration(main={"size": (640, 360), "format": "RGB888"}))
picam2.start()

@app.route('/')
def index():
    return flask.render_template('index.html')  # Make sure index.html exists in the 'templates' folder


def gen():
    while True:
        frame = picam2.capture_array()
        ret, jpeg = cv2.imencode('.jpg', frame)
        if not ret:
            continue
        yield (b'--frame\r\n'
               b'Content-Type: image/jpeg\r\n\r\n' + jpeg.tobytes() + b'\r\n')


@app.route('/stream')
def video_feed():
    return flask.Response(gen(),
                          mimetype='multipart/x-mixed-replace; boundary=frame')


if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000)
