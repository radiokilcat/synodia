#!/usr/bin/env python

import argparse
import os
import subprocess
import sys
import zipfile
import ast


class DirectoryHolder(object):

    def __init__(self, cd):
        self.work_dir = cd
        self.base_dir = os.getcwd()

    def __enter__(self):
        os.makedirs(self.work_dir, exist_ok=True)
        os.chdir(self.work_dir)
        return self

    def __exit__(self, exc_type, exc_val, exc_tb):
        os.chdir(self.base_dir)


def generator():
    return "Visual Studio 16 2019"


def cmake_generate(source_dir, build_type, build_name):
    return ['cmake', '{}'.format(source_dir),
            '-G', generator(),
            '-DCMAKE_BUILD_TYPE={}'.format(build_type)]


def exec_command_list(command_list):
    for command in command_list:
        p = subprocess.Popen(command, stderr=subprocess.PIPE)
        output, error = p.communicate()
        if p.returncode != 0:
            raise ValueError("\n\n{}\n\ncommand: {}".format(error.decode(), ' '.join(command)))


with DirectoryHolder(cd='build/win32') as dh:
    exec_command_list([cmake_generate(dh.base_dir, "Debug", "win32"),])