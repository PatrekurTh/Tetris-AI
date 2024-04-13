import cv2 as cv
from time import sleep
from utils.window_capture import WindowCapture
import numpy as np
import pyautogui as pyg
from build2.Release.agents import Action, call_agent_get_best_move, MCTSAgent, NAgent

pyg.PAUSE = 0.005 #! If your computer/network is slow, increase this value. If it's fast, set to 0

END_COLOR = np.array([130, 130, 130])
BOARD_COLOR = np.array([57, 57, 57])

# MAC (BGR)
# PIECE_COLORS = {
#     "I": [190, 158, 110],
#     "J": [187, 99, 95],
#     "L": [101, 140, 185],
#     "O": [108, 193, 193],
#     "S": [106, 191, 129],
#     "T": [188, 98, 167],
#     "Z": [110, 89, 203],
#     "TRASH": [187, 187, 187]
# }

# WINDOWS (BGR)
PIECE_COLORS = {
    "I": [193, 160, 94],
    "J": [193, 99, 94],
    "L": [94, 138, 193],
    "O": [94, 193, 193],
    "S": [94, 193, 109],
    "T": [193, 94, 178],
    "Z": [109, 79, 219],
    "TRASH": [187, 187, 187]
}

# MAC
# BUTTONS = {
#     Action.LEFT: "left",
#     Action.RIGHT: "right",
#     Action.CCW: "s", # Rotate left
#     Action.CW: "d", # Rotate right
#     Action.HOLD: "a",
#     Action.NONE: "down",
#     "DROP": "space",
# }

# WINDOWS
BUTTONS = {
    Action.LEFT: "j",
    Action.RIGHT: "l",
    Action.CCW: "s", # Rotate left
    Action.CW: "d", # Rotate right
    Action.HOLD: "a",
    Action.NONE: "k",
    "DROP": "space",
}

def set_current_piece(screenshot, board_x, board_y, block_width, block_height):
    top_area_height = block_height
    for key in PIECE_COLORS:
        if (key == "TRASH"):
            continue
        bgr_color = np.array(PIECE_COLORS[key])
        mask = cv.inRange(screenshot, bgr_color, bgr_color)
        contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            x, y, w, h = cv.boundingRect(cnt)

            if (
                board_y <= y <= board_y + top_area_height
                and board_x <= x <= board_x + block_width * 10
            ):
                return key
    return None


def set_hold_piece(screenshot, board_x):
    for key in PIECE_COLORS:
        if (key == "TRASH"):
            continue
        bgr_color = np.array(PIECE_COLORS[key])
        mask = cv.inRange(screenshot, bgr_color, bgr_color)
        contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            x, y, w, h = cv.boundingRect(cnt)

            if x < board_x:
                return key
    return None


def set_queue(screenshot, board_x, board_w):
    pieces = []
    y_values = []
    for key in PIECE_COLORS:
        if (key == "TRASH"):
            continue
        bgr_color = np.array(PIECE_COLORS[key])
        mask = cv.inRange(screenshot, bgr_color, bgr_color)
        contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)

        for cnt in contours:
            x, y, w, h = cv.boundingRect(cnt)
            if x > (board_x + board_w):
                pieces.append(key)
                y_values.append(y)

    for i in range(len(pieces) - 1, 0, -1):
        if i % 4 != 0:
            pieces.pop(i)
            y_values.pop(i)

    return [x for _, x in sorted(zip(y_values, pieces), reverse=True)]


def check_end(screenshot, board_x, board_y, board_w, block_height):
    bgr_color = END_COLOR
    mask = cv.inRange(screenshot, bgr_color, bgr_color)

    contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    for cnt in contours:
        x, y, w, h = cv.boundingRect(cnt)
        if board_y <= y <= board_y + (block_height * 20) and board_x <= x <= board_x + board_w:
            return True
    return False

