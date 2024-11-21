def find_duplicate_lines(file_path):
    try:
        # 用于存储已经读取的行
        lines_seen = set()
        # 用于存储重复的行
        duplicates = set()

        # 打开文件并逐行读取
        with open(file_path, 'r', encoding='utf-8') as file:
            for line in file:
                line = line.strip()  # 去掉行首和行尾的空格或换行符
                if line in lines_seen:
                    duplicates.add(line)
                else:
                    lines_seen.add(line)

        # 输出结果
        if duplicates:
            print("Duplicate lines found:")
            for dup in duplicates:
                print(f"- {dup}")
        else:
            print("No duplicate lines found.")
    except FileNotFoundError:
        print(f"Error: File '{file_path}' not found.")
    except Exception as e:
        print(f"An error occurred: {e}")

# 调用函数
if __name__ == "__main__":
    # 替换为你的文件路径
    file_path = "../log.txt"
    find_duplicate_lines(file_path)