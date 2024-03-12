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
        if d in {"Objects", "Listings", ".vscode"}:
            remove_all_files(os.path.join(dirpath, d))  # 递归的删除目标目录下的所有文件
            os.removedirs(os.path.join(dirpath, d))     # 删除文件夹
            print(os.path.join(dirpath, d),"和其下的所有文件都被删除")
    for f in filenames:
        if f.split('.')[-1] in {"scvd", "yadon", "uvoptx"}:
            os.remove(os.path.join(dirpath, f))  # 删除指定拓展名的文件
            print(os.path.join(dirpath, f),"被删除")
            