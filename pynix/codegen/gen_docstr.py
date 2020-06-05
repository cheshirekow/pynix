"""
Generate .h/.c files containg docstrings for pynix
"""

import argparse
import io
import json
import re
import os

import jinja2


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

    line = json.dumps(line).strip('"')
    current_section.append(line)

  return out


def main():
  parser = argparse.ArgumentParser(description=__doc__)
  parser.add_argument("--out-dir", default=".")
  args = parser.parse_args()

  thisdir = os.path.dirname(os.path.realpath(__file__))
  docdb_path = os.path.join(thisdir, "docdb.rst")

  with io.open(docdb_path, "r", encoding="utf-8") as infile:
    content = infile.read()
  docdict = split_sections(content)

  headerpath = os.path.join(args.out_dir, "docstr.h")
  sourcepath = os.path.join(args.out_dir, "docstr.c")

  env = jinja2.Environment(
      loader=jinja2.FileSystemLoader(thisdir))
  env.globals["sorted"] = sorted

  tpl = env.get_template("docstr.h.in")
  content = tpl.render(docdict=docdict)
  with io.open(headerpath, "w", encoding="utf-8") as outfile:
    outfile.write(content)

  tpl = env.get_template("docstr.c.in")
  content = tpl.render(docdict=docdict)
  with io.open(sourcepath, "w", encoding="utf-8") as outfile:
    outfile.write(content)


if __name__ == "__main__":
  main()
