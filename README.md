# python_tkinter

## oss
libimobiledevice under GNU Affero General Public License is distributed from
<https://libimobiledevice.org>

## 翻訳
python3 extra/i18n/pygettext.py -d messages -p locale python_tkinter.py iphone_export/ui.py

python3 extra/i18n/msgfmt.py locale/ja_JP/LC_MESSAGES/messages.po

## フォーマッタ
for a in `find . -name "*.py"`; do autopep8 -i $a; done

## 静的解析
flake8 --exclude=./extra --statistics
