import subprocess
from pathlib import Path

root = Path(__file__).parent    

def build():
    # shell lets us not have separate shells per call
    subprocess.call("C:\\Program Files (x86)\\Microsoft Visual Studio 8\\VC\\vcvarsall.bat", shell=True)
    subprocess.call(['msbuild', "RBXGSdecomp.sln", "/p:Configuration=Release"], shell=True, cwd=root)

if __name__ == "__main__":
    build()