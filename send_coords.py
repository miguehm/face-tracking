import serial
import time

def send_coords(x_axis, y_axis):

    print(f"X: {x_axis}, Y: {y_axis}")

    # Abre la conexión con el Arduino
    arduino = serial.Serial("COM3", 9600)
    time.sleep(2)  # Espera a que el puerto serie esté listo

    # Envia la coordenada x
    arduino.write(b'x')
    if x_axis < 0:
        arduino.write(b'n')
    x_axis = abs(x_axis)
    num_binary = bin(x_axis)[2:]  # Convert blink to binary
    print(f"x_axis binary value: {num_binary}")
    arduino.write(num_binary.encode())  # Send binary value to Arduino
    arduino.write(b'f')

    # Envia la coordenada y
    arduino.write(b'y')
    if y_axis < 0:
        arduino.write(b'n')
    y_axis = abs(y_axis)
    num_binary = bin(y_axis)[2:]  # Convert blink to binary
    print(f"y_axis binary value: {num_binary}")
    arduino.write(num_binary.encode())  # Send binary value to Arduino
    arduino.write(b'f')

    arduino.write(b'r') # coordinates ready

    # Cierra la conexión
    arduino.close()

if __name__ == '__main__':
    send_coords(-11, 13)