import serial

# Establecer la conexión serial con la placa Arduino
arduino = serial.Serial('COM3', 9600)  # Reemplaza 'COM3' con el puerto serial correcto

# Enviar datos a la placa Arduino
data = 'Hola Arduino!'
arduino.write(data.encode())

# Cerrar la conexión serial
arduino.close()