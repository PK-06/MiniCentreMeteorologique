import sqlite3
from urllib.request import urlopen
import time
import json

url = "http://192.168.0.63/" # URL de votre capteur

conn = sqlite3.connect("sensor_data.db")
cursor = conn.cursor()

while True:
    try:
        time.sleep(60)  # Pause d'une minute
        response = urlopen(url)
        data_json = json.loads(response.read())
        
        cursor.execute("""
        INSERT INTO sensor_data (time, luminosity, temperature, pressure, humidity)
        VALUES (?, ?, ?, ?, ?)
        """, (data_json["time"], data_json["luminosity"], data_json["temperature"], 
              data_json["pressure"], data_json["humidity"]))
        conn.commit()

        print("Données insérées:", data_json)
    except Exception as e:
        print("Erreur:", e)

conn.close()
