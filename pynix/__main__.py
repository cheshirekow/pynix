import argparse
import sys

import pynix


def main():
  parser = argparse.ArgumentParser(description=pynix.__doc__)
  parser.add_argument("--version", action="version", version=pynix.VERSION)
  _args = parser.parse_args()
  return 0


if __name__ == "__main__":
  sys.exit(main())
