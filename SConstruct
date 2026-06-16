########################################################################
#
# File Name: SConstruct
#
# Objective: This is the scons based build file for a ray tracer
#
# Revision History:
#
# Login          MM/DD/YYYY      Description
#
# rlange         10/14/2010      Initial Draft
#
###########################################################################

SOURCE_DIR = './src/'
list_of_src_files_to_compile = ['main.cpp',
                                'collisionMap.cpp',
                                'paddle.cpp',
                                'target.cpp',
                                'targetHandler.cpp']

list_of_src_files_with_path = []

for file in list_of_src_files_to_compile:
    list_of_src_files_with_path += [SOURCE_DIR + file]

env = Environment(CPPPATH = ['#/include'])

env.ParseConfig('pkg-config allegro-5 allegro_image-5 allegro_primitives-5 --libs --cflags')

env.Program(target='Pinball',
            source= list_of_src_files_with_path,
            CCFLAGS = '-O2 -W -Wall -Wextra -Wno-unused-parameter')
