import json
from pathlib import Path

root = Path(__file__).parent    

def configure():
    targetPath = root / "RBXGS"
    basePath = root / "Client/App/Release"

    # Error names could be clearer
    if not targetPath.is_dir():
        print("No targets to generate objdiff.json from.")
        return
    
    config = {
        "$schema": "https://raw.githubusercontent.com/encounter/objdiff/main/config.schema.json",
        "custom_make": "python3", 
        "custom_args": ["./build.py"],
        "build_target": False,
        "build_base": True,
        "watch_patterns": [
            "*.c",
            "*.cpp",
            "*.h",
            "*.hpp",
        ],
        "units": [],
    }
    
    with open("objdiff.json", "w", encoding='utf-8') as file:
        for i, targetObj in enumerate(targetPath.iterdir()):
            objFilename = targetObj.name

            config["units"].append({
                "name": objFilename.split(".")[0],
                "target_path": str(targetPath / objFilename)
            })

            if basePath.joinpath(objFilename).is_file():
                config["units"][i].update({
                    "base_path": str(basePath / objFilename)
                })

        json.dump(config, file, indent=4)            

if __name__ == "__main__":
    configure()