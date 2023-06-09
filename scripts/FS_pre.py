import os
import gzip
import shutil
import requests
import html
import time
from SCons.Script import COMMAND_LINE_TARGETS        

Import("env")

print("Extra Script (Pre): FS_pre.py")


def checkFSBuild():

  for t in COMMAND_LINE_TARGETS:
    if type(t) == str:
      name = t
    else:
      name = t.name
    return bool(name == "buildfs" or name == "uploadfs")
  return False



def createfolders():
    # Create folders
    try:
        os.mkdir("data")
    except OSError:
        print ("<data> folder exists")
    else:
        print ("Created <data> folder")

    try:
        os.mkdir("data/tmp")
    except OSError:
        print ("<data/tmp> folder exists")
    else:
        print ("Created <data/tmp> folder")

    try:
        os.mkdir("data/www")
    except OSError:
        print ("<data/www> folder exists")
    else:
        print ("Created <data/www> folder")

    try:
        os.mkdir("dynamic")
    except OSError:
        print ("<dynamic> folder exists")
    else:
        print ("Created <dynamic> folder")


# Get build flags values from env
def get_build_flag_value(flag_name):
    build_flags = env.ParseFlags(env['BUILD_FLAGS'])
    flags_with_value_list = [build_flag for build_flag in build_flags.get('CPPDEFINES') if type(build_flag) == list]
    defines = {k: v for (k, v) in flags_with_value_list}
    return defines.get(flag_name)


def minify_file(file,url):
    print("Minifying: " + str(file))
    data = {'input': open(file, 'rb').read()}
    response = requests.post(url, data=data)
    time.sleep(1)
    minified = response.content
    with open(file, 'wb') as f_out:
        f_out.write(minified)


def replaceBuildFlags(file):
    with open(file, 'rb') as f_in:
        content = f_in.read()
        soup = BeautifulSoup(content, 'html')
        for item in soup.find_all(class_='pre-replace'):
            if str(item) != 'None':
                flag = item.string
                print('Replacing ' + flag + ' with \'' + get_build_flag_value(flag) + '\' in ' + file )
                item.replaceWith(get_build_flag_value(flag))
        f_in.close()
    with open(file,'w') as f_out:
        f_out.write(str(soup))


def conditionalElements(file):
    with open(file, 'rb') as f_in:
        content = f_in.read()
        soup = BeautifulSoup(content, 'html')
        for item in soup.find_all(class_='pre-conditional'):
            if str(item) != 'None':
                flag = item['buildflag']
                if str(get_build_flag_value(flag)) != 'None':
                    print('Removing conditional element ' + flag + ' in ' + file )
                    item.replaceWith('')
        f_in.close()
    with open(file,'w') as f_out:
        f_out.write(str(soup))


def includeHTMLfile(file,sources):
    with open(file, 'rb') as f_in:
        content = f_in.read()
        soup = BeautifulSoup(content, 'html')
        for item in soup.find_all(class_='pre-include'):
            if str(item) != 'None':
                includeFile = item['src']
                print('Including ' + includeFile + ' in ' + file )
                with open(os.path.join(sources, includeFile), 'r') as f_incl:
                    newcontent = f_incl.read()
                    item.replaceWith(BeautifulSoup(newcontent, 'html'))
        f_in.close()
    with open(file,'w') as f_out:
        f_out.write(str(soup))


# Replace build codes and move to new folder
def parse_replace(sourceFolder,destFolder):
    p_sourceFolder = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + sourceFolder)
    p_destFolder = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + destFolder)

    print('Parsing html files in ' + p_sourceFolder)

    for folder, subfolders, files in os.walk(p_sourceFolder):
        for file in files:
            if file.endswith('.html'):
                shutil.copyfile(os.path.join(folder, file), os.path.join(p_destFolder,file))
                includeHTMLfile(os.path.join(p_destFolder,file),folder)
                replaceBuildFlags(os.path.join(p_destFolder,file))
                conditionalElements(os.path.join(p_destFolder,file))
            elif file.endswith('.js'):
                shutil.copyfile(os.path.join(folder, file), os.path.join(p_destFolder,file))
            elif file.endswith('.css'):
                shutil.copyfile(os.path.join(folder, file), os.path.join(p_destFolder,file))
            elif not file.endswith('.html'):
                shutil.copyfile(os.path.join(folder, file), os.path.join(p_destFolder,file))



