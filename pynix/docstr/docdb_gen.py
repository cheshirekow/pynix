"""
Generate .h/.c files containg docstrings for pynix
"""

import argparse
import io
import json
import re
import os


def split_sections(content):
  out = {}
  current_section = []

  delimeter_pattern = re.compile(r"\.\. symbol: (.*)")

  for line in content.split("\n"):
    match = delimeter_pattern.match(line)
    if match:
      symbol = match.group(1)
      current_section = out[symbol] = []
      continue

    current_section.append(line)

  return out


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument("--out-dir", default=".")
  parser.add_argument("docdb")
  args = parser.parse_args()

  with io.open(args.docdb, "r", encoding="utf-8") as infile:
    content = infile.read()
  docdict = split_sections(content)

  headerpath = os.path.join(args.out_dir, "docstr.h")
  sourcepath = os.path.join(args.out_dir, "docstr.c")

  with io.open(headerpath, "w", encoding="utf-8") as outfile:
    outfile.write("#pragma once\n")
    outfile.write(
        "// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>\n\n")
    for symbol in sorted(docdict.keys()):
      outfile.write("const char* g_{}_docstr;\n".format(symbol))

  with io.open(sourcepath, "w", encoding="utf-8") as outfile:
    outfile.write(
        "// Copyright 2020 Josh Bialkowski <josh.bialkowski@gmail.com>\n\n")
    outfile.write("#include \"pynix/docstr.h\"\n\n")
    for symbol, lines in sorted(docdict.items()):
      outfile.write("const char* g_{}_docstr = \n".format(symbol))
      for line in lines:
        printme = json.dumps(line).strip('"')
        outfile.write('"{}\\n"\n'.format(printme))
      outfile.write(";\n\n")


if __name__ == "__main__":
  main()
