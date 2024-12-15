import sqlite3
from urllib.request import urlopen
import time
import json

conn = sqlite3.connect("sensor_data.db")
cursor = conn.cursor()

cursor.execute("""
CREATE TABLE IF NOT EXISTS sensor_data (
    id INTEGER PRIMARY KEY AUTOINCREMENT,
    timestamp DATETIME DEFAULT CURRENT_TIMESTAMP,
    time TEXT NOT NULL,
    luminosity REAL NOT NULL,
    temperature REAL NOT NULL,
    pressure REAL NOT NULL,
    humidity REAL NOT NULL
);
""")
conn.commit()
