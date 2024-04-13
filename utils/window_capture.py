import platform
import numpy as np

if platform.system() == "Darwin":
    import Quartz as QZ

    class WindowCapture:
        window_name = None
        window = None
        window_id = None
        window_width = 0
        window_height = 0

        def __init__(self, given_window_name=None):
            if given_window_name is not None:

                self.window_name = given_window_name
                self.window = self.get_window()

                if self.window is None:
                    raise Exception("Unable to find window: {}".format(given_window_name))

                self.window_id = self.get_window_id()

                self.window_width = self.get_window_width()
                self.window_height = self.get_window_height()

                self.window_x = self.get_window_pos_x()
                self.window_y = self.get_window_pos_y()
            else:
                raise Exception("No window name given")

        def get_window(self):
            windows = QZ.CGWindowListCopyWindowInfo(
                QZ.kCGWindowListOptionAll, QZ.kCGNullWindowID
            )
            for window in windows:
                name = window.get("kCGWindowName", "Unknown")
                if name and self.window_name in name:

                    return window
            return None

        def get_window_id(self):
            return self.window["kCGWindowNumber"]

        def get_window_width(self):
            return int(self.window["kCGWindowBounds"]["Width"])

        def get_window_height(self):
            return int(self.window["kCGWindowBounds"]["Height"])

        def get_window_pos_x(self):
            return int(self.window["kCGWindowBounds"]["X"])

        def get_window_pos_y(self):
            return int(self.window["kCGWindowBounds"]["Y"])

        def get_image_from_window(self):
            core_graphics_image = QZ.CGWindowListCreateImage(
                QZ.CGRectNull,
                QZ.kCGWindowListOptionIncludingWindow,
                self.window_id,
                QZ.kCGWindowImageBoundsIgnoreFraming | QZ.kCGWindowImageNominalResolution,
            )

            bytes_per_row = QZ.CGImageGetBytesPerRow(core_graphics_image)
            width = QZ.CGImageGetWidth(core_graphics_image)
            height = QZ.CGImageGetHeight(core_graphics_image)

            core_graphics_data_provider = QZ.CGImageGetDataProvider(core_graphics_image)
            core_graphics_data = QZ.CGDataProviderCopyData(core_graphics_data_provider)

            np_raw_data = np.frombuffer(core_graphics_data, dtype=np.uint8)

            numpy_data = np.lib.stride_tricks.as_strided(
                np_raw_data,
                shape=(height, width, 3),
                strides=(bytes_per_row, 4, 1),
                writeable=False,
            )

            final_output = np.ascontiguousarray(numpy_data, dtype=np.uint8)

            return final_output
elif platform.system() == "Windows":
    import win32gui
    import win32ui
    import win32con
    from ctypes import windll

    class WindowCapture:
        window_name = None
        window = None
        window_id = None
        window_width = 0
        window_height = 0

        def __init__(self, given_window_name=None):
            if given_window_name is not None:
                windll.user32.SetProcessDPIAware()
                self.window_name = given_window_name
                self.get_window()
                if self.window is None:
                    raise Exception("Unable to find window: {}".format(given_window_name))
                
                self.window_width, self.window_height = self.get_window_size()
                self.window_x, self.window_y = self.get_window_position()
            else:
                raise Exception("No window name given")

        def get_window(self):
            def callback(hwnd, _):
                if win32gui.IsWindowVisible(hwnd):
                    if self.window_name in win32gui.GetWindowText(hwnd):
                        self.window = hwnd
            win32gui.EnumWindows(callback, None)

        def get_window_size(self):
            rect = win32gui.GetClientRect(self.window)
            width = rect[2] - rect[0]
            height = rect[3] - rect[1]
            return width, height

        def get_window_position(self):
            rect = win32gui.GetWindowRect(self.window)
            x = rect[0]
            y = rect[1]
            return x, y

        # https://stackoverflow.com/questions/76373625/pywin32-cannot-capture-certain-windows-giving-black-screen-python-windows
        def get_image_from_window(self):
            wDC = win32gui.GetWindowDC(self.window)
            dcObj = win32ui.CreateDCFromHandle(wDC)
            cDC = dcObj.CreateCompatibleDC()

            dataBitMap = win32ui.CreateBitmap()
            dataBitMap.CreateCompatibleBitmap(dcObj, self.window_width, self.window_height)
            cDC.SelectObject(dataBitMap)

            result = windll.user32.PrintWindow(self.window, cDC.GetSafeHdc(), 3)

            bmpinfo = dataBitMap.GetInfo()
            bmpstr = dataBitMap.GetBitmapBits(True)

            img = np.frombuffer(bmpstr, dtype=np.uint8).reshape((bmpinfo["bmHeight"], bmpinfo["bmWidth"], 4))
            img = np.ascontiguousarray(img)[..., :-1]

            if not result:
                win32gui.DeleteObject(dataBitMap.GetHandle())
                cDC.DeleteDC()
                dcObj.DeleteDC()
                win32gui.ReleaseDC(self.window, wDC)
                raise RuntimeError(f"Unable to acquire screenshot! Result: {result}")

            return img