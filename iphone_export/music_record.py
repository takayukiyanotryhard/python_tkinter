from enum import IntEnum, auto
import tkinter as tk


class MusicRecord:
    def __init__(self):
        self.value = [None for x in range(int(col.MAX_ITEM))]
        self.need_export = tk.BooleanVar(False)

    # Todo: Null対策は外でも出来るので
    # 一旦よく使う、かつ可能性の比較的高いものだけ実装

    @property
    def title(self):
        # タグデータが存在しない場合はファイル名で代替とする
        title = self.remove_null_value(col.TITLE)
        file_name = self.remove_null_value(col.FILE_NAME)
        if (title == "") and (file_name != ""):
            title = file_name
        return title

    @property
    def album(self):
        return self.remove_null_value(col.ALBUM)

    @property
    def album_artist(self):
        # 交互に呼ぶとスタックオーバーフローになる可能性があるのでそれぞれ取ってくる
        # 曲毎だけ入っていてアルバムのアーティスト情報が無い場合がある
        album_artist = self.remove_null_value(col.ALBUM_ARTIST)
        artist = self.remove_null_value(col.ARTIST)
        if (album_artist == "") and (artist != "") :
            album_artist = artist
        return album_artist

    @property
    def artist(self):
        # 曲毎のアーティスト情報はアルバムアーティストで代替する
        artist = self.remove_null_value(col.ARTIST)
        album_artist = self.remove_null_value(col.ALBUM_ARTIST)
        if (artist == "") and (album_artist != ""):
            artist = album_artist
        return artist

    def remove_null_value(self, c):
        s = self.value[int(c)]
        return s if s != "(null)" else ""


class col(IntEnum):
    """
    並び順はdllの仕様と一致させること
    """
    ITEM_PID = 0
    ALBUM = auto()
    ALBUM_ARTIST = auto()
    GENRE = auto()
    TRACK_NO = auto()
    TTL_TRACK = auto()
    DISC_NO = auto()
    TITLE = auto()
    ARTIST = auto()
    YEAR = auto()
    DIRECTORY = auto()
    FILE_NAME = auto()
    FILE_SIZE = auto()
    COMPOSER = auto()
    ART_PATH = auto()
    MAX_ITEM = auto()
