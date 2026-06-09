import os
import json
import glob

def generate_json():
    images = []
    # 遍历当前目录下的所有PNG文件
    for png_file in glob.glob("*.png"):
        # 获取文件名（不包括扩展名）
        name = os.path.splitext(png_file)[0].lower().replace(" ", "-")
        # 构建路径
        path = f"A:./assets/images/{png_file}"
        # 添加到列表
        images.append({"name": name, "path": path})

    # 构建最终的JSON结构
    json_data = {"images": images}

    # 打印JSON
    print(json.dumps(json_data, indent=4))

    # 可选：将JSON保存到文件
    with open("output.json", "w") as f:
        json.dump(json_data, f, indent=4)

if __name__ == "__main__":
    generate_json()
