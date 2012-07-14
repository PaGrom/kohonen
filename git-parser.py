import os
import sys

if len(sys.argv) < 2:
    sys.exit('Usage: %s /path/to/git-folder' % sys.argv[0])

NumOfCommits = 1000
gitlog = r"--pretty=tformat:'%cE %H'"
gitshow = r"--raw --name-only --pretty=tformat:''"

parameters = ['arm', 'x86', 'drivers']

commit_list = os.popen("cd %s && git log -n %d %s" % (sys.argv[1], NumOfCommits, gitlog)).read()
commit_list = commit_list.splitlines()

d_list = {}

for i in xrange(len(commit_list)):
	commit_list[i] = commit_list[i].split()

	diff = os.popen("cd %s && git show %s %s" % (sys.argv[1], commit_list[i][1], gitshow)).read()
	diff = diff[2:].splitlines()

	commit_list[i].pop()
	commit_list[i] += diff
	
	if commit_list[i][0] not in d_list:
		d_list[commit_list[i][0]] = {}
		d_list[commit_list[i][0]].update({}.fromkeys(parameters, 0))
	else:
		for j in xrange(1, len(commit_list[i])):
			path = commit_list[i][j].split('/')

			for k in parameters:
				if k in path:
					d_list[commit_list[i][0]][k] += 1
				
xml_file = open('d_list.xml', 'w')

xml_file.write('<?xml version="1.0" ?>\n<GitLinux>\n')

for par in parameters:
	xml_file.write("""\t<Parameter par="%s"/>\n""" % par)

#for key in d_list.keys():
#	if d_list[key]['arm'] + d_list[key]['x86'] + d_list[key]['drivers'] == 0:
#		continue
		
#	xml_file.write("""\t<Developer Email="%s" arm="%d" x86="%d" drivers="%d" />\n""" % (key, d_list[key]['arm'], d_list[key]['x86'], d_list[key]['drivers']))

for key in d_list.keys():
	sum = 0
	for k in parameters:
		sum+=d_list[key][k]
	if sum == 0:
		continue
	st = """\t<Developer Email=\"""" + key + """" """
	for k in parameters:
		st += k + """=\"""" + str(d_list[key][k]) + """" """
	st += "/>\n"

	xml_file.write(st)
	
xml_file.write('</GitLinux>')
