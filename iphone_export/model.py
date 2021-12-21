"""
Created on 2021/12/20

@author: Yano, Takayuki
"""
import global_variables as g


class IPhoneExport(object):
    """
    classdocs
    """

    EXPORT_TYPE_MUSIC = 1
    EXPORT_TYPE_VIDEO = 2
    EXPORT_TYPE_PICTURE = 4

    def __init__(self):
        """
        Constructor
        """

    def get_music_list(self):
        # ライブラリの一覧取得をcall
        # val = g.dll.get_ripping_progress_value()
        # print(str(val))
        return ["abc", "def"]

    def test(self, arg):
        print("" + arg)

    def setCallbackOnDeviceStatusChanged(self, callback):
        self.st_callback = callback

    def setCallbackProgress(self, callback):
        self.pg_callback = callback

    def export_video(self, output_dir):
        #
        self.test("video")

    def export_picture(self, output_dir):
        #
        self.test("picture")

    def export_music(self, output_dir):
        #
        self.test("music")

    def export_specific(self, dst, src):
        self.text("dst:" + dst + " src:" + src)

    def export(self, output_dir, kind: int, src=""):
        if kind & self.EXPORT_TYPE_MUSIC == self.EXPORT_TYPE_MUSIC:
            self.export_music(output_dir + "/music")
        elif kind & self.EXPORT_TYPE_VIDEO == self.EXPORT_TYPE_VIDEO:
            self.export_music(output_dir + "/video")
        elif kind & self.EXPORT_TYPE_PICTURE == self.EXPORT_TYPE_PICTURE:
            self.export_music(output_dir + "/picture")
        else:
            self.export_specific(output_dir, src)
