from send_coords import send_coords
from face_detection import face_detection

def main():
    error_x, error_y = face_detection()
    send_coords(error_x, error_y)

if __name__ == '__main__':
    main()