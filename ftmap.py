import os
import re
import docutils
from docutils import parsers
from docutils.parsers import rst
from docutils.parsers.rst import directives
import sphinx
from sphinx import parsers
from bs4 import BeautifulSoup
from xml.dom import minidom
from os import path
from pathlib import Path
import argparse 

ROOT_DIR = os.path.abspath(os.curdir)
ROOT_DIR = str(ROOT_DIR).replace("\\", "/")
print(ROOT_DIR)



#handled_rst_filelist
handled_rst_filelist = []
#tranverse index
root = minidom.Document()
output_file = "index.ftmap"
ftmap = root.createElement('ft:map') 
root.appendChild(ftmap)
ftmap.setAttribute('ft:editorialType', 'book')
ftmap.setAttribute('ft:lang', 'en-US')
ftmap.setAttribute('ft:originID', "TechDocs")
ftmap.setAttribute('ft:title', "TechDocs")
ftmap.setAttribute('xmlns:ft', 'http://ref.fluidtopics.com/v3/ft#')
ftmap.setAttribute('xmlns:xsi', 'http://www.w3.org/2001/XMLSchema-instance')
tocChild = root.createElement('ft:toc')
  
ftmap.appendChild(tocChild)

def createNode(href, title):
    ftnode = root.createElement('ft:node')
    ftnode.setAttribute('href', href)
    ftnode.setAttribute('ft:title', title)
    ftnode.setAttribute('type', "topics")
    return ftnode    

  
def createExternalMeta():
    metas = root.createElement('ft:metas')
    meta1 = root.createElement('ft:meta')
    meta1.setAttribute('key', 'Group')
    meta1.appendChild(root.createTextNode('test'))
    
    metas.appendChild(meta1)
    #metas.appendChild(meta2)
    # metas.appendChild(meta3)

    return metas  

ftmap.appendChild(createExternalMeta())
 
def check_if_string_in_file(file_name, title, parentnode, prefix=""):
    """ Check if any line in the file contains given string """
    # Open the file in read only mode

    if prefix.startswith('/') or prefix.startswith('\\'):
        prefix = prefix[1:]
    if prefix == "":
        href = file_name
    else:
        href = prefix + '/' + file_name
    if href in handled_rst_filelist:
        return
    else:
        handled_rst_filelist.append(href)
    #deal with html
    html_file_name = file_name.split(".")[0]
    if prefix == "":
        relative_html_path = html_file_name + ".html"
    else:
        relative_html_path = prefix + "/" + html_file_name + ".html"
    print("file name: " + href)


    node = createNode(relative_html_path, title)

    parentnode.appendChild(node)     
    if parentnode != tocChild:
        parentnode.setAttribute('type', "topic")
    is_rst_exists = os.path.exists(href)
    if is_rst_exists == False:
        return
    with open(href, 'r', encoding="UTF-8", errors='ignore') as read_obj:
        # Read all lines in the file one by one
        current_path =Path(read_obj.name).parent.absolute()
        print(current_path)
        # current_path = str(current_path).replace('\\','/')
        data = read_obj.readlines()
        sub_files = []
        i =0;
        while i<len(data):
            line = data[i]  
            m = re.search(r'\.\. *([^:]+):: *(.*)', line)
            if m:
               
                if "toctree" in m.group():
                    j=i+1;
                    while j<len(data):
                        sub_line=data[j]
                        indent = len(re.findall("^ *", sub_line)[0])

                        if indent ==0 and len(sub_line) >1:
                            break;
                        else:
                            if ".md" in sub_line:
                                sub_files.append(sub_line.strip())
                            j+=1
                    i=j;
                else:
                    i+=1
            else:
                i+=1
        for sub_file in sub_files:
            if "http" in sub_file:
                continue
            if ".md" not in sub_file:
                sub_file = sub_file + ".md" 
            if sub_file.startswith("./"):
                sub_file = sub_file[2:]
            if sub_file.startswith("/"):
                sub_file = sub_file[1:]
            line = sub_file.split('/')
            if len(line) == 1:
                if not line[0].startswith('../'):
                    check_if_string_in_file(line[0], sub_file.replace(".md",""), node, prefix)
                else:
                    prefix2 = ""

                    while line[0].startswith('../'):
                        x = x[1:]
                        prefix2 = "/".join(x)
                        parentpath = current_path.parent
            else:
                x=line[:-1]
                y=line[-1]
                prefix2 = ""
                if x:
                    prefix2 = "/".join(x)
                if prefix2 != '.':
                    #prefix = prefix2
                    parentpath = current_path
                    while prefix2.startswith('..'):
                        x = x[1:]
                        prefix2 = "/".join(x)
                        parentpath = parentpath.parent
                    print("prefix2:: " + prefix2)
                    print("parentpath:: " + str(parentpath))
                    abspath = str(parentpath) + "/" +prefix2

                else:
                    abspath = str(ROOT_DIR) + "/" +prefix2
                abspath = abspath.replace("\\", "/")
                if abspath.endswith("/"):
                    abspath = abspath[:-1]
                print("abspath   " + abspath)
                print("ROOT_DIR  " + ROOT_DIR)
                print(str(abspath).replace(str(ROOT_DIR), ''))
                check_if_string_in_file(y, sub_file.replace(".md",""), node, str(abspath).replace(str(ROOT_DIR), ''))

    return False

        
check_if_string_in_file("index.rst", "TechDocs", tocChild)


  
xml_str = root.toprettyxml(indent ="\t") 
  
with open( output_file, "w", encoding='utf-8') as f:
    f.write(xml_str) 
