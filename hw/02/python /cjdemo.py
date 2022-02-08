
import os
import sys
import json
import random


def main():
  
    #-- read the CityJSON file with the json library of Python
    j = json.loads(open('../data/twobuildings.city.json').read())

    #-- print out the number of Buildings in the file
    nobuildings = 0
    for co in j['CityObjects']: 
        if j['CityObjects'][co]['type'] == 'Building':
            nobuildings += 1
    print("There are {} Buildings in the file".format(nobuildings))

    #-- print out the number of vertices in the file
    print("There are {} vertices in the file".format(len(j['vertices'])))

    #-- add an attribute "volume"
    for co in j['CityObjects']: 
        if j['CityObjects'][co]['type'] == 'Building':
            j['CityObjects'][co]['attributes']['volume'] = random.uniform(100, 5000)

    #-- write to disk the modified city model (myfile.city.json)
    f = open('myfile.city.json', "w")
    json_str = json.dumps(j, indent=2)
    f.write(json_str)
    print("File myfile.city.json written to disk.")
            
if __name__ == "__main__":
    main()  