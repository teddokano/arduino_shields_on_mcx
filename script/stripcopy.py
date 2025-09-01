#!/usr/bin/env python3

import	os
import	subprocess
import	argparse

suppported_boards	= [ "FRDM_MCXA153", "FRDM_MCXA156", "FRDM_MCXN236", "FRDM_MCXN947" ]
libraries			= [ "_r01lib_frdm_mcxa153", "_r01lib_frdm_mcxa156", "_r01lib_frdm_mcxn236", "_r01lib_frdm_mcxn947" ]
lib_dict			= dict( zip( suppported_boards, libraries ) )

new_folder_place	= "../"
new_folder_suffix	= "_strippedcopy"

def main():
	base_dir	= os.getcwd()

	source_folder_name	= os.path.basename( base_dir )
	
	new_folder_path		= f"{base_dir}/{new_folder_place}/{source_folder_name}{new_folder_suffix}/"
#	new_folder_path		= base_dir + "/" + new_folder_place + "/" + source_folder_name + new_folder_suffix + "/"
	
	source_path	= [ i.path for i in os.scandir( base_dir )  if "/." not in i.path ]
	source_path	= [ i for i in source_path if "/." not in i ]
	source_path.sort()

	prj_name	= [ os.path.basename( i ) for i in source_path ]

	print( "======= process started for .. =======" )
	print( base_dir )
	print( source_folder_name )
	print( source_path )
	print( new_folder_path )
	print( "project sources:\n    " + "\n    ".join( prj_name ) )

	comm_exec( [ f"mkdir {new_folder_path}" ], True )


	for p in prj_name:
		if "_r01lib" in p:
			continue

		if "app_template" in p:
			continue
	
		commands	= []
		
		src	= f"{base_dir}/{p}/source/"
		
		for t in suppported_boards:
			p	= p.replace( f"_{t}", '' )
		
		trg	= f"{new_folder_path}/{p}/"
		
		commands	+= [ f"cp -r {src} {trg}" ]
		
		comm_exec( commands, True )

def comm_exec( commands, exe_flag ):
	for c in commands:
		print( "    executing command: " + c )

		if exe_flag:
			subprocess.run( c, shell = True )


if __name__ == "__main__":
	main()

