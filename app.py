from flask import Flask, jsonify, render_template
import sqlite3

app = Flask(__name__)

def fetch_all_sensor_data():
    conn = sqlite3.connect("sensor_data.db")
    cursor = conn.cursor()
    
    cursor.execute("SELECT time, luminosity, temperature, pressure, humidity FROM sensor_data")
    rows = cursor.fetchall()
    conn.close()
    
    data = [
        {
            "time": row[0],
            "luminosity": row[1],
            "temperature": row[2],
            "pressure": row[3],
            "humidity": row[4]
        }
        for row in rows
    ]
    return data


@app.route("/")
def get_sensor_data():
    try:
        data = fetch_all_sensor_data()
        return render_template('index.html', data=data)
    except Exception as e:
        return jsonify({"error": str(e)}), 500

if __name__ == "__main__":
    app.run(debug=True, host="0.0.0.0", port=5055)
