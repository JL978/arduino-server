from flask import Flask
import serial
import time

app = Flask(__name__)

# Open the serial port
ser = serial.Serial("/dev/ttyACM0", 9600, timeout=5)


@app.route("/shocker/on")
def led_on():
    # Send a command to turn the LED on
    ser.write(b"1\n")
    input_str = ser.readline().decode("utf-8").strip()
    print(input_str)
    return "LED turned on"


@app.route("/shocker/off")
def led_off():
    # Send a command to turn the LED off
    ser.write(b"0\n")
    input_str = ser.readline().decode("utf-8").strip()
    print(input_str)
    return "LED turned off"


if __name__ == "__main__":
    # Wait for the serial connection to initialize
    time.sleep(2)

    # Run the Flask server
    app.run(host="0.0.0.0", port=5000)

    # Close the serial port when the server is stopped
    ser.close()
