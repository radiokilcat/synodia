import os

# Set the base directory to traverse
base_dir = "../apps/helicopter/assets/images/synodia/lady"

result = {}

for root, dirs, files in os.walk(base_dir):
    for file in files:
        if file.lower().endswith('.png'):
            rel_dir = os.path.relpath(root, base_dir)
            file_name_no_ext = os.path.splitext(file)[0].lower()

            if rel_dir == ".":
                key = f"lady_{file_name_no_ext}"
            else:
                norm_dir = rel_dir.replace(os.sep, "_").lower()
                if file_name_no_ext.startswith(norm_dir):
                    key = f"lady_{file_name_no_ext}"
                else:
                    key = f"lady_{norm_dir}_{file_name_no_ext}"

            file_path = os.path.join(root, file).replace("\\", "/")
            if not file_path.startswith("."):
                file_path = "./" + file_path

            result[key] = file_path

for k, v in result.items():
    print(f'"{k}": "{v}",')
