import cv2
import numpy as np
import serial
import time

# Detector de rostros con opencv y usando la webcam
def face_detection():
    # Cargamos la plantilla e inicializamos la webcam:
    print("Cargando el clasificador de rostros...")
    face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')
    print("Iniciando la webcam...")
    cap = cv2.VideoCapture(0)

    # Abre la conexión con el Arduino
    print("Abriendo conexión con el Arduino...")
    arduino = serial.Serial("COM5", 9600)
    time.sleep(2)  # Espera a que el puerto serie esté listo

    # Imprime el tamaño del canvas (640x480)
    image_width = int(cap.get(cv2.CAP_PROP_FRAME_WIDTH))
    image_height = int(cap.get(cv2.CAP_PROP_FRAME_HEIGHT))
    print(f"El tamaño del canvas es {image_width}x{image_height}")

    # Define el centro del canvas
    image_center_x = image_width // 2
    image_center_y = image_height // 2

    error_x = 0
    error_y = 0

    while True:
        # Leemos un frame y lo guardamos
        ret, img = cap.read()

        # Espejamos la imagen en el eje Y
        #img = cv2.flip(img, 1)  # El segundo argumento es 1 para espejar horizontalmente

        # espejamos la imagen en el eje X
        img = cv2.flip(img, 1)  # El segundo argumento es 0 para espejar verticalmente

        # dibujamos dos lineas que representen el eje x y el eje y de la imagen
        cv2.line(img, (0, image_center_y), (image_width, image_center_y), (0, 0, 255), 2)
        cv2.line(img, (image_center_x, 0), (image_center_x, image_height), (0, 255, 0), 2)

        # Convertimos la imagen a blanco
        gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

        # Buscamos las coordenadas de los rostros (si los hay) y
        # guardamos su posición
        faces = face_cascade.detectMultiScale(gray, 1.3, 5)
        face = faces[0] if len(faces) > 0 else None

        # Dibujamos un rectángulo en las coordenadas de face
        if face is not None:
            (x, y, w, h) = face
            cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
            # Dibujamos un punto en el centro del rectángulo
            center_x = x + w // 2
            center_y = y + h // 2
            cv2.circle(img, (center_x, center_y), 5, (0, 255, 0), -1)
            #print(f"El centro del rostro es x={center_x}, y={center_y}")

            error_x = -1*(image_center_x - center_x)
            error_y = image_center_y - center_y
            #print(f"Rostro en x={error_x}, y={error_y}")

            # Pasando como -1 en el comparador
            error_x = -1*error_x
            error_y = -1*error_y
            #print(f"Error en x={error_x}, y={error_y}")

        # ---------------------------------
        # Enviar las coordenadas al Arduino
        x_axis = error_x
        y_axis = error_y

        print(f"x_error: {x_axis}, y_error: {y_axis}")

        # Envia la coordenada x
        arduino.write(b'x')
        if x_axis < 0:
            arduino.write(b'n')
        x_axis = abs(x_axis)
        num_binary = bin(x_axis)[2:]
        #print(f"x_axis binary value: {num_binary}")
        arduino.write(num_binary.encode())
        arduino.write(b'f')

        # Envia la coordenada y
        arduino.write(b'y')
        if y_axis < 0:
            arduino.write(b'n')
        y_axis = abs(y_axis)
        num_binary = bin(y_axis)[2:]
        #print(f"y_axis binary value: {num_binary}")
        arduino.write(num_binary.encode())
        arduino.write(b'f')

        arduino.write(b'r')  # coordinates ready
        # ---------------------------------

        # Mostramos la imagen
        cv2.imshow('img', img)

        # Con la tecla 'q' salimos del programa
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break

    # Liberamos la webcam y cerramos todas las ventanas
    cap.release()
    cv2.destroyAllWindows()

    # Cierra la conexión con el Arduino
    arduino.close()

if __name__ == '__main__':
    face_detection()