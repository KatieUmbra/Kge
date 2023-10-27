#!/usr/bin/env python

from pathlib import Path
import subprocess
import logging
import argparse
import sys
import os
import shutil
import requests
import ctypes

logger = logging.getLogger()
logger.setLevel(logging.INFO)
logging.basicConfig(format="[%(levelname)s]: %(message)s\n")
parser = argparse.ArgumentParser(description = "Kge build helper script")
parser.add_argument(
        "do",
        metavar="A",
        type=str,
        nargs=1,
        choices=["run", "run_no_build", "build", "clean", "install"],
        help="Action to do [run, build, clean, install]")

cmake_cmd = "cmake"

def program_exists(program: str) -> bool:
    return shutil.which(program) is not None

def verify_program(program: str):
    if (program_exists(program)):
        logging.info("%s found", program)
    else:
        shutdown(f"{program} doesn't exist, install it before running the script.", 1)

def check_cmake():
    output = subprocess.run(["cmake", "--version"], capture_output=True).stdout
    if "3.28" not in output.decode("utf-8"):
        logging.error("Minimum cmake version is 3.28 since this project uses modules")
    else:
        return
    try:
        output = subprocess.run("~/.local/cmake-3.28.0-rc2/bin/cmake --version", capture_output=True, shell=True).stdout
    except subprocess.CalledProcessError:
        logging.error("No cmake 3.28 installation in ~/.local, install cmake 3.28 in ~/.local/cmake-3.28.0-rc2/")
        shutdown("invalid cmake", 1)

    if "3.28" not in output.decode("utf-8"):
        logging.error("Minimum cmake version is 3.28 since this project uses modules")
        shutdown("invalid cmake", 1)

    global cmake_cmd
    cmake_cmd = "~/.local/cmake-3.28.0-rc2/bin/cmake"

def shutdown(message: str, code: int):
    logging.error("script had to exit: %s", message)
    sys.exit(code)

def install():

    if os.name == "nt":
        admin = ctypes.windll.shell32.IsUserAnAdmin() != 0
        if not admin:
            logging.warn("ALWAYS READ THE SOURCE CODE OF SCRIPTS YOUR RUN.")
            shutdown("install must be ran with administrator privileges on windows", 1)

    if Path("./libs/install.lock").exists():
        shutdown("Libs were already installed.", 0)

    output = ""
    try:
        output = subprocess.run(["python3", "-m", "glad"], capture_output=True).stderr
    except subprocess.CalledProcessError:
        if "No module named glad" in output:
            logging.error("Glad is not present")
            logging.info("install glad: pip3 install --user glad2")
            logging.info("for arch linux users: yay -S python-glad")
            shutdown("No glad", 1)

    command = ["python3", "-m", "glad", "--profile", "core", "--out-path", "libs/glad", "--generator", "c"]

    logging.info("installing GLAD")
    subprocess.run(command, capture_output=True)

    logging.info("Copying stb header(s)")
    url = "https://raw.githubusercontent.com/nothings/stb/master/stb_image.h"
    request = requests.get(url)
    with open("./libs/include/STB/stb_image.h", "wb") as file:
        file.write(request.content)

    logging.info("creating symlinks")
    
    Path("./libs/include/GLFW").symlink_to("./libs/glfw/include/GLFW")
    Path("./libs/include/GLM").symlink_to("./libs/glm/glm")
    Path("./libs/include/GLAD").symlink_to("./libs/glad/include/glad/")

    logging.info("writing lock file")
    with open("./libs/install.lock", "w") as file:
        file.write("Libs installed")


def build():
    verify_software()
    check_cmake()
    if not Path("./libs/install.lock").exists():
        shutdown("Libs are not installed, run 'python3 kge.py install'.", 0)
    clean()
    global cmake_cmd
    logging.info("Building cmake files")
    try:
        if os.name == "nt":
            subprocess.run(f"{cmake_cmd} -B build -G 'Visual Studio 17 2022'", shell=True)
        else:
            subprocess.run(f"{cmake_cmd} -B build -G Ninja", shell=True)
    
        logging.info("Building")
        subprocess.run(f"{cmake_cmd} --build build", shell=True)

    except subprocess.CalledProcessError:
        shutdown("cmake error", 1)


def run():
    build()
    try:
        subprocess.run("./build/oglgame", shell=True)
    except subprocess.CalledProcessError:
        logging.error("error during the execution of the binary")

def clean():
    if os.name == "nt":
        subprocess.run(["rd", "/s", "/q", "build"])
    else:
        subprocess.run(["rm", "-rf", "build"])

if not Path("./kge.py").exists():
    shutdown("Run this script in the root of kge.", 1)
args = parser.parse_args()
argument = args.do[0]

def verify_software():
    verify_program("cmake")
    if os.name == 'nt':
        logging.info("verifying visual studio install.")
        verify_program("cl")
    else:
        verify_program("ninja")
        clang = program_exists("clang")
        gnu = program_exists("gcc")
        if not (gnu or clang):
            logging.error("No valid compiler was found, install llvm or gnu gcc.")
            shutdown("no compiler", 1)

if argument == "install":
    install()
    logging.info("libs installed succesfully")
elif argument == "build":
    build()
    logging.info("build was succesfull")
elif argument == "run":
    run()
    logging.info("execution finished")
elif argument == "run_no_build":
    try:
        subprocess.run("./build/oglgame", shell=True)
    except subprocess.CalledProcessError:
        logging.error("error during the execution of the binary")
    logging.info("execution finished")
elif argument == "clean":
    clean()
    logging.info("cleaned build files")
