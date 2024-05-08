import cv2

face_cascade = cv2.CascadeClassifier('haarcascade_frontalface_default.xml')

img = cv2.imread('test.jpg')
gray = cv2.cvtColor(img, cv2.COLOR_BGR2GRAY)

# Imprimir el tamaño del canvas
height, width, _ = img.shape
print(f"El tamaño del canvas es {width}x{height}")

faces = face_cascade.detectMultiScale(gray, 1.1, 4)

print(faces)

for (x, y, w, h) in faces:
    cv2.rectangle(img, (x, y), (x+w, y+h), (255, 0, 0), 2)
    print(f"Rostro detectado en la posición x={x}, y={y} con ancho={w} y alto={h}")

# Mostrar la imagen con los rostros detectados
cv2.imshow('Rostros Detectados', img)

# Esperar a que el usuario cierre la ventana antes de terminar el programa
cv2.waitKey(0)
cv2.destroyAllWindows()