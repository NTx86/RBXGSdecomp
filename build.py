# TODO: find a cross platform way to do this
import subprocess
from pathlib import Path
import configure

root = Path(__file__).parent

def build():
    configure()
    
    # shell lets us not have separate shells per call
    subprocess.run("C:\\Program Files (x86)\\Microsoft Visual Studio 8\\VC\\vcvarsall.bat", shell=True)
    subprocess.run(['msbuild', "RBXGSdecomp.sln", "/p:Configuration=Release"], shell=True, cwd=root)

if __name__ == "__main__":
    build()