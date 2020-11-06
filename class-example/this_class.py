# used to list all files in directory
from os import listdir
from os.path import isfile, join

class version:

    def __init__(self, version):
        self.__version = version

    def get_version(self):
        return self.__version

    def set_version(self, version):
        self.__version = version

    def get_version_list(self):
        version_list = [f for f in listdir(mypath) if isfile(join(mypath, f))]
        return version_list