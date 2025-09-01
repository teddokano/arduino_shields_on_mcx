#!/usr/bin/env python3

import	os
import	subprocess

build_configs	= [ "Debug", "Release" ]

dox_file_name	= "Doxyfile"
dox_folder_path	= "./docs/r01lib_docs"

base_dir	= os.getcwd() + "/"

prjs_path	= [ i.path for i in os.scandir( base_dir )  if "/." not in i.path ]
prjs_path	= [ i for i in prjs_path if "/." not in i ]
prjs_path	= [ i for i in prjs_path if "RTOS_TAD_logs" not in i ]
prjs_path.sort()

doxy_path	= []

###
### scan workspace and find directories/files
###

for prj in prjs_path:
	for rootdir, dirs, files in os.walk( prj ):
		for f in files:
			if ( dox_file_name == f ):
				doxy_path	+= [ rootdir ]

###
### show found directories/files
###

print( "======= process started for .. =======" )
print( "projects / files:\n  " + "\n  ".join( [ os.path.basename( i ) for i in prjs_path ] ) )
print( "Doxyfile path:\n  " + "\n  ".join( doxy_path ) )
print( "Generated docs will be in \n  " dox_folder_path )
print( "" )

###
### clear directory on the dox_folder_path
###

print( "####### clearing \"" + dox_folder_path + "\" folder" )
subprocess.run( "rm -rf " + dox_folder_path, shell = True )

###
### choose one of "Doxyfile" and generate docments in dox_folder_path
###

try:
	os.chdir( doxy_path[ 0 ] )
except:
	pass
else:
	print( "####### generating documents: " + target )
	subprocess.run( "doxygen", shell = True )
