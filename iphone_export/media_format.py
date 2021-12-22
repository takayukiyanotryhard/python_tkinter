from ctypes import *
import iphone_export.music_record as music


def get_music_list(pp, column_num):
    retval = []
    print("ppvalue:" + str(pp.value) + " col:" + str(column_num))
    if (pp.value is None) or (column_num <= 0):
        return retval

    out_data = cast(pp, POINTER(c_char_p))

    count = 0
    flag = True
    while(flag):
        record = music.MusicRecord()
        for i in range(column_num):
            p = cast(out_data[count], c_char_p)
            if p.value is None:
                flag = False
                break
            # バイト配列を一回変数に落とさないと自動変換が走ってしまう
            b = p.value
            record.value[i] = b.decode()
            print(str(i) + ":" + record.value[i])
            count = count + 1
        if flag:
            retval.append(record)

    return retval
