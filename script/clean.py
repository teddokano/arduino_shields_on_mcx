#!/usr/bin/env python3

###
### MCUXpresso workspace cleaning script
### 	Cleans all projects with keeping files needed for MCUXpresso VSC importing
###

import	os
import	subprocess

build_configs	= [ "Debug", "Release" ]

dox_file_name	= "Doxyfile"
dox_folder_name	= "r01lib_docs"

base_dir	= os.getcwd() + "/"

prjs_path	= [ i.path for i in os.scandir( base_dir )  if "/." not in i.path ]
prjs_path	= [ i for i in prjs_path if "/." not in i ]
prjs_path	= [ i for i in prjs_path if "RTOS_TAD_logs" not in i ]
prjs_path.sort()

"""
doxy_path	= []
dox_folders	= []

for prj in prjs_path:
	for rootdir, dirs, files in os.walk( prj ):
		for d in dirs:
			if ( dox_folder_name == d ):
					dox_folders	+= [ rootdir + "/" + d ]
		for f in files:
			if ( dox_file_name == f ):
				doxy_path	+= [ rootdir ]
"""

print( "======= process started for .. =======" )
print( "projects / files:\n  " + "\n  ".join( [ os.path.basename( i ) for i in prjs_path ] ) )
#print( "Doxyfile path:\n  " + "\n  ".join( doxy_path ) )
#print( dox_folder_name + " are in ..\n  " + "\n  ".join( dox_folders ) )
#print( "" )

"""
for target in dox_folders:
	try:
		os.chdir( target )
	except:
		pass
	else:
		print( "####### clearing \"" + dox_folder_name + "\" folders" )
		subprocess.run( "rm -rf " + target, shell = True )

for target in doxy_path:
	try:
		os.chdir( target )
	except:
		pass
	else:
		print( "####### generating documents: " + target )
		subprocess.run( "doxygen", shell = True )
"""

for target in prjs_path:
	print( "####### cleaning: " + target )			
	subprocess.run( "rm " + target + "/.gitignore", shell = True )	# Delete ".gitignore" file added by IDE

	for cnfg in build_configs:
		try:
			os.chdir( target + "/" + cnfg + "/" )
		except:
			pass
		else:
			print( "####### cleaning: " + target + " [" + cnfg + "]" )
			subprocess.run( "make -r -j9 clean", shell = True )
	
	try:
		os.chdir( target )
	except:
		pass
	else:
		for cnfg in build_configs:
			if os.path.isdir( cnfg ) and os.path.isfile( cnfg + "/makefile" ):
				"""
				subprocess.run( "rm -rf " + cnfg + "/", shell = True )
				"""
				copying_files	= cnfg + "/*.ld " + cnfg + "/makefile " + cnfg + "/sources.mk "
				subprocess.run( "mkdir " + cnfg + "-ld", shell = True )
				subprocess.run( "cp " + copying_files + cnfg + "-ld/", shell = True )
				subprocess.run( "rm -rf " + cnfg + "/", shell = True )
				subprocess.run( "mv " + cnfg + "-ld/ " + cnfg + "/", shell = True )
			else:
				subprocess.run( "rm -rf " + cnfg + "/", shell = True )
					
		subprocess.run( "rm -rf *.launch *.mex CMakeLists.txt flags.cmake CMakePresets.json mcux_include.json user_override.cmake .vscode/", shell = True )
	
try:
	os.chdir( base_dir )
except:
	pass

subprocess.run( "rm -rf .metadata/.plugins/org.eclipse.cdt.core", shell = True )

print( "" )
print( "======= process completed for .. =======" )
print( "projects / files:\n  " + "\n  ".join( [ os.path.basename( i ) for i in prjs_path ] ) )
print( "done" )
print( "" )
