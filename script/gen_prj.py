#!/usr/bin/env python3

import	os
import	subprocess
import	argparse

suppported_boards	= [ "FRDM_MCXA153", "FRDM_MCXA156", "FRDM_MCXN236", "FRDM_MCXN947" ]
libraries			= [ "_r01lib_frdm_mcxa153", "_r01lib_frdm_mcxa156", "_r01lib_frdm_mcxn236", "_r01lib_frdm_mcxn947" ]
lib_dict			= dict( zip( suppported_boards, libraries ) )

app_template_prj	= "app_template_"
build_configs		= [ "Debug", "Release" ]

lib_and_template	= "library_and_template_projects"

filehead_text		= "FILEHEAD.txt"
sdk_version_file	= "script/sdk_version_identifier.txt"
ide					= "/Applications/MCUXpressoIDE_24.9.25/ide/MCUXpressoIDE.app/Contents/MacOS/mcuxpressoide"

def main():

	###
	### SDK version number update
	###
	
	with open( sdk_version_file ) as f:
		sdk_version	= f.read()
	
	commands	= []

	sdk_version_update_projects	 = [ f"{lib_and_template}/{folder}" for folder in libraries ]
	sdk_version_update_projects	+= [ f"{lib_and_template}/{app_template_prj}{farget}" for farget in suppported_boards ]
	
	for prj in sdk_version_update_projects:
		commands	+= [ f"sed -i -e s/'<sdkVersion>.*<\/sdkVersion>'/'<sdkVersion>{sdk_version}<\/sdkVersion>'/ {prj}/.cproject" ]

	comm_exec( commands, not args.no_exec )

	###
	### prepare project information
	###
	
	base_dir	= os.getcwd() + "/"

	source_folder_path	= os.path.dirname( args.input_folder + "/" )
	source_folder_name	= os.path.basename( source_folder_path )

	if args.target:
		args.target		= args.target.strip( "'" )
		target_boards	= []
		for n in suppported_boards:
			if args.target.lower() in n.lower():
				target_boards	+= [ n ]
	else:
		target_boards	= suppported_boards
	
	library_folders		= [ lib_dict[ i ] for i in target_boards if i in target_boards ]

	if args.zipfile:
		output_zip_name		= args.zipfile
	else:
		output_zip_name		= source_folder_name + "_" + "_".join( target_boards ) + ".zip"

	source_path	= [ i.path for i in os.scandir( base_dir + "/" + source_folder_path )  if "/." not in i.path ]
	source_path	= [ i for i in source_path if "/." not in i ]
	source_path.sort()

	prj_name	= [ os.path.basename( i ) for i in source_path ]
	prj_name	= [ i for i in prj_name if i != filehead_text ]

	print( "======= process started for .. =======" )
	print( "project sources:\n    " + "\n    ".join( prj_name ) )

	template_folders	= []
	app_folders			= []
	
	###
	### copying projects for each targets
	###   this part works even if the script deleting projects 
	###

	for t in target_boards:
		template	= app_template_prj + t
		print( "using template : " + template )
		template_folders	+= [ template ]
		
		for p in prj_name:
			commands	= []
			new_prj	= p + "_" + t
			
			message	= "deleting" if args.delete else "generating"
			
			print( f"    {message} project: {new_prj}" )
			app_folders	+= [ new_prj ]
			
			commands	+= [ f"cp -r {lib_and_template}/{template}/ {new_prj}/" ]	#	copy template
			commands	+= [ f"cp -r {source_folder_path}/{p}/ {new_prj}/source/" ]	#	copy source files
			commands	+= [ "rm -rf " + " ".join( [ new_prj + "/" + i + "/" for i in build_configs ] ) ]		#	delete built folders
			commands	+= [ f"sed -i -e s/'<name>{template}'/'<name>{new_prj}'/ {new_prj}/.project" ]
			
			if not args.delete:
				comm_exec( commands, not args.no_exec )

	if not args.delete:

		###
		### filehead replacing
		###
		
		commands	= []
		
		for a in app_folders:
			filehead	= f"{source_folder_path}/FILEHEAD.txt"
			src_files	= []

			for pathname, dirnames, filenames in os.walk( f"{a}" ):
				for f in filenames:
					src_files	+= [ f"{pathname}/{f}" ]
			
			src_files	= [ s for s in src_files if "/." not in s ]
			
			rep_comm	= "awk 'NR==FNR{a=a $0 ORS; next} {gsub(/\/\/FILEHEAD/, a)}1'"
			
			for src in src_files:
				commands	+= [ f"{rep_comm} {filehead} {src} > tmp && mv tmp {src}" ]
		
		comm_exec( commands, not args.no_exec and not args.delete )

		"""
		###
		### build --- This could not been done. It should be done in active workspace
		###
		
		commands	 = []
		
		for lib in library_folders:
			commands	+= [ f"cp -r {lib_and_template}/{lib}/ ./{lib}/" ]	#	copy template
			commands	+= [ f"{ide} -nosplash --launcher.suppressErrors -application org.eclipse.cdt.managedbuilder.core.headlessbuild -data '{base_dir}' -cleanBuild {lib}" ]

		comm_exec( commands, not args.no_exec )	
		"""	
		
		###
		### zipping
		###
		
		try:
			os.chdir( base_dir + lib_and_template )
		except:
			print( "  !!!!!!!!!!  error: couldn't 'cd' to 'library_and_template_projects/'" )
		else:
			commands	 = []
			commands	+= [ f"rm -rf ../{output_zip_name}" ]
			commands	+= [ f"zip -r ../{output_zip_name} " + " ".join( library_folders ) + " " + " ".join( template_folders ) + "> /dev/null" ]

			comm_exec( commands, not args.no_exec and not args.delete )

		try:
			os.chdir( base_dir )
		except:
			print( "  !!!!!!!!!!  error: couldn't 'cd' to base directory" )

		commands	 = []
		commands	+= [ f"zip -r {output_zip_name} " + " ".join( app_folders ) + "> /dev/null" ]

		comm_exec( commands, not args.no_exec and not args.delete )

	###
	### deleting projects after zipping
	###
	
	if not args.keep:
		commands	 = []
		commands	+= [ "rm -rf " + " ".join( app_folders ) ]

		comm_exec( commands, not args.no_exec )

###
###	command executer
###

def comm_exec( commands, exe_flag ):
	for c in commands:
		print( "    executing command: " + c )

		if exe_flag:
			subprocess.run( c, shell = True )

def command_line_handling():
	parser	= argparse.ArgumentParser( description = "r01lib MCUXpresso project generator" )
	qv_grp	= parser.add_mutually_exclusive_group()
	
	parser.add_argument( "input_folder",			help = "input source folder name" )
	parser.add_argument( "-t", "--target",			help = "target board", 				type=ascii )
	parser.add_argument( "-z", "--zipfile",			help = "output zip file name", 		type=ascii )
	qv_grp.add_argument( "-d", "--delete", 			help = "delete project folders",	action = "store_true" )
	qv_grp.add_argument( "-k", "--keep", 			help = "keep project folders",		action = "store_true" )
	qv_grp.add_argument( "-N", "--no_exec", 		help = "no execution",				action = "store_true" )
	
	return	parser.parse_args()

if __name__ == "__main__":
	args	= command_line_handling()
	
	if args.no_exec:
		exec	= False
	main()