# Inline files in Flash
def inlineFlashFiles(sourceFolder,destFile):
    p_sourceFolder = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + sourceFolder)
    p_destFile = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + destFile)

    print("Inlining files from " + p_sourceFolder + " into " + destFile)

    fileList = []

    with open(p_destFile, 'w' ) as f_out:

        f_out.write("#ifndef WEBFILES_H\n")
        f_out.write("#define WEBFILES_H\n\n")

        f_out.write("#include <Arduino.h>\n\n\n")

        # Each file here

        for folder, subfolders, files in os.walk(p_sourceFolder):
            for file in files:
                print("Creating inline Flash for file: " + file)
                fileName = os.path.basename(file)
                h_fileName = fileName.replace(" ","")
                h_fileName = h_fileName.replace(".","_")
                h_fileName = h_fileName.replace("-","_")
                url = os.path.splitext(fileName)[0]

                f_out.write("const uint8_t __" + h_fileName + "__ [] PROGMEM = {")
                with open(os.path.join(p_sourceFolder,file), 'rb') as f_in:
                    byte = f_in.read(1)
                    while byte:
                        out = "0x00{:X},".format(ord(byte))
                        f_out.write(out)
                        byte = f_in.read(1)
                f_out.write("\n};\n\n")

                # Capture table entry
                reftable = "\t{\n\t\t.path = \"/" + url + "\",\n\t\t.content = __" + h_fileName + "__,\n\t\t.len = sizeof(__" + h_fileName + "__),\n\t},\n\n"
                fileList.append(reftable)

        f_out.write("\n")

        f_out.write("static struct t_websitefiles {\n")
        f_out.write("\tconst char* path;\n")
        f_out.write("\tconst uint8_t* content;\n")
        f_out.write("\tconst uint len;\n")
        f_out.write("} websiteFiles[] PROGMEM = {\n\n")

        for listItem in fileList:
            f_out.write(listItem)

        f_out.write("};\n\n")
        f_out.write("#endif\n")


# Replace build codes and move to new folder
def minify_files(sourceFolder):
    p_sourceFolder = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + sourceFolder)

    print('Minifying files in ' + p_sourceFolder)

    for folder, subfolders, files in os.walk(p_sourceFolder):
        for file in files:
            if file.endswith('.html'):
                minify_file(os.path.join(p_sourceFolder,file),'https://www.toptal.com/developers/html-minifier/api/raw')
            elif file.endswith('.js'):
                minify_file(os.path.join(p_sourceFolder,file),'https://javascript-minifier.com/raw')
            elif file.endswith('.css'):
                minify_file(os.path.join(p_sourceFolder,file),'https://www.toptal.com/developers/cssminifier/api/raw')


# Empty folder
def deletecontents(folder):
    for root, dirs, files in os.walk(folder):
        for file in files:
            os.remove(os.path.join(root, file))


# Gzip files in one folder to another
def deflate_www(sourceFolder,destFolder):

    p_sourceFolder = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + sourceFolder)
    p_destFolder = os.path.abspath(env.subst("$PROJECT_DIR") + "/" + destFolder)

    print('Deflating www files in ' + p_sourceFolder)
    print('To ' + p_destFolder)

    for folder, subfolders, files in os.walk(p_sourceFolder):
        for file in files:
            with open(os.path.join(folder, file), 'rb') as f_in:
                with gzip.open(os.path.join(p_destFolder, file) + '.gz', 'wb') as f_out:
                    shutil.copyfileobj(f_in, f_out)


# Main

if checkFSBuild():

    # Install packages
    os.system("python -m pip install --upgrade pip")
    os.system("python -m pip install beautifulsoup4")
    from bs4 import BeautifulSoup

    # Clean up
    createfolders()
    deletecontents("data/tmp")
    deletecontents("data")

    # Parse and replace 
    parse_replace("www","data/tmp")

    # Minify files
    minify_files("data/tmp")

    # Deflate www into data
    deflate_www("data/tmp","data/www")

    # Inline files into flash
    inlineFlashFiles("data/www","dynamic/WebFiles.h")

    # Clean up
    shutil.rmtree("data/tmp")

else:
    print("Not FS Build")
