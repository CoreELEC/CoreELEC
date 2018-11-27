
import json
import codecs
import urllib.error
import urllib.request
import configparser



import locale


def getpreferredencoding(do_setlocale = True):
    return "utf-8"


locale.getpreferredencoding = getpreferredencoding
# Загружаем настройки из файла


class MyConfigParser(configparser.RawConfigParser):
    def get(self, section, option):
        val = configparser.RawConfigParser.get(self, section, option)
        return val.strip('"')


if __name__ == "__main__":
    conf = MyConfigParser()

conf.read("/storage/.config/acelist/settings.conf")
ace_ip = conf.get("settings", "ip")
ace_port = conf.get("settings", "port")
pomoyka_url = conf.get("settings", "pomoyka_url")

list_path = conf.get("path", "list_path")
fav_path = conf.get("path", "fav_path")
logos_path = conf.get("path", "logos_path")
json_path = conf.get("path", "json_path")


# Класс логитопв
class Logos(object):
    """__init__() functions as the class constructor"""
    def __init__(self, name=None, link=None):
        self.name = name
        self.link = link


# Класс списка каналов
class TtvChannel(object):
    """__init__() functions as the class constructor"""
    def __init__(self, name=None, cat=None, fname=None, cid=None):
        self.name = name
        self.cat = cat
        self.fname = fname
        self.cid = cid


def printRAW(*Text):
    RAWOut = open(1, 'w', encoding='utf8', closefd=False)
    print(*Text, file=RAWOut)
    RAWOut.flush()
    RAWOut.close()


# Append fav file to list
try:
    with codecs.open(fav_path+'fav.txt', 'r', encoding='utf8') as fav:
        fav_list = fav.read().splitlines()
except IOError:
    fav_list = None
    print("Fav file not found")


try:
    with open(logos_path + 'logos.json', 'r', encoding='utf-8') as read_file:
        logos_json = json.load(read_file)

except IOError:
    logos_json = None
    print("Logo file not found")

logos_list = []
for logo in logos_json:
    logos_list.append(Logos(name=logo["name"], link=logo["link"]))

# Download json from pomoyka.win
#try:
#    acelive_json = urllib.request.urlopen("http://"+pomoyka_url+"/trash/ttv-list/acelive.json").read()
#    f = open(json_path + 'acelive.json', 'wb')
#    f.write(acelive_json)
#    f.close()
#except urllib.error.URLError:
#    print('Can`t download')

try:
    with open(json_path + 'acelive.json', 'r', encoding='utf-8') as read_file:
        data = json.load(read_file)
except IOError:
    data = None
    print("File not found")

# Создаем лист объектов и заполняем его
ttv_channel_list = []


# Ищем имя канала в списке избранного
with codecs.open(fav_path + 'fav_full.txt', 'w', encoding='utf-8') as origin_fav:
    for item in data:
        if item["source"] == 'ttv.json':  # Провйдер ttv

            ttv_channel_list.append(TtvChannel(item["name"],item["cat"],item["fname"],item["cid"]))
            origin_fav.write(item['name']+'\n')

def byName_key(item):
    return item.name

sorted_list = sorted(ttv_channel_list, key=byName_key)

with codecs.open(list_path + 'playlist.m3u', 'w', encoding='utf-8') as acelive:
    acelive.write('#EXTM3U\n')
    for ttv in sorted_list:
        if (any(ttv.name in f for f in fav_list)):
            logo_url =  [x.link for x in logos_list if x.name == ttv.name]
            string_logo_url = ''.join(logo_url)

            acelive.write('#EXTINF:-1 group-title='+'"'+ttv.cat+'" tvg-logo="'+string_logo_url+'", '+ttv.name+'\n')
            acelive.write('http://'+ace_ip+':'+ace_port+'/ace/getstream?url=http://91.92.66.82/trash/ttv-list/acelive/'+ttv.fname + '\n')
print('AceLive Playlist successfully created')





