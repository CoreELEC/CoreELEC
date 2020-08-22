#!/usr/bin/env python

import jinja2
import yaml

from os import getcwd, path
from sys import argv

yaml_file = argv[1]
try:
    output_location = argv[2]
except IndexError:
    output_location = getcwd() 

templateLoader = jinja2.FileSystemLoader(searchpath=".")
templateEnv = jinja2.Environment(loader=templateLoader)
port_template = templateEnv.get_template("port.sh.j2")
gamelist_template = templateEnv.get_template("gamelist.xml.j2")

with open(yaml_file) as ports_file:
    ports = yaml.load(ports_file, Loader=yaml.SafeLoader)

for name, data in ports.items():
    # Create port script
    with open(path.join(output_location, "{}.sh".format(name)), "w") as port_file:
        port_file.write(port_template.render(name=name, data=data))
        ports_file.close()

# Create gamelist.xml
with open(path.join(output_location, "gamelist.xml"), "w") as gamelist_file:
    gamelist_file.write(gamelist_template.render(games=ports.items()))
    gamelist_file.close()
