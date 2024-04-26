import cv2
import numpy as np
cam = cv2.VideoCapture('C:/Users/Robert/Desktop/ISA/Week9/Week9/Lane_Detection_Test_Video_01.mp4')


def create_trapezoid_mask(frame_shape):

    height, width = frame_shape[:2]

    upper_left = (int(width * 0.47), int(height * 0.74))
    upper_right = (int(width * 0.54), int(height * 0.74))
    lower_left = (int(width * 0.03), int(height * 1))
    lower_right = (int(width * 1), int(height * 1))

    trapezoid_frame = np.zeros((height, width), dtype=np.uint8)

    pts = np.array([upper_left, upper_right, lower_right, lower_left], np.int32)
    cv2.fillConvexPoly(trapezoid_frame, pts, 1)
    return trapezoid_frame, pts

while True:
    ret, frame = cam.read()

    if ret is False:
        break


    #ex2 Shrink the frame

    original_height, original_width = frame.shape[:2]

    new_width = original_width // 3
    new_height = original_height // 3

    resized_frame = cv2.resize(frame, (new_width, new_height))

    #cv2.imshow('Small', resized_frame)
    #cv2.moveWindow('Small', 0, 0)


    #ex3 greyscale

    grayscale_frame = cv2.cvtColor(resized_frame, cv2.COLOR_BGR2GRAY)

    cv2.imshow('Greyscale', grayscale_frame)
    cv2.moveWindow('Greyscale', 425, 0)

    #ex4 select only the road

    trapezoid_mask_255, trapezoid_coordinates = create_trapezoid_mask(grayscale_frame.shape)

    road_only = cv2.bitwise_and(resized_frame, resized_frame, mask=trapezoid_mask_255)
    road_only = cv2.cvtColor(road_only, cv2.COLOR_BGR2GRAY)

    cv2.imshow('Trapezoid', trapezoid_mask_255*255)
    cv2.moveWindow('Trapezoid', 850, 0)
    cv2.imshow('road', road_only)
    cv2.moveWindow('road', 1200, 0)

    #ex5 top-down view

    #a
    screen_corners = np.float32([[0, 0], [new_width, 0], [new_width, new_height], [0, new_height]])
    trapezoid_coordinates = trapezoid_coordinates.astype(np.float32)
    screen_corners = screen_corners.astype(np.float32)

    #b
    perspective_matrix = cv2.getPerspectiveTransform(trapezoid_coordinates, screen_corners)
    #c
    stretched_trapezoid = cv2.warpPerspective(road_only, perspective_matrix, (new_width, new_height))

    cv2.imshow('Top-Down', stretched_trapezoid)
    cv2.moveWindow('Top-Down', 0, 270)

    #ex6 blur

    blurred_trapezoid = cv2.blur(stretched_trapezoid, ksize=(3, 3))

    cv2.imshow('Blur', blurred_trapezoid)
    cv2.moveWindow('Blur', 425, 270)

    #ex7 edge detection

    #a
    sobel_vertical = np.array([[-1, 0, 1],
                               [-2, 0, 2],
                               [-1, 0, 1]], dtype=np.float32)

    sobel_horizontal = np.transpose(sobel_vertical)

    #b
    blurred_trapezoid_float32 = blurred_trapezoid.astype(np.float32)

    sobel_vertical_result = cv2.filter2D(blurred_trapezoid_float32, -1, sobel_vertical)
    sobel_horizontal_result = cv2.filter2D(blurred_trapezoid_float32, -1, sobel_horizontal)

    sobel_combined = np.sqrt(np.square(sobel_vertical_result) + np.square(sobel_horizontal_result))

    sobel_combined = cv2.convertScaleAbs(sobel_combined)

    cv2.imshow('Sobel', sobel_combined)
    cv2.moveWindow('Sobel', 850, 270)

    #ex8 binarize the image

    threshold_value = 130
    ret_Val, binary_frame = cv2.threshold(sobel_combined, threshold_value, 255, cv2.THRESH_BINARY)

    cv2.imshow('Binarized frame', binary_frame)
    cv2.moveWindow('Binarized frame', 0, 540)

    #ex9 get coordinates

    frame_copy = binary_frame.copy()

    width = frame_copy.shape[1]

    num_columns_5_percent = int(width * 0.12)

    frame_copy[:, :num_columns_5_percent] = 0

    frame_copy[:, -num_columns_5_percent:] = 0

    left_half = frame_copy[:, :frame_copy.shape[1] // 2]
    right_half = frame_copy[:, frame_copy.shape[1] // 2:]

    left_half_float = left_half.astype(np.float32)
    right_half_float = right_half.astype(np.float32)

    left_coordinates = np.argwhere(left_half_float > 1)
    right_coordinates = np.argwhere(right_half_float > 1)

    right_coordinates[:, 1] += frame_copy.shape[1] // 2

    left_xs, left_ys = left_coordinates[:, 1], left_coordinates[:, 0]
    right_xs, right_ys = right_coordinates[:, 1], right_coordinates[:, 0]


    #ex10 find lines, detect edges of line

    #a
    left_coefficients = np.polynomial.polynomial.polyfit(left_xs, left_ys, deg=1)

    right_coefficients = np.polynomial.polynomial.polyfit(right_xs, right_ys, deg=1)

    #b
    height = frame_copy.shape[0]
    left_top_y = 0
    left_bottom_y = height - 1

    left_top_x = int((left_top_y - left_coefficients[0]) / left_coefficients[1])
    left_bottom_x = int((left_bottom_y - left_coefficients[0]) / left_coefficients[1])


    right_top_y = 0
    right_bottom_y = height - 1

    right_top_x = int((right_top_y - right_coefficients[0]) / right_coefficients[1])
    right_bottom_x = int((right_bottom_y - right_coefficients[0]) / right_coefficients[1])


    #d
    left_top = int(left_top_x), int(left_top_y)
    left_bottom = int(left_bottom_x) , int(left_bottom_y)

    right_top = int(right_top_x), int(right_top_y)
    right_bottom = int(right_bottom_x), int(right_bottom_y)

    cv2.line(frame_copy, left_top , left_bottom, (200, 0, 0), 2)
    cv2.line(frame_copy, right_top, right_bottom, (100, 0, 0), 2)

    cv2.imshow('Lines', frame_copy)
    cv2.moveWindow('Lines', 1200, 270)

    #ex.11

    #frame for left

    frame_copy_height, frame_copy_width = frame_copy.shape[:2]

    blank_frame_left = np.zeros_like(frame_copy)

    cv2.line(blank_frame_left, left_top, left_bottom, (255, 0, 0), 3)

    perspective_matrix1 = cv2.getPerspectiveTransform(screen_corners, trapezoid_coordinates)

    left_piece = cv2.warpPerspective(blank_frame_left, perspective_matrix1, (new_width, new_height))

    left_piece_copy = left_piece.copy()

    ONLY_left_piece = left_piece_copy[:, :left_piece_copy.shape[1] // 2]

    ONLY_left_piece_float = ONLY_left_piece.astype(np.float32)

    left_piece_coordinates = np.argwhere(ONLY_left_piece_float > 1)

    left_piece_xs, left_piece_ys = left_piece_coordinates[:, 1], left_piece_coordinates[:, 0]

    cv2.imshow('Left', blank_frame_left)
    cv2.moveWindow('Left', 425, 540)

    #frame for right

    blank_frame_right = np.zeros_like(frame_copy)

    cv2.line(blank_frame_right, right_top, right_bottom, (255, 0, 0), 3)

    perspective_matrix2 = cv2.getPerspectiveTransform(screen_corners, trapezoid_coordinates)

    right_piece = cv2.warpPerspective(blank_frame_right, perspective_matrix2, (new_width, new_height))

    right_piece_copy = right_piece.copy()

    ONLY_right_piece = right_piece_copy[:, right_piece_copy.shape[1] // 2:]

    ONLY_right_piece_float = ONLY_right_piece.astype(np.float32)

    right_piece_coordinates = np.argwhere(ONLY_right_piece_float > 1)

    right_piece_coordinates[:, 1] += right_piece_copy.shape[1] // 2

    right_piece_xs, right_piece_ys = right_piece_coordinates[:, 1], right_piece_coordinates[:, 0]

    cv2.imshow('Right', blank_frame_right)
    cv2.moveWindow('Right', 850, 540)

    # combine left and right

    final_result = resized_frame.copy()

    color_left = (250, 50, 50)
    color_right = (50, 250, 50)

    for x, y in zip(right_piece_xs, right_piece_ys):
        final_result[y, x] = color_right

    for x, y in zip(left_piece_xs, left_piece_ys):
        final_result[y, x] = color_left

    cv2.imshow('Final', final_result)
    cv2.moveWindow('Final', 0, 0)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cam.release()
cv2.destroyAllWindows()
