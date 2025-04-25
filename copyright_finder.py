import os

search_text = '<p class="sphinxhide" align="center"><sup><a href="https://www.amd.com/en/corporate/copyright">Terms and Conditions</a></sup></p>'
markdown_extensions = {".md", ".markdown", ".mkd", ".mkdn", ".mdown"}
file_list = []

def search_files(path):
    for root, dirs, files in os.walk(path):
        for file in files:
            if os.path.splitext(file)[1].lower() in markdown_extensions:
                file_path = os.path.join(root, file)
                with open(file_path, encoding='utf-8') as f:
                    contents = f.read()
                    if search_text not in contents:
                        file_list.append(file_path)
                print(f"Processed file: {file_path}")

search_path = os.getcwd()
search_files(search_path)

# Save the file names and paths in a text file
with open("files_missing_copyright.txt", "w", encoding="utf-8") as output:
    for filepath in file_list:
        output.write(filepath + "\n")

print("Search completed.")

input("Press Enter to close the script.")