def process_screenshot(screenshot, board_x, board_y, board_w, block_width, block_height):
    piece_positions = {'current': None, 'hold': None, 'queue': []}
    piece_contours = {key: [] for key in PIECE_COLORS}

    for key, color in PIECE_COLORS.items():
        if key == "TRASH": continue
        bgr_color = np.array(color)
        mask = cv.inRange(screenshot, bgr_color, bgr_color)
        contours, _ = cv.findContours(mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
        piece_contours[key].extend(contours)

    for key, contours in piece_contours.items():
        for cnt in contours:
            x, y, w, h = cv.boundingRect(cnt)

            if board_y <= y <= board_y + block_height and (board_x + block_width * 3) <= x <= board_x + block_width * 7:
                piece_positions['current'] = key
            
            elif x < board_x:
                piece_positions['hold'] = key
            
            elif x > (board_x + board_w):
                piece_positions['queue'].append((y, key))
    
    for i in range(len(piece_positions["queue"]) - 1, 0, -1):
        if i % 4 != 0:
            piece_positions["queue"].pop(i)
    piece_positions['queue'] = [x[1] for x in sorted(piece_positions['queue'], key=lambda x: x[0], reverse=True)]

    return piece_positions

def get_board_representation(screenshot, board_x, board_y, board_w, board_h, block_width, block_height):
    board_img = screenshot[board_y:board_y+board_h, board_x:board_x+board_w]    
    board_matrix = np.zeros((20, 10), dtype=np.uint8)
    black_threshold = 50
    
    for row in range(20):
        for col in range(10):
            cell_x1 = col * block_width
            cell_y1 = row * block_height
            cell_x2 = cell_x1 + block_width
            cell_y2 = cell_y1 + block_height
            
            cell_img = board_img[cell_y1:cell_y2, cell_x1:cell_x2]
            
            avg_color = np.mean(cell_img, axis=(0, 1))
            
            if np.all(avg_color <= black_threshold):
                board_matrix[row, col] = 0
            else:
                board_matrix[row, col] = 1
    
    board_matrix[:1, :] = 0
    return board_matrix




def main(agent):
    print("Initializing...")
    wincap = WindowCapture("Jstris")

    screenshot = wincap.get_image_from_window()
    rows, cols, _ = screenshot.shape

    print("Setting up board...")
    board_mask = cv.inRange(screenshot, BOARD_COLOR, BOARD_COLOR)
    contours, _ = cv.findContours(board_mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=cv.contourArea, reverse=True)

    cnt = contours[0]
    x1, y1, w1, h1 = cv.boundingRect(cnt)

    # ? Sometimes the piece is at the top of the board and cuts the contour giving an imcomplete board
    print("Waiting for board to be complete...")
    sleep(3)
    screenshot = wincap.get_image_from_window()

    board_mask = cv.inRange(screenshot, BOARD_COLOR, BOARD_COLOR)
    contours, _ = cv.findContours(board_mask, cv.RETR_EXTERNAL, cv.CHAIN_APPROX_SIMPLE)
    contours = sorted(contours, key=cv.contourArea, reverse=True)

    cnt = contours[0]
    x2, board_y, w2, board_h = cv.boundingRect(cnt)
    board_x = min(x1, x2)
    board_w = max(w1, w2)

    block_width = board_w // 10
    block_height = board_h // 20

    hold_used = False


    print("Board Setup Complete.")
    while True:

        screenshot = wincap.get_image_from_window()

        piece_positions = process_screenshot(screenshot, board_x, board_y, board_w, block_width, block_height)
        board = get_board_representation(screenshot, board_x, board_y, board_w, board_h, block_width, block_height)

        if piece_positions['current'] is None:
            continue
        if piece_positions['hold'] is None:
            piece_positions['hold'] = 'X'

        best_move = call_agent_get_best_move(agent, board, piece_positions['current'], piece_positions['hold'], piece_positions['queue'], hold_used)
        for move in best_move:
            pyg.press(BUTTONS[Action(move)])
            pyg.keyUp("fn")
        if (Action.HOLD in best_move):
            hold_used = True
        else:
            hold_used = False
            pyg.press(BUTTONS["DROP"])


if __name__ == "__main__":
    agent = NAgent(2)
    main(agent)
