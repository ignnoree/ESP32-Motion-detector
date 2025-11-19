from flask import Flask, request,render_template
from flask_cors import CORS

app = Flask(__name__)
CORS(app)

readings = []

@app.route("/data", methods=["POST"])
def api():
    entry = {
        "motion_detected": request.json.get("motion_detected"),
        "timestamp": request.json.get("timestamp"),
    }

    readings.append(entry)
    return {"status": "ok", "saved": entry}, 200



@app.route("/", methods=["GET"])
def home():
    latest = readings[-1] if readings else {
        "motion_detected": False,
        "timestamp": "—",
    }
    return render_template("index.html", latest=latest)

if __name__ == '__main__':
    app.run(host='0.0.0.0', port=5000, debug=False)
