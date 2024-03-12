import os

# 递归的删除目录下的文件
def remove_all_files(path):
    for dpath, dnames, fnames in os.walk(path, topdown=False):
        for d in dnames:
            os.remove(os.path.join(dpath, d))
        for f in fnames:
            os.remove(os.path.join(dpath, f))


for dirpath, dirnames, filenames in os.walk(os.getcwd()):
    for d in dirnames:
        if d in {"Objects", "Listings"}:
            remove_all_files(os.path.join(dirpath, d))
            print(os.path.join(dirpath, d) + " is deleted.")