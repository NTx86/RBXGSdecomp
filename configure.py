import json
from pathlib import Path

root = Path(__file__).parent    

def configure():
    targetPath = root.joinpath("RBXGS")
    basePath = root.joinpath("Client/App/Release")

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

    units = config["units"]
    
    for targetObj in targetPath.iterdir():
        name = targetObj.name
        targetName = targetObj.name.split(".")[0]

        units.append({
            "name": targetName,
            "target_path": str(targetPath.joinpath(name)),
            "base_path": str(basePath.joinpath(name)),
        })

    with open(root.joinpath("objdiff.json"), "w") as file:
        file.write(json.dumps(config, indent=4))

if __name__ == "__main__":
    configure()