import cv2

def face_detection(): # modify to use a camera
    face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

    img = cv2.imread('image2.jpg')
    gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

    # Imprimir el tamaño del canvas
    height, width, _ = img.shape
    print(f"El tamaño del canvas es {width}x{height}")

    faces = face_cascade.detectMultiScale(gray, 1.1, 4)

    print(faces)

    # Dibuja un rectangulo en el primer rostro en faces
    image_center_x = width // 2
    image_center_y = height // 2
    print(f"El centro de la imagen es x={image_center_x}, y={image_center_y}")
    center_x = 0
    center_y = 0
    error_x = 0
    error_y = 0

    # Dibuja dos lineas que representen el eje x y el eje y de la imagen
    cv2.line(img, (0, image_center_y), (width, image_center_y), (0, 0, 255), 2)
    cv2.line(img, (image_center_x, 0), (image_center_x, height), (0, 0, 255), 2)

    if len(faces) > 0:
        (x, y, w, h) = faces[0]
        cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
        # Dibuja un punto en el centro del rectángulo
        center_x = x + w // 2
        center_y = y + h // 2
        cv2.circle(img, (center_x, center_y), 5, (0, 255, 0), -1)
        #print(f"Rostro detectado en la posición x={x}, y={y} con ancho={w} y alto={h}")
        
        error_x = -1*(image_center_x - center_x)
        error_y = image_center_y - center_y
        print(f"Rostro en x={error_x}, y={error_y}")

        # Pasando como -1 en el comparador
        error_x = -1*error_x
        error_y = -1*error_y

        print(f"Error en x={error_x}, y={error_y}")
    else:
        print("No se encontraron rostros en la imagen")

    # Mostrar la imagen con los rostros detectados
    cv2.imshow('Rostros Detectados', img)

    # Esperar a que el usuario cierre la ventana antes de terminar el programa
    cv2.waitKey(0)
    cv2.destroyAllWindows()

    return error_x, error_y

if __name__ == "__main__":
    print(face_detection